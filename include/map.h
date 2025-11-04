#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Bucket {
    const char *key;
    void *value;
    struct Bucket *next;
} Bucket;

typedef struct {
    Bucket **buckets;
    size_t bucket_count;
    bool owns_keys;
} HashMap;

static inline uint64_t
hashmap_hash(const char *key) {
    // FNV-1a 64-bit hash
    uint64_t hash = 1469598103934665603ull;
    for (const unsigned char *p = (const unsigned char*)key; *p; ++p)
        hash = (hash ^ *p) * 1099511628211ull;
    return hash;
}

static inline HashMap
hashmap_make(size_t bucket_count, bool owns_keys) {
    HashMap map = {0};
    map.bucket_count = bucket_count;
    map.owns_keys = owns_keys;
    map.buckets = calloc(bucket_count, sizeof(Bucket*));
    return map;
}

static inline void
hashmap_put(HashMap *map, const char *key, void *value) {
    uint64_t index = hashmap_hash(key) % map->bucket_count;
    Bucket *head = map->buckets[index], *prev = NULL;

    while (head) {
        if (strcmp(head->key, key) == 0) {
            head->value = value;
            return;
        }
        prev = head;
        head = head->next;
    }

    Bucket *entry = malloc(sizeof *entry);
    entry->key = map->owns_keys ? strdup(key) : key;
    entry->value = value;
    entry->next = NULL;

    if (prev)
        prev->next = entry;
    else
        map->buckets[index] = entry;
}

static inline void*
hashmap_get(HashMap *map, const char *key) {
    uint64_t index = hashmap_hash(key) % map->bucket_count;
    Bucket *b = map->buckets[index];
    while (b) {
        if (strcmp(b->key, key) == 0)
            return b->value;
        b = b->next;
    }
    return NULL;
}

static inline void
hashmap_free(HashMap *map) {
    for (size_t n = 0; n < map->bucket_count; n++) {
        Bucket *b = map->buckets[n];
        while (b) {
            Bucket *next = b->next;
            if (map->owns_keys)
                free((void*)b->key);
            free(b);
            b = next;
        }
    }
    free(map->buckets);
    map->buckets = NULL;
    map->bucket_count = 0;
}

