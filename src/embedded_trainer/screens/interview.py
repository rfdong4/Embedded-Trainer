"""Interview simulation screens: configuration, session, and results."""

from __future__ import annotations

import time
from pathlib import Path

from textual.app import ComposeResult
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.screen import Screen
from textual.widgets import (
    Button, Footer, Header, Label, ListItem, ListView, Markdown, Static,
)

from embedded_trainer.core.code_runner import compile_and_test
from embedded_trainer.core.content_loader import load_topics
from embedded_trainer.core.gamification import update_streak
from embedded_trainer.core.interview_engine import (
    build_question_bank, score_interview, verdict_text,
)
from embedded_trainer.models.interview import (
    INTERVIEW_PRESETS, InterviewConfig, InterviewResult, QuestionResponse,
)
from embedded_trainer.models.question import CodingChallenge, Question
from embedded_trainer.widgets.achievement_toast import format_achievement
from embedded_trainer.widgets.code_editor import CodeEditor
from embedded_trainer.widgets.question_card import QuestionCard


# ---------------------------------------------------------------------------
# 1. CONFIG SCREEN: pick level + topic focus
# ---------------------------------------------------------------------------


class InterviewConfigScreen(Screen):
    """Entry point: choose level and topic focus for a mock interview."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "interview.tcss"

    BINDINGS = [
        ("escape", "go_back", "Back"),
    ]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._selected_level: str = "mid"
        self._selected_topic: str = "mixed"

    def compose(self) -> ComposeResult:
        yield Header()
        yield Label("Interview Simulator", classes="section-title")
        yield Label(
            "Simulate a technical interview for embedded systems roles. "
            "Mix of conceptual and coding questions under a time limit.",
            classes="interview-subtitle",
        )

        with Vertical(id="interview-config"):
            yield Label("Select Level:", classes="config-label")
            with Horizontal(id="level-buttons"):
                yield Button("Junior\n15 min | 5Q + 1 Code", id="btn-level-junior",
                             variant="success", classes="level-btn")
                yield Button("Mid-Level\n30 min | 6Q + 2 Code", id="btn-level-mid",
                             variant="primary", classes="level-btn selected")
                yield Button("Senior\n45 min | 7Q + 2 Code", id="btn-level-senior",
                             variant="warning", classes="level-btn")

            yield Label("Topic Focus:", classes="config-label")
            topic_list = ListView(id="topic-focus-list")
            topic_list.compose_add_child(
                ListItem(Label("Mixed (all topics) - recommended"), id="tf-mixed")
            )
            for topic in load_topics():
                topic_list.compose_add_child(
                    ListItem(
                        Label(f"  {topic['name']} only"),
                        id=f"tf-{topic['id']}",
                    )
                )
            yield topic_list

            # Show user's personal best for each level
            profile = self.app.user_profile
            pb_lines = []
            for lvl in ("junior", "mid", "senior"):
                best = self.app.db.get_best_interview_score(profile.id, lvl)
                if best is not None:
                    pb_lines.append(f"  {lvl.title()}: {best:.1f}%")
            if pb_lines:
                yield Label("Your Best Scores:", classes="config-label")
                yield Label("\n".join(pb_lines), id="pb-display")

            with Horizontal(id="start-row"):
                yield Button("Start Interview", id="btn-start-interview",
                             variant="success")

        yield Footer()

    def on_button_pressed(self, event: Button.Pressed):
        bid = event.button.id or ""
        if bid.startswith("btn-level-"):
            level = bid[len("btn-level-"):]
            self._selected_level = level
            # Visual feedback: toggle "selected" class
            for btn in self.query(".level-btn"):
                btn.remove_class("selected")
            event.button.add_class("selected")
        elif bid == "btn-start-interview":
            self._launch_interview()

    def on_list_view_selected(self, event: ListView.Selected):
        tid = event.item.id or ""
        if tid == "tf-mixed":
            self._selected_topic = "mixed"
        elif tid.startswith("tf-"):
            self._selected_topic = tid[len("tf-"):]

    def _launch_interview(self):
        config = INTERVIEW_PRESETS[self._selected_level]
        # Clone preset with chosen topic focus
        config = InterviewConfig(
            level=config.level,
            topic_focus=self._selected_topic,
            duration_minutes=config.duration_minutes,
            num_conceptual=config.num_conceptual,
            num_coding=config.num_coding,
            difficulty_mix=config.difficulty_mix,
        )
        self.app.push_screen(InterviewSessionScreen(config))

    def action_go_back(self):
        self.app.pop_screen()


# ---------------------------------------------------------------------------
# 2. SESSION SCREEN: the actual interview with timer
# ---------------------------------------------------------------------------


class InterviewSessionScreen(Screen):
    """Runs the interview: conceptual questions then coding challenges, with a live timer."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "interview.tcss"

    BINDINGS = [
        ("ctrl+r", "run_coding", "Run Tests (coding)"),
        ("ctrl+s", "submit_coding", "Submit (coding)"),
        ("ctrl+q", "abandon", "Abandon Interview"),
    ]

    def __init__(self, config: InterviewConfig, **kwargs):
        super().__init__(**kwargs)
        self.config = config
        self.questions: list[Question] = []
        self.challenges: list[CodingChallenge] = []
        self.responses: list[QuestionResponse] = []

        # Combined ordered queue: all conceptual first, then coding
        self._queue: list[tuple[str, object]] = []
        self._current_index = 0

        # Timing
        self._start_time = 0.0
        self._question_start_time = 0.0
        self._total_seconds = config.duration_minutes * 60
        self._timer_handle = None

        # Coding-specific
        self._coding_submitted = False
        self._coding_test_result = None

    def compose(self) -> ComposeResult:
        yield Header()
        # Build the question bank
        self.questions, self.challenges = build_question_bank(self.config)
        for q in self.questions:
            self._queue.append(("conceptual", q))
        for c in self.challenges:
            self._queue.append(("coding", c))

        if not self._queue:
            yield Label("No questions could be selected. Try a different configuration.",
                        classes="section-title")
            yield Button("Back", id="btn-back-empty")
            yield Footer()
            return

        # Header info bar
        with Horizontal(id="interview-header-bar"):
            yield Label(
                f"Interview: {self.config.level.title()} Level",
                id="interview-level-label",
            )
            yield Label("", id="interview-timer")
            yield Label(
                f"Q 1 / {len(self._queue)}",
                id="interview-progress",
            )

        # Main content area — replaced per question
        with VerticalScroll(id="interview-content"):
            yield Label("Starting...", id="interview-loading")

        yield Footer()

    def on_mount(self):
        if not self._queue:
            return
        self._start_time = time.monotonic()
        # Textual's set_interval for a 1-second tick
        self._timer_handle = self.set_interval(1.0, self._tick)
        self._tick()
        self._show_current()

    def _tick(self):
        elapsed = time.monotonic() - self._start_time
        remaining = max(0, self._total_seconds - elapsed)
        mm = int(remaining // 60)
        ss = int(remaining % 60)
        timer_label = self.query_one("#interview-timer", Label)
        timer_label.update(f"Time: {mm:02d}:{ss:02d}")
        # Color-code under 2 minutes
        if remaining < 120:
            timer_label.add_class("timer-warning")
        if remaining <= 0:
            self._end_interview(ran_out_of_time=True)

    def _show_current(self):
        content = self.query_one("#interview-content", VerticalScroll)
        content.remove_children()

        if self._current_index >= len(self._queue):
            self._end_interview(ran_out_of_time=False)
            return

        # Update progress label
        progress = self.query_one("#interview-progress", Label)
        progress.update(f"Q {self._current_index + 1} / {len(self._queue)}")

        kind, item = self._queue[self._current_index]
        self._question_start_time = time.monotonic()
        self._coding_submitted = False
        self._coding_test_result = None

        if kind == "conceptual":
            content.mount(QuestionCard(
                question=item,
                question_num=self._current_index + 1,
                total=len(self._queue),
            ))
            content.mount(Button("Skip Question", id="btn-skip", variant="default"))
            content.mount(Button("Next", id="btn-next-conceptual",
                                 variant="primary", disabled=True))
        else:  # coding
            challenge: CodingChallenge = item  # type: ignore
            content.mount(Label(
                f"CODING CHALLENGE: {challenge.title}  [{challenge.difficulty}]",
                classes="coding-title",
            ))
            content.mount(Markdown(
                f"{challenge.description}\n\n"
                f"**Signature:** `{challenge.function_signature}`"
            ))
            content.mount(CodeEditor(challenge.skeleton_code, id="coding-editor"))
            run_row = Horizontal(id="coding-actions")
            content.mount(run_row)
            run_row.mount(Button("Run Tests (Ctrl+R)", id="btn-run-tests",
                                 variant="success"))
            run_row.mount(Button("Submit (Ctrl+S)", id="btn-submit-code",
                                 variant="primary"))
            run_row.mount(Button("Skip", id="btn-skip-coding",
                                 variant="default"))
            content.mount(Static("", id="coding-feedback"))

    def on_question_card_answered(self, event: QuestionCard.Answered):
        next_btn = self.query_one("#btn-next-conceptual", Button)
        next_btn.disabled = False
        # Record response
        elapsed = time.monotonic() - self._question_start_time
        self.responses.append(QuestionResponse(
            kind="conceptual",
            topic_id=event.question.topic_id,
            difficulty=event.question.difficulty,
            correct=event.correct,
            time_seconds=elapsed,
            question_id=event.question.id,
        ))

    def on_button_pressed(self, event: Button.Pressed):
        bid = event.button.id or ""
        if bid == "btn-next-conceptual":
            self._advance()
        elif bid == "btn-skip":
            self._record_skip_conceptual()
            self._advance()
        elif bid == "btn-skip-coding":
            self._record_skip_coding()
            self._advance()
        elif bid == "btn-run-tests":
            self.action_run_coding()
        elif bid == "btn-submit-code":
            self.action_submit_coding()
        elif bid == "btn-back-empty":
            self.app.pop_screen()

    def _record_skip_conceptual(self):
        _, item = self._queue[self._current_index]
        q: Question = item  # type: ignore
        # Only record if they haven't already submitted via card
        if len(self.responses) < self._current_index + 1:
            elapsed = time.monotonic() - self._question_start_time
            self.responses.append(QuestionResponse(
                kind="conceptual",
                topic_id=q.topic_id,
                difficulty=q.difficulty,
                correct=False,
                time_seconds=elapsed,
                question_id=q.id,
            ))

    def _record_skip_coding(self):
        _, item = self._queue[self._current_index]
        c: CodingChallenge = item  # type: ignore
        if len(self.responses) < self._current_index + 1:
            elapsed = time.monotonic() - self._question_start_time
            self.responses.append(QuestionResponse(
                kind="coding",
                topic_id=c.topic_id,
                difficulty=c.difficulty,
                correct=False,
                time_seconds=elapsed,
                question_id=c.id,
                tests_passed=0,
                tests_total=0,
                user_code="",
            ))

    def _advance(self):
        self._current_index += 1
        if self._current_index >= len(self._queue):
            self._end_interview(ran_out_of_time=False)
        else:
            self._show_current()

    def action_run_coding(self):
        kind, item = self._queue[self._current_index]
        if kind != "coding":
            return
        challenge: CodingChallenge = item  # type: ignore
        editor = self.query_one("#coding-editor", CodeEditor)
        user_code = editor.text
        result = compile_and_test(user_code, challenge)
        self._coding_test_result = result

        feedback = self.query_one("#coding-feedback", Static)
        if not result.compiled:
            feedback.update(
                f"[red]Compile error:[/red]\n{result.compiler_output[:500]}"
            )
        else:
            passed = sum(1 for t in result.test_results if t.passed)
            total = len(result.test_results)
            color = "green" if result.all_passed else "yellow"
            feedback.update(
                f"[{color}]{passed}/{total} tests passed.[/{color}]"
                f"{' All tests green — ready to submit!' if result.all_passed else ''}"
            )

    def action_submit_coding(self):
        kind, item = self._queue[self._current_index]
        if kind != "coding":
            return
        challenge: CodingChallenge = item  # type: ignore
        editor = self.query_one("#coding-editor", CodeEditor)
        user_code = editor.text

        # Run tests if not already run
        if self._coding_test_result is None:
            self._coding_test_result = compile_and_test(user_code, challenge)

        result = self._coding_test_result
        tests_passed = sum(1 for t in result.test_results if t.passed) if result.compiled else 0
        tests_total = len(result.test_results) if result.compiled else 0
        elapsed = time.monotonic() - self._question_start_time

        self.responses.append(QuestionResponse(
            kind="coding",
            topic_id=challenge.topic_id,
            difficulty=challenge.difficulty,
            correct=result.all_passed if result.compiled else False,
            time_seconds=elapsed,
            question_id=challenge.id,
            tests_passed=tests_passed,
            tests_total=tests_total,
            user_code=user_code,
        ))
        self._advance()

    def action_abandon(self):
        self._end_interview(ran_out_of_time=False)

    def _end_interview(self, ran_out_of_time: bool):
        if self._timer_handle is not None:
            self._timer_handle.stop()
            self._timer_handle = None
        elapsed = time.monotonic() - self._start_time
        result = score_interview(
            self.config, self.responses, elapsed, ran_out_of_time,
        )

        # Persist results and XP
        self._persist_result(result)

        # Push results screen (replace, not push — prevent going back)
        self.app.switch_screen(InterviewResultsScreen(result))

    def _persist_result(self, result: InterviewResult):
        profile = self.app.user_profile
        streak = update_streak(profile)
        profile.total_xp += result.xp_earned
        self.app.db.update_user(profile)

        self.app.db.save_interview_result(
            user_id=profile.id,
            level=result.config.level,
            topic_focus=result.config.topic_focus,
            duration_seconds_used=result.duration_seconds_used,
            duration_seconds_total=result.config.duration_minutes * 60,
            conceptual_correct=result.conceptual_correct,
            conceptual_total=result.conceptual_total,
            coding_passed=result.coding_passed,
            coding_total=result.coding_total,
            overall_score=result.overall_score,
            readiness_score=result.readiness_score,
            grade=result.grade,
            xp_earned=result.xp_earned,
            ran_out_of_time=result.ran_out_of_time,
        )

        # Achievement checks
        from embedded_trainer.models.achievement import ACHIEVEMENTS
        newly_unlocked: list = []

        def try_unlock(key: str):
            if key in ACHIEVEMENTS and self.app.db.unlock_achievement(profile.id, key):
                newly_unlocked.append(ACHIEVEMENTS[key])

        # First interview
        try_unlock("first_interview")
        # 70%+ score = "interview_pass"
        if result.readiness_score >= 70:
            try_unlock("interview_pass")
        # 90%+ = ace
        if result.readiness_score >= 90:
            try_unlock("interview_ace")
        # Senior interview 70%+ = "senior_ready"
        if result.config.level == "senior" and result.readiness_score >= 70:
            try_unlock("senior_ready")

        for ach in newly_unlocked:
            self.notify(format_achievement(ach), timeout=5)


# ---------------------------------------------------------------------------
# 3. RESULTS SCREEN: score + breakdown + feedback
# ---------------------------------------------------------------------------


class InterviewResultsScreen(Screen):
    """Shows the outcome of a completed interview session."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "interview.tcss"

    BINDINGS = [
        ("escape", "go_home", "Home"),
        ("r", "retry", "Retry"),
    ]

    def __init__(self, result: InterviewResult, **kwargs):
        super().__init__(**kwargs)
        self.result = result

    def compose(self) -> ComposeResult:
        yield Header()
        r = self.result

        # Big header with grade
        grade_class = self._grade_class(r.grade)
        with Vertical(id="results-header"):
            yield Label(
                f"Interview Complete — Grade: {r.grade}",
                id="grade-display", classes=grade_class,
            )
            yield Label(verdict_text(r), classes="verdict-text")

        with VerticalScroll(id="results-content"):
            # Scorecard
            yield Label("Scorecard", classes="results-section-title")
            time_mm = int(r.duration_seconds_used // 60)
            time_ss = int(r.duration_seconds_used % 60)
            total_min = r.config.duration_minutes
            stats = (
                f"- **Readiness Score:** {r.readiness_score:.1f}/100\n"
                f"- **Raw Accuracy:** {r.overall_score:.1f}%\n"
                f"- **Conceptual:** {r.conceptual_correct}/{r.conceptual_total} correct\n"
                f"- **Coding:** {r.coding_passed}/{r.coding_total} challenges passed\n"
                f"- **Time Used:** {time_mm:02d}:{time_ss:02d} / {total_min}:00\n"
                f"- **XP Earned:** +{r.xp_earned}"
            )
            if r.ran_out_of_time:
                stats += "\n- **Ran out of time!**"
            yield Markdown(stats)

            # Per-topic breakdown
            if r.topic_performance:
                yield Label("Per-Topic Performance", classes="results-section-title")
                lines = []
                topic_names = {t["id"]: t["name"] for t in load_topics()}
                for tid, (correct, total) in sorted(r.topic_performance.items()):
                    name = topic_names.get(tid, tid.replace("_", " ").title())
                    pct = (correct / total * 100) if total else 0
                    bar = self._render_bar(pct)
                    lines.append(f"- {name}: {bar} {correct}/{total} ({pct:.0f}%)")
                yield Markdown("\n".join(lines))

            # Strengths and weaknesses
            if r.strengths:
                yield Label("Strengths", classes="results-section-title")
                yield Markdown("\n".join(f"- {s}" for s in r.strengths))
            if r.weaknesses:
                yield Label("Areas to Improve", classes="results-section-title")
                yield Markdown("\n".join(f"- {w}" for w in r.weaknesses))

            if not r.strengths and not r.weaknesses:
                yield Label(
                    "Keep practicing to see your strengths and weaknesses emerge.",
                    classes="verdict-text",
                )

            # Action buttons
            with Horizontal(id="results-actions"):
                yield Button("Retry Interview [R]", id="btn-retry", variant="primary")
                yield Button("Back to Home [Esc]", id="btn-home", variant="default")

        yield Footer()

    def _render_bar(self, pct: float, width: int = 10) -> str:
        filled = int(round(pct / 100 * width))
        return "[" + "#" * filled + "-" * (width - filled) + "]"

    def _grade_class(self, grade: str) -> str:
        return {
            "A": "grade-a", "B": "grade-b", "C": "grade-c",
            "D": "grade-d", "F": "grade-f",
        }.get(grade, "grade-c")

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "btn-retry":
            self.action_retry()
        elif event.button.id == "btn-home":
            self.action_go_home()

    def action_retry(self):
        # Pop back to config screen
        self.app.pop_screen()  # this pops the results, the session was already switched out

    def action_go_home(self):
        # Pop until we're at home
        while len(self.app.screen_stack) > 1:
            self.app.pop_screen()
