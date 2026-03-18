"""Compile and test user C code against challenge test harnesses."""

from __future__ import annotations

import re
import shutil
import subprocess
import tempfile
import time
from pathlib import Path

from embedded_trainer.models.question import CodeRunResult, CodingChallenge, TestCaseResult


def check_gcc_available() -> bool:
    """Check if gcc is available on the system."""
    return shutil.which("gcc") is not None


def compile_and_test(
    user_code: str,
    challenge: CodingChallenge,
) -> CodeRunResult:
    """Compile user's C code and run it against test cases.

    Steps:
    1. Write user code to solution.c in a temp directory
    2. Copy the test harness (which #includes solution.c)
    3. Compile with gcc
    4. Execute and compare output to expected
    """
    temp_dir = tempfile.mkdtemp(prefix="embedded_trainer_")
    try:
        temp_path = Path(temp_dir)

        # Write user code
        solution_file = temp_path / "solution.c"
        solution_file.write_text(user_code)

        # Copy test harness
        harness_src = Path(challenge.test_harness_path)
        harness_dst = temp_path / "test_main.c"
        shutil.copy2(harness_src, harness_dst)

        # Compile
        compile_result = subprocess.run(
            ["gcc", "-Wall", "-Wextra", "-std=c11", "-o", "test_runner", "test_main.c"],
            cwd=temp_dir,
            capture_output=True,
            text=True,
            timeout=10,
        )

        if compile_result.returncode != 0:
            return CodeRunResult(
                compiled=False,
                compiler_output=compile_result.stderr,
                test_results=[],
                all_passed=False,
            )

        compiler_warnings = compile_result.stderr

        # Execute
        start = time.monotonic()
        try:
            run_result = subprocess.run(
                ["./test_runner"],
                cwd=temp_dir,
                capture_output=True,
                text=True,
                timeout=challenge.time_limit_seconds,
            )
        except subprocess.TimeoutExpired:
            return CodeRunResult(
                compiled=True,
                compiler_output=compiler_warnings,
                test_results=[TestCaseResult(
                    test_number=0, passed=False,
                    description="Execution timed out",
                )],
                all_passed=False,
            )
        elapsed_ms = (time.monotonic() - start) * 1000

        # Parse output
        test_results = _parse_test_output(run_result.stdout)

        # Also compare with expected output if available
        expected_path = Path(challenge.expected_output_path)
        if expected_path.exists() and not test_results:
            expected = expected_path.read_text().strip()
            actual = run_result.stdout.strip()
            test_results = _compare_outputs(expected, actual)

        all_passed = all(t.passed for t in test_results) and len(test_results) > 0

        return CodeRunResult(
            compiled=True,
            compiler_output=compiler_warnings,
            test_results=test_results,
            all_passed=all_passed,
            execution_time_ms=elapsed_ms,
        )

    except subprocess.TimeoutExpired:
        return CodeRunResult(
            compiled=False,
            compiler_output="Compilation timed out",
            test_results=[],
            all_passed=False,
        )
    finally:
        shutil.rmtree(temp_dir, ignore_errors=True)


def _parse_test_output(output: str) -> list[TestCaseResult]:
    """Parse test output lines like 'TEST 1: PASS' or 'TEST 2: FAIL (expected X, got Y)'."""
    results = []
    pattern = re.compile(
        r"TEST\s+(\d+):\s+(PASS|FAIL)(?:\s*\((.+)\))?", re.IGNORECASE
    )
    for line in output.strip().splitlines():
        m = pattern.match(line.strip())
        if m:
            test_num = int(m.group(1))
            passed = m.group(2).upper() == "PASS"
            detail = m.group(3) or ""
            expected = None
            actual = None
            if not passed and "expected" in detail.lower():
                parts = re.match(r"expected\s+(.+),\s*got\s+(.+)", detail, re.IGNORECASE)
                if parts:
                    expected = parts.group(1).strip()
                    actual = parts.group(2).strip()
            results.append(TestCaseResult(
                test_number=test_num,
                passed=passed,
                description=line.strip(),
                expected=expected,
                actual=actual,
            ))
    return results


def _compare_outputs(expected: str, actual: str) -> list[TestCaseResult]:
    """Line-by-line comparison of expected vs actual output."""
    expected_lines = expected.splitlines()
    actual_lines = actual.splitlines()
    results = []
    for i, exp_line in enumerate(expected_lines):
        act_line = actual_lines[i] if i < len(actual_lines) else "<missing>"
        passed = exp_line.strip() == act_line.strip()
        results.append(TestCaseResult(
            test_number=i + 1,
            passed=passed,
            description=f"Line {i + 1}",
            expected=exp_line.strip() if not passed else None,
            actual=act_line.strip() if not passed else None,
        ))
    return results
