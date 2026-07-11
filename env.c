#include "env.h"

#include <stdlib.h>

Env* newEnv() {
    Env* env = (Env*)malloc(sizeof(Env));
    env->parent = NULL;
    env->vars = new_hash_map();

    return env;
}

Env* extend(Env* parent) {
    Env* env = newEnv();
    env->parent = parent;
}

Object* env_get(Env* env, String* key) {
    HashMapEntry* entry = hash_map_find(env->vars, key);
    if (entry) {
        return entry->value;
    }
    if (env->parent) {
        return env_get(env->parent, key);
    }
    return NULL;
}

void env_set(Env* env, String* key, Object* value) {
    hash_map_put(env->vars, key, value);
}