#include "btree.h"
#include <stdio.h>

/**
 * @brief Displays the key and value of a binary search tree node.
 * 
 * @details Outputs the key and value of the given node in a standardized format
 *          for easy identification and review, typically during traversal or debugging.
 * 
 * @param node Pointer to the node of the binary search tree to print.
 * 
 * @pre 'node' should not be NULL and should point to a valid BST node containing a key
 *      and value to print.
 * 
 * @post The key and value of the node are printed to the standard output in the format
 *       "[key,value]". The state of the node and the tree remains unchanged.
 * 
 * @note This function is used throughout various traversal functions to process and
 *       display the contents of a node.
 * 
 * @code
 * bst_node_t *node = ...; // assume node is initialized and populated
 * bst_print_node(node); // Prints the node's key and value
 * @endcode
 * 
 * @warning There is no check for a NULL 'node' pointer; passing a NULL pointer will result
 *          in undefined behavior.
 * 
 * @return This function does not return a value.
 */
void bst_print_node(bst_node_t *node) {
    printf("[%c,%d]", node->key, node->value);
}

/* End of btree/btree.c */
