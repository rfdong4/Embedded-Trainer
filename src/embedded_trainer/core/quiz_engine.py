"""Quiz session logic: load questions, check answers, compute scores."""

from __future__ import annotations

import random

from embedded_trainer.core.content_loader import load_quiz_questions
from embedded_trainer.models.question import Question


def get_quiz_questions(
    topic_id: str, count: int = 10, shuffle: bool = True,
) -> list[Question]:
    """Load and optionally shuffle questions for a quiz session."""
    questions = load_quiz_questions(topic_id)
    if shuffle:
        random.shuffle(questions)
    return questions[:count]


def get_adaptive_questions(
    topic_id: str,
    history: dict[str, list[bool]],
    count: int = 10,
) -> list[Question]:
    """Select questions weighted by past performance.

    Questions the user has never seen or answered incorrectly get higher weight.
    """
    questions = load_quiz_questions(topic_id)
    if not questions:
        return []

    weights = []
    for q in questions:
        past = history.get(q.id, [])
        if not past:
            # Never attempted — high weight
            weights.append(3.0)
        else:
            correct_rate = sum(past) / len(past)
            # Lower correct rate → higher weight (1.0 to 3.0)
            weights.append(3.0 - 2.0 * correct_rate)

    # Weighted random sample without replacement
    selected: list[Question] = []
    available = list(range(len(questions)))
    available_weights = list(weights)

    for _ in range(min(count, len(questions))):
        if not available:
            break
        chosen = random.choices(available, weights=available_weights, k=1)[0]
        idx = available.index(chosen)
        selected.append(questions[chosen])
        available.pop(idx)
        available_weights.pop(idx)

    random.shuffle(selected)
    return selected


def check_answer(question: Question, user_answer: str) -> bool:
    """Check if the user's answer matches the correct answer."""
    return user_answer.strip().upper() == question.correct_answer.strip().upper()


def score_quiz(
    results: list[tuple[Question, bool]],
    streak: int = 0,
) -> tuple[int, int, int]:
    """Score a completed quiz.

    Returns (total_xp, correct_count, total_count).
    """
    total_xp = 0
    correct = 0
    total = len(results)

    for question, is_correct in results:
        if is_correct:
            correct += 1
            total_xp += question.xp_value

    # Perfect score bonus: +50%
    if total > 0 and correct == total:
        total_xp = int(total_xp * 1.5)

    # Streak multiplier
    multiplier = 1.0 + 0.1 * min(streak, 7)
    total_xp = int(total_xp * multiplier)

    return total_xp, correct, total
