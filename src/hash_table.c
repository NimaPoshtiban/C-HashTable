#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"
#include "prime.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};
int HT_INITIAL_BASE_SIZE = 53;
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
//  creating hash tables of a certain size
static ht_hash_table *ht_new_sized(const int based_size)
{
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));

    if (ht == NULL)
    {
        exit(0);
    }

    ht->size = next_prime(based_size);

    ht->count = 0;

    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
    return ht;
}

// initialises a new hash table
ht_hash_table *ht_new(void)
{
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
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

// inserting item into hash table
void ht_insert(ht_hash_table *ht, const char *key, const char *value)
{
    ht_item *item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item *current_item = ht->items[index];
    int i = 1;
    while (current_item != NULL)
    {
        if (current_item != &HT_DELETED_ITEM)
        {
            // check if the item already exist in the hash table
            if (strcmp(current_item->key, key) == 0)
            {
                ht_del_item(current_item);
                ht->items[index] = item;
                return;
            }
            index = ht_get_hash(item->key, ht->size, i);
            current_item = ht->items[i];
            i++;
        }
    }
    ht->items[index] = item;
    ht->count++;
}

// searching for a value in the hash table
// returns null if no value found
char *ht_search(ht_hash_table *ht, const char *key)
{
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(key, item->key) == 0)
            {
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

// deleting item from hashTable
void ht_delete(ht_hash_table *ht, const char *key)
{
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}