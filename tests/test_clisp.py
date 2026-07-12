import subprocess
import pytest # type: ignore

def run_script(commands):
    if commands[-1] != "exit":
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

def test_less_than_is_true():
    result = run_script(["(< -2 1)"])
    expected = [
        "clisp> true",
        "clisp> ",
    ]
    assert result == expected

def test_less_than_is_false():
    result = run_script(["(< 1 0)"])
    expected = [
        "clisp> false",
        "clisp> ",
    ]
    assert result == expected

def test_greater_than_is_true():
    result = run_script(["(> 1 -2)"])
    expected = [
        "clisp> true",
        "clisp> ",
    ]
    assert result == expected

def test_greater_than_is_false():
    result = run_script(["(> 0 1)"])
    expected = [
        "clisp> false",
        "clisp> ",
    ]
    assert result == expected

def test_eq_is_true():
    result = run_script(["(= -2 -2)"])
    expected = [
        "clisp> true",
        "clisp> ",
    ]
    assert result == expected

def test_eq_is_false():
    result = run_script(["(= 0 1)"])
    expected = [
        "clisp> false",
        "clisp> ",
    ]
    assert result == expected

def test_if_execute_true_stmt():
    result = run_script(["(if (< 1 2) 3 4)"])
    expected = [
        "clisp> 3",
        "clisp> ",
    ]
    assert result == expected

def test_if_execute_false_stmt():
    result = run_script(["(if (> 1 2) 3 4)"])
    expected = [
        "clisp> 4",
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

def test_define_variable_and_use_it():
    result = run_script([
        "(define a 1)",
        "(define b 3)",
        "(+ a b)"])
    expected = [
        "clisp> [VOID]",
        "clisp> [VOID]",
        "clisp> 4",
        "clisp> ",
    ]
    assert result == expected

def test_redefine_variable():
    result = run_script([
        "(define a 1)",
        "(+ a 2)",
        "(define a 3)",
        "(+ a 2)"])
    expected = [
        "clisp> [VOID]",
        "clisp> 3",
        "clisp> [VOID]",
        "clisp> 5",
        "clisp> ",
    ]
    assert result == expected

def test_define_when_value_node_include_variable():
    result = run_script([
        "(define a 3)",
        "(define b (+ 1 a))",
        "(+ b 1)"])
    expected = [
        "clisp> [VOID]",
        "clisp> [VOID]",
        "clisp> 5",
        "clisp> ",
    ]
    assert result == expected

def test_function_call_wo_variable_capture():
    result = run_script([
        "(define f (lambda (x) (+ x 1)))",
        "(f 2)"])
    expected = [
        "clisp> [VOID]",
        "clisp> 3",
        "clisp> ",
    ]
    assert result == expected


def test_function_call_two_args_wo_variable_capture():
    result = run_script([
        "(define f (lambda (x y) (/ x y)))",
        "(f 4 3)"])
    expected = [
        "clisp> [VOID]",
        "clisp> 1",
        "clisp> ",
    ]
    assert result == expected

def test_function_call_two_args_wo_variable_capture_with_variable_args():
    result = run_script([
        "(define a 1)",
        "(define b 3)",
        "(define f (lambda (x y) (+ x y)))",
        "(f (* a 1) (* b 2))"])
    expected = [
        "clisp> [VOID]",
        "clisp> [VOID]",
        "clisp> [VOID]",
        "clisp> 7",
        "clisp> ",
    ]
    assert result == expected

def test_function_call_with_variable_capture():
    result = run_script([
        "(define a 1)",
        "(define f (lambda (x) (+ x a)))",
        "(f 3)"])
    expected = [
        "clisp> [VOID]",
        "clisp> [VOID]",
        "clisp> 4",
        "clisp> ",
    ]
    assert result == expected

def test_function_recursive_call_fib():
    result = run_script([
        "(define fib (lambda (n) (if (< n 2) 1 (+ (fib (- n 1)) (fib (- n 2))))))",
        "(fib 1)",
        "(fib 2)",
        "(fib 10)",
        ])
    expected = [
        "clisp> [VOID]",
        "clisp> 1",
        "clisp> 2",
        "clisp> 89",
        "clisp> ",
    ]
    assert result == expected

def test_function_recursive_call_fact():
    result = run_script([
        "(define fact (lambda (n) (if (< n 2) 1 (* n (fact (- n 1))))))",
        "(fact 1)",
        "(fact 2)",
        "(fact 5)",
        ])
    expected = [
        "clisp> [VOID]",
        "clisp> 1",
        "clisp> 2",
        "clisp> 120",
        "clisp> ",
    ]
    assert result == expected