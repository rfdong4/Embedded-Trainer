"""Achievement notification widget."""

from __future__ import annotations

from embedded_trainer.models.achievement import Achievement


def format_achievement(achievement: Achievement) -> str:
    """Format an achievement for display as a notification string."""
    return f"{achievement.icon} Achievement Unlocked: {achievement.name} - {achievement.description}"
