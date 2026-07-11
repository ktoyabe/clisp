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