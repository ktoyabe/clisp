#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstring.h"
#include "token.h"

int main(int argc, char** argv) {
    char* input = "(+ 1 2)";
    Token* tokens = tokenize(input);

    Token* t = tokens;
    while (t->kind != TK_EOF) {
        print_token(stdout, t);
        printf("\n");
        t = t->next;
    }
    return 0;
}