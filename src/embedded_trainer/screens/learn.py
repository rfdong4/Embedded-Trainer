"""Learn mode screens: article list and article reader."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.screen import Screen
from textual.widgets import Footer, Header, Label, ListItem, ListView, Markdown, Static


class ArticleListScreen(Screen):
    """List of articles for a topic."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "learn.tcss"

    BINDINGS = [
        ("escape", "go_back", "Back"),
    ]

    def __init__(self, topic_id: str, topic_name: str, **kwargs):
        super().__init__(**kwargs)
        self.topic_id = topic_id
        self.topic_name = topic_name

    def compose(self) -> ComposeResult:
        yield Header()
        yield Label(f"Learn: {self.topic_name}", classes="section-title")

        from embedded_trainer.core.content_loader import load_articles
        self.articles = load_articles(self.topic_id)

        if not self.articles:
            yield Static("No articles available for this topic yet.", classes="empty-msg")
        else:
            list_view = ListView(id="article-list")
            for article in self.articles:
                minutes = article.get("estimated_minutes", "?")
                label = f"{article['title']}  (~{minutes} min)"
                list_view.compose_add_child(
                    ListItem(Label(label), id=f"article-{article['id']}")
                )
            yield list_view

        yield Footer()

    def on_list_view_selected(self, event: ListView.Selected):
        item_id = event.item.id
        if not item_id or not item_id.startswith("article-"):
            return
        article_id = item_id[len("article-"):]
        article = next((a for a in self.articles if a["id"] == article_id), None)
        if article:
            self.app.push_screen(ArticleScreen(article=article))

    def action_go_back(self):
        self.app.pop_screen()


class ArticleScreen(Screen):
    """Displays a single article."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "learn.tcss"

    BINDINGS = [
        ("escape", "go_back", "Back"),
    ]

    def __init__(self, article: dict, **kwargs):
        super().__init__(**kwargs)
        self.article = article

    def compose(self) -> ComposeResult:
        yield Header()
        minutes = self.article.get("estimated_minutes", "?")
        yield Label(
            f"{self.article['title']}  |  ~{minutes} min read",
            id="article-header",
        )
        yield Markdown(self.article.get("content", ""), id="article-body")
        yield Footer()

    def action_go_back(self):
        self.app.pop_screen()
