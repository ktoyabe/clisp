#pragma once

#include "env.h"
#include "object.h"

Object* eval_obj(Object* obj, Env* env);
Object* eval_list(ObjectNode* objs, Env* env);