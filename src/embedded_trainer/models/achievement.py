"""Achievement definitions and registry."""

from __future__ import annotations

from dataclasses import dataclass
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from embedded_trainer.models.user import UserProgress


@dataclass
class Achievement:
    id: str
    name: str
    description: str
    icon: str


# All available achievements
ACHIEVEMENTS: dict[str, Achievement] = {
    "first_quiz": Achievement(
        "first_quiz", "First Steps",
        "Complete your first quiz", "^",
    ),
    "perfect_quiz": Achievement(
        "perfect_quiz", "Perfectionist",
        "Score 100% on any quiz", "*",
    ),
    "first_code": Achievement(
        "first_code", "Hello, Embedded World",
        "Pass your first coding challenge", ">",
    ),
    "streak_3": Achievement(
        "streak_3", "On a Roll",
        "Achieve a 3-day streak", "~",
    ),
    "streak_7": Achievement(
        "streak_7", "Dedicated Learner",
        "Achieve a 7-day streak", "#",
    ),
    "level_5": Achievement(
        "level_5", "Senior Engineer",
        "Reach level 5", "+",
    ),
    "level_10": Achievement(
        "level_10", "Embedded Master",
        "Reach level 10", "@",
    ),
    "speed_demon": Achievement(
        "speed_demon", "Speed Demon",
        "Pass a coding challenge on the first attempt", "!",
    ),
    "50_questions": Achievement(
        "50_questions", "Quiz Machine",
        "Answer 50 quiz questions", "?",
    ),
    "10_challenges": Achievement(
        "10_challenges", "Code Warrior",
        "Pass 10 coding challenges", "&",
    ),
    "bit_wizard": Achievement(
        "bit_wizard", "Bit Wizard",
        "Complete all bit manipulation challenges", "%",
    ),
    "bug_hunter": Achievement(
        "bug_hunter", "Bug Hunter",
        "Find 10 bugs in Bug Hunt mode", "$",
    ),
    "review_streak": Achievement(
        "review_streak", "Memory Palace",
        "Complete 5 spaced repetition reviews", "=",
    ),
    "register_master": Achievement(
        "register_master", "Register Master",
        "Complete 10 register simulator exercises", "|",
    ),
    "100_questions": Achievement(
        "100_questions", "Knowledge Seeker",
        "Answer 100 quiz questions", ":",
    ),
    "25_challenges": Achievement(
        "25_challenges", "Code Master",
        "Pass 25 coding challenges", ";",
    ),
}

# Level definitions
LEVELS = [
    (0, "Novice"),
    (100, "Apprentice"),
    (300, "Technician"),
    (600, "Engineer"),
    (1000, "Senior Engineer"),
    (1500, "Architect"),
    (2100, "Principal"),
    (2800, "Fellow"),
    (3600, "Distinguished"),
    (4500, "Embedded Master"),
]


def get_level(total_xp: int) -> tuple[int, str]:
    """Return (level_number, level_title) for the given XP total."""
    level_num = 1
    level_name = LEVELS[0][1]
    for i, (threshold, name) in enumerate(LEVELS):
        if total_xp >= threshold:
            level_num = i + 1
            level_name = name
        else:
            break
    return level_num, level_name


def xp_for_next_level(total_xp: int) -> tuple[int, int]:
    """Return (xp_into_current_level, xp_needed_for_next_level).

    If already at max level, returns (0, 0).
    """
    current_threshold = 0
    next_threshold = 0
    for i, (threshold, _) in enumerate(LEVELS):
        if total_xp >= threshold:
            current_threshold = threshold
            if i + 1 < len(LEVELS):
                next_threshold = LEVELS[i + 1][0]
            else:
                return 0, 0  # Max level
        else:
            break
    progress = total_xp - current_threshold
    needed = next_threshold - current_threshold
    return progress, needed
