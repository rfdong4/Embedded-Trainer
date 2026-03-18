"""Widget for displaying a quiz question with answer options."""

from __future__ import annotations

from textual.app import ComposeResult
from textual.containers import Vertical
from textual.message import Message
from textual.widgets import Button, Label, RadioButton, RadioSet, Static

from embedded_trainer.models.question import Question


class QuestionCard(Static):
    """Displays a question with selectable options and submit button."""

    DEFAULT_CSS = """
    QuestionCard {
        padding: 1 2;
        margin: 1 0;
    }
    QuestionCard .question-text {
        text-style: bold;
        margin-bottom: 1;
    }
    QuestionCard .question-meta {
        color: $text-muted;
        margin-bottom: 1;
    }
    QuestionCard .feedback-correct {
        color: $success;
        text-style: bold;
        margin-top: 1;
    }
    QuestionCard .feedback-wrong {
        color: $error;
        text-style: bold;
        margin-top: 1;
    }
    QuestionCard .explanation {
        color: $text-muted;
        margin-top: 0;
        padding: 0 2;
    }
    """

    class Answered(Message):
        """Sent when the user submits an answer."""
        def __init__(self, question: Question, selected: str, correct: bool):
            super().__init__()
            self.question = question
            self.selected = selected
            self.correct = correct

    def __init__(self, question: Question, question_num: int, total: int, **kwargs):
        super().__init__(**kwargs)
        self.question = question
        self.question_num = question_num
        self.total = total
        self._selected: str | None = None
        self._submitted = False

    def compose(self) -> ComposeResult:
        q = self.question
        yield Label(
            f"Question {self.question_num}/{self.total}",
            classes="question-meta",
        )
        yield Label(q.text, classes="question-text")

        # Extract option keys (A, B, C, D or True, False)
        radio_set = RadioSet(id="options")
        for option in q.options:
            radio_set.compose_add_child(RadioButton(option))
        yield radio_set
        yield Button("Submit Answer", id="submit-answer", variant="primary")

    def on_radio_set_changed(self, event: RadioSet.Changed):
        if event.pressed is not None:
            label = str(event.pressed.label)
            # Extract the option key (first character before the parenthesis)
            if ")" in label:
                self._selected = label.split(")")[0].strip()
            else:
                self._selected = label.strip()

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "submit-answer" and not self._submitted and self._selected:
            self._submitted = True
            correct = self._selected.upper() == self.question.correct_answer.upper()

            # Show feedback
            event.button.disabled = True
            if correct:
                self.mount(Label(
                    f"Correct! +{self.question.xp_value} XP",
                    classes="feedback-correct",
                ))
            else:
                self.mount(Label(
                    f"Incorrect. The answer was {self.question.correct_answer}.",
                    classes="feedback-wrong",
                ))
            if self.question.explanation:
                self.mount(Label(self.question.explanation, classes="explanation"))

            self.post_message(self.Answered(
                self.question, self._selected, correct
            ))
