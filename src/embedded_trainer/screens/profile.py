"""Profile and achievements screen."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.containers import VerticalScroll
from textual.screen import Screen
from textual.widgets import Footer, Header, Label, Static

from embedded_trainer.models.achievement import ACHIEVEMENTS, get_level, xp_for_next_level
from embedded_trainer.widgets.xp_bar import XPBar


class ProfileScreen(Screen):
    CSS_PATH = Path(__file__).parent.parent / "styles" / "profile.tcss"

    BINDINGS = [("escape", "go_back", "Back")]

    def compose(self) -> ComposeResult:
        yield Header()
        profile = self.app.user_profile
        progress = self.app.db.get_user_progress(profile.id)

        with VerticalScroll(id="profile-header"):
            yield Label(f"Player: {profile.name}", classes="section-title")
            yield XPBar(profile.total_xp)
            yield Label(f"Current Streak: {profile.current_streak} days")
            yield Label(f"Longest Streak: {profile.longest_streak} days")
            yield Label(
                f"Questions Answered: {self.app.db.get_total_questions_answered(profile.id)}"
            )
            yield Label(
                f"Challenges Passed: {self.app.db.get_total_challenges_passed(profile.id)}"
            )

            yield Label("Achievements", classes="section-title")
            unlocked = set(progress.achievements)
            for aid, ach in ACHIEVEMENTS.items():
                if aid in unlocked:
                    yield Label(
                        f"  {ach.icon} {ach.name} - {ach.description}",
                        classes="achievement-unlocked",
                    )
                else:
                    yield Label(
                        f"  [ ] {ach.name} - {ach.description}",
                        classes="achievement-locked",
                    )

            yield Label("Topic Progress", classes="section-title")
            from embedded_trainer.core.content_loader import load_topics
            topics = load_topics()
            for topic in topics:
                tid = topic["id"]
                name = topic["name"]
                tp = progress.topics.get(tid)
                if tp:
                    yield Label(
                        f"  {name}: Quiz best {tp.quiz_best_score:.0f}%, "
                        f"{tp.quiz_completed} quizzes, "
                        f"{tp.coding_completed} challenges"
                    )
                else:
                    yield Label(f"  {name}: Not started")

        yield Footer()

    def action_go_back(self):
        self.app.pop_screen()
