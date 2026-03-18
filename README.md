# Embedded Trainer

A gamified terminal application for learning embedded systems fundamentals. Practice conceptual quizzes and hands-on C coding challenges across 10 core topics.

## Quickstart

### Prerequisites

- Python 3.10+
- GCC (for compiling and testing C coding challenges)

### Install

```bash
cd "Embedded Trainer"
python -m venv .venv
source .venv/bin/activate
pip install -e .
```

### Run

```bash
python -m embedded_trainer
```

Or use the installed script:

```bash
embedded-trainer
```

## What's Inside

### Quiz Mode

96 multiple-choice and true/false questions covering:

| Topic | Questions |
|-------|-----------|
| Bit Manipulation | 10 |
| ARM Cortex-M Registers | 10 |
| GPIO | 10 |
| Memory Management | 10 |
| Interrupts & NVIC | 10 |
| Timers & PWM | 10 |
| UART | 10 |
| SPI | 8 |
| I2C | 8 |
| RTOS Basics | 10 |

Each question includes an explanation shown after answering.

### Coding Challenges

72 C programming challenges with automated test harnesses:

| Topic | Challenges | Difficulty Range |
|-------|-----------|-----------------|
| Bit Manipulation | 8 | beginner - advanced |
| GPIO | 6 | beginner - advanced |
| UART | 7 | beginner - advanced |
| Timers | 7 | beginner - advanced |
| Memory Management | 7 | beginner - advanced |
| Interrupts | 7 | beginner - advanced |
| SPI | 5 | beginner - advanced |
| I2C | 5 | beginner - advanced |
| RTOS Basics | 7 | beginner - advanced |
| Registers | 5 | beginner - advanced |

Write your solution in the built-in editor, press **Ctrl+R** to compile and run the test suite. Each challenge has 4 automated tests.

## Gamification

- **XP System** — earn XP for correct quiz answers and passing coding challenges. Harder questions are worth more.
- **Streaks** — consecutive daily sessions earn bonus XP multipliers.
- **Levels** — progress through ranks as you accumulate XP.
- **Achievements** — unlock milestones like "Perfect Quiz", "Speed Demon", and more.
- **Progress Tracking** — per-topic completion tracked across sessions (stored in `~/.embedded_trainer/progress.db`).

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `Ctrl+R` | Run tests (coding screen) |
| `Escape` | Go back |
| `d` | Toggle dark/light mode |

## Project Structure

```
src/embedded_trainer/     # Application source
  app.py                  # Main Textual app
  screens/                # Quiz, coding, home screens
  widgets/                # Question card, code editor, XP bar
  core/                   # Quiz engine, code runner, gamification logic
  storage/                # SQLite persistence
  models/                 # Data models
  styles/                 # Textual CSS

content/
  quiz/                   # YAML quiz question banks (10 topics)
  coding/                 # C coding challenges with test harnesses
    <topic>/
      <challenge>.yaml    # Challenge definition
      <challenge>_tests/
        test_main.c       # Test harness (4 tests)
        expected_output.txt
```
