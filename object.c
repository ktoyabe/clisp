#include "object.h"

#include <stdlib.h>

Object* new_object(ObjectKind kind) {
    Object* obj = calloc(1, sizeof(Object));
    obj->kind = kind;
    return obj;
}

ObjectNode* new_node(ObjectNode* cur, Object* object) {
    ObjectNode* node = calloc(1, sizeof(ObjectNode));
    node->content = object;
    cur->next = node;

    return node;
}