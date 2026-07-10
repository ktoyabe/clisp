#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char* str;
    size_t len;
} String;

String* new_string(char* src);
String* new_string_with_len(char* src, size_t len);