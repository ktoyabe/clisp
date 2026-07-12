#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>

#include "common.h"

void print_token(FILE* stream, Token* token) {
    switch (token->kind) {
        case TK_LPAREN:
            fprintf(stream, "%c[LPAREN]", token->value.as_char);
            return;
        case TK_RPAREN:
            fprintf(stream, "%c[RPAREN]", token->value.as_char);
            return;
        case TK_RESERVED:
            fprintf(stream, "%c[RESERVED]", token->value.as_char);
            return;
        case TK_NUM:
            fprintf(stream, "%d[NUM]", token->value.as_int);
            return;
        case TK_SYMBOL:
            fprintf(stream, "%s[STRING]", token->value.as_symbol->str);
            return;
        default:
            fprintf(stream, "%s[UNKNOWN]", token->str);
            return;
    }
}

void print_tokens(FILE* stream, Token* tokens) {
    Token* t = tokens;
    while (t->kind != TK_EOF) {
        print_token(stream, t);
        fprintf(stream, "\n");
        t = t->next;
    }
}

Token* new_token(TokenKind kind, Token* cur, char* str) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;

    return tok;
}

bool is_reserved(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '>' ||
           c == '<' || c == '%' || c == '=' || c == '|' || c == '&';
}

bool is_parents(char c) { return c == '(' || c == ')'; }

Token* tokenize(char* p) {
    Token head;
    head.next = NULL;
    Token* cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (*p == '(') {
            cur = new_token(TK_LPAREN, cur, p);
            cur->value.as_char = *p;
            p++;
            continue;
        }
        if (*p == ')') {
            cur = new_token(TK_RPAREN, cur, p);
            cur->value.as_char = *p;
            p++;
            continue;
        }
        if (*p == '-' && isdigit(*(p + 1))) {  // parse as minus integer
            p++;
            cur = new_token(TK_NUM, cur, p);
            cur->value.as_int = -strtol(p, &p, 10);
            continue;
        }
        if (is_reserved(*p)) {
            // parse as reserved charactor
            cur = new_token(TK_RESERVED, cur, p);
            cur->value.as_char = *p;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->value.as_int = strtol(p, &p, 10);
            continue;
        }

        if (*p == '#') {
            char next = *(p + 1);
            if (next == 't') {
                cur = new_token(TK_BOOL, cur, p);
                cur->value.as_bool = true;
                p++;
                p++;
                continue;
            } else if (next == 'f') {
                cur = new_token(TK_BOOL, cur, p);
                cur->value.as_bool = false;
                p++;
                p++;
                continue;
            }
        }

        // parse symbol
        {
            char* start = p;
            while (*p && !isspace(*p) && !is_reserved(*p) && !is_parents(*p)) {
                p++;
            }
            size_t len = p - start;
            String* str = new_string_with_len(start, len);
            cur = new_token(TK_SYMBOL, cur, p);
            cur->value.as_symbol = str;
            continue;
        }

        error("failed to tokenize. input=%s", p);
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}