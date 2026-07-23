#include "object.h"

#include <stdlib.h>

#include "common.h"
#include "env.h"

static const char* OK_VOID_STR = "OK_VOID";
static const char* OK_INTEGER_STR = "OK_INTEGER";
static const char* OK_BINARYOP_STR = "OK_BINARYOP";
static const char* OK_BOOL_STR = "OK_BOOL";
static const char* OK_SYMBOL_STR = "OK_SYMBOL";
static const char* OK_LAMBDA_STR = "OK_LAMBDA";
static const char* OK_LIST_STR = "OK_LIST";
static const char* OK_IF_STR = "OK_IF";
static const char* OK_KEYWORD_STR = "OK_KEYWORD";

const char* ObjectKind_to_str(ObjectKind kind) {
    switch (kind) {
        case OK_VOID:
            return OK_VOID_STR;
        case OK_INTEGER:
            return OK_INTEGER_STR;
        case OK_BINARYOP:
            return OK_BINARYOP_STR;
        case OK_BOOL:
            return OK_BOOL_STR;
        case OK_SYMBOL:
            return OK_SYMBOL_STR;
        case OK_LAMBDA:
            return OK_LAMBDA_STR;
        case OK_LIST:
            return OK_LIST_STR;
        case OK_KEYWORD:
            return OK_KEYWORD_STR;
    }
}

StringNode* newStringNode(StringNode* cur, String* value) {
    StringNode* node = (StringNode*)calloc(1, sizeof(StringNode));
    node->value = value;
    cur->next = node;

    return node;
}

size_t StringNode_len(StringNode* cur) {
    size_t len = 0;
    while (cur) {
        len++;
        cur = cur->next;
    }
    return len;
}

ObjectLambda* newObjectLambda(StringNode* params, ObjectNode* body,
                              struct Env* env) {
    ObjectLambda* o = (ObjectLambda*)calloc(1, sizeof(ObjectLambda));
    o->params = params;
    o->body = body;
    o->env = env;

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

Object* object_list_concat(Object* lhs, Object* rhs) {
    if (lhs->kind != OK_LIST) {
        error("object_list_const: lhs kind be OK_LIST. kind=%s",
              ObjectKind_to_str(lhs->kind));
    }
    if (rhs->kind != OK_LIST) {
        error("object_list_const: rhs kind be OK_LIST. kind=%s",
              ObjectKind_to_str(rhs->kind));
    }
    ObjectNode head;
    head.next = NULL;
    ObjectNode* tail = &head;
    ObjectNode* cur = lhs->value.as_list;
    while (cur) {
        tail = new_node(tail, cur->content);
        cur = cur->next;
    }

    cur = rhs->value.as_list;
    while (cur) {
        tail = new_node(tail, cur->content);
        cur = cur->next;
    }

    Object* o = new_object(OK_LIST);
    o->value.as_list = head.next;
    return o;
}

ObjectNode* new_node(ObjectNode* cur, Object* object) {
    ObjectNode* node = calloc(1, sizeof(ObjectNode));
    node->content = object;
    cur->next = node;

    return node;
}

size_t ObjectNode_len(ObjectNode* cur) {
    size_t len = 0;
    while (cur) {
        len++;
        cur = cur->next;
    }
    return len;
}

void print_obj_list(FILE* stream, Object* obj);

void print_obj_raw(FILE* stream, Object* obj) {
    switch (obj->kind) {
        case OK_BINARYOP:
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
            fprintf(stream, "%s", obj->value.as_string->str);
            return;
        case OK_STRING:
            fprintf(stream, "\"%s\"", obj->value.as_string->str);
            return;
        case OK_LAMBDA:
            fprintf(stream, "%s", "[LAMBDA]");
            return;
        case OK_IF:
            fprintf(stream, "%s", obj->value.as_string->str);
            return;
        case OK_KEYWORD:
            fprintf(stream, "%s", obj->value.as_string->str);
            return;
        case OK_LIST:
            print_obj_list(stream, obj);
            return;
        default:
            fprintf(stream, "UnknownKind. kind=%d", obj->kind);
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