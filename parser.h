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
void print_objs(FILE* stream, ObjectNode* objs);  // TODO: move to object.h
void print_obj(FILE* stream, Object* obj, int indent);  // TODO: move to
                                                        // object.h