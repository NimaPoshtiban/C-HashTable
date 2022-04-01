#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"

int HT_PRIME_1 = 163;
int HT_PRIME_2 = 199;

//  initialisation functions for ht_item
// This function allocates a chunk of memory the size of an ht_item,
// and saves a copy of the strings k and v in the new chunk of memory
static ht_item *ht_new_item(const char *k, const char *v)
{
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

// initialises a new hash table
// TODO: change the size of the hash table
ht_hash_table *ht_new(void)
{
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));
    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
    return ht;
}

// deletes item from hash table
static void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

// deletes the hash table
void ht_del_hash_table(ht_hash_table *ht)
{
    for (size_t i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL)
        {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

// hash function
static int ht_hash(const char *s, const int a, const int m)
{
    long hash = 0;
    const int len_s = strlen(s);
    for (size_t i = 0; i < len_s; i++)
    {
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

// hash collision resolution function
static int ht_get_hash(const char *s, const int num_buckets, const int attempt)
{
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}