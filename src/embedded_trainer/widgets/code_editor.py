"""Code editor widget wrapping Textual's TextArea for C code."""

from __future__ import annotations

from textual.widgets import TextArea


class CodeEditor(TextArea):
    """A TextArea configured for C code editing."""

    DEFAULT_CSS = """
    CodeEditor {
        height: 1fr;
    }
    """

    def __init__(self, initial_code: str = "", **kwargs):
        super().__init__(
            initial_code,
            language="c",
            theme="monokai",
            show_line_numbers=True,
            tab_behavior="indent",
            **kwargs,
        )
