#include "parser.h"

#include <stdlib.h>

#include "common.h"
#include "lexer.h"
#include "object.h"

Parser* new_parser(Token* tokens) {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->token = NULL;

    return parser;
}

Token* consume(Parser* parser) {
    parser->token = parser->token->next;
    return parser->token;
}

ObjectNode* parse_impl(Parser* parser) {
    ObjectNode head;
    head.next = NULL;
    ObjectNode* cur = &head;

    while (TK_EOF != parser->token->kind) {
        switch (parser->token->kind) {
            case TK_RESERVED: {
                Object* o = new_object(OK_RESERVED);
                o->value.as_char = parser->token->value.as_char;
                cur = new_node(cur, o);
                consume(parser);
                continue;
            }
            case TK_NUM: {
                Object* o = new_object(OK_INTEGER);
                o->value.as_int = parser->token->value.as_int;
                cur = new_node(cur, o);
                consume(parser);
                continue;
            }
            case TK_SYMBOL: {
                Object* o = new_object(OK_SYMBOL);
                o->value.as_symbol = parser->token->value.as_symbol;
                cur = new_node(cur, o);
                consume(parser);
                continue;
            }
            case TK_STRING: {
                Object* o = new_object(OK_STRING);
                o->value.as_string = parser->token->value.as_string;
                cur = new_node(cur, o);
                consume(parser);
                continue;
            }
            case TK_BOOL: {
                Object* o = new_bool_object(parser->token->value.as_bool);
                cur = new_node(cur, o);
                consume(parser);
                continue;
            }
            case TK_RPAREN: {
                new_node(cur, new_object(OK_EOF));
                consume(parser);
                return head.next;
            }
            case TK_LPAREN: {
                consume(parser);
                ObjectNode* objs = parse_impl(parser);
                Object* o = new_object(OK_LIST);
                o->value.as_list = objs;
                cur = new_node(cur, o);
                continue;
            }
            default:
                error("unsupported token type. token=%s", parser->token->str);
        }
    }
    error("unreachable code");
}

ObjectNode* parse(Parser* parser) {
    if (!parser->tokens) {
        error("parser: tokens must not be null.");
    }
    if (parser->tokens->kind != TK_LPAREN) {
        error("parser: tokens must start with  '('");
    }
    parser->token = parser->tokens->next;
    return parse_impl(parser);
}
