/*
 * Binary search tree — recursive variant
 *
 * Using data types from the file btree.h and prepared function skeletons,
 * implement a binary search tree using recursion.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Tree initialization.
 *
 * The user must ensure that the initialization is not repeatedly called on an
 * already initialized tree. Otherwise, there may be a memory leak. Since an uninitialized
 * pointer has an undefined value, it is not possible to detect this in the function.
 */
void bst_init(bst_node_t **tree) {
    // NULL check
    if (tree != NULL) {
        *tree = NULL;
    }
}

/*
 * Finding a node in the tree.
 *
 * If successful, the function returns true and writes the value of the node
 * to the 'value' variable. Otherwise, the function returns false and 'value' remains unchanged.
 *
 * Implement the function recursively without using your own helper functions.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

    // If the tree is empty
    if (tree == NULL) {
        return false;
    }
    // If the node with the searched key is found
    else if (tree->key == key) {
        *value = tree->value;
        return true;
    }
    else {// The node with the searched key was not found
        // If the key is on the left
        if (key < tree->key) {
            return (bst_search(tree->left, key, value));
        }
        else {// The key is on the right
            return (bst_search(tree->right, key, value));
        }
    }
}

/*
 * Inserting a node into the tree.
 *
 * If a node with the specified key already exists in the tree, replace its value.
 * Otherwise, insert a new leaf node.
 *
 * The resulting tree must satisfy the search tree condition — the left subtree of a node
 * contains only smaller keys, and the right one contains larger keys.
 *
 * Implement the function recursively without using your own helper functions.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Current tree root
    bst_node_t *rootPtr = *tree;

    // If the subtree is empty
    if (rootPtr == NULL) {
        // Allocate and initialize it
        rootPtr = malloc(sizeof(bst_node_t));
        if (rootPtr == NULL) {
            return;
        }
        rootPtr->key = key;
        rootPtr->left = NULL;
        rootPtr->right = NULL;
        rootPtr->value = value;

        // If the entire tree is empty
        if (*tree == NULL) {
            *tree = rootPtr;
        }
    }
    else {// The subtree is not empty
        // If the key is on the left
        if (key < rootPtr->key) {
            bst_insert(&rootPtr->left, key, value);
        }
        // If the key is on the right
        else if (rootPtr->key < key) {
            bst_insert(&rootPtr->right, key, value);
        }
        // The keys are equal
        else {
            rootPtr->value = value;
        }
    }
}

/*
 * Helper function to replace a node with the rightmost descendant.
 *
 * The key and value of the target node will be replaced with the key and 
 * value of the rightmost node of the tree subtree. The rightmost child will be removed.
 * The function properly frees all allocated resources of the removed node.
 *
 * The function assumes that the tree value is not NULL.
 *
 * This helper function will be used in the implementation of the bst_delete function.
 *
 * Implement the function recursively without using your own helper functions.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Current tree root
    bst_node_t *rootPrt = *tree;
    // If there's a path to the right, move right
    if (rootPrt->right != NULL) {
        bst_replace_by_rightmost(target, &rootPrt->right);
    }
    else {// No path to the right, update target and remove the node
        target->key = rootPrt->key;
        target->value = rootPrt->value;
        bst_delete(tree, rootPrt->key);
    }
}

/*
 * Removing a node from the tree.
 *
 * If a node with the specified key does not exist, the function does nothing.
 * If the removed node has one subtree, it is inherited by the parent of the removed node.
 * If the removed node has both subtrees, it is replaced by the rightmost node of 
 * the left subtree. The rightmost node does not have to be a leaf!
 * The function properly frees all allocated resources of the removed node.
 *
 * Implement the function recursively using bst_replace_by_rightmost and without 
 * using your own helper functions.
 */
void bst_delete(bst_node_t **tree, char key) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Current tree root
    bst_node_t *rootPrt = *tree;

    // If the tree is empty
    if (*tree == NULL) {
        return;
    }
    // If the searched key is on the left
    else if (key < rootPrt->key) {
        bst_delete(&(rootPrt->left), key);
    }
    // If the searched key is on the right
    else if (rootPrt->key < key) {
        bst_delete(&(rootPrt->right), key);
    }
    // If the key is found
    else {
        // If the subtree has no descendants
        if (rootPrt->left == NULL && rootPrt->right == NULL) {
            *tree = NULL;
        }
        // If the subtree has only a right descendant
        else if (rootPrt->right != NULL && rootPrt->left == NULL) {
            *tree = rootPrt->right;
        }
        // If the subtree has only a left descendant
        else if (rootPrt->right == NULL && rootPrt->left != NULL) {
            *tree = rootPrt->left;
        }
        // If the subtree has both descendants
        else {
            bst_replace_by_rightmost(rootPrt, &((*tree)->left));
            return;
        }
        // Free the node
        free(rootPrt);
    }
}

/*
 * Dispose of the entire tree.
 *
 * After disposal, the entire tree will be in the same state as after
 * initialization. The function properly frees all allocated resources of the removed nodes.
 *
 * Implement the function recursively without using your own helper functions.
 */
void bst_dispose(bst_node_t **tree) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // If the tree is not empty
    if (*tree != NULL) {
        // Dispose the left and right subtrees
        bst_dispose(&((*tree)->left));
        bst_dispose(&((*tree)->right));
        // Free the root
        free(*tree);
        *tree = NULL;
    }
}

/*
 * Preorder traversal of the tree.
 *
 * For the currently processed node above it, call the bst_print_node function.
 *
 * Implement the function recursively without using your own helper functions.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_print_node(tree);
        bst_preorder(tree->left);
        bst_preorder(tree->right);
    }
}

/*
 * Inorder traversal of the tree.
 *
 * For the currently processed node above it, call the bst_print_node function.
 *
 * Implement the function recursively without using your own helper functions.
 */
void bst_inorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_inorder(tree->left);
        bst_print_node(tree);
        bst_inorder(tree->right);
    }
}

/*
 * Postorder traversal of the tree.
 *
 * For the currently processed node above it, call the bst_print_node function.
 *
 * Implement the function recursively without using your own helper functions.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_postorder(tree->left);
        bst_postorder(tree->right);
        bst_print_node(tree);
    }
}

/* End of btree/rec/btree.c */
