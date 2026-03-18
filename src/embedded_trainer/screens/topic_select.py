"""Topic selection screen for quiz and coding modes."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.screen import Screen
from textual.widgets import Button, Footer, Header, Label, ListItem, ListView, Static


class TopicSelectScreen(Screen):
    CSS_PATH = Path(__file__).parent.parent / "styles" / "app.tcss"

    BINDINGS = [
        ("escape", "go_back", "Back"),
    ]

    def __init__(self, mode: str = "quiz", **kwargs):
        super().__init__(**kwargs)
        self.mode = mode

    def compose(self) -> ComposeResult:
        yield Header()
        title = "Select a Topic - Quiz Mode" if self.mode == "quiz" else "Select a Topic - Coding Mode"
        yield Label(title, classes="section-title")

        from embedded_trainer.core.content_loader import load_topics
        topics = load_topics()
        topic_progress = self.app.db.get_topic_progress(self.app.user_profile.id)

        list_view = ListView(id="topic-list")
        for topic in topics:
            tid = topic["id"]
            name = topic["name"]
            difficulty = topic.get("difficulty", "beginner")
            progress = topic_progress.get(tid)

            if self.mode == "quiz":
                count = topic.get("quiz_count", "?")
                completed = progress.quiz_completed if progress else 0
                info = f"  [{difficulty}] {count} questions, {completed} completed"
            else:
                count = topic.get("coding_count", "?")
                completed = progress.coding_completed if progress else 0
                info = f"  [{difficulty}] {count} challenges, {completed} completed"

            list_view.compose_add_child(
                ListItem(Label(f"{name}{info}"), id=f"topic-{tid}")
            )
        yield list_view
        yield Footer()

    def on_list_view_selected(self, event: ListView.Selected):
        item_id = event.item.id
        if not item_id or not item_id.startswith("topic-"):
            return
        topic_id = item_id[len("topic-"):]

        if self.mode == "quiz":
            from embedded_trainer.screens.quiz import QuizScreen
            self.app.push_screen(QuizScreen(topic_id=topic_id))
        else:
            from embedded_trainer.screens.coding import CodingSelectScreen
            self.app.push_screen(CodingSelectScreen(topic_id=topic_id))

    def action_go_back(self):
        self.app.pop_screen()
