"""Spaced repetition review screen."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.containers import Vertical, VerticalScroll
from textual.screen import Screen
from textual.widgets import Button, Footer, Header, Label, Static

from embedded_trainer.core.gamification import check_achievements, update_streak
from embedded_trainer.core.quiz_engine import score_quiz
from embedded_trainer.core.spaced_repetition import get_due_questions, schedule_review
from embedded_trainer.models.question import Question
from embedded_trainer.widgets.achievement_toast import format_achievement
from embedded_trainer.widgets.question_card import QuestionCard


class ReviewScreen(Screen):
    """Review due questions from spaced repetition across all topics."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "quiz.tcss"

    BINDINGS = [
        ("escape", "go_back", "Back"),
    ]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.questions: list[Question] = []
        self.results: list[tuple[Question, bool]] = []
        self.current_index = 0
        self._finished = False

    def compose(self) -> ComposeResult:
        yield Header()

        self.questions = get_due_questions(
            self.app.db, self.app.user_profile.id
        )

        if not self.questions:
            yield Label("No questions due for review!", classes="section-title")
            yield Label("Complete some quizzes first, then come back later.")
            yield Footer()
            return

        yield Label(
            f"Review: {len(self.questions)} questions due",
            id="quiz-header",
        )

        with VerticalScroll(id="quiz-content"):
            yield QuestionCard(
                self.questions[0],
                question_num=1,
                total=len(self.questions),
            )

        with Vertical(id="quiz-nav"):
            yield Button("Next Question", id="btn-next", variant="primary", disabled=True)

        yield Footer()

    def on_question_card_answered(self, event: QuestionCard.Answered):
        self.results.append((event.question, event.correct))
        # Update spaced repetition schedule
        schedule_review(
            self.app.db, self.app.user_profile.id,
            event.question.id, event.question.topic_id, event.correct,
        )
        next_btn = self.query_one("#btn-next", Button)
        next_btn.disabled = False
        if self.current_index >= len(self.questions) - 1:
            next_btn.label = "Finish Review"

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "btn-next":
            self.current_index += 1
            if self.current_index < len(self.questions):
                self._show_next_question()
            elif not self._finished:
                self._finish_review()
        elif event.button.id == "btn-back":
            self.app.pop_screen()

    def _show_next_question(self):
        content = self.query_one("#quiz-content", VerticalScroll)
        content.remove_children()
        content.mount(QuestionCard(
            self.questions[self.current_index],
            question_num=self.current_index + 1,
            total=len(self.questions),
        ))
        next_btn = self.query_one("#btn-next", Button)
        next_btn.disabled = True

    def _finish_review(self):
        self._finished = True
        profile = self.app.user_profile
        streak = update_streak(profile)
        total_xp, correct, total = score_quiz(self.results, streak)

        profile.total_xp += total_xp
        self.app.db.update_user(profile)

        newly_unlocked = check_achievements(self.app.db, profile)

        content = self.query_one("#quiz-content", VerticalScroll)
        content.remove_children()

        score_pct = (correct / total * 100) if total > 0 else 0
        content.mount(Label("Review Complete!", classes="section-title"))
        content.mount(Label(f"Score: {correct}/{total} ({score_pct:.0f}%)"))
        content.mount(Label(f"XP Earned: +{total_xp}"))

        for ach in newly_unlocked:
            self.notify(format_achievement(ach), timeout=5)

        nav = self.query_one("#quiz-nav", Vertical)
        nav.remove_children()
        nav.mount(Button("Back to Home", id="btn-back", variant="primary"))

    def action_go_back(self):
        self.app.pop_screen()
