#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "env.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"

char* read_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        error("filed to open file. file=%s", filename);
    }

    int ret = fseek(fp, 0, SEEK_END);  // move to file-pointer to end
    if (ret != 0) {
        error("filed to seek end. ret=%d", ret);
    }
    long fsize = ftell(fp);
    ret = fseek(fp, 0L, SEEK_SET);  // move to file-pointer to head
    if (ret != 0) {
        error("filed to seek set. ret=%d", ret);
    }

    char* buffer = (char*)malloc((size_t)(fsize + 1) * sizeof(char));

    // read file
    fsize = fread(buffer, sizeof(char), (size_t)fsize, fp);
    fclose(fp);

    buffer[fsize] = '\0';

    return buffer;
}

int repl();

void eval_object_from_file(char* filename, Env* env) {
    char* input = read_file(filename);
    Token* tokens = tokenize(input);
    Parser* parser = new_parser(tokens);
    ObjectNode* objs = parse(parser);

    ObjectNode* cur = objs;
    while (cur) {
        Object* result = eval_list(cur, env);
        cur = cur->next;
    }
}

int main(int argc, char** argv) {
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        Env* env = newEnv();
        eval_object_from_file(argv[1], env);
    }
    return 0;
}