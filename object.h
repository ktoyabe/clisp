#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "cstring.h"

typedef struct Object Object;

typedef enum {
    OK_VOID,
    OK_RESERVED,
    OK_INTEGER,
    OK_BOOL,
    OK_SYMBOL,
    OK_LAMBDA,
    OK_LIST,
    OK_EOF,
} ObjectKind;

typedef struct ObjectNode ObjectNode;

typedef union {
    char as_char;
    int as_int;
    bool as_bool;
    String* as_symbol;
    void* as_lambda;
    ObjectNode* as_list;
} ObjectValue;

struct Object {
    ObjectKind kind;
    ObjectValue value;
};

Object* new_object(ObjectKind kind);
Object* new_int_object(int val);
Object* new_bool_object(bool val);

struct ObjectNode {
    Object* content;
    ObjectNode* next;
};

ObjectNode* new_node(ObjectNode* cur, Object* object);
