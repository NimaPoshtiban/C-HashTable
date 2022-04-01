#ifndef hash_table_h
#define hash_table_h
#endif

typedef struct
{
    char *key;
    char *value;
} ht_item;

typedef struct
{
    int size;
    int count;
    ht_item **items;
} ht_hash_table;
