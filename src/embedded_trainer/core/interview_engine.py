"""Core logic for the interview simulation: question selection and scoring."""

from __future__ import annotations

import random
from dataclasses import dataclass

from embedded_trainer.core.content_loader import (
    load_coding_challenges,
    load_quiz_questions,
    load_topics,
)
from embedded_trainer.models.interview import (
    InterviewConfig,
    InterviewResult,
    QuestionResponse,
)
from embedded_trainer.models.question import CodingChallenge, Question


def build_question_bank(config: InterviewConfig) -> tuple[list[Question], list[CodingChallenge]]:
    """Select the conceptual questions and coding challenges for an interview.

    The selection is random but filtered by:
    - difficulty matching the level's allowed difficulties
    - topic_focus ("mixed" = all topics, else only that topic_id)
    """
    # Decide which topics to pull from
    if config.topic_focus == "mixed":
        topic_ids = [t["id"] for t in load_topics()]
    else:
        topic_ids = [config.topic_focus]

    # Gather all candidate questions
    all_questions: list[Question] = []
    all_challenges: list[CodingChallenge] = []
    for tid in topic_ids:
        all_questions.extend(load_quiz_questions(tid))
        all_challenges.extend(load_coding_challenges(tid))

    # Filter by difficulty
    allowed = set(config.difficulty_mix)
    questions = [q for q in all_questions if q.difficulty in allowed]
    challenges = [c for c in all_challenges if c.difficulty in allowed]

    # Fallback: if filtering left us short, widen the net
    if len(questions) < config.num_conceptual:
        questions = all_questions
    if len(challenges) < config.num_coding:
        challenges = all_challenges

    # Shuffle & pick — weight toward topic diversity by shuffling then dedup per topic
    random.shuffle(questions)
    random.shuffle(challenges)

    picked_questions = _pick_diverse(questions, config.num_conceptual)
    picked_challenges = _pick_diverse(challenges, config.num_coding)

    return picked_questions, picked_challenges


def _pick_diverse(items: list, count: int) -> list:
    """Prefer picking items from different topics before repeating."""
    if count <= 0 or not items:
        return []
    picked: list = []
    seen_topics: set[str] = set()
    # First pass: one per topic
    for item in items:
        if len(picked) >= count:
            break
        if item.topic_id not in seen_topics:
            picked.append(item)
            seen_topics.add(item.topic_id)
    # Second pass: fill remaining slots with anything
    i = 0
    while len(picked) < count and i < len(items):
        if items[i] not in picked:
            picked.append(items[i])
        i += 1
    return picked[:count]


def compute_grade(score_pct: float) -> str:
    """Letter grade from percentage."""
    if score_pct >= 90:
        return "A"
    if score_pct >= 80:
        return "B"
    if score_pct >= 70:
        return "C"
    if score_pct >= 60:
        return "D"
    return "F"


def score_interview(
    config: InterviewConfig,
    responses: list[QuestionResponse],
    duration_seconds_used: float,
    ran_out_of_time: bool,
) -> InterviewResult:
    """Compute final results from a list of question responses.

    Readiness score formula (0-100):
        - 70% weight: weighted accuracy (coding weighted 2x conceptual)
        - 20% weight: time efficiency (finishing under time = full marks)
        - 10% weight: completion (answered all questions)

    XP earned = base_xp * (readiness_score / 100), rounded.
    Base XP by level: junior=50, mid=100, senior=200.
    """
    result = InterviewResult(
        config=config,
        responses=responses,
        duration_seconds_used=duration_seconds_used,
        ran_out_of_time=ran_out_of_time,
    )

    # Counts
    for r in responses:
        if r.kind == "conceptual":
            result.conceptual_total += 1
            if r.correct:
                result.conceptual_correct += 1
        else:  # coding
            result.coding_total += 1
            if r.correct:
                result.coding_passed += 1
        # Per-topic tracking
        t = result.topic_performance.setdefault(r.topic_id, (0, 0))
        result.topic_performance[r.topic_id] = (
            t[0] + (1 if r.correct else 0), t[1] + 1,
        )

    # Weighted accuracy: coding weighted 2x conceptual
    conc_weight = 1.0
    code_weight = 2.0
    total_weight = (
        result.conceptual_total * conc_weight
        + result.coding_total * code_weight
    )
    if total_weight > 0:
        weighted_correct = (
            result.conceptual_correct * conc_weight
            + result.coding_passed * code_weight
        )
        accuracy = (weighted_correct / total_weight) * 100
    else:
        accuracy = 0.0

    result.overall_score = accuracy

    # Time efficiency: full credit if finished under time
    total_time = config.duration_minutes * 60
    if ran_out_of_time:
        time_score = 50.0  # half credit
    elif duration_seconds_used <= total_time * 0.6:
        time_score = 100.0
    elif duration_seconds_used <= total_time:
        # Linear between 60% and 100% of total time => 100 -> 70
        pct_used = duration_seconds_used / total_time
        time_score = 100 - ((pct_used - 0.6) / 0.4) * 30
    else:
        time_score = 60.0

    # Completion
    expected_responses = config.num_conceptual + config.num_coding
    completion = (len(responses) / expected_responses * 100) if expected_responses else 0

    # Readiness
    result.readiness_score = (
        accuracy * 0.7 + time_score * 0.2 + completion * 0.1
    )
    result.readiness_score = round(result.readiness_score, 1)
    result.overall_score = round(result.overall_score, 1)
    result.grade = compute_grade(result.readiness_score)

    # XP
    base_xp = {"junior": 50, "mid": 100, "senior": 200}.get(config.level, 50)
    result.xp_earned = int(base_xp * (result.readiness_score / 100))

    # Strengths and weaknesses analysis
    _analyze(result)

    return result


def _analyze(result: InterviewResult) -> None:
    """Derive strengths and weaknesses from per-topic performance."""
    perf = result.topic_performance
    topic_names = {t["id"]: t["name"] for t in load_topics()}

    for tid, (correct, total) in perf.items():
        if total == 0:
            continue
        pct = correct / total
        name = topic_names.get(tid, tid.replace("_", " ").title())
        if pct >= 0.75:
            result.strengths.append(f"{name} ({correct}/{total})")
        elif pct < 0.5:
            result.weaknesses.append(f"{name} ({correct}/{total})")

    # Coding vs conceptual commentary
    if result.coding_total > 0:
        coding_pct = result.coding_passed / result.coding_total
        if coding_pct >= 0.75:
            result.strengths.append(
                f"Coding ({result.coding_passed}/{result.coding_total} passed)"
            )
        elif coding_pct < 0.5:
            result.weaknesses.append(
                f"Coding ({result.coding_passed}/{result.coding_total} passed)"
            )


def verdict_text(result: InterviewResult) -> str:
    """A short interview-recruiter-style verdict message."""
    r = result.readiness_score
    level = result.config.level
    level_name = {"junior": "junior", "mid": "mid-level", "senior": "senior"}[level]

    if r >= 90:
        return f"Outstanding. You're ready for {level_name} embedded roles."
    if r >= 80:
        return f"Strong performance. You'd pass most {level_name} interviews."
    if r >= 70:
        return f"Solid, but there's room to sharpen. Review your weak topics."
    if r >= 60:
        return f"Borderline. Keep practicing before a real {level_name} interview."
    return f"Needs work. Focus on fundamentals before interviewing."
