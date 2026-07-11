#pragma once

#include "lexer.h"
#include "object.h"
#include "parser.h"

typedef struct {
    Token* tokens;  // Parseするtoken全体
    Token* token;   // 現在着目しているtoken
} Parser;

Parser* new_parser(Token* tokens);

ObjectNode* parse(Parser* parser);
