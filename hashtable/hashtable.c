/**
 * @file hashtable.c
 * @brief Hashtable implementation with explicitly linked synonyms.
 * @details Implements a hashtable using the data structures defined in hashtable.h.
 *          This file includes functions for initializing the table, searching for items, 
 *          inserting and deleting entries, and more. The hashtable supports explicit
 *          linking of synonyms (items with the same hash) and is designed to handle
 *          collisions using chaining.
 * 
 *          Key functions implemented:
 *          - ht_init: Initializes the hashtable.
 *          - ht_search: Searches for an item in the hashtable.
 *          - ht_insert: Inserts a new item into the hashtable.
 *          - ht_get: Retrieves an item's value from the hashtable.
 *          - ht_delete: Removes an item from the hashtable.
 *          - ht_delete_all: Deletes all items from the hashtable.
 * 
 *          The hashtable is of fixed size MAX_HT_SIZE, and the hash function used
 *          is defined as `get_hash`.
 * 
 * @note This code file is designed for task-specific implementation and solving. 
 *       It is not meant to function as a standalone program. 
 *       However, it is modular and can be integrated into any larger program as needed.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table);
 * ht_insert(&my_table, "key1", 1.0f);
 * float *value = ht_get(&my_table, "key1");
 * if (value) {
 *     printf("Found value: %f\n", *value);
 * }
 * ht_delete_all(&my_table);
 * @endcode
 *
 * @see hashtable.h for type definitions and constants.
 * 
 * @see https://github.com/Jekwwer/IAL-Project02-2021 for the project repository.
 * 
 * @author Evgenii Shiliaev 
 * @date December 2021 (solution)
 *       November 2023 (code comments updated)
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/**
 * @brief Allocates memory and duplicates a given string.
 * 
 * @details Allocates the necessary memory for a duplicate of the provided string 's' and
 *          copies its contents into the new memory location. The responsibility to free
 *          the allocated memory for the duplicate string falls on the caller of this function.
 * 
 * @param s The string to be duplicated.
 * 
 * @pre 's' must be a valid null-terminated string.
 * 
 * @post On success, returns a pointer to a new string which is a duplicate of 's'. On failure,
 *       returns NULL.
 * 
 * @note This function is a common utility and may be replaced by `strdup` from the standard
 *       library if available.
 * 
 * @code
 * const char *original = "sample";
 * char *copy = strdup(original);
 * if (copy) {
 *     printf("Duplicated string: %s\n", copy);
 *     free(copy); // Remember to free the duplicated string
 * }
 * @endcode
 * 
 * @todo Ensure proper error handling when using this function
 *       due to potential memory allocation failure.
 * 
 * @warning The caller must free the memory allocated for the duplicated string to avoid leaks.
 * 
 * @retval NULL Memory allocation failed, or 's' was NULL.
 * @retval non-NULL Pointer to the duplicated string.
 */
char *strdup(const char *s) {
    char *duplicate = malloc(strlen(s) + 1);// Allocate memory for the duplicated string
    if (duplicate == NULL) return NULL;     // Check if allocation succeeded
    strcpy(duplicate, s);                   // Copy the original string to the new one
    return duplicate;                       // Return the new string
}

