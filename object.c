#include "object.h"

#include <stdlib.h>

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

void print_obj(FILE* stream, Object* obj, int indent) {
    switch (obj->kind) {
        case OK_RESERVED:
            fprintf(stream, "%*c\n", indent, obj->value.as_char);
            return;
        case OK_VOID:
            fprintf(stream, "%*s\n", indent, "[VOID]");
            return;
        case OK_BOOL:
            fprintf(stream, "%*c\n", indent, obj->value.as_bool);
            return;
        case OK_INTEGER:
            fprintf(stream, "%*d\n", indent, obj->value.as_int);
            return;
        case OK_LIST: {
            int new_indent = indent + 4;
            ObjectNode* cur = obj->value.as_list;
            while (cur->content->kind != OK_EOF) {
                print_obj(stream, cur->content, new_indent);
                cur = cur->next;
            }
            return;
        }
        case OK_LAMBDA:
            fprintf(stream, "%*s\n", indent, "[LAMBDA]");
            return;
        default:
            fprintf(stream, "UnknownKind");
            return;
    }
}

void print_objs(FILE* stream, ObjectNode* objs) {
    ObjectNode* cur = objs;
    int indent = 0;
    while (cur->content->kind != OK_EOF) {
        print_obj(stream, cur->content, indent);
        cur = cur->next;
    }
}