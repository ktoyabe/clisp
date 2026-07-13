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
        "clisp> #t",
        "clisp> ",
    ]
    assert result == expected

def test_less_than_is_false():
    result = run_script(["(< 1 0)"])
    expected = [
        "clisp> #f",
        "clisp> ",
    ]
    assert result == expected

def test_greater_than_is_true():
    result = run_script(["(> 1 -2)"])
    expected = [
        "clisp> #t",
        "clisp> ",
    ]
    assert result == expected

def test_greater_than_is_false():
    result = run_script(["(> 0 1)"])
    expected = [
        "clisp> #f",
        "clisp> ",
    ]
    assert result == expected

def test_eq_is_true():
    result = run_script(["(= -2 -2)"])
    expected = [
        "clisp> #t",
        "clisp> ",
    ]
    assert result == expected

def test_eq_is_false():
    result = run_script(["(= 0 1)"])
    expected = [
        "clisp> #f",
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


def test_bool_op_or():
    result = run_script([
        "(| #f #f)",
        "(| #t #f)",
        "(| #f #t)",
        "(| #t #t)",
        ])
    expected = [
        "clisp> #f",
        "clisp> #t",
        "clisp> #t",
        "clisp> #t",
        "clisp> ",
    ]
    assert result == expected

def test_bool_op_and():
    result = run_script([
        "(& #f #f)",
        "(& #t #f)",
        "(& #f #t)",
        "(& #t #t)",
        ])
    expected = [
        "clisp> #f",
        "clisp> #f",
        "clisp> #f",
        "clisp> #t",
        "clisp> ",
    ]
    assert result == expected

def test_modulo_operator():
    result = run_script([
        "(% 0 3)",
        "(% 1 3)",
        "(% 2 3)",
        "(% 3 3)",
        "(% 4 3)",
        ])
    expected = [
        "clisp> 0",
        "clisp> 1",
        "clisp> 2",
        "clisp> 0",
        "clisp> 1",
        "clisp> ",
    ]
    assert result == expected

def test_string_concat():
    result = run_script([
        '(+ "ab" "cde")',
        '(+ "ab" "")',
        '(+ "" "cde")',
        ])
    expected = [
        'clisp> "abcde"',
        'clisp> "ab"',
        'clisp> "cde"',
        'clisp> ',
    ]
    assert result == expected

def test_string_equal():
    result = run_script([
        '(= "ab" "cde")',
        '(= "ab" "cd")',
        '(= "ab" "ab")',
        '(= "ab" "")',
        '(= "" "ab")',
        '(= "" "")',
        ])
    expected = [
        'clisp> #f',
        'clisp> #f',
        'clisp> #t',
        'clisp> #f',
        'clisp> #f',
        'clisp> #t',
        'clisp> ',
    ]
    assert result == expected

def test_float_binary_op():
    result = run_script([
        '(+ -1.1 2.3)',
        '(+ 1.1 -2.3)',
        '(+ 1.1 2.3)',
        '(- 1.1 2.3)',
        '(* 1.1 2.3)',
        '(/ 1.1 2.2)',
        '(> 1.1 2.2)',
        '(< 1.1 2.2)',
        '(= 1.1 2.2)',
        '(= 1.1 1.1)',
        ])
    expected = [
        'clisp> 1.2000',
        'clisp> -1.2000',
        'clisp> 3.4000',
        'clisp> -1.2000',
        'clisp> 2.5300',
        'clisp> 0.5000',
        'clisp> #f',
        'clisp> #t',
        'clisp> #f',
        'clisp> #t',
        'clisp> ',
    ]
    assert result == expected

def test_binary_op_float_vs_int():
    result = run_script([
        '(+ -1.1 2)',
        '(+ 1.1 2)',
        '(- 1.1 2)',
        '(* 1.1 2)',
        '(/ 1.1 2)',
        '(> 1.1 2)',
        '(< 1.1 2)',
        '(= 1.1 2)',
        '(= 1.00 1)',
        ])
    expected = [
        'clisp> 0.9000',
        'clisp> 3.1000',
        'clisp> -0.9000',
        'clisp> 2.2000',
        'clisp> 0.5500',
        'clisp> #f',
        'clisp> #t',
        'clisp> #f',
        'clisp> #t',
        'clisp> ',
    ]
    assert result == expected


def test_binary_op_int_vs_float():
    result = run_script([
        '(+ 2 -2.5)',
        '(+ 2 2.5)',
        '(- 2 2.5)',
        '(* 2 2.5)',
        '(/ 2 2.5)',
        '(> 2 2.5)',
        '(< 2 2.5)',
        '(= 2 2.5)',
        '(= 2 2.00)',
        ])
    expected = [
        'clisp> -0.5000',
        'clisp> 4.5000',
        'clisp> -0.5000',
        'clisp> 5.0000',
        'clisp> 0.8000',
        'clisp> #f',
        'clisp> #t',
        'clisp> #f',
        'clisp> #t',
        'clisp> ',
    ]
    assert result == expected

def test_print():
    result = run_script([
        '(define a 1)',
        '(define b #f)',
        '(define c 1.0)',
        '(define d "abc")',
        '(print a)',
        '(print b c d)',
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> 1',
        '[VOID]',
        'clisp> #f',
        '1.0000',
        '"abc"',
        '[VOID]',
        'clisp> ',
    ]
    assert result == expected

def test_list():
    result = run_script([
        '(define a 1)',
        '(list a 3 4)'
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> (1 3 4)',
        'clisp> ',
    ]
    assert result == expected

def test_empty_list():
    result = run_script([
        '(list )'
        ])
    expected = [
        'clisp> ()',
        'clisp> ',
    ]
    assert result == expected


def test_list_concat():
    result = run_script([
        '(+ (list 1 2) (list 3 4))',
        '(+ (list 1) (list 3 4))',
        '(+ (list 1 2) (list 4))',
        '(+ (list ) (list 4))',
        '(+ (list 1 ) (list ))',
        '(+ (list ) (list ))',
        ])
    expected = [
        'clisp> (1 2 3 4)',
        'clisp> (1 3 4)',
        'clisp> (1 2 4)',
        'clisp> (4)',
        'clisp> (1)',
        'clisp> ()',
        'clisp> ',
    ]
    assert result == expected

def test_map():
    result = run_script([
        '(define l (list 2 3))',
        '(define addr (lambda (x) (+ x 1)))',
        '(map addr l)',
        '(map (lambda (x) (* x x)) l)',
        '(map addr (list 4 5))',
        '(map (lambda (x) (+ x 1)) (list 1 2))',
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> (3 4)',
        'clisp> (4 9)',
        'clisp> (5 6)',
        'clisp> (2 3)',
        'clisp> ',
    ]
    assert result == expected

def test_map_apply_empty_list():
    result = run_script([
        '(define l (list))',
        '(define addr (lambda (x) (+ x 1)))',
        '(map addr l)',
        '(map addr (list))',
        '(map (lambda (x) (* x x)) (list))',
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> ()',
        'clisp> ()',
        'clisp> ()',
        'clisp> ',
    ]
    assert result == expected


def test_filter():
    result = run_script([
        '(define l (list 1 2 4 3))',
        '(define isodd (lambda (x) (= (% x 2) 1)))',
        '(filter isodd l)',
        '(filter isodd (list 1 3 4 5))',
        '(filter isodd (list 2 4))',
        '(filter (lambda (x) (= (% x 2) 0)) l)',
        '(filter (lambda (x) (= (% x 2) 0)) (list 1 2 3))',
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> (1 3)',
        'clisp> (1 3 5)',
        'clisp> ()',
        'clisp> (2 4)',
        'clisp> (2)',
        'clisp> ',
    ]
    assert result == expected

def test_filter_when_result_is_emtpy():
    result = run_script([
        '(define isodd (lambda (x) (= (% x 2) 1)))',
        '(filter isodd (list 2 4))',
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> ()',
        'clisp> ',
    ]
    assert result == expected

def test_filter_when_list_is_empty():
    result = run_script([
        '(define l (list))',
        '(define isodd (lambda (x) (= (% x 2) 1)))',
        '(filter isodd l)',
        '(filter isodd (list))',
        ])
    expected = [
        'clisp> [VOID]',
        'clisp> [VOID]',
        'clisp> ()',
        'clisp> ()',
        'clisp> ',
    ]
    assert result == expected