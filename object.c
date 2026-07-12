#include "object.h"

#include <stdlib.h>

static const char* OK_VOID_STR = "OK_VOID";
static const char* OK_INTEGER_STR = "OK_INTEGER";
static const char* OK_RESERVED_STR = "OK_RESERVED";
static const char* OK_BOOL_STR = "OK_BOOL";
static const char* OK_SYMBOL_STR = "OK_SYMBOL";
static const char* OK_LAMBDA_STR = "OK_LAMBDA";
static const char* OK_LIST_STR = "OK_LIST";

const char* ObjectKind_to_str(ObjectKind kind) {
    switch (kind) {
        case OK_VOID:
            return OK_VOID_STR;
        case OK_INTEGER:
            return OK_INTEGER_STR;
        case OK_RESERVED:
            return OK_RESERVED_STR;
        case OK_BOOL:
            return OK_BOOL_STR;
        case OK_SYMBOL:
            return OK_SYMBOL_STR;
        case OK_LAMBDA:
            return OK_LAMBDA_STR;
        case OK_LIST:
            return OK_LIST_STR;
    }
}

StringNode* newStringNode(StringNode* cur, String* value) {
    StringNode* node = (StringNode*)calloc(1, sizeof(StringNode));
    node->value = value;
    cur->next = node;

    return node;
}

ObjectLambda* newObjectLambda(StringNode* params, ObjectNode* body) {
    ObjectLambda* o = (ObjectLambda*)calloc(1, sizeof(ObjectLambda));
    o->params = params;
    o->body = body;

    return o;
}

Object* new_object(ObjectKind kind) {
    Object* obj = calloc(1, sizeof(Object));
    obj->kind = kind;
    return obj;
}

Object* new_int_object(int val) {
    Object* o = new_object(OK_INTEGER);
    o->value.as_int = val;
    return o;
}

Object* new_float_object(double val) {
    Object* o = new_object(OK_FLOAT);
    o->value.as_float = val;
    return o;
}

Object* new_bool_object(bool val) {
    Object* o = new_object(OK_BOOL);
    o->value.as_bool = val;
    return o;
}

ObjectNode* new_node(ObjectNode* cur, Object* object) {
    ObjectNode* node = calloc(1, sizeof(ObjectNode));
    node->content = object;
    cur->next = node;

    return node;
}

void print_obj_raw(FILE* stream, Object* obj) {
    switch (obj->kind) {
        case OK_RESERVED:
            fprintf(stream, "%c", obj->value.as_char);
            return;
        case OK_VOID:
            fprintf(stream, "%s", "[VOID]");
            return;
        case OK_BOOL:
            fprintf(stream, "%s", (obj->value.as_bool ? "#t" : "#f"));
            return;
        case OK_INTEGER:
            fprintf(stream, "%d", obj->value.as_int);
            return;
        case OK_FLOAT:
            fprintf(stream, "%.4lf", obj->value.as_float);
            return;
        case OK_SYMBOL:
            fprintf(stream, "%s", obj->value.as_symbol->str);
            return;
        case OK_STRING:
            fprintf(stream, "\"%s\"", obj->value.as_string->str);
            return;
        case OK_LAMBDA:
            fprintf(stream, "%s", "[LAMBDA]");
            return;
        default:
            fprintf(stream, "UnknownKind. kind=%s",
                    ObjectKind_to_str(obj->kind));
            return;
    }
}

void print_obj_list(FILE* stream, Object* obj) {
    ObjectNode* node = obj->value.as_list;
    ObjectNode* cur = node;

    fprintf(stream, "(");
    int i = 0;
    while (cur) {
        if (i > 0) {
            fprintf(stream, " ");
        }
        print_obj_raw(stream, cur->content);
        cur = cur->next;
        i++;
    }
    fprintf(stream, ")");
}

void print_obj(FILE* stream, Object* obj, int indent) {
    fprintf(stream, "%*s", indent, "");
    switch (obj->kind) {
        case OK_LIST: {
            print_obj_list(stream, obj);
            break;
        }
        default: {
            print_obj_raw(stream, obj);
            break;
        }
    }
    fprintf(stream, "\n");
}

void print_objs(FILE* stream, ObjectNode* objs) {
    ObjectNode* cur = objs;
    int indent = 0;
    while (cur) {
        print_obj(stream, cur->content, indent);
        cur = cur->next;
    }
}