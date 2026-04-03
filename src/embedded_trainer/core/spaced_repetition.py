"""SM-2 spaced repetition algorithm for quiz question review scheduling."""

from __future__ import annotations

from datetime import date, timedelta

from embedded_trainer.core.content_loader import load_quiz_questions
from embedded_trainer.models.question import Question
from embedded_trainer.storage.database import Database


def sm2_update(
    quality: int,
    ease_factor: float = 2.5,
    interval: int = 1,
    repetitions: int = 0,
) -> tuple[float, int, int]:
    """Apply the SM-2 algorithm.

    Args:
        quality: Grade from 0-5 (0-2 = fail, 3-5 = pass).
        ease_factor: Current ease factor (minimum 1.3).
        interval: Current interval in days.
        repetitions: Number of consecutive correct reviews.

    Returns:
        (new_ease_factor, new_interval, new_repetitions)
    """
    if quality >= 3:
        if repetitions == 0:
            new_interval = 1
        elif repetitions == 1:
            new_interval = 6
        else:
            new_interval = int(interval * ease_factor)
        new_repetitions = repetitions + 1
    else:
        new_interval = 1
        new_repetitions = 0

    new_ef = ease_factor + (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02))
    new_ef = max(1.3, new_ef)

    return new_ef, new_interval, new_repetitions


def schedule_review(
    db: Database,
    user_id: int,
    question_id: str,
    topic_id: str,
    correct: bool,
):
    """Update the spaced repetition schedule for a question after answering."""
    state = db.get_sr_state(user_id, question_id)
    quality = 4 if correct else 1

    if state:
        ef = state["ease_factor"]
        interval = state["interval_days"]
        reps = state["repetitions"]
    else:
        ef = 2.5
        interval = 1
        reps = 0

    new_ef, new_interval, new_reps = sm2_update(quality, ef, interval, reps)
    next_review = (date.today() + timedelta(days=new_interval)).isoformat()

    db.upsert_sr_state(
        user_id, question_id, topic_id,
        new_ef, new_interval, new_reps, next_review,
    )


def get_due_questions(db: Database, user_id: int) -> list[Question]:
    """Get all questions that are due for review today."""
    due_items = db.get_due_reviews(user_id)
    questions = []

    for item in due_items:
        topic_questions = load_quiz_questions(item["topic_id"])
        for q in topic_questions:
            if q.id == item["question_id"]:
                questions.append(q)
                break

    return questions
