#include "eval.h"

#include "common.h"

const char* ObjectKind_to_str(ObjectKind kind) {
    switch (kind) {
        case OK_VOID:
            return "OK_VOID";
        case OK_INTEGER:
            return "OK_INTEGER";
        case OK_RESERVED:
            return "OK_RESERVED";
        case OK_BOOL:
            return "OK_BOOL";
        case OK_SYMBOL:
            return "OK_SYMBOL";
        case OK_LAMBDA:
            return "OK_LAMBDA";
        case OK_LIST:
            return "OK_LIST";
        case OK_EOF:
            return "OK_EOF";
    }
}

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
    Object* value = eval_obj(value_node->content, env);
    env_set(env, key, value);

    return new_object(OK_VOID);
}

StringNode* eval_params_define(ObjectNode* params, Env* env) {
    ObjectNode* param = params;
    StringNode head;
    StringNode* cur = &head;
    while (param->content->kind != OK_EOF) {
        if (param->content->kind != OK_SYMBOL) {
            error("params object kind must be symbol.");
        }
        cur = newStringNode(cur, param->content->value.as_symbol);
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
    while (node->content->kind != OK_EOF) {
        Object* o = eval_obj(node->content, env);
        print_obj(stdout, o, 0);
        node = node->next;
    }
    Object* ret = new_object(OK_VOID);
    return ret;
}

Object* eval_function_call(ObjectNode* objs, Env* env) {
    String* function_name = objs->content->value.as_symbol;
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
    while (param->content->kind != OK_EOF) {
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
            if (string_chars_eq(head->value.as_symbol, "lambda")) {
                return eval_function_definition(objs, env);
            }
            if (string_chars_eq(head->value.as_symbol, "print")) {
                return eval_print(objs, env);
            }
            return eval_function_call(objs, env);
        }
        default: {
            return eval_obj(head, env);
        }
    }
}
