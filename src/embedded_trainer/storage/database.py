"""SQLite persistence layer for user progress."""

from __future__ import annotations

import sqlite3
from datetime import date, datetime
from pathlib import Path

from embedded_trainer.models.user import TopicProgress, UserProfile, UserProgress

DEFAULT_DB_PATH = Path.home() / ".embedded_trainer" / "progress.db"


class Database:
    def __init__(self, db_path: Path = DEFAULT_DB_PATH):
        db_path.parent.mkdir(parents=True, exist_ok=True)
        self.conn = sqlite3.connect(str(db_path))
        self.conn.row_factory = sqlite3.Row
        self._create_tables()

    def _create_tables(self):
        self.conn.executescript("""
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT UNIQUE NOT NULL,
                total_xp INTEGER DEFAULT 0,
                current_streak INTEGER DEFAULT 0,
                longest_streak INTEGER DEFAULT 0,
                last_activity_date TEXT,
                created_at TEXT DEFAULT (datetime('now'))
            );

            CREATE TABLE IF NOT EXISTS quiz_results (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER REFERENCES users(id),
                topic_id TEXT NOT NULL,
                questions_total INTEGER,
                questions_correct INTEGER,
                xp_earned INTEGER,
                completed_at TEXT DEFAULT (datetime('now'))
            );

            CREATE TABLE IF NOT EXISTS coding_results (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER REFERENCES users(id),
                challenge_id TEXT NOT NULL,
                passed BOOLEAN NOT NULL,
                attempts INTEGER DEFAULT 1,
                xp_earned INTEGER,
                user_code TEXT,
                completed_at TEXT DEFAULT (datetime('now'))
            );

            CREATE TABLE IF NOT EXISTS achievements (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER REFERENCES users(id),
                achievement_id TEXT NOT NULL,
                unlocked_at TEXT DEFAULT (datetime('now')),
                UNIQUE(user_id, achievement_id)
            );

            CREATE TABLE IF NOT EXISTS topic_progress (
                user_id INTEGER REFERENCES users(id),
                topic_id TEXT NOT NULL,
                quiz_completed INTEGER DEFAULT 0,
                quiz_best_score REAL DEFAULT 0,
                coding_completed INTEGER DEFAULT 0,
                PRIMARY KEY (user_id, topic_id)
            );
        """)
        self.conn.commit()

    def get_or_create_user(self, name: str = "Player") -> UserProfile:
        row = self.conn.execute(
            "SELECT * FROM users WHERE name = ?", (name,)
        ).fetchone()
        if row:
            return UserProfile(
                id=row["id"],
                name=row["name"],
                total_xp=row["total_xp"],
                current_streak=row["current_streak"],
                longest_streak=row["longest_streak"],
                last_activity_date=row["last_activity_date"],
            )
        cursor = self.conn.execute(
            "INSERT INTO users (name) VALUES (?)", (name,)
        )
        self.conn.commit()
        return UserProfile(id=cursor.lastrowid, name=name)

    def update_user(self, profile: UserProfile):
        self.conn.execute(
            """UPDATE users SET total_xp = ?, current_streak = ?,
               longest_streak = ?, last_activity_date = ? WHERE id = ?""",
            (profile.total_xp, profile.current_streak,
             profile.longest_streak, profile.last_activity_date, profile.id),
        )
        self.conn.commit()

    def save_quiz_result(
        self, user_id: int, topic_id: str,
        total: int, correct: int, xp_earned: int,
    ):
        self.conn.execute(
            """INSERT INTO quiz_results
               (user_id, topic_id, questions_total, questions_correct, xp_earned)
               VALUES (?, ?, ?, ?, ?)""",
            (user_id, topic_id, total, correct, xp_earned),
        )
        # Update topic progress
        score_pct = (correct / total * 100) if total > 0 else 0
        self.conn.execute(
            """INSERT INTO topic_progress (user_id, topic_id, quiz_completed, quiz_best_score)
               VALUES (?, ?, 1, ?)
               ON CONFLICT(user_id, topic_id) DO UPDATE SET
               quiz_completed = quiz_completed + 1,
               quiz_best_score = MAX(quiz_best_score, ?)""",
            (user_id, topic_id, score_pct, score_pct),
        )
        self.conn.commit()

    def save_coding_result(
        self, user_id: int, challenge_id: str, topic_id: str,
        passed: bool, xp_earned: int, user_code: str,
    ):
        # Check if there's already a result for this challenge
        existing = self.conn.execute(
            "SELECT id, attempts FROM coding_results WHERE user_id = ? AND challenge_id = ?",
            (user_id, challenge_id),
        ).fetchone()

        if existing:
            self.conn.execute(
                """UPDATE coding_results SET passed = ?, attempts = attempts + 1,
                   xp_earned = ?, user_code = ? WHERE id = ?""",
                (passed, xp_earned, user_code, existing["id"]),
            )
        else:
            self.conn.execute(
                """INSERT INTO coding_results
                   (user_id, challenge_id, passed, xp_earned, user_code)
                   VALUES (?, ?, ?, ?, ?)""",
                (user_id, challenge_id, passed, xp_earned, user_code),
            )

        if passed:
            self.conn.execute(
                """INSERT INTO topic_progress (user_id, topic_id, coding_completed)
                   VALUES (?, ?, 1)
                   ON CONFLICT(user_id, topic_id) DO UPDATE SET
                   coding_completed = coding_completed + 1""",
                (user_id, topic_id),
            )
        self.conn.commit()

    def get_coding_attempts(self, user_id: int, challenge_id: str) -> int:
        row = self.conn.execute(
            "SELECT attempts FROM coding_results WHERE user_id = ? AND challenge_id = ?",
            (user_id, challenge_id),
        ).fetchone()
        return row["attempts"] if row else 0

    def unlock_achievement(self, user_id: int, achievement_id: str) -> bool:
        """Returns True if newly unlocked, False if already had it."""
        try:
            self.conn.execute(
                "INSERT INTO achievements (user_id, achievement_id) VALUES (?, ?)",
                (user_id, achievement_id),
            )
            self.conn.commit()
            return True
        except sqlite3.IntegrityError:
            return False

    def get_user_achievements(self, user_id: int) -> list[str]:
        rows = self.conn.execute(
            "SELECT achievement_id FROM achievements WHERE user_id = ?",
            (user_id,),
        ).fetchall()
        return [r["achievement_id"] for r in rows]

    def get_topic_progress(self, user_id: int) -> dict[str, TopicProgress]:
        rows = self.conn.execute(
            "SELECT * FROM topic_progress WHERE user_id = ?", (user_id,)
        ).fetchall()
        result = {}
        for r in rows:
            result[r["topic_id"]] = TopicProgress(
                topic_id=r["topic_id"],
                quiz_completed=r["quiz_completed"],
                quiz_best_score=r["quiz_best_score"],
                coding_completed=r["coding_completed"],
            )
        return result

    def get_completed_challenges(self, user_id: int) -> set[str]:
        """Return set of challenge_ids that the user has passed."""
        rows = self.conn.execute(
            "SELECT challenge_id FROM coding_results WHERE user_id = ? AND passed = 1",
            (user_id,),
        ).fetchall()
        return {r["challenge_id"] for r in rows}

    def get_total_questions_answered(self, user_id: int) -> int:
        row = self.conn.execute(
            "SELECT COALESCE(SUM(questions_total), 0) as total FROM quiz_results WHERE user_id = ?",
            (user_id,),
        ).fetchone()
        return row["total"]

    def get_total_challenges_passed(self, user_id: int) -> int:
        row = self.conn.execute(
            "SELECT COUNT(*) as total FROM coding_results WHERE user_id = ? AND passed = 1",
            (user_id,),
        ).fetchone()
        return row["total"]

    def get_user_progress(self, user_id: int) -> UserProgress:
        profile = self.conn.execute(
            "SELECT * FROM users WHERE id = ?", (user_id,)
        ).fetchone()
        if not profile:
            raise ValueError(f"User {user_id} not found")

        user_profile = UserProfile(
            id=profile["id"],
            name=profile["name"],
            total_xp=profile["total_xp"],
            current_streak=profile["current_streak"],
            longest_streak=profile["longest_streak"],
            last_activity_date=profile["last_activity_date"],
        )
        topics = self.get_topic_progress(user_id)
        achievements = self.get_user_achievements(user_id)
        return UserProgress(
            profile=user_profile, topics=topics, achievements=achievements
        )

    def close(self):
        self.conn.close()
