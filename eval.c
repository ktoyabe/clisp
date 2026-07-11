#include "eval.h"

#include "common.h"

Object* eval_symbol(Object* obj, Env* env);

Object* eval_obj(Object* obj, Env* env) {
    switch (obj->kind) {
        case OK_VOID:
            return new_object(OK_VOID);
        case OK_BOOL: {
            return new_bool_object(obj->value.as_bool);
        }
        case OK_INTEGER: {
            return new_int_object(obj->value.as_int);
        }
        case OK_SYMBOL: {
            return eval_symbol(obj, env);
        }
        case OK_LIST: {
            return eval_list(obj->value.as_list, env);
        }
        default: {
            error("Unsupported ObjectKind.");
        }
    }
}

Object* eval_binary_op(ObjectNode* objs, Env* env) {
    Object* operator = objs->content;
    ObjectNode* lhs = objs->next;
    Object* lhs_val = eval_obj(lhs->content, env);

    ObjectNode* rhs = lhs->next;
    Object* rhs_val = eval_obj(rhs->content, env);

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
        case '>': {
            return new_bool_object(lhs_val->value.as_int >
                                   rhs_val->value.as_int);
        }
        case '<': {
            return new_bool_object(lhs_val->value.as_int <
                                   rhs_val->value.as_int);
        }
        case '=': {
            return new_bool_object(lhs_val->value.as_int ==
                                   rhs_val->value.as_int);
        }
        default: {
            error("eval_binary_op: unsupported operator '%c'",
                  operator->value.as_char);
        }
    }
}

Object* eval_if(ObjectNode* objs, Env* env) {
    ObjectNode* cond_obj = objs->next;
    ObjectNode* true_stmt_obj = cond_obj->next;
    ObjectNode* false_stmt_obj = true_stmt_obj->next;

    Object* cond = eval_obj(cond_obj->content, env);
    if (cond->kind != OK_BOOL) {
        error("Condition must be a bool");
    }

    if (cond->value.as_bool) {
        return eval_obj(true_stmt_obj->content, env);
    } else {
        return eval_obj(false_stmt_obj->content, env);
    }
}

Object* eval_define(ObjectNode* objs, Env* env) {
    ObjectNode* symbol_node = objs->next;
    ObjectNode* value_node = symbol_node->next;

    if (symbol_node->content->kind != OK_SYMBOL) {
        error("symbol node must be OK_SYBOL");
    }
    String* key = symbol_node->content->value.as_symbol;
    Object* value = value_node->content;
    env_set(env, key, value);

    return new_object(OK_VOID);
}

Object* eval_symbol(Object* obj, Env* env) {
    Object* value = env_get(env, obj->value.as_symbol);
    if (!value) {
        error("Undefined symbol. symbol=%s", obj->value.as_symbol);
    }
    return value;
}

Object* eval_list(ObjectNode* objs, Env* env) {
    Object* head = objs->content;
    switch (head->kind) {
        case OK_RESERVED: {
            return eval_binary_op(objs, env);
        }
        case OK_SYMBOL: {
            if (string_chars_eq(head->value.as_symbol, "if")) {
                return eval_if(objs, env);
            }
            if (string_chars_eq(head->value.as_symbol, "define")) {
                return eval_define(objs, env);
            }
        }
        default: {
            error("eval_list: Unsupported ObjectKind");
        }
    }
}
