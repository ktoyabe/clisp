#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

static const char* KW_DEFINE = "define";
static const char* KW_LIST = "list";
static const char* KW_PRINT = "print";
static const char* KW_LAMBDA = "lambda";
static const char* KW_MAP = "map";
static const char* KW_FILTER = "filter";
static const char* KW_REDUCE = "reduce";
static const char* KW_LENGTH = "length";
static const char* KW_RANGE = "range";

bool is_keyword(char* p) {
    return strcmp(p, KW_DEFINE) == 0 || strcmp(p, KW_LIST) == 0 ||
           strcmp(p, KW_PRINT) == 0 || strcmp(p, KW_LAMBDA) == 0 ||
           strcmp(p, KW_MAP) == 0 || strcmp(p, KW_FILTER) == 0 ||
           strcmp(p, KW_REDUCE) == 0 || strcmp(p, KW_LENGTH) == 0 ||
           strcmp(p, KW_RANGE) == 0;
}

void print_token(FILE* stream, Token* token) {
    switch (token->kind) {
        case TK_LPAREN:
            fprintf(stream, "%c[LPAREN]", token->value.as_char);
            return;
        case TK_RPAREN:
            fprintf(stream, "%c[RPAREN]", token->value.as_char);
            return;
        case TK_BINARYOP:
            fprintf(stream, "%c[BINARYOP]", token->value.as_char);
            return;
        case TK_IF:
            fprintf(stream, "[IF]");
            return;
        case TK_KEYWORD:
            fprintf(stream, "%s[KEYWORD]", token->value.as_string->str);
            return;
        case TK_NUM:
            fprintf(stream, "%d[NUM]", token->value.as_int);
            return;
        case TK_SYMBOL:
            fprintf(stream, "%s[SYMBOL]", token->value.as_string->str);
            return;
        case TK_STRING:
            fprintf(stream, "%s[STRING]", token->value.as_string->str);
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

bool is_binaryop(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '>' ||
           c == '<' || c == '%' || c == '=' || c == '|' || c == '&';
}

bool is_parents(char c) { return c == '(' || c == ')'; }

Token* tokenize_number(char** p, Token* cur) {
    char* q = *p;
    int int_val = strtol(q, &q, 10);
    if (*q != '.') {  // parse as integer
        cur = new_token(TK_NUM, cur, *p);
        cur->value.as_int = int_val;
        *p = q;
        return cur;
    } else {  // parse as float
        cur = new_token(TK_FLOAT, cur, *p);
        double val = (double)strtof(*p, &(*p));
        cur->value.as_float = val;
        return cur;
    }
}

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
            cur = tokenize_number(&p, cur);
            if (cur->kind == TK_NUM) {
                cur->value.as_int *= -1;
            } else if (cur->kind == TK_FLOAT) {
                cur->value.as_float *= -1;
            }
            continue;
        }
        if (is_binaryop(*p)) {
            // parse as reserved charactor
            cur = new_token(TK_BINARYOP, cur, p);
            cur->value.as_char = *p;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            cur = tokenize_number(&p, cur);
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

        if (*p == '"') {  // parse string
            p++;          // skip '"'
            char* start = p;
            while (*p && *p != '"') {
                p++;
            }
            size_t len = p - start;
            String* str = new_string_with_len(start, len);
            cur = new_token(TK_STRING, cur, p);
            cur->value.as_string = str;
            p++;  // skip '"'
            continue;
        }

        // parse symbol
        {
            char* start = p;
            while (*p && !isspace(*p) && !is_binaryop(*p) && !is_parents(*p)) {
                p++;
            }
            size_t len = p - start;
            String* str = new_string_with_len(start, len);
            if (string_chars_eq(str, "if")) {
                cur = new_token(TK_IF, cur, start);
                continue;
            } else if (is_keyword(str->str)) {
                cur = new_token(TK_KEYWORD, cur, start);
                cur->value.as_string = str;
                continue;
            } else {
                cur = new_token(TK_SYMBOL, cur, start);
                cur->value.as_string = str;
                continue;
            }
        }

        error("failed to tokenize. input=%s", p);
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}