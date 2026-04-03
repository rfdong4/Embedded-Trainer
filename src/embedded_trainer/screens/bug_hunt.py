"""Bug Hunt mode screens: find bugs in embedded C code."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.screen import Screen
from textual.widgets import Button, Footer, Header, Label, ListItem, ListView, RadioButton, RadioSet, Static

from embedded_trainer.core.content_loader import load_bug_hunt_challenges
from embedded_trainer.core.gamification import calculate_coding_xp, check_achievements, update_streak
from embedded_trainer.models.question import BugHuntChallenge
from embedded_trainer.widgets.achievement_toast import format_achievement
from embedded_trainer.widgets.buggy_code_display import BuggyCodeDisplay


class BugHuntSelectScreen(Screen):
    """Select a bug hunt challenge from a topic."""

    BINDINGS = [("escape", "go_back", "Back")]

    def __init__(self, topic_id: str, **kwargs):
        super().__init__(**kwargs)
        self.topic_id = topic_id
        self._challenges: list[BugHuntChallenge] = []

    def compose(self) -> ComposeResult:
        yield Header()
        yield Label(
            f"Bug Hunt: {self.topic_id.replace('_', ' ').title()}",
            classes="section-title",
        )

        self._challenges = load_bug_hunt_challenges(self.topic_id)
        if not self._challenges:
            yield Label("No bug hunt challenges available for this topic yet.")
            yield Footer()
            return

        list_view = ListView(id="bug-hunt-list")
        completed = self.app.db.get_completed_bug_hunts(self.app.user_profile.id)
        for ch in self._challenges:
            check = "\u2713 " if ch.id in completed else "  "
            list_view.compose_add_child(
                ListItem(Label(f"{check}{ch.title}  [{ch.difficulty}] +{ch.xp_value} XP"))
            )
        yield list_view
        yield Footer()

    def on_list_view_selected(self, event: ListView.Selected):
        idx = list(self.query_one("#bug-hunt-list", ListView).children).index(event.item)
        if 0 <= idx < len(self._challenges):
            self.app.push_screen(BugHuntScreen(self._challenges[idx]))

    def action_go_back(self):
        self.app.pop_screen()


class BugHuntScreen(Screen):
    """Display buggy code and let the user identify the bug."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "bug_hunt.tcss"

    BINDINGS = [("escape", "go_back", "Back")]

    def __init__(self, challenge: BugHuntChallenge, **kwargs):
        super().__init__(**kwargs)
        self.challenge = challenge
        self._answered = False

    def compose(self) -> ComposeResult:
        yield Header()

        with Horizontal(id="bug-hunt-layout"):
            with Vertical(id="code-panel"):
                yield Label(f"  {self.challenge.title}", classes="section-title")
                yield BuggyCodeDisplay(self.challenge.buggy_code, id="buggy-code")

            with Vertical(id="answer-panel"):
                yield Label("What's the bug?", classes="section-title")
                with RadioSet(id="options"):
                    for option in self.challenge.options:
                        yield RadioButton(option)
                yield Button("Submit Answer", id="btn-submit", variant="success")
                with VerticalScroll(id="feedback-panel"):
                    yield Static("", id="feedback-text")

        yield Footer()

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "btn-submit" and not self._answered:
            self._check_answer()
        elif event.button.id == "btn-back":
            self.app.pop_screen()

    def _check_answer(self):
        radio_set = self.query_one("#options", RadioSet)
        if radio_set.pressed_index < 0:
            self.notify("Please select an answer.", timeout=2)
            return

        self._answered = True
        selected_letter = chr(ord("A") + radio_set.pressed_index)
        correct = selected_letter == self.challenge.correct_answer.strip().upper()

        feedback = self.query_one("#feedback-text", Static)

        if correct:
            xp = self._award_xp()
            feedback.update(
                f"[green]Correct![/green] +{xp} XP\n\n"
                f"{self.challenge.explanation}"
            )
        else:
            feedback.update(
                f"[red]Incorrect.[/red] The correct answer was "
                f"{self.challenge.correct_answer}.\n\n"
                f"{self.challenge.explanation}"
            )
            self.app.db.save_bug_hunt_result(
                self.app.user_profile.id, self.challenge.id,
                self.challenge.topic_id, False, 0,
            )

        submit_btn = self.query_one("#btn-submit", Button)
        submit_btn.disabled = True

    def _award_xp(self) -> int:
        profile = self.app.user_profile
        streak = update_streak(profile)
        xp = calculate_coding_xp(self.challenge.xp_value, streak, True)

        profile.total_xp += xp
        self.app.db.update_user(profile)
        self.app.db.save_bug_hunt_result(
            profile.id, self.challenge.id,
            self.challenge.topic_id, True, xp,
        )

        newly_unlocked = check_achievements(self.app.db, profile)
        for ach in newly_unlocked:
            self.notify(format_achievement(ach), timeout=5)

        return xp

    def action_go_back(self):
        self.app.pop_screen()
