#include "eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        case OK_FLOAT: {
            return new_float_object(obj->value.as_float);
        }
        case OK_SYMBOL: {
            return eval_symbol(obj, env);
        }
        case OK_STRING: {
            Object* o = new_object(OK_STRING);
            o->value.as_string = obj->value.as_string;
            return o;
        }
        case OK_LIST: {
            return eval_list(obj->value.as_list, env);
        }
        default: {
            error("Unsupported ObjectKind. kind=%s",
                  ObjectKind_to_str(obj->kind));
        }
    }
}

Object* eval_binary_op_int(char op, int lhs, int rhs) {
    switch (op) {
        case '+': {
            return new_int_object(lhs + rhs);
        }
        case '-': {
            return new_int_object(lhs - rhs);
        }
        case '*': {
            return new_int_object(lhs * rhs);
        }
        case '/': {
            return new_int_object(lhs / rhs);
        }
        case '%': {
            return new_int_object(lhs % rhs);
        }
        case '>': {
            return new_bool_object(lhs > rhs);
        }
        case '<': {
            return new_bool_object(lhs < rhs);
        }
        case '=': {
            return new_bool_object(lhs == rhs);
        }
        default: {
            error("eval_binary_op_int: unsupported operator '%c'", op);
        }
    }
}

Object* eval_binary_op_float(char op, double lhs, double rhs) {
    switch (op) {
        case '+': {
            return new_float_object(lhs + rhs);
        }
        case '-': {
            return new_float_object(lhs - rhs);
        }
        case '*': {
            return new_float_object(lhs * rhs);
        }
        case '/': {
            return new_float_object(lhs / rhs);
        }
        case '>': {
            return new_bool_object(lhs > rhs);
        }
        case '<': {
            return new_bool_object(lhs < rhs);
        }
        case '=': {
            return new_bool_object(lhs == rhs);
        }
        default: {
            error("eval_binary_op_int: unsupported operator '%c'", op);
        }
    }
}

Object* eval_binary_op_string(char op, String* lhs, String* rhs) {
    switch (op) {
        case '+': {
            Object* o = new_object(OK_STRING);
            o->value.as_string = string_concat(lhs, rhs);
            return o;
        }
        case '=': {
            return new_bool_object(string_eq(lhs, rhs));
        }
        default: {
            error("eval_binary_op_string: unsupported operator '%c'", op);
        }
    }
}

Object* eval_binary_op(ObjectNode* objs, Env* env) {
    Object* operator = objs->content;
    ObjectNode* lhs = objs->next;
    Object* lhs_val = eval_obj(lhs->content, env);
    ObjectKind lhs_kind = lhs_val->kind;

    ObjectNode* rhs = lhs->next;
    Object* rhs_val = eval_obj(rhs->content, env);
    ObjectKind rhs_kind = rhs_val->kind;

    if (lhs_kind == OK_INTEGER && rhs_kind == OK_INTEGER) {
        return eval_binary_op_int(operator->value.as_char,
                                  lhs_val->value.as_int, rhs_val->value.as_int);
    }
    if (lhs_kind == OK_STRING && rhs_kind == OK_STRING) {
        return eval_binary_op_string(operator->value.as_char,
                                     lhs_val->value.as_string,
                                     rhs_val->value.as_string);
    }
    if (lhs_kind == OK_FLOAT && rhs_kind == OK_FLOAT) {
        return eval_binary_op_float(operator->value.as_char,
                                    lhs_val->value.as_float,
                                    rhs_val->value.as_float);
    }
    if (lhs_kind == OK_FLOAT && rhs_kind == OK_INTEGER) {
        return eval_binary_op_float(operator->value.as_char,
                                    lhs_val->value.as_float,
                                    (double)rhs_val->value.as_int);
    }
    if (lhs_kind == OK_INTEGER && rhs_kind == OK_FLOAT) {
        return eval_binary_op_float(operator->value.as_char,
                                    (double)lhs_val->value.as_int,
                                    rhs_val->value.as_float);
    }
    if (lhs_kind == OK_BOOL && rhs_kind == OK_BOOL) {
        switch (operator->value.as_char) {
            case '|': {
                return new_bool_object(lhs_val->value.as_bool ||
                                       rhs_val->value.as_bool);
            }
            case '&': {
                return new_bool_object(lhs_val->value.as_bool &&
                                       rhs_val->value.as_bool);
            }
            default: {
                error("eval_binary_op: unsupported operator '%c'",
                      operator->value.as_char);
            }
        }
    }
    if (lhs_kind == OK_LIST && rhs_kind == OK_LIST) {
        switch (operator->value.as_char) {
            case '+': {
                return object_list_concat(lhs_val, rhs_val);
            }
            default: {
                error("eval_binary_op: unsupported operator '%c' in ObjstList",
                      operator->value.as_char);
            }
        }
    }
    error("unsupported binary_operator. op=%c, lhs_type=%s, rhs_type=%s",
          operator->value.as_char, ObjectKind_to_str(lhs_kind),
          ObjectKind_to_str(rhs_kind));
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
    String* key = symbol_node->content->value.as_string;
    Object* value = eval_obj(value_node->content, env);
    env_set(env, key, value);

    return new_object(OK_VOID);
}

