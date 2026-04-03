"""ASCII register visualization widget for the Register Simulator."""

from __future__ import annotations

import re

from textual.app import ComposeResult
from textual.containers import Vertical
from textual.message import Message
from textual.reactive import reactive
from textual.widget import Widget
from textual.widgets import Input, Label, Static


class RegisterDisplay(Widget):
    """Displays a 32-bit register as an ASCII bit diagram with interactive input."""

    DEFAULT_CSS = """
    RegisterDisplay {
        height: auto;
        padding: 1;
    }

    RegisterDisplay #reg-diagram {
        height: auto;
        margin-bottom: 1;
    }

    RegisterDisplay #expr-input {
        margin-top: 1;
    }

    RegisterDisplay #history-log {
        height: auto;
        max-height: 10;
        margin-top: 1;
        color: $text-muted;
    }
    """

    value = reactive(0)

    class ExpressionApplied(Message):
        """Sent when an expression is successfully applied."""

        def __init__(self, expression: str, old_value: int, new_value: int):
            super().__init__()
            self.expression = expression
            self.old_value = old_value
            self.new_value = new_value

    def __init__(
        self,
        register_name: str = "REG",
        address: str = "0x0000_0000",
        initial_value: int = 0,
        bit_width: int = 32,
        **kwargs,
    ):
        super().__init__(**kwargs)
        self.register_name = register_name
        self.address = address
        self.value = initial_value
        self.bit_width = bit_width
        self._history: list[str] = []

    def compose(self) -> ComposeResult:
        yield Static(id="reg-diagram")
        yield Input(placeholder="Enter expression: reg |= (1 << 4)", id="expr-input")
        yield Static("", id="history-log")

    def on_mount(self):
        self._update_diagram()

    def watch_value(self, new_value: int):
        self._update_diagram()

    def _update_diagram(self):
        """Render the ASCII register diagram."""
        v = self.value & ((1 << self.bit_width) - 1)
        name = self.register_name
        addr = self.address

        lines = []
        if self.bit_width == 32:
            lines.append(f"\u250c\u2500 {name} ({addr}) " + "\u2500" * max(0, 49 - len(name) - len(addr)) + "\u2510")

            # Upper 16 bits
            bit_nums_hi = " ".join(f"{i:>2}" for i in range(31, 15, -1))
            bit_vals_hi = " ".join(f" {(v >> i) & 1}" for i in range(31, 15, -1))
            lines.append(f"\u2502 {bit_nums_hi} \u2502")
            lines.append(f"\u2502 {bit_vals_hi} \u2502")

            # Lower 16 bits
            bit_nums_lo = " ".join(f"{i:>2}" for i in range(15, -1, -1))
            bit_vals_lo = " ".join(f" {(v >> i) & 1}" for i in range(15, -1, -1))
            lines.append(f"\u2502 {bit_nums_lo} \u2502")
            lines.append(f"\u2502 {bit_vals_lo} \u2502")

            lines.append(f"\u2514" + "\u2500" * 50 + "\u2518")
        else:
            # 8 or 16 bit
            lines.append(f"\u250c\u2500 {name} ({addr}) " + "\u2500" * max(0, 25 - len(name) - len(addr)) + "\u2510")
            bit_nums = " ".join(f"{i:>2}" for i in range(self.bit_width - 1, -1, -1))
            bit_vals = " ".join(f" {(v >> i) & 1}" for i in range(self.bit_width - 1, -1, -1))
            lines.append(f"\u2502 {bit_nums} \u2502")
            lines.append(f"\u2502 {bit_vals} \u2502")
            lines.append(f"\u2514" + "\u2500" * (self.bit_width * 3 + 2) + "\u2518")

        lines.append(f"  Hex: 0x{v:0{self.bit_width // 4}X}  Dec: {v}")

        diagram = self.query_one("#reg-diagram", Static)
        diagram.update("\n".join(lines))

    def on_input_submitted(self, event: Input.Submitted):
        expr = event.value.strip()
        if not expr:
            return

        old_value = self.value
        new_value = self._evaluate_expression(expr, old_value)

        if new_value is not None:
            self.value = new_value
            self._history.append(
                f"  {expr}  // 0x{old_value:08X} -> 0x{new_value:08X}"
            )
            self.post_message(self.ExpressionApplied(expr, old_value, new_value))
        else:
            self._history.append(f"  {expr}  // ERROR: invalid expression")

        # Update history display
        history_widget = self.query_one("#history-log", Static)
        history_widget.update("\n".join(self._history[-8:]))

        # Clear input
        event.input.value = ""

    def _evaluate_expression(self, expr: str, reg: int) -> int | None:
        """Safely evaluate a C-style bitmask expression."""
        try:
            # Normalize: replace 'reg' or register name with current value
            normalized = expr.lower().strip()

            # Handle assignment forms: reg = ..., reg |= ..., reg &= ..., etc.
            reg_pattern = r'\b(reg|register|' + re.escape(self.register_name.lower()) + r')\b'

            if re.search(r'\|=', normalized):
                rhs = re.split(r'\|=', normalized, 1)[1]
                rhs_val = self._eval_c_expr(rhs, reg)
                return reg | rhs_val if rhs_val is not None else None
            elif re.search(r'&=', normalized):
                rhs = re.split(r'&=', normalized, 1)[1]
                rhs_val = self._eval_c_expr(rhs, reg)
                return reg & rhs_val if rhs_val is not None else None
            elif re.search(r'\^=', normalized):
                rhs = re.split(r'\^=', normalized, 1)[1]
                rhs_val = self._eval_c_expr(rhs, reg)
                return reg ^ rhs_val if rhs_val is not None else None
            elif re.search(r'<<=', normalized):
                rhs = re.split(r'<<=', normalized, 1)[1]
                rhs_val = self._eval_c_expr(rhs, reg)
                return (reg << rhs_val) if rhs_val is not None else None
            elif re.search(r'>>=', normalized):
                rhs = re.split(r'>>=', normalized, 1)[1]
                rhs_val = self._eval_c_expr(rhs, reg)
                return (reg >> rhs_val) if rhs_val is not None else None
            elif '=' in normalized:
                rhs = normalized.split('=', 1)[1]
                return self._eval_c_expr(rhs, reg)
            else:
                # Treat as a direct expression
                return self._eval_c_expr(normalized, reg)
        except Exception:
            return None

    def _eval_c_expr(self, expr: str, reg: int) -> int | None:
        """Evaluate a simple C-style expression safely."""
        try:
            expr = expr.strip()
            # Replace C-style hex
            expr = re.sub(r'0x([0-9a-fA-F]+)', r'0x\1', expr)
            # Replace C-style binary
            expr = re.sub(r'0b([01]+)', r'0b\1', expr)
            # Replace ~ (C bitwise NOT) with Python equivalent
            expr = expr.replace('~', ' ~')
            # Replace reg references with actual value
            reg_pattern = r'\b(reg|register|' + re.escape(self.register_name.lower()) + r')\b'
            expr = re.sub(reg_pattern, str(reg), expr)

            # Only allow safe characters
            if not re.match(r'^[\s0-9a-fA-FxXbB\+\-\*\/\%\&\|\^\~\(\)\<\>]+$', expr):
                return None

            result = eval(expr)  # noqa: S307
            return int(result) & ((1 << self.bit_width) - 1)
        except Exception:
            return None

    def reset(self, value: int = 0):
        """Reset the register to a given value."""
        self._history.clear()
        history_widget = self.query_one("#history-log", Static)
        history_widget.update("")
        self.value = value
