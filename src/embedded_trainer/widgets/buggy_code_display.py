"""Read-only code display widget for Bug Hunt mode."""

from __future__ import annotations

from textual.widgets import TextArea


class BuggyCodeDisplay(TextArea):
    """A read-only TextArea configured for displaying buggy C code."""

    DEFAULT_CSS = """
    BuggyCodeDisplay {
        height: auto;
        max-height: 20;
    }
    """

    def __init__(self, code: str = "", **kwargs):
        super().__init__(
            code,
            language="c",
            theme="monokai",
            show_line_numbers=True,
            read_only=True,
            **kwargs,
        )
