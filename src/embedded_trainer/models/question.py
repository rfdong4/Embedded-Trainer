"""Data models for quiz questions and coding challenges."""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Literal


@dataclass
class Question:
    id: str
    topic_id: str
    question_type: Literal["mcq", "true_false"]
    difficulty: Literal["beginner", "intermediate", "advanced"]
    text: str
    options: list[str]
    correct_answer: str
    explanation: str
    xp_value: int
    tags: list[str] = field(default_factory=list)


@dataclass
class TestCaseResult:
    test_number: int
    passed: bool
    description: str
    expected: str | None = None
    actual: str | None = None


@dataclass
class CodeRunResult:
    compiled: bool
    compiler_output: str
    test_results: list[TestCaseResult]
    all_passed: bool
    execution_time_ms: float = 0.0


@dataclass
class CodingChallenge:
    id: str
    topic_id: str
    title: str
    difficulty: Literal["beginner", "intermediate", "advanced"]
    description: str
    function_signature: str
    skeleton_code: str
    test_harness_path: str
    expected_output_path: str
    xp_value: int
    hints: list[str] = field(default_factory=list)
    time_limit_seconds: int = 5
    solution: str = ""
    solution_explanation: str = ""


@dataclass
class BugHuntChallenge:
    id: str
    topic_id: str
    title: str
    difficulty: Literal["beginner", "intermediate", "advanced"]
    buggy_code: str
    bug_description: str
    correct_answer: str
    options: list[str]
    explanation: str
    xp_value: int
    tags: list[str] = field(default_factory=list)
