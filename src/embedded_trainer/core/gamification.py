"""Gamification logic: XP, streaks, achievement checking."""

from __future__ import annotations

from datetime import date, timedelta

from embedded_trainer.models.achievement import ACHIEVEMENTS, Achievement, get_level
from embedded_trainer.models.user import UserProfile
from embedded_trainer.storage.database import Database


def update_streak(profile: UserProfile) -> int:
    """Update the user's streak based on today's date.

    Returns the new streak value.
    """
    today = date.today().isoformat()

    if profile.last_activity_date is None:
        profile.current_streak = 1
    elif profile.last_activity_date == today:
        pass  # Already active today
    elif profile.last_activity_date == (date.today() - timedelta(days=1)).isoformat():
        profile.current_streak += 1
    else:
        profile.current_streak = 1

    profile.last_activity_date = today
    if profile.current_streak > profile.longest_streak:
        profile.longest_streak = profile.current_streak

    return profile.current_streak


def calculate_coding_xp(
    base_xp: int, streak: int, first_attempt: bool,
) -> int:
    """Calculate XP for a coding challenge."""
    xp = base_xp
    if first_attempt:
        xp = int(xp * 1.25)
    multiplier = 1.0 + 0.1 * min(streak, 7)
    return int(xp * multiplier)


def check_achievements(
    db: Database, profile: UserProfile,
) -> list[Achievement]:
    """Check all achievement conditions and return newly unlocked ones."""
    newly_unlocked = []
    existing = set(db.get_user_achievements(profile.id))

    checks = {
        "first_quiz": lambda: db.get_total_questions_answered(profile.id) > 0,
        "first_code": lambda: db.get_total_challenges_passed(profile.id) > 0,
        "streak_3": lambda: profile.current_streak >= 3,
        "streak_7": lambda: profile.current_streak >= 7,
        "level_5": lambda: get_level(profile.total_xp)[0] >= 5,
        "level_10": lambda: get_level(profile.total_xp)[0] >= 10,
        "50_questions": lambda: db.get_total_questions_answered(profile.id) >= 50,
        "10_challenges": lambda: db.get_total_challenges_passed(profile.id) >= 10,
    }

    for achievement_id, condition in checks.items():
        if achievement_id not in existing and condition():
            if db.unlock_achievement(profile.id, achievement_id):
                newly_unlocked.append(ACHIEVEMENTS[achievement_id])

    return newly_unlocked
