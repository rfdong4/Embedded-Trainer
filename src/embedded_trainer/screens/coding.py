"""Coding challenge screens."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.screen import Screen
from textual.widgets import Button, Footer, Header, Label, ListItem, ListView, Markdown, Static

from embedded_trainer.core.code_runner import compile_and_test
from embedded_trainer.core.content_loader import load_coding_challenges
from embedded_trainer.core.gamification import calculate_coding_xp, check_achievements, update_streak
from embedded_trainer.models.question import CodingChallenge
from embedded_trainer.widgets.achievement_toast import format_achievement
from embedded_trainer.widgets.code_editor import CodeEditor
from embedded_trainer.widgets.test_results import TestResultsPanel


class CodingSelectScreen(Screen):
    """Select a coding challenge from a topic."""

    BINDINGS = [("escape", "go_back", "Back")]

    def __init__(self, topic_id: str, **kwargs):
        super().__init__(**kwargs)
        self.topic_id = topic_id
        self._challenges: list[CodingChallenge] = []

    def compose(self) -> ComposeResult:
        yield Header()
        yield Label(
            f"Coding Challenges: {self.topic_id.replace('_', ' ').title()}",
            classes="section-title",
        )

        self._challenges = load_coding_challenges(self.topic_id)
        if not self._challenges:
            yield Label("No coding challenges available for this topic yet.")
            yield Footer()
            return

        yield ListView(id="challenge-list")
        yield Footer()

    def on_mount(self):
        self._refresh_list()

    def on_screen_resume(self):
        """Refresh the challenge list when returning from a challenge."""
        self._refresh_list()

    def _refresh_list(self):
        """Update challenge labels in-place with current completion status."""
        if not self._challenges:
            return
        completed = self.app.db.get_completed_challenges(self.app.user_profile.id)
        list_view = self.query_one("#challenge-list", ListView)

        # First mount: populate the list
        if not list_view.children:
            for ch in self._challenges:
                check = "✓ " if ch.id in completed else "  "
                list_view.append(
                    ListItem(Label(f"{check}{ch.title}  [{ch.difficulty}] +{ch.xp_value} XP"))
                )
        else:
            # Update existing labels in-place (no clear/re-mount needed)
            for i, ch in enumerate(self._challenges):
                if i < len(list_view.children):
                    check = "✓ " if ch.id in completed else "  "
                    label = list_view.children[i].query_one(Label)
                    label.update(f"{check}{ch.title}  [{ch.difficulty}] +{ch.xp_value} XP")

    def on_list_view_selected(self, event: ListView.Selected):
        idx = list(self.query_one("#challenge-list", ListView).children).index(event.item)
        if idx < 0 or idx >= len(self._challenges):
            return
        challenge_id = self._challenges[idx].id
        self.app.push_screen(CodingScreen(
            topic_id=self.topic_id, challenge_id=challenge_id
        ))

    def action_go_back(self):
        self.app.pop_screen()


class CodingScreen(Screen):
    """The main coding challenge screen with editor and test results."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "coding.tcss"

    BINDINGS = [
        ("escape", "go_back", "Back"),
        ("ctrl+r", "run_tests", "Run Tests"),
    ]

    def __init__(self, topic_id: str, challenge_id: str, **kwargs):
        super().__init__(**kwargs)
        self.topic_id = topic_id
        self.challenge_id = challenge_id
        self.challenge: CodingChallenge | None = None
        self._hint_index = 0

    def compose(self) -> ComposeResult:
        from embedded_trainer.core.content_loader import load_coding_challenge
        self.challenge = load_coding_challenge(self.topic_id, self.challenge_id)
        if not self.challenge:
            yield Label("Challenge not found.")
            return

        yield Header()

        with Horizontal():
            with Vertical(id="code-panel"):
                yield Label(f"  {self.challenge.title}", classes="section-title")
                yield CodeEditor(self.challenge.skeleton_code, id="editor")
                with Horizontal(id="code-actions"):
                    yield Button("Run Tests [Ctrl+R]", id="btn-run", variant="success")
                    yield Button("Hint", id="btn-hint", variant="warning")
                    yield Button("Solution", id="btn-solution", variant="default")
                    yield Button("Reset", id="btn-reset", variant="error")

            with Vertical(id="info-panel"):
                with VerticalScroll(id="challenge-description"):
                    desc = self.challenge.description
                    sig = self.challenge.function_signature
                    yield Markdown(
                        f"## Description\n\n{desc}\n\n"
                        f"**Signature:** `{sig}`\n\n"
                        f"**Difficulty:** {self.challenge.difficulty}\n\n"
                        f"**XP:** {self.challenge.xp_value}"
                    )
                with VerticalScroll(id="hints-panel"):
                    hint_count = len(self.challenge.hints) if self.challenge.hints else 0
                    yield Label(
                        f"💡 Hints (0/{hint_count} revealed) — click Hint to reveal",
                        id="hints-header",
                    )
                yield TestResultsPanel(id="results-panel")

        yield Footer()

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "btn-run":
            self.action_run_tests()
        elif event.button.id == "btn-hint":
            self._show_hint()
        elif event.button.id == "btn-solution":
            self._show_solution()
        elif event.button.id == "btn-reset":
            self._reset_code()
        elif event.button.id == "btn-back":
            self.app.pop_screen()

    def action_run_tests(self):
        if not self.challenge:
            return
        editor = self.query_one("#editor", CodeEditor)
        user_code = editor.text
        result = compile_and_test(user_code, self.challenge)

        results_panel = self.query_one("#results-panel", TestResultsPanel)
        results_panel.show_result(result)

        if result.all_passed:
            self._award_xp(user_code)

    def _award_xp(self, user_code: str):
        profile = self.app.user_profile
        streak = update_streak(profile)
        attempts = self.app.db.get_coding_attempts(profile.id, self.challenge_id)
        first_attempt = attempts == 0

        xp = calculate_coding_xp(
            self.challenge.xp_value, streak, first_attempt
        )
        profile.total_xp += xp
        self.app.db.update_user(profile)
        self.app.db.save_coding_result(
            profile.id, self.challenge_id, self.topic_id,
            passed=True, xp_earned=xp, user_code=user_code,
        )

        self.notify(f"+{xp} XP earned!", timeout=3)

        if first_attempt:
            from embedded_trainer.models.achievement import ACHIEVEMENTS
            if self.app.db.unlock_achievement(profile.id, "speed_demon"):
                self.notify(format_achievement(ACHIEVEMENTS["speed_demon"]), timeout=5)

        newly_unlocked = check_achievements(self.app.db, profile)
        for ach in newly_unlocked:
            self.notify(format_achievement(ach), timeout=5)

    def _show_hint(self):
        if not self.challenge or not self.challenge.hints:
            self.notify("No hints available.", timeout=2)
            return
        if self._hint_index < len(self.challenge.hints):
            hint = self.challenge.hints[self._hint_index]
            self._hint_index += 1
            total = len(self.challenge.hints)

            # Update the header with count
            header = self.query_one("#hints-header", Label)
            header.update(f"💡 Hints ({self._hint_index}/{total} revealed)")

            # Append the new hint to the persistent panel
            hints_panel = self.query_one("#hints-panel", VerticalScroll)
            hints_panel.mount(
                Label(f"  {self._hint_index}. {hint}", classes="hint-text")
            )
            hints_panel.scroll_end(animate=False)
        else:
            self.notify("No more hints available.", timeout=2)

    def _show_solution(self):
        if not self.challenge:
            return
        if not self.challenge.solution:
            self.notify("No solution available for this challenge.", timeout=2)
            return

        desc_panel = self.query_one("#challenge-description", VerticalScroll)
        desc_panel.remove_children()

        solution_md = (
            f"## Solution\n\n"
            f"```c\n{self.challenge.solution}\n```\n\n"
        )
        if self.challenge.solution_explanation:
            solution_md += f"## Explanation\n\n{self.challenge.solution_explanation}"

        desc_panel.mount(Markdown(solution_md))
        desc_panel.scroll_home(animate=False)

    def _reset_code(self):
        if not self.challenge:
            return
        editor = self.query_one("#editor", CodeEditor)
        editor.clear()
        editor.insert(self.challenge.skeleton_code)

    def action_go_back(self):
        self.app.pop_screen()
