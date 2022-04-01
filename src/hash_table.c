#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

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