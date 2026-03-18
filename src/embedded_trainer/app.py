"""Main Embedded Trainer application."""

from __future__ import annotations

import shutil
from pathlib import Path

from textual.app import App

from embedded_trainer.models.user import UserProfile
from embedded_trainer.storage.database import Database


class EmbeddedTrainerApp(App):
    """A gamified terminal app for learning embedded systems."""

    TITLE = "Embedded Trainer"
    SUB_TITLE = "Learn Embedded Systems"
    CSS_PATH = Path(__file__).parent / "styles" / "app.tcss"

    BINDINGS = [
        ("d", "toggle_dark", "Toggle Dark Mode"),
    ]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.db = Database()
        self.user_profile: UserProfile = self.db.get_or_create_user("Player")

    def on_mount(self):
        # Check for gcc
        if not shutil.which("gcc"):
            self.notify(
                "Warning: gcc not found. Coding challenges require gcc to compile C code.",
                severity="warning",
                timeout=10,
            )

        from embedded_trainer.screens.home import HomeScreen
        self.push_screen(HomeScreen())

    def on_button_pressed_btn_back(self):
        self.pop_screen()
