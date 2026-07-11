#include "eval.h"

#include "common.h"

Object* eval_obj(Object* obj) {
    switch (obj->kind) {
        case OK_VOID:
            return new_object(OK_VOID);
        case OK_BOOL: {
            return new_bool_object(obj->value.as_bool);
        }
        case OK_INTEGER: {
            return new_int_object(obj->value.as_int);
        }
        case OK_LIST: {
            return eval_list(obj->value.as_list);
        }
        default: {
            error("Unsupported ObjectKind.");
        }
    }
}

Object* eval_binary_op(ObjectNode* objs) {
    Object* operator = objs->content;
    ObjectNode* lhs = objs->next;
    Object* lhs_val = eval_obj(lhs->content);

    ObjectNode* rhs = lhs->next;
    Object* rhs_val = eval_obj(rhs->content);

    switch (operator->value.as_char) {
        case '+': {
            return new_int_object(lhs_val->value.as_int +
                                  rhs_val->value.as_int);
        }
        case '-': {
            return new_int_object(lhs_val->value.as_int -
                                  rhs_val->value.as_int);
        }
        case '*': {
            return new_int_object(lhs_val->value.as_int *
                                  rhs_val->value.as_int);
        }
        case '/': {
            return new_int_object(lhs_val->value.as_int /
                                  rhs_val->value.as_int);
        }
        default: {
            error("eval_binary_op: unsupported operator '%c'",
                  operator->value.as_char);
        }
    }
}

Object* eval_list(ObjectNode* objs) {
    Object* head = objs->content;
    switch (head->kind) {
        case OK_RESERVED: {
            return eval_binary_op(objs);
        }
        default: {
            error("eval_list: Unsupported ObjectKind");
        }
    }
}
