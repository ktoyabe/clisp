#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "cstring.h"

typedef struct Object Object;
typedef struct ObjectNode ObjectNode;
typedef struct StringNode StringNode;

typedef enum {
    OK_VOID,
    OK_RESERVED,
    OK_INTEGER,
    OK_FLOAT,
    OK_BOOL,
    OK_SYMBOL,
    OK_STRING,
    OK_LAMBDA,
    OK_LIST,
} ObjectKind;

const char* ObjectKind_to_str(ObjectKind kind);

struct StringNode {
    String* value;
    StringNode* next;
};

StringNode* newStringNode(StringNode* cur, String* value);
size_t StringNode_len(StringNode* cur);

typedef struct {
    StringNode* params;
    ObjectNode* body;
} ObjectLambda;

ObjectLambda* newObjectLambda(StringNode* cur, ObjectNode* body);

typedef union {
    char as_char;
    int as_int;
    double as_float;
    bool as_bool;
    String* as_symbol;
    String* as_string;
    ObjectLambda* as_lambda;
    ObjectNode* as_list;
} ObjectValue;

struct Object {
    ObjectKind kind;
    ObjectValue value;
};

Object* new_object(ObjectKind kind);
Object* new_int_object(int val);
Object* new_float_object(double val);
Object* new_bool_object(bool val);
Object* object_list_concat(Object* lhs, Object* rhs);

struct ObjectNode {
    Object* content;
    ObjectNode* next;
};

ObjectNode* new_node(ObjectNode* cur, Object* object);

void print_objs(FILE* stream, ObjectNode* objs);
void print_obj(FILE* stream, Object* obj, int indent);