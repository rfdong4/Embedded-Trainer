"""Widget for displaying C code compilation and test results."""

from __future__ import annotations

from textual.app import ComposeResult
from textual.widgets import Label, Static

from embedded_trainer.models.question import CodeRunResult


class TestResultsPanel(Static):
    """Displays compiler output and test case pass/fail results."""

    DEFAULT_CSS = """
    TestResultsPanel {
        height: auto;
        padding: 1;
    }
    TestResultsPanel .compile-error {
        color: $error;
    }
    TestResultsPanel .test-pass {
        color: $success;
    }
    TestResultsPanel .test-fail {
        color: $error;
    }
    TestResultsPanel .summary {
        text-style: bold;
        margin-top: 1;
    }
    """

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._result: CodeRunResult | None = None

    def compose(self) -> ComposeResult:
        yield Label("Run your code to see results here.", classes="placeholder")

    def show_result(self, result: CodeRunResult):
        self._result = result
        self.remove_children()

        if not result.compiled:
            self.mount(Label("Compilation Failed:", classes="compile-error"))
            for line in result.compiler_output.strip().splitlines():
                self.mount(Label(f"  {line}", classes="compile-error"))
            return

        if result.compiler_output.strip():
            self.mount(Label("Compiler Warnings:"))
            for line in result.compiler_output.strip().splitlines():
                self.mount(Label(f"  {line}"))

        passed = 0
        total = len(result.test_results)
        for tr in result.test_results:
            if tr.passed:
                passed += 1
                self.mount(Label(f"  [PASS] Test {tr.test_number}", classes="test-pass"))
            else:
                msg = f"  [FAIL] Test {tr.test_number}"
                if tr.expected and tr.actual:
                    msg += f" (expected {tr.expected}, got {tr.actual})"
                self.mount(Label(msg, classes="test-fail"))

        if result.all_passed:
            self.mount(Label(
                f"All {total} tests passed! ({result.execution_time_ms:.0f}ms)",
                classes="summary test-pass",
            ))
        else:
            self.mount(Label(
                f"{passed}/{total} tests passed",
                classes="summary test-fail",
            ))
