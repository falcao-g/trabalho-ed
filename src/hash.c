#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEED 0x12345678

typedef struct {
    uintptr_t *table;
    int size;
    int max;
    uintptr_t deleted;
    char *(*get_key)(void *);
} thash;

uint32_t hashf(const char *str, uint32_t h) {
    /* One-byte-at-a-time Murmur hash
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

int hash_insere(thash *h, void *bucket) {
    uint32_t hash = hashf(h->get_key(bucket), SEED);
    int pos = hash % (h->max);
    int i = 0;

    // se esta cheio
    if (h->max == (h->size) + 1) {
        free(bucket);
        return EXIT_FAILURE;
    }

    // inserindo
    while (h->table[pos] != 0) {
        if (h->table[pos] == h->deleted)
            break;
        pos = (hash + i * hash) % h->max;
        i += 1;
    }

    h->table[pos] = (uintptr_t)bucket;
    h->size += 1;
    return EXIT_SUCCESS;
}

int hash_constroi(thash *h, int nbuckets, char *(*get_key)(void *)) {
    h->table = calloc(
        sizeof(void *),
        nbuckets + 1);  // pois para caber 10 elementos precisamos de 11 espaços

    if (h->table == NULL)
        return EXIT_FAILURE;

    h->max = nbuckets + 1;
    h->size = 0;
    h->deleted = (uintptr_t) & (h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;
}

void *hash_busca(thash h, const char *key) {
    uint32_t hash = hashf(key, SEED);
    int pos = hash % (h.max);
    int i = 0;

    while (h.table[pos] != 0) {
        if (strcmp(h.get_key((void *)h.table[pos]), key) == 0)
            return (void *)h.table[pos];
        pos = (hash + i * hash) % h.max;
        i += 1;
    }

    return NULL;
}

void **hash_busca_repetidos(thash h, const char *key) {
    uint32_t hash = hashf(key, SEED);
    int pos = hash % (h.max);
    int i = 0;
    void **ret = malloc(sizeof(void *));
    ret[0] = NULL;
    int rep = 0;

    while (h.table[pos] != 0) {
        if (strcmp(h.get_key((void *)h.table[pos]), key) == 0) {
            // se ja tem a cidade na lista, não adiciona
            int ja_tem = 0;
            for (int j = 0; ret[j] != NULL; j++) {
                if (ret[j] == (void *)h.table[pos]) {
                    ja_tem = 1;
                    break;
                }
            }
            if (ja_tem == 0) {
                ret = realloc(ret, sizeof(void *) * (rep + 1));
                ret[rep] = (void *)h.table[pos];
                ret[rep + 1] = NULL;
                rep += 1;
            }
        }
        pos = (hash + i * hash) % h.max;
        i += 1;
    }

    return ret;
}

int hash_remove(thash *h, const char *key) {
    uint32_t hash = hashf(key, SEED);
    int pos = hash % (h->max);
    int i = 0;

    while (h->table[pos] != 0) {
        if (strcmp(h->get_key((void *)h->table[pos]), key) == 0) {
            h->table[pos] = h->deleted;
            h->size -= 1;
            return EXIT_SUCCESS;
        }
        pos = (hash + i * hash) % h->max;
        i += 1;
    }

    return EXIT_FAILURE;
}

void hash_apaga(thash *h) {
    int pos;
    for (pos = 0; pos < h->max; pos++) {
        if (h->table[pos] != 0 && h->table[pos] != h->deleted)
            free((void *)h->table[pos]);
    }
    free(h->table);
}