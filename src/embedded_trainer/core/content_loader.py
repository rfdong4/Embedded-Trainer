"""Load quiz questions and coding challenges from YAML content files."""

from __future__ import annotations

from pathlib import Path

import yaml

from embedded_trainer.models.question import CodingChallenge, Question


def _find_content_dir() -> Path:
    """Locate the content/ directory relative to the package."""
    # Walk up from this file to find the project root
    current = Path(__file__).resolve().parent
    for _ in range(5):
        candidate = current / "content"
        if candidate.is_dir():
            return candidate
        current = current.parent
    raise FileNotFoundError("Could not locate content/ directory")


CONTENT_DIR = None


def get_content_dir() -> Path:
    global CONTENT_DIR
    if CONTENT_DIR is None:
        CONTENT_DIR = _find_content_dir()
    return CONTENT_DIR


def load_topics() -> list[dict]:
    """Load the master topic manifest."""
    topics_file = get_content_dir() / "topics.yaml"
    with open(topics_file) as f:
        data = yaml.safe_load(f)
    return data.get("topics", [])


def load_quiz_questions(topic_id: str) -> list[Question]:
    """Load all quiz questions for a given topic."""
    quiz_file = get_content_dir() / "quiz" / f"{topic_id}.yaml"
    if not quiz_file.exists():
        return []
    with open(quiz_file) as f:
        data = yaml.safe_load(f)

    questions = []
    for q in data.get("questions", []):
        questions.append(Question(
            id=q["id"],
            topic_id=data.get("topic_id", topic_id),
            question_type=q.get("type", "mcq"),
            difficulty=q.get("difficulty", "beginner"),
            text=q["text"],
            options=q["options"],
            correct_answer=q["correct"],
            explanation=q.get("explanation", ""),
            xp_value=q.get("xp", 10),
            tags=q.get("tags", []),
        ))
    return questions


def load_coding_challenges(topic_id: str) -> list[CodingChallenge]:
    """Load all coding challenges for a given topic."""
    coding_dir = get_content_dir() / "coding" / topic_id
    if not coding_dir.is_dir():
        return []

    challenges = []
    for yaml_file in sorted(coding_dir.glob("*.yaml")):
        with open(yaml_file) as f:
            c = yaml.safe_load(f)

        test_harness = coding_dir / c["test_harness"]
        expected_output = coding_dir / c["expected_output"]

        challenges.append(CodingChallenge(
            id=c["id"],
            topic_id=c.get("topic_id", topic_id),
            title=c["title"],
            difficulty=c.get("difficulty", "beginner"),
            description=c["description"],
            function_signature=c["function_signature"],
            skeleton_code=c["skeleton_code"],
            test_harness_path=str(test_harness),
            expected_output_path=str(expected_output),
            xp_value=c.get("xp", 25),
            hints=c.get("hints", []),
            time_limit_seconds=c.get("time_limit", 5),
        ))
    return challenges


def load_coding_challenge(topic_id: str, challenge_id: str) -> CodingChallenge | None:
    """Load a single coding challenge by ID."""
    challenges = load_coding_challenges(topic_id)
    for c in challenges:
        if c.id == challenge_id:
            return c
    return None
