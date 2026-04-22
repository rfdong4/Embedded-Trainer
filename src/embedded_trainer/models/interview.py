"""Data models for the interview simulation feature."""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Literal

from embedded_trainer.models.question import CodingChallenge, Question


InterviewLevel = Literal["junior", "mid", "senior"]


@dataclass
class InterviewConfig:
    """Configuration for a single interview session."""
    level: InterviewLevel
    topic_focus: str  # topic_id or "mixed"
    duration_minutes: int
    num_conceptual: int
    num_coding: int
    difficulty_mix: list[str]  # e.g. ["beginner", "intermediate"]


# Preset configurations by level
INTERVIEW_PRESETS: dict[str, InterviewConfig] = {
    "junior": InterviewConfig(
        level="junior",
        topic_focus="mixed",
        duration_minutes=15,
        num_conceptual=5,
        num_coding=1,
        difficulty_mix=["beginner"],
    ),
    "mid": InterviewConfig(
        level="mid",
        topic_focus="mixed",
        duration_minutes=30,
        num_conceptual=6,
        num_coding=2,
        difficulty_mix=["beginner", "intermediate"],
    ),
    "senior": InterviewConfig(
        level="senior",
        topic_focus="mixed",
        duration_minutes=45,
        num_conceptual=7,
        num_coding=2,
        difficulty_mix=["intermediate", "advanced"],
    ),
}


@dataclass
class QuestionResponse:
    """A single answered question within an interview session."""
    kind: Literal["conceptual", "coding"]
    topic_id: str
    difficulty: str
    correct: bool
    time_seconds: float
    # For conceptual: the Question itself; for coding: the challenge
    question_id: str
    # Coding-specific: how many test cases passed / total
    tests_passed: int = 0
    tests_total: int = 0
    user_code: str = ""


@dataclass
class InterviewResult:
    """Final computed results for an interview session."""
    config: InterviewConfig
    responses: list[QuestionResponse] = field(default_factory=list)
    duration_seconds_used: float = 0.0
    ran_out_of_time: bool = False

    # Computed after session completes
    conceptual_correct: int = 0
    conceptual_total: int = 0
    coding_passed: int = 0
    coding_total: int = 0
    overall_score: float = 0.0  # 0-100
    readiness_score: float = 0.0  # 0-100 (weighted)
    grade: str = "F"
    xp_earned: int = 0
    # Analysis
    topic_performance: dict[str, tuple[int, int]] = field(default_factory=dict)
    strengths: list[str] = field(default_factory=list)
    weaknesses: list[str] = field(default_factory=list)
