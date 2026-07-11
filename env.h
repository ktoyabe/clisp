#pragma once
#include "hashmap.h"

typedef struct Env Env;

struct Env {
    Env* parent;
    HashMap* vars;
};

Env* newEnv();
Env* extend(Env* parent);
Object* env_get(Env* env, String* key);
void env_set(Env* env, String* key, Object* value);