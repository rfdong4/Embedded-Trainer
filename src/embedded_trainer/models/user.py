"""Data models for user profile and progress."""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class UserProfile:
    id: int
    name: str
    total_xp: int = 0
    current_streak: int = 0
    longest_streak: int = 0
    last_activity_date: str | None = None


@dataclass
class TopicProgress:
    topic_id: str
    quiz_completed: int = 0
    quiz_best_score: float = 0.0
    coding_completed: int = 0


@dataclass
class UserProgress:
    profile: UserProfile
    topics: dict[str, TopicProgress] = field(default_factory=dict)
    achievements: list[str] = field(default_factory=list)
