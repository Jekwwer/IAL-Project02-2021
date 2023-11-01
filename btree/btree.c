#include "btree.h"
#include <stdio.h>

/*
 * A helper function that prints the tree node.
 */
void bst_print_node(bst_node_t *node) {
  printf("[%c,%d]", node->key, node->value);
}

/* End of btree/btree.c */
