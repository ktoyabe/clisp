#include "clisp.h"

#include <string.h>

BinaryOperator str_to_binary_op(char* p, int* len) {
    char c = *p;
    switch (c) {
        case '+':
            *len = 1;
            return BO_PLUS;
        case '-':
            *len = 1;
            return BO_MINUS;
        case '*':
            *len = 1;
            return BO_MUL;
        case '/':
            *len = 1;
            return BO_DIV;
        case '>':
            *len = 1;
            return BO_GREATER;
        case '<':
            *len = 1;
            return BO_LESS;
        case '%':
            *len = 1;
            return BO_MODULO;
        case '=':
            *len = 1;
            return BO_EQUAL;
        case '|':
            *len = 1;
            return BO_OR;
        case '&':
            *len = 1;
            return BO_AND;
    }
    if (strcmp(p, "!=") == 0) {
        *len = 2;
        return BO_NOT_EQUAL;
    } else if (strcmp(p, ">=") == 0) {
        *len = 2;
        return BO_GREATER_EQ;
    } else if (strcmp(p, "<=") == 0) {
        *len = 2;
        return BO_LESS_EQ;
    }
    return BO_UNKNOWN;
}
