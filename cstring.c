#include "cstring.h"

#include <stdlib.h>
#include <string.h>

String* new_string(char* src) {
    size_t len = strlen(src);
    String* val = (String*)calloc(1, sizeof(String));
    val->str = (char*)malloc(len + 1);
    val->len = len;
    strncpy(val->str, src, len);

    return val;
}

String* new_string_with_len(char* src, size_t len) {
    String* val = (String*)calloc(1, sizeof(String));
    val->str = (char*)malloc(len + 1);
    val->len = len;
    strncpy(val->str, src, len);
    val->str[len] = '\0';

    return val;
}

bool string_eq(String* lhs, String* rhs) {
    return strcmp(lhs->str, rhs->str) == 0;
}

bool string_chars_eq(String* lhs, char* rhs) {
    return strcmp(lhs->str, rhs) == 0;
}

String* string_concat(String* lhs, String* rhs) {
    return new_string(strcat(lhs->str, rhs->str));
}