#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char** argv) {
    char* input = " (+ (- 2 4) (* (+ 5 6) 4))";
    Token* tokens = tokenize(input);
    print_tokens(stdout, tokens);
    printf("-----------------\n");
    Parser* parser = new_parser(tokens);
    ObjectNode* objs = parse(parser);
    print_objs(stdout, objs);

    return 0;
}