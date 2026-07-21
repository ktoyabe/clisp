import subprocess
import pytest # type: ignore

def run_clisp(filename):
    process = subprocess.run(
        ["./clisp", filename],
        capture_output=True,
        text=True
    )

    raw_output = process.stdout
    if raw_output == "":
        return []
    return raw_output.rstrip("\n").split("\n")


def test_print_vars():
    result = run_clisp("tests/scripts/print_vars.lisp")
    ["2", "3"] == result


def test_print_objects():
    result = run_clisp("tests/scripts/print_objects.lisp")
    ['#t',
    '"foo bar"',
    '1',
    '2.0000',
    '(2 3 4)',
    '[VOID]',
    '[LAMBDA]',
    ] == result