/**
 * @brief Computes a hash value for a given string key.
 * 
 * @details The hash function takes a string key and computes a hash value, which
 *          is an index in the interval [0, HT_SIZE - 1]. The function sums the ASCII
 *          values of the characters in the key and then takes the modulus with the size
 *          of the hash table to ensure the result fits within the table's bounds.
 * 
 * @param key The string key for which the hash value is to be computed.
 * 
 * @pre 'key' must be a valid null-terminated string.
 * 
 * @post The function returns an integer hash value corresponding to the input string key.
 * 
 * @note The effectiveness of the hash function is critical to the performance of the hash table.
 *       It should distribute keys uniformly across all indices.
 * 
 * @code
 * char *my_key = "example";
 * int index = get_hash(my_key);
 * @endcode
 * 
 * @todo Evaluate and possibly enhance the hash function to improve its distribution properties,
 *       especially for a large number of keys.
 * 
 * @warning The quality of the hash function directly impacts the performance of the hash table
 *          in terms of search, insert, and delete operations.
 * 
 * @retval The computed hash value for the given key.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }
    return (result % HT_SIZE);
}

/**
 * @brief Initializes a hashtable.
 * 
 * @details Sets all entries in the provided hashtable to NULL, preparing the table for use.
 *          This function must be called before any operations are performed on the hashtable.
 * 
 * @param table A pointer to the hashtable to be initialized.
 * 
 * @pre 'table' must not be NULL and should reference an array of pointers with size MAX_HT_SIZE.
 * 
 * @post The hashtable is initialized, with all entries set to NULL.
 * 
 * @note It is essential to call this function before using the hashtable to prevent undefined behavior.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table); // Initializes the hashtable
 * @endcode
 * 
 * @todo No todos are necessary for this function as it serves a simple and direct purpose.
 * 
 * @warning Failing to initialize the hashtable before use can result in undefined behavior.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Searches for an item with the specified key in the hashtable.
 * 
 * @details Traverses the hashtable looking for an item that matches the provided key.
 *          If found, it returns a pointer to the item. Otherwise, it returns NULL.
 *          The search is conducted within the linked list at the index determined by the hash function.
 * 
 * @param table A pointer to the hashtable.
 * @param key The key of the item to search for.
 * 
 * @pre 'table' must be a valid, initialized hashtable. 'key' must be a null-terminated string.
 * 
 * @post If an item with the key is found, a pointer to it is returned. Otherwise, NULL is returned.
 * 
 * @note The search time depends on the distribution of items in the hashtable and could degrade
 *       to O(n) if the hash function does not distribute keys well.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table);
 * // assume items have been inserted into 'my_table'
 * ht_item_t *item = ht_search(&my_table, "my_key");
 * @endcode
 * 
 * @todo Ensure that the comparison of keys is done correctly, considering the possibility of
 *       string interning.
 * 
 * @warning If 'table' is NULL or not properly initialized, the function may return incorrect results.
 * 
 * @retval NULL The key was not found or 'table' is NULL.
 * @return A pointer to the found item or NULL if the item is not found.
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

/**
 * @brief Inserts or updates an item in the hashtable.
 * 
 * @details If an item with the specified key exists, its value is updated. Otherwise, a new
 *          item is created and inserted into the hashtable. The new item is added to the
 *          beginning of the synonym list for efficiency. The `ht_search` function is used
 *          to check for the existence of the item.
 * 
 * @param table A pointer to the hashtable.
 * @param key The key associated with the item.
 * @param value The value to be inserted or updated in the hashtable.
 * 
 * @pre 'table' should be an initialized hashtable, and 'key' should be a null-terminated string.
 * 
 * @post The item with the specified key has its value updated, or a new item is added to the
 *       hashtable.
 * 
 * @note Utilizes string duplication and hashing functions for item insertion.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table);
 * ht_insert(&my_table, "key1", 1.0f); // Inserts or updates an item with key "key1"
 * @endcode
 * 
 * @todo Handle memory allocation failures more gracefully, potentially returning a status code.
 * 
 * @warning The function assumes that 'key' points to a valid null-terminated string. If 'key'
 *          is NULL or memory allocation fails, the insertion will not be completed.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Retrieves the value associated with a key from the hashtable.
 * 
 * @details Uses the `ht_search` function to locate the item corresponding to the given key.
 *          If the item is found, returns a pointer to its value; otherwise, returns NULL.
 * 
 * @param table A pointer to the hashtable.
 * @param key The key string associated with the desired value.
 * 
 * @pre 'table' must be a valid and initialized hashtable, and 'key' must be a null-terminated string.
 * 
 * @post No modifications are made to the hashtable. If the key is found, a pointer to its value is returned.
 * 
 * @note This function is commonly used to retrieve the value of an element if it exists in the hashtable.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table);
 * // assume items have been inserted into 'my_table'
 * float *value = ht_get(&my_table, "my_key");
 * if (value) {
 *     printf("Value found: %f\n", *value);
 * }
 * @endcode
 * 
 * @todo Ensure that the keys are being compared securely, accounting for potential hash collisions.
 * 
 * @warning This function will return NULL not only when the key is not found but also if 'table' is NULL.
 * 
 * @retval NULL If the key is not found or 'table' is NULL.
 * @retval non-NULL A pointer to the value associated with the key.
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

/**
 * @brief Removes an item with a specified key from the hashtable.
 * 
 * @details Searches for and, if found, deletes the item from the hashtable that matches the provided key.
 *          If the item exists, all associated memory is released. The item's removal is performed
 *          directly without using the `ht_search` function to navigate the hashtable.
 * 
 * @param table A pointer to the hashtable.
 * @param key The key of the item to delete.
 * 
 * @pre 'table' must be a valid, initialized hashtable, and 'key' must be a null-terminated string.
 * 
 * @post If the item is found, it is removed from the hashtable and its memory is freed.
 * 
 * @note The deletion process involves searching for the item, unlinking it from the chain, and freeing its memory.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table);
 * // assume items have been inserted into 'my_table'
 * ht_delete(&my_table, "my_key");
 * @endcode
 * 
 * @todo Consider returning a status code to indicate whether the deletion was successful or if the item was not found.
 * 
 * @warning The function does not validate the 'key' parameter against NULL. Passing a NULL 'key' can lead to undefined behavior.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Deletes all items from the hashtable and frees associated memory.
 * 
 * @details Iterates over all entries in the hashtable, freeing memory for each item and its key.
 *          Resets each entry in the hashtable to NULL, effectively returning the hashtable to its
 *          initial state after `ht_init`.
 * 
 * @param table A pointer to the hashtable from which all items are to be deleted.
 * 
 * @pre 'table' must be a valid, initialized hashtable.
 * 
 * @post The hashtable is emptied, with all entries set to NULL, and all dynamically allocated memory is freed.
 * 
 * @note This function is intended for final cleanup of a hashtable, ensuring no memory leaks occur.
 * 
 * @code
 * ht_table_t my_table;
 * ht_init(&my_table);
 * // assume items have been added to 'my_table'
 * ht_delete_all(&my_table);
 * // 'my_table' is now in its post-initialization state
 * @endcode
 * 
 * @warning Failing to call this function before discarding a hashtable can lead to memory leaks.
 * 
 * @return This function does not return a value.
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
