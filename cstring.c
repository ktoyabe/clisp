#include "cstring.h"

#include <stdlib.h>
#include <string.h>

String* new_string(char* src) {
    size_t len = strlen(src);
    String* val = (String*)malloc(sizeof(String));
    val->str = (char*)malloc(len + 1);
    val->len = len;
    strncpy(val->str, src, len);

    return val;
}

String* new_string_with_len(char* src, size_t len) {
    String* val = (String*)malloc(sizeof(String));
    val->str = (char*)malloc(len + 1);
    val->len = len;
    strncpy(val->str, src, len);

    return val;
}