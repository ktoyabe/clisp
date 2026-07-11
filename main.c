#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void free_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

void print_prompt() { printf("clisp> "); }

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read =
        getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <= 0) {
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = input_buffer->buffer_length - 1;
    input_buffer->buffer[bytes_read - 1] = '\0';
}

void eval_object(char* input) {
    Token* tokens = tokenize(input);
    Parser* parser = new_parser(tokens);
    ObjectNode* objs = parse(parser);
    Object* result = eval_list(objs);
    print_obj(stdout, result, 0);
}

int main(int argc, char** argv) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (strcmp(input_buffer->buffer, "exit") == 0) {
            break;
        }

        eval_object(input_buffer->buffer);
    }

    free_input_buffer(input_buffer);

    return 0;
}