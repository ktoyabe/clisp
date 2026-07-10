#pragma once
#include <stdbool.h>

#include "cstring.h"

typedef enum {
    TK_RESERVED,  // 記号
    TK_NUM,       // 数値
    TK_BOOL,      // boolean
    TK_STRING,    // 文字列
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token* next;
    char* str;
    int ival;
    bool bval;
    String* sval;
};

typedef struct {
    Token* token;  // 現在着目しているトークン
} Tokenizer;

void print_token(FILE* stream, Token* token);
Token* tokenize(char* p);