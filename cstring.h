#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char* str;
    size_t len;
} String;

String* new_string(char* src);
String* new_string_with_len(char* src, size_t len);

bool string_eq(String* lhs, String* rhs);
bool string_chars_eq(String* lhs, char* rhs);

String* string_concat(String* lhs, String* rhs);