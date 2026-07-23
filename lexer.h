#pragma once
#include <stdbool.h>

#include "clisp.h"
#include "cstring.h"

typedef enum {
    TK_LPAREN,    // (
    TK_RPAREN,    // )
    TK_BINARYOP,  // 記号
    TK_IF,        // if
    TK_KEYWORD,   // 予約語(if除く)
    TK_NUM,       // int
    TK_FLOAT,     // float
    TK_BOOL,      // boolean
    TK_SYMBOL,    // シンボル(!= 文字列)
    TK_STRING,    // 文字列
    TK_EOF,
} TokenKind;

typedef struct Token Token;

union token_value {
    char as_char;
    int as_int;
    double as_float;
    bool as_bool;
    String* as_string;
    BinaryOperator as_binary_op;
};

struct Token {
    TokenKind kind;
    Token* next;
    char* str;
    union token_value value;
};

typedef struct {
    Token* token;  // 現在着目しているトークン
} Tokenizer;

void print_token(FILE* stream, Token* token);
void print_tokens(FILE* stream, Token* tokens);

Token* tokenize(char* p);