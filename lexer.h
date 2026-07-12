#pragma once
#include <stdbool.h>

#include "cstring.h"

typedef enum {
    TK_LPAREN,    // (
    TK_RPAREN,    // )
    TK_RESERVED,  // 記号
    TK_NUM,       // 数値
    TK_BOOL,      // boolean
    TK_SYMBOL,    // シンボル(!= 文字列)
    TK_EOF,
} TokenKind;

typedef struct Token Token;

union token_value {
    char as_char;
    int as_int;
    bool as_bool;
    String* as_symbol;
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