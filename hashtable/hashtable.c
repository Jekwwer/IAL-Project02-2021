/*
 * Hashtable
 *
 * Using data types from the file hashtable.h and prepared function skeletons,
 * implement a hashtable with explicitly linked synonyms.
 *
 * When implementing, consider the size of the table HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/**
 * @brief Duplicate a string by allocating new memory and copying its content.
 *
 * This function creates a new copy of the given string by allocating the necessary 
 * memory dynamically and then copying the content of the original string to the 
 * newly allocated space. The caller is responsible for freeing the memory of the 
 * duplicated string using free().
 *
 * @param s Pointer to the original string to be duplicated.
 * @return Returns a pointer to the duplicated string or NULL if memory allocation fails.
 */
char* strdup(const char* s) {
    char* duplicate = malloc(strlen(s) + 1);  // Allocate memory for the duplicated string
    if (duplicate == NULL) return NULL;       // Check if allocation succeeded
    strcpy(duplicate, s);                     // Copy the original string to the new one
    return duplicate;                         // Return the new string
}

/*
 * Hash function that assigns a given key an index from the interval
 * <0,HT_SIZE-1>. An ideal hash function would distribute keys
 * evenly across all indices. Reflect on the quality of the chosen function.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }
    return (result % HT_SIZE);
}

/*
 * Table initialization — call before the table's first use.
 */
void ht_init(ht_table_t *table) {

    // Check for NULL
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < MAX_HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Search for an item in the table.
 *
 * If successful, returns a pointer to the found item; otherwise, returns NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

    // Check for NULL
    if (table == NULL) {
        return NULL;
    }

    // Getting an index from the table
    int index = get_hash(key);
    // Storing a pointer to the cell with our index
    ht_item_t *cellElement = (*table)[index];

    // Loop until we go through the entire cell
    while (cellElement != NULL) {
        // If the keys match
        if (cellElement->key == key) {
            return cellElement;
        }
        // Moving further in the cell
        cellElement = cellElement->next;
    }

    // Nothing was found
    return NULL;
}

/*
 * Inserting a new item into the table.
 *
 * If an item with a given key already exists in the table, replace its value.
 *
 * Use the ht_search function in the implementation. When inserting an item into 
 * the list of synonyms, choose the most efficient option and insert the item 
 * at the beginning of the list.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

    // Check for NULL
    if (table == NULL) {
        return;
    }

    // Searching for an element with this key in the table
    ht_item_t *element = ht_search(table, key);
    // If it's in the table
    if (element != NULL) {
        element->value = value;
    }
    else {// Not in the table
        // Allocate a new item
        ht_item_t *newElement = malloc(sizeof(ht_item_t));
        if (newElement == NULL) {
            return;
        }
        newElement->key = strdup(key);
        if (newElement->key == NULL) {
            free(newElement);
            return;
        }
        newElement->value = value;
        newElement->next = NULL;

        // Getting an index from the table
        int index = get_hash(key);

        // If the cell is empty
        if ((*table)[index] == NULL) {
            (*table)[index] = newElement;
        }
        else {// Cell is not empty
            // Copy the pointer to the first item in the cell
            ht_item_t *tmp = (*table)[index];
            // Inserting the item as the first in the cell
            (*table)[index] = newElement;
            // Update the pointer to the rest of the cell
            newElement->next = tmp;
        }
    }
}

/*
 * Retrieve value from the table.
 *
 * If successful, the function returns a pointer to the item's value; 
 * otherwise, it returns NULL.
 *
 * Use the ht_search function in the implementation.
 */
float *ht_get(ht_table_t *table, char *key) {

    // Check for NULL
    if (table == NULL) {
        return NULL;
    }

    // Searching for an element with this key in the table
    ht_item_t *element = ht_search(table, key);
    // If it's in the table
    if (element != NULL) {
        return &(element->value);
    }
    else {// Not in the table
        return NULL;
    }
}

/*
 * Delete an item from the table.
 *
 * The function will correctly release all allocated resources associated 
 * with the given item. If the item does not exist, do nothing.
 *
 * Do NOT use the ht_search function in the implementation.
 */
void ht_delete(ht_table_t *table, char *key) {

    // Check for NULL
    if (table == NULL) {
        return;
    }

    // Getting an index from the table
    int index = get_hash(key);
    // Storing a pointer to the cell with our index
    ht_item_t *cellElement = (*table)[index];
    // Storing a pointer to the previous cell item
    ht_item_t *prevCellElement = NULL;

    bool isDeleted = false;

    // Loop until the item is deleted or not found (if it's in the table)
    while (!isDeleted && cellElement != NULL) {
        // If found
        if (cellElement->key == key) {
            // If it's the first in the cell
            if (prevCellElement == NULL) {
                // Update the start of the cell
                (*table)[index] = cellElement->next;
            }
            else {// It's not the first
                prevCellElement->next = cellElement->next;
            }
            // Free the item
            free(cellElement);
            // Mark as deleted
            isDeleted = true;
        }
        // Updating pointers for navigating the cell
        prevCellElement = cellElement;
        cellElement = cellElement->next;
    }
}

/*
 * Delete all items from the table.
 *
 * The function will correctly release all allocated resources and 
 * reset the table to its post-initialization state.
 */
void ht_delete_all(ht_table_t *table) {
    // Check for NULL
    if (table == NULL) {
        return;
    }

    // Looping through each cell
    for (int i = 0; i < MAX_HT_SIZE; i++) {
        ht_item_t *current = (*table)[i];
        // Looping through each item in the cell and deleting it
        while (current != NULL) {
            ht_item_t *nextItem = current->next;
            free(current->key);
            free(current);
            current = nextItem;
        }
        // Reset the table entry to NULL after deleting all items in the cell
        (*table)[i] = NULL;
    }
}

/* End of hashtable.c */
