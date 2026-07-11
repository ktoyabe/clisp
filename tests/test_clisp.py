import subprocess
import pytest

def run_script(commands):
    if len(commands) == 1:
        commands.append("exit")
    input_data = "\n".join(commands) + "\n"

    process = subprocess.run(
        ["./clisp"],
        input=input_data,
        capture_output=True,
        text=True
    )

    raw_output = process.stdout
    if raw_output == "":
        return []
    return raw_output.rstrip("\n").split("\n")


def test_add_two_elem():
    result = run_script(["(+ 2 3)"])
    expected = [
        "clisp> 5",
        "clisp> ",
    ]
    assert result == expected

def test_sub_two_elem():
    result = run_script(["(- 2 3)"])
    expected = [
        "clisp> -1",
        "clisp> ",
    ]
    assert result == expected

def test_mul_two_elem():
    result = run_script(["(* 2 3)"])
    expected = [
        "clisp> 6",
        "clisp> ",
    ]
    assert result == expected

def test_div_two_elem():
    result = run_script(["(* 7 2)"])
    expected = [
        "clisp> 14",
        "clisp> ",
    ]
    assert result == expected

def test_minus_int():
    result = run_script(["(+ -2 1)"])
    expected = [
        "clisp> -1",
        "clisp> ",
    ]
    assert result == expected


def test_multi_objs():
    result = run_script(["(+ (- 2 4) (* 3 5))"])
    expected = [
        "clisp> 13",
        "clisp> ",
    ]
    assert result == expected