StringNode* eval_params_define(ObjectNode* params, Env* env) {
    ObjectNode* param = params;
    StringNode head;
    StringNode* cur = &head;
    while (param) {
        if (param->content->kind != OK_SYMBOL) {
            error("params object kind must be symbol.");
        }
        cur = newStringNode(cur, param->content->value.as_string);
        param = param->next;
    }

    return head.next;
}

Object* eval_function_definition(ObjectNode* objs, Env* env) {
    ObjectNode* params_node = objs->next;
    if (!params_node) {
        error("eval_function_define: params_node must not be null.");
    }
    if (params_node->content->kind != OK_LIST) {
        error("params_node must be OK_LIST.");
    }
    StringNode* params =
        eval_params_define(params_node->content->value.as_list, env);

    ObjectNode* body_node = params_node->next;
    if (!body_node) {
        error("eval_function_define: body_node must not be null.");
    }

    ObjectLambda* lambda = newObjectLambda(params, body_node);
    Object* o = new_object(OK_LAMBDA);
    o->value.as_lambda = lambda;

    return o;
}

Object* eval_print(ObjectNode* objs, Env* env) {
    ObjectNode* node = objs->next;
    while (node) {
        Object* o = eval_obj(node->content, env);
        print_obj(stdout, o, 0);
        node = node->next;
    }
    Object* ret = new_object(OK_VOID);
    return ret;
}

Object* eval_function_call(ObjectNode* objs, Env* env) {
    String* function_name = objs->content->value.as_string;
    Object* lambda = env_get(env, function_name);
    if (!lambda) {
        error("undefined function. name=%s", function_name->str);
    }
    if (lambda->kind != OK_LAMBDA) {
        error("Not a lambda object.");
    }
    Env* function_scope = env_extend(env);
    ObjectNode* param = objs->next;
    StringNode* name = lambda->value.as_lambda->params;
    while (param) {
        Object* val = eval_obj(param->content, env);
        env_set(function_scope, name->value, val);
        param = param->next;
        name = name->next;
    }
    ObjectNode* body = lambda->value.as_lambda->body;
    if (body->content->kind != OK_LIST) {
        error("lambda body type must be OK_LIST. type=%s",
              ObjectKind_to_str(body->content->kind));
    }
    Object* ret = eval_list(body->content->value.as_list, function_scope);
    // TODO: free function_scope object

    return ret;
}

Object* eval_symbol(Object* obj, Env* env) {
    Object* value = env_get(env, obj->value.as_string);
    if (!value) {
        error("Undefined symbol. symbol=%s", obj->value.as_string);
    }
    return value;
}

Object* eval_list_data(ObjectNode* objs, Env* env) {
    ObjectNode head;
    head.next = NULL;
    ObjectNode* tail = &head;

    ObjectNode* cur = objs->next;  // skip list symbol node
    while (cur) {
        Object* o = eval_obj(cur->content, env);
        tail = new_node(tail, o);
        cur = cur->next;
    }

    Object* o = new_object(OK_LIST);
    o->value.as_list = head.next;

    return o;
}

#define ERROR_MSG_LEN 255

typedef struct {
    char* error_msg;
    Object* object;
} object_result;

object_result ObjectErr(const char* msg) {
    object_result result;
    result.object = NULL;
    result.error_msg = (char*)malloc(sizeof(strlen(msg) + 1));
    strcpy(result.error_msg, msg);
    return result;
}

object_result ObjectOk(Object* o) {
    object_result result;
    result.error_msg = NULL;
    result.object = o;

    return result;
}

object_result pickup_lambda_object(ObjectNode* func_node, Env* env) {
    if (!func_node) {
        return ObjectErr("function node is null.");
    }
    Object* func_obj = eval_obj(func_node->content, env);
    if (!func_obj || func_obj->kind != OK_LAMBDA) {
        char msg[ERROR_MSG_LEN];
        sprintf(msg, "function object kind must be Lambda. kind=%s",
                ObjectKind_to_str(func_node->content->kind));
        return ObjectErr(msg);
    }
    return ObjectOk(func_obj);
}

object_result pickup_list_object(ObjectNode* list_node, Env* env) {
    if (!list_node) {
        return ObjectErr("list node is null.");
    }
    Object* list = eval_obj(list_node->content, env);
    if (!list || list->kind != OK_LIST) {
        char msg[ERROR_MSG_LEN];
        sprintf(msg, "eval_map: list object must be List. kind=%s",
                ObjectKind_to_str(list->kind));
        return ObjectErr(msg);
    }
    return ObjectOk(list);
}

object_result pickup_int_object(ObjectNode* node, Env* env) {
    if (!node) {
        char msg[ERROR_MSG_LEN];
        sprintf(msg, "node is null.");
        return ObjectErr(msg);
    }
    Object* o = eval_obj(node->content, env);
    if (o->kind != OK_INTEGER) {
        char msg[ERROR_MSG_LEN];
        sprintf(msg, "object type must be int. kind=%s",
                ObjectKind_to_str(o->kind));
        return ObjectErr(msg);
    }
    return ObjectOk(o);
}

Object* eval_map(ObjectNode* objs, Env* env) {
    ObjectNode* func_node = objs->next;
    object_result result = pickup_lambda_object(func_node, env);
    if (result.error_msg) {
        error("eval_map: %s", result.error_msg);
    }
    ObjectLambda* lambda = result.object->value.as_lambda;
    size_t args_len = StringNode_len(lambda->params);
    if (args_len != 1) {
        error("eval_map: function args length must be 1. len=%d", args_len);
    }

    ObjectNode* list_node = func_node->next;
    result = pickup_list_object(list_node, env);
    if (result.error_msg) {
        error("eval_map: %s", result.error_msg);
    }
    if (list_node->next) {
        error("eval_map: list next value must be null.");
    }
    Object* list = result.object;

    // apply lambda function to list
    ObjectNode head;
    head.next = NULL;
    ObjectNode* tail = &head;
    ObjectNode* cur = list->value.as_list;
    String* arg = lambda->params->value;
    while (cur) {
        Env* func_scope = env_extend(env);
        env_set(func_scope, arg, cur->content);

        Object* o = eval_list(lambda->body->content->value.as_list, func_scope);
        tail = new_node(tail, o);
        cur = cur->next;
    }

    Object* ret = new_object(OK_LIST);
    ret->value.as_list = head.next;
    return ret;
}

Object* eval_filter(ObjectNode* objs, Env* env) {
    ObjectNode* func_node = objs->next;
    object_result result = pickup_lambda_object(func_node, env);
    if (result.error_msg) {
        error("eval_filter: %s", result.error_msg);
    }
    ObjectLambda* lambda = result.object->value.as_lambda;
    size_t args_len = StringNode_len(lambda->params);
    if (args_len != 1) {
        error("eval_filter: function args length must be 1. len=%d", args_len);
    }

    ObjectNode* list_node = func_node->next;
    result = pickup_list_object(list_node, env);
    if (result.error_msg) {
        error("eval_filter: %s", result.error_msg);
    }
    if (list_node->next) {
        error("eval_filter: list next value must be null.");
    }
    Object* list = result.object;

    // apply lambda function to list
    ObjectNode head;
    head.next = NULL;
    ObjectNode* tail = &head;
    ObjectNode* cur = list->value.as_list;
    String* arg = lambda->params->value;
    while (cur) {
        Env* func_scope = env_extend(env);
        env_set(func_scope, arg, cur->content);

        Object* o = eval_list(lambda->body->content->value.as_list, func_scope);
        if (o->kind != OK_BOOL) {
            error("eval_filter: labmda msut return bool type. kind=%s",
                  ObjectKind_to_str(o->kind));
        }
        if (o->value.as_bool) {
            tail = new_node(tail, cur->content);
        }
        cur = cur->next;
    }

    Object* ret = new_object(OK_LIST);
    ret->value.as_list = head.next;
    return ret;
}

Object* eval_reduce(ObjectNode* objs, Env* env) {
    ObjectNode* func_node = objs->next;
    object_result result = pickup_lambda_object(func_node, env);
    if (result.error_msg) {
        error("eval_reduce: %s", result.error_msg);
    }
    ObjectLambda* lambda = result.object->value.as_lambda;
    size_t args_len = StringNode_len(lambda->params);
    if (args_len != 2) {
        error("eval_reduce: function args length must be 2. len=%d", args_len);
    }

    ObjectNode* list_node = func_node->next;
    result = pickup_list_object(list_node, env);
    if (result.error_msg) {
        error("eval_reduce: %s", result.error_msg);
    }
    if (list_node->next) {
        error("eval_reduce: list next value must be null.");
    }
    Object* list = result.object;

    // apply lambda function to list
    ObjectNode head;
    head.next = NULL;
    ObjectNode* tail = &head;
    ObjectNode* cur = list->value.as_list;
    String* arg0 = lambda->params->value;
    String* arg1 = lambda->params->next->value;

    Object* acc = cur->content;  // list head
    if (!acc) {
        error("eval_reduce: list is empty.");
    }
    cur = cur->next;
    if (!cur) {
        error("eval_reduce: list length >= 2. length=1");
    }
    while (cur) {
        Env* func_scope = env_extend(env);
        env_set(func_scope, arg0, acc);
        env_set(func_scope, arg1, cur->content);

        Object* o = eval_list(lambda->body->content->value.as_list, func_scope);
        if (o->kind != acc->kind) {
            error(
                "eval_reduce: acc.kind != reduce_result.king . acc.kind=%s, "
                "reduce_result.king=%s",
                ObjectKind_to_str(acc->kind), ObjectKind_to_str(o->kind));
        }
        acc = o;
        cur = cur->next;
    }

    return acc;
}

Object* eval_length(ObjectNode* objs, Env* env) {
    ObjectNode* list_node = objs->next;
    object_result result = pickup_list_object(list_node, env);
    if (result.error_msg) {
        error("eval_map: %s", result.error_msg);
    }
    if (list_node->next) {
        error("eval_map: list next value must be null.");
    }
    Object* list = result.object;
    size_t len = ObjectNode_len(list->value.as_list);

    return new_int_object(len);
}

Object* eval_range(ObjectNode* objs, Env* env) {
    ObjectNode* begin_node = objs->next;
    object_result result = pickup_int_object(begin_node, env);
    if (result.error_msg) {
        error("eval_range at begin_node: %s", result.error_msg);
    }
    int begin = result.object->value.as_int;

    ObjectNode* end_node = begin_node->next;
    result = pickup_int_object(end_node, env);
    int end = result.object->value.as_int;

    ObjectNode* step_node = end_node->next;
    result = pickup_int_object(step_node, env);
    int step = result.object->value.as_int;

    if (step_node->next) {
        error("eval_range input length error.");
    }
    ObjectNode head;
    head.next = NULL;
    ObjectNode* tail = &head;
    for (int i = begin; i < end; i += step) {
        Object* o = new_int_object(i);
        tail = new_node(tail, o);
    }
    Object* list = new_object(OK_LIST);
    list->value.as_list = head.next;
    return list;
}

Object* eval_list(ObjectNode* objs, Env* env) {
    Object* head = objs->content;
    switch (head->kind) {
        case OK_BINARYOP: {
            return eval_binary_op(objs, env);
        }
        case OK_IF: {
            return eval_if(objs, env);
        }
        case OK_KEYWORD: {
            if (string_chars_eq(head->value.as_string, "define")) {
                return eval_define(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "lambda")) {
                return eval_function_definition(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "print")) {
                return eval_print(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "list")) {
                return eval_list_data(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "map")) {
                return eval_map(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "filter")) {
                return eval_filter(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "reduce")) {
                return eval_reduce(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "length")) {
                return eval_length(objs, env);
            }
            if (string_chars_eq(head->value.as_string, "range")) {
                return eval_range(objs, env);
            }
        }
        case OK_SYMBOL: {
            return eval_function_call(objs, env);
        }
        default: {
            return eval_obj(head, env);
        }
    }
}
