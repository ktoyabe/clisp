#pragma once

#include "object.h"

typedef struct {
    String* key;
    Object* value;
} HashMapEntry;

typedef struct hash_map_entry_node hash_map_entry_node;

struct hash_map_entry_node {
    HashMapEntry* entry;
    hash_map_entry_node* next;
};

// TODO: use HashTable Data Structure.
// Current implemantation is key-value pair linked-list.
typedef struct {
    hash_map_entry_node* nodes;
} HashMap;

HashMap* new_hash_map();
HashMapEntry* hash_map_find(HashMap* map, String* key);
void hash_map_put(HashMap* map, String* key, Object* value);