#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char** argv) {
    char* input = " (+ (- 1 3) (* 2 (/ 8 2)))";
    Token* tokens = tokenize(input);
    print_tokens(stdout, tokens);
    printf("-----------------\n");
    Parser* parser = new_parser(tokens);
    ObjectNode* objs = parse(parser);
    print_objs(stdout, objs);

    printf("-------------------\n");
    Object* result = eval_list(objs);
    print_obj(stdout, result, 0);

    return 0;
}