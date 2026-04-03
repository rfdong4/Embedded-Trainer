"""Register simulator screen with guided exercises."""

from __future__ import annotations

from pathlib import Path

from textual.app import ComposeResult
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.screen import Screen
from textual.widgets import Button, Footer, Header, Label, ListItem, ListView, Markdown, Static

from embedded_trainer.core.content_loader import load_register_exercises
from embedded_trainer.core.gamification import check_achievements, update_streak
from embedded_trainer.widgets.achievement_toast import format_achievement
from embedded_trainer.widgets.register_display import RegisterDisplay


class RegisterSimSelectScreen(Screen):
    """Select a register simulator exercise set."""

    BINDINGS = [("escape", "go_back", "Back")]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._topics: list[dict] = []

    def compose(self) -> ComposeResult:
        yield Header()
        yield Label("Register Simulator", classes="section-title")

        from embedded_trainer.core.content_loader import load_topics
        topics = load_topics()

        list_view = ListView(id="reg-sim-list")
        for topic in topics:
            exercises = load_register_exercises(topic["id"])
            if exercises:
                self._topics.append(topic)
                list_view.compose_add_child(
                    ListItem(
                        Label(f"{topic['name']}  [{len(exercises)} exercises]"),
                        id=f"regsim-{topic['id']}",
                    )
                )

        if not self._topics:
            yield Label("No register simulator exercises available yet.")
        else:
            yield list_view
        yield Footer()

    def on_list_view_selected(self, event: ListView.Selected):
        item_id = event.item.id
        if not item_id or not item_id.startswith("regsim-"):
            return
        topic_id = item_id[len("regsim-"):]
        self.app.push_screen(RegisterExerciseListScreen(topic_id))

    def action_go_back(self):
        self.app.pop_screen()


class RegisterExerciseListScreen(Screen):
    """List exercises for a specific topic."""

    BINDINGS = [("escape", "go_back", "Back")]

    def __init__(self, topic_id: str, **kwargs):
        super().__init__(**kwargs)
        self.topic_id = topic_id
        self._exercises: list[dict] = []

    def compose(self) -> ComposeResult:
        yield Header()
        yield Label(
            f"Register Exercises: {self.topic_id.replace('_', ' ').title()}",
            classes="section-title",
        )

        self._exercises = load_register_exercises(self.topic_id)
        list_view = ListView(id="exercise-list")
        for ex in self._exercises:
            list_view.compose_add_child(
                ListItem(Label(f"{ex['title']}  [{ex.get('difficulty', 'beginner')}]"))
            )
        yield list_view
        yield Footer()

    def on_list_view_selected(self, event: ListView.Selected):
        idx = list(self.query_one("#exercise-list", ListView).children).index(event.item)
        if 0 <= idx < len(self._exercises):
            self.app.push_screen(RegisterSimScreen(self._exercises[idx]))

    def action_go_back(self):
        self.app.pop_screen()


class RegisterSimScreen(Screen):
    """Interactive register simulator with a guided exercise."""

    CSS_PATH = Path(__file__).parent.parent / "styles" / "register_sim.tcss"

    BINDINGS = [("escape", "go_back", "Back")]

    def __init__(self, exercise: dict, **kwargs):
        super().__init__(**kwargs)
        self.exercise = exercise
        self._step_index = 0
        self._steps = exercise.get("steps", [])
        self._completed_steps = 0

    def compose(self) -> ComposeResult:
        yield Header()

        with Horizontal(id="sim-layout"):
            with Vertical(id="register-panel"):
                yield Label(f"  {self.exercise['title']}", classes="section-title")
                yield RegisterDisplay(
                    register_name=self.exercise.get("register_name", "REG"),
                    address=self.exercise.get("address", "0x0000_0000"),
                    initial_value=self.exercise.get("initial_value", 0),
                    bit_width=self.exercise.get("bit_width", 32),
                    id="reg-display",
                )

            with Vertical(id="exercise-panel"):
                with VerticalScroll(id="instructions"):
                    desc = self.exercise.get("description", "")
                    yield Markdown(desc, id="exercise-desc")
                    if self._steps:
                        step = self._steps[0]
                        yield Label(
                            f"Step {self._step_index + 1}/{len(self._steps)}: {step['instruction']}",
                            id="current-step",
                        )
                    else:
                        yield Label("Free mode — experiment with the register.", id="current-step")
                yield Static("", id="step-feedback")
                with Horizontal(id="sim-actions"):
                    yield Button("Check", id="btn-check", variant="success")
                    yield Button("Reset", id="btn-reset", variant="error")

        yield Footer()

    def on_register_display_expression_applied(self, event: RegisterDisplay.ExpressionApplied):
        """Auto-check after each expression if we have steps."""
        pass

    def on_button_pressed(self, event: Button.Pressed):
        if event.button.id == "btn-check":
            self._check_step()
        elif event.button.id == "btn-reset":
            reg = self.query_one("#reg-display", RegisterDisplay)
            reg.reset(self.exercise.get("initial_value", 0))
            self._step_index = 0
            self._completed_steps = 0
            if self._steps:
                step_label = self.query_one("#current-step", Label)
                step_label.update(
                    f"Step 1/{len(self._steps)}: {self._steps[0]['instruction']}"
                )
            feedback = self.query_one("#step-feedback", Static)
            feedback.update("")

    def _check_step(self):
        if not self._steps or self._step_index >= len(self._steps):
            return

        reg = self.query_one("#reg-display", RegisterDisplay)
        step = self._steps[self._step_index]
        expected = step.get("expected_value", 0)
        feedback = self.query_one("#step-feedback", Static)

        if reg.value == expected:
            self._completed_steps += 1
            self._step_index += 1

            if self._step_index >= len(self._steps):
                feedback.update("[green]All steps completed![/green]")
                self._award_xp()
            else:
                next_step = self._steps[self._step_index]
                feedback.update(f"[green]Correct![/green] {step.get('explanation', '')}")
                step_label = self.query_one("#current-step", Label)
                step_label.update(
                    f"Step {self._step_index + 1}/{len(self._steps)}: "
                    f"{next_step['instruction']}"
                )
        else:
            feedback.update(
                f"[red]Not quite.[/red] Expected 0x{expected:08X}, "
                f"got 0x{reg.value:08X}. "
                f"{step.get('hint', 'Try again.')}"
            )

    def _award_xp(self):
        profile = self.app.user_profile
        streak = update_streak(profile)
        xp = int(self.exercise.get("xp", 20) * (1.0 + 0.1 * min(streak, 7)))
        profile.total_xp += xp
        self.app.db.update_user(profile)

        self.notify(f"+{xp} XP earned!", timeout=3)

        newly_unlocked = check_achievements(self.app.db, profile)
        for ach in newly_unlocked:
            self.notify(format_achievement(ach), timeout=5)

    def action_go_back(self):
        self.app.pop_screen()
