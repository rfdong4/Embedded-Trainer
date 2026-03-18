"""Home dashboard screen."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.containers import Horizontal, Vertical
from textual.screen import Screen
from textual.widgets import Button, Footer, Header, Label, Static

from embedded_trainer.models.achievement import get_level, xp_for_next_level
from embedded_trainer.widgets.xp_bar import XPBar


class HomeScreen(Screen):
    CSS_PATH = Path(__file__).parent.parent / "styles" / "home.tcss"

    BINDINGS = [
        ("q", "quit_app", "Quit"),
        ("1", "quiz_mode", "Quiz Mode"),
        ("2", "coding_mode", "Coding Mode"),
        ("p", "profile", "Profile"),
    ]

    def compose(self) -> ComposeResult:
        yield Header()
        profile = self.app.user_profile

        yield Label(
            f"Welcome back, {profile.name}!",
            id="welcome",
        )

        yield XPBar(profile.total_xp, id="xp-bar")

        with Horizontal(id="stats-bar"):
            yield Label(f"Streak: {profile.current_streak} days")
            level_num, level_name = get_level(profile.total_xp)
            yield Label(f"Level {level_num}: {level_name}")
            yield Label(f"Total XP: {profile.total_xp}")

        with Horizontal(id="mode-buttons"):
            yield Button(
                "Quiz Mode\n[1] Test your knowledge",
                id="btn-quiz",
                variant="primary",
                classes="mode-button",
            )
            yield Button(
                "Coding Mode\n[2] Write C code",
                id="btn-coding",
                variant="success",
                classes="mode-button",
            )
            yield Button(
                "Profile\n[P] Stats & Achievements",
                id="btn-profile",
                variant="default",
                classes="mode-button",
            )

        yield Footer()

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "btn-quiz":
            self.action_quiz_mode()
        elif event.button.id == "btn-coding":
            self.action_coding_mode()
        elif event.button.id == "btn-profile":
            self.action_profile()

    def action_quit_app(self):
        self.app.exit()

    def action_quiz_mode(self):
        from embedded_trainer.screens.topic_select import TopicSelectScreen
        self.app.push_screen(TopicSelectScreen(mode="quiz"))

    def action_coding_mode(self):
        from embedded_trainer.screens.topic_select import TopicSelectScreen
        self.app.push_screen(TopicSelectScreen(mode="coding"))

    def action_profile(self):
        from embedded_trainer.screens.profile import ProfileScreen
        self.app.push_screen(ProfileScreen())
