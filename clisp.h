#pragma once

#include <stdint.h>

typedef enum {
    BO_PLUS,
    BO_MINUS,
    BO_MUL,
    BO_DIV,
    BO_GREATER,
    BO_LESS,
    BO_MODULO,
    BO_EQUAL,
    BO_OR,
    BO_AND,
    BO_NOT_EQUAL,
    BO_GREATER_EQ,
    BO_LESS_EQ,
    BO_UNKNOWN,
} BinaryOperator;

BinaryOperator str_to_binary_op(char* p, int* len);