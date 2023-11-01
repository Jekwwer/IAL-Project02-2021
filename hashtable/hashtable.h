/*
 * Header file for the hash table with scattered items.
 * Do not modify this file.
 */

#ifndef IAL_HASHTABLE_H
#define IAL_HASHTABLE_H

#include <stdbool.h>

/*
 * Maximum array size for the table implementation.
 * Functions working with the table assume the size HT_SIZE.
 */
#define MAX_HT_SIZE 101

/*
 * The size of the table that the implemented functions work with.
 * For testing purposes, it's useful to have the ability to change the table size.
 * To function properly, the size must be a prime number.
 */
extern int HT_SIZE;

// Table item
typedef struct ht_item {
  char *key;            // key
  float value;          // value
  struct ht_item *next; // pointer to the next synonym
} ht_item_t;

// Table with the actual size of MAX_HT_SIZE
typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

int get_hash(char *key);
void ht_init(ht_table_t *table);
ht_item_t *ht_search(ht_table_t *table, char *key);
void ht_insert(ht_table_t *table, char *key, float data);
float *ht_get(ht_table_t *table, char *key);
void ht_delete(ht_table_t *table, char *key);
void ht_delete_all(ht_table_t *table);

#endif

/* End of hashtable.h */
