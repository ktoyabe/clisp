#include "hashmap.h"

#include <stdlib.h>

HashMap* new_hash_map() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->nodes = NULL;

    return map;
}

hash_map_entry_node* add_entry(hash_map_entry_node* cur, HashMapEntry* entry) {
    hash_map_entry_node* node =
        (hash_map_entry_node*)malloc(sizeof(hash_map_entry_node));
    node->entry = entry;
    node->next = NULL;

    cur->next = node;

    return node;
}

HashMapEntry* newHashMapEntry(String* key, Object* value) {
    HashMapEntry* entry = (HashMapEntry*)malloc(sizeof(HashMapEntry));
    entry->key = key;
    entry->value = value;

    return entry;
}

HashMapEntry* hash_map_find(HashMap* map, String* key) {
    if (!map->nodes) {
        return NULL;
    }
    hash_map_entry_node* cur = map->nodes;
    while (cur) {
        if (string_eq(cur->entry->key, key)) {
            return cur->entry;
        }
        cur = cur->next;
    }
    return NULL;
}

void hash_map_put(HashMap* map, String* key, Object* value) {
    HashMapEntry* entry = hash_map_find(map, key);
    if (entry) {  // entry found
        entry->value = value;
        return;
    }
    if (!(map->nodes)) {
        hash_map_entry_node head;
        head.next = NULL;
        map->nodes = add_entry(&head, newHashMapEntry(key, value));
        return;
    }

    hash_map_entry_node* cur = map->nodes;
    hash_map_entry_node* prev = NULL;
    while (cur) {
        prev = cur;
        cur = cur->next;
    }
    // cur is null, prev is the last node of map->nodes.
    add_entry(prev, newHashMapEntry(key, value));
}
