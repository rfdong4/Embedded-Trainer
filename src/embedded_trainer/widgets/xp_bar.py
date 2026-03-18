"""XP progress bar widget showing level and progress to next level."""

from __future__ import annotations

from textual.app import ComposeResult
from textual.containers import Horizontal
from textual.widgets import Label, ProgressBar, Static

from embedded_trainer.models.achievement import get_level, xp_for_next_level


class XPBar(Static):
    """Displays current level, XP bar, and progress to next level."""

    DEFAULT_CSS = """
    XPBar {
        height: 3;
        padding: 0 1;
    }
    XPBar .xp-level {
        width: auto;
        color: $accent;
        text-style: bold;
        padding-right: 1;
    }
    XPBar .xp-text {
        width: auto;
        color: $text-muted;
        padding-left: 1;
    }
    XPBar ProgressBar {
        width: 1fr;
    }
    """

    def __init__(self, total_xp: int = 0, **kwargs):
        super().__init__(**kwargs)
        self._total_xp = total_xp

    def compose(self) -> ComposeResult:
        level_num, level_name = get_level(self._total_xp)
        progress, needed = xp_for_next_level(self._total_xp)

        yield Label(f"Lv.{level_num} {level_name}", classes="xp-level")
        with Horizontal():
            bar = ProgressBar(total=max(needed, 1), show_eta=False, show_percentage=True)
            bar.advance(progress)
            yield bar
            if needed > 0:
                yield Label(f" {progress}/{needed} XP", classes="xp-text")
            else:
                yield Label(" MAX LEVEL", classes="xp-text")

    def update_xp(self, total_xp: int):
        self._total_xp = total_xp
        self.remove_children()
        self.mount_all(list(self.compose()))
