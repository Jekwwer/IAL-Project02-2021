/*
 * Binary search tree — iterative version
 *
 * Utilizing data types from the btree.h file, stacks from the stack.h and
 * stack.c files and prepared function skeletons, implement a binary search
 * tree without the use of recursion.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Tree initialization.
 *
 * The user must ensure that the initialization will not be called repeatedly on
 * an already initialized tree. Otherwise, a memory leak can occur. Since an
 * uninitialized pointer has an undefined value, it is not possible to detect
 * this in the function.
 */
void bst_init(bst_node_t **tree) {

    // NULL check
    if (tree != NULL) {
        (*tree) = NULL;
    }
}

/*
 * Finding a node in the tree.
 *
 * In case of success, the function returns true and writes the value of the 
 * node into the variable value. Otherwise, the function returns false and the 
 * value variable remains unchanged.
 *
 * Implement the function iteratively without using your own helper functions.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // Condition for node found
    bool found = false;
     // Temporary tree root for jumping left/right
    bst_node_t *tmpRoot = tree;

    // If not found and subtree is not empty
    while (!found && tmpRoot != NULL) {

        // If the searched key is on the left
        if (key < tmpRoot->key) {
            tmpRoot = tmpRoot->left;
        }
        // If the searched key is on the right
        else if (tmpRoot->key < key) {
            tmpRoot = tmpRoot->right;
        }
        // If the keys are equal
        else {
            found = true;
            *value = tmpRoot->value;
        }
    }
    return found;
}

/*
 * Inserting a node into the tree.
 *
 * If a node with the specified key already exists in the tree, replace its value.
 * Otherwise, insert a new leaf node.
 *
 * The resulting tree must meet the search tree condition — the left subtree of
 * a node only contains smaller keys, the right one larger.
 *
 * Implement the function iteratively without using your own helper functions.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Previous tree node
    bst_node_t *prevRootPtr = NULL;
    // Current tree node
    bst_node_t *rootPtr = *tree;
    // Condition for loop termination
    bool isDone = false;

    while (!isDone) {
        // If current node is not empty
        if (rootPtr != NULL) {
            prevRootPtr = rootPtr;
            // If the searched key is on the left
            if (key < rootPtr->key) {
                rootPtr = rootPtr->left;
            }
            // If the searched key is on the right
            else if (rootPtr->key < key) {
                rootPtr = rootPtr->right;
            }
            // If the keys are equal
            else {
                rootPtr->value = value;
                isDone = true;
            }
        }
        else {// If current node is empty/key was not found
            bst_node_t *newNode = malloc(sizeof(bst_node_t));
            if (newNode == NULL) {
                return;
            }
            // Initialize new node
            newNode->key = key;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->value = value;

            // If tree is empty
            if (prevRootPtr == rootPtr) {
                *tree = newNode;
            }
            else {// Tree is not empty
                if (key < prevRootPtr->key) {
                    prevRootPtr->left = newNode;
                }
                else {
                    prevRootPtr->right = newNode;
                }
            }
            isDone = true;
        }
    }
}

/*
 * Helper function that replaces the node with its rightmost descendant.
 *
 * The key and value of the target node will be replaced by the key and value of the 
 * rightmost node of the subtree tree. The rightmost descendant will be removed. 
 * The function correctly frees all allocated resources of the removed node.
 *
 * The function assumes that the value of tree is not NULL.
 *
 * This helper function will be used in the implementation of the bst_delete function.
 *
 * Implement the function iteratively without using your own helper functions.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Current tree node
    bst_node_t *rootPtr = *tree;

    // Look for the node that is the furthest to the right
    while (rootPtr->right != NULL) {
        rootPtr = rootPtr->right;
    }

    // Overwrite the data
    target->key = rootPtr->key;
    target->value = rootPtr->value;
    // Remove the rightmost element
    bst_delete(tree, rootPtr->key);
}

/*
 * Removing a node in the tree.
 *
 * If a node with the specified key does not exist, the function does nothing.
 * If the removed node has one subtree, its parent inherits it.
 * If the removed node has both subtrees, it is replaced by the rightmost node 
 * of the left subtree. The rightmost node does not have to be a leaf!
 * The function correctly frees all allocated resources of the removed node.
 *
 * Implement the function iteratively using bst_replace_by_rightmost and without 
 * using your own helper functions.
 */
void bst_delete(bst_node_t **tree, char key) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Previous tree node
    bst_node_t *prevRootPtr = NULL;
    // Current tree node
    bst_node_t *rootPtr = *tree;
    // Temporary variable just for calling bst_search()
    int tmp = 0;

    // If there is no node with such a key in the tree
    if (!bst_search(rootPtr, key, &tmp)) {
        return;
    }
    else {// Key is in the tree
        // looking for the node with the searched key
        while (rootPtr->key != key) {
            prevRootPtr = rootPtr;
            if (key < rootPtr->key) {
                rootPtr = rootPtr->left;
            }
            else {
                rootPtr = rootPtr->right;
            }
        }
    }

    // If node has no descendants
    if (rootPtr->right == NULL && rootPtr->left == NULL) {
        // If it's the only node in the tree
        if (prevRootPtr == NULL) {
            *tree = NULL;
        }
        else {// Not a single node in the tree
            // If it is the left descendant of the parent
            if (prevRootPtr->left == rootPtr) {
                prevRootPtr->left = NULL;
            }
            else {// Is a right descendant
                prevRootPtr->right = NULL;
            }
        }
    }
    // If the node has only a left descendant
    else if (rootPtr->left != NULL && rootPtr->right == NULL) {
        // If we remove the root of the tree
        if (prevRootPtr == NULL) {
            *tree = rootPtr->left;
        }
        else {
            // If we cancel the left descendant
            if (rootPtr == prevRootPtr->left) {
                prevRootPtr->left = rootPtr->left;
            }
            else {// We cancel the right descendant
                prevRootPtr->right = rootPtr->left;
            }
        }
    }
    // If the node has only a right descendant
    else if (rootPtr->right != NULL && rootPtr->left == NULL) {
        // If we remove the root of the tree
        if (prevRootPtr == NULL) {
            *tree = rootPtr->right;
        }
        else {
            // If we cancel the left descendant
            if (rootPtr == prevRootPtr->left) {
                prevRootPtr->left = rootPtr->right;
            }
            else {// We cancel the right descendant
                prevRootPtr->right = rootPtr->right;
            }
        }
    }
    else {// Has both descendants
        bst_replace_by_rightmost(rootPtr, &(rootPtr->left));
        // Release the cancelled node bst_replace_by_rigthmost()
        return;
    }

    // Releasing memory
    free(rootPtr);
}

/*
 * Deletion of the entire tree.
 *
 * After deletion, the entire tree will be in the same state as after
 * initialization. The function properly releases all allocated resources of the
 * deleted nodes.
 *
 * Implement the function iteratively using a stack of nodes and without using
 * your own helper functions.
 */
void bst_dispose(bst_node_t **tree) {

    // NULL check
    if (tree == NULL) {
        return;
    }

    // Allocate and initialize the stack for nodes
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);

    // Current node of the tree
    bst_node_t *rootPtr = *tree;

    // While the subtree root is not empty or the stack is not empty
    while (rootPtr != NULL || !stack_bst_empty(stack)) {
        // If the root is empty
        if (rootPtr == NULL) {
            if (!stack_bst_empty(stack)) {
                // Get the new root from the stack
                rootPtr = stack_bst_top(stack);
                stack_bst_pop(stack);
            }
        }
        else {// Root is not empty
            // If it has a right child, store it in the stack
            if (rootPtr->right != NULL) {
                stack_bst_push(stack, rootPtr->right);
            }
            // Go left and also release the root
            bst_node_t *tmpPtr = rootPtr;
            rootPtr = rootPtr->left;
            free(tmpPtr);
        }
    }
    // NULLify the tree root reference
    *tree = NULL;

    // Free the memory for the stack
    free(stack);
}

/*
 * Helper function for iterative preorder.
 *
 * It goes along the left branch to the leftmost node of the subtree.
 * Calls bst_print_node on the processed nodes and stores them in a stack of nodes.
 *
 * Implement the function iteratively using a stack of nodes and without using
 * your own helper functions.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    // While the subtree is not empty
    while (tree != NULL) {
        // Store the root in the stack
        stack_bst_push(to_visit, tree);
        // Print the root value
        bst_print_node(tree);
        // Go left
        tree = tree->left;
    }
}

/*
 * Preorder traversal of the tree.
 *
 * For the currently processed node, call the bst_print_node function on it.
 *
 * Implement the function iteratively using the bst_leftmost_preorder function and
 * a stack of nodes without using your own helper functions.
 */
void bst_preorder(bst_node_t *tree) {
    // Allocate and initialize the stack
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) {
        return;
    }
    stack_bst_init(stack);

    // Go as left as possible
    bst_leftmost_preorder(tree, stack);

    // While the stack is not empty
    while (!stack_bst_empty(stack)) {
        // Get a node from it
        bst_node_t *tmpRoot = stack_bst_top(stack);
        stack_bst_pop(stack);
        // Root and left child are already processed, go right
        bst_leftmost_preorder(tmpRoot->right, stack);
    }

    // Free the memory for the stack
    free(stack);
}

/*
 * Helper function for iterative inorder.
 *
 * It goes along the left branch to the leftmost node of the subtree and stores
 * the nodes in a stack of nodes.
 *
 * Implement the function iteratively using a stack of nodes and without using
 * your own helper functions.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

    // While the subtree is not empty
    while (tree != NULL) {
        // Store the root in the stack
        stack_bst_push(to_visit, tree);
        // Go left
        tree = tree->left;
    }
}

/*
 * Inorder traversal of the tree.
 *
 * For the currently processed node, call the bst_print_node function on it.
 *
 * Implement the function iteratively using the bst_leftmost_inorder function and
 * a stack of nodes without using your own helper functions.
 */
void bst_inorder(bst_node_t *tree) {
    // Allocate and initialize the stack
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) {
        return;
    }
    stack_bst_init(stack);

    // Go as left as possible
    bst_leftmost_inorder(tree, stack);

    // While the stack is not empty
    while (!stack_bst_empty(stack)) {
        // Get a node from it
        tree = stack_bst_top(stack);
        stack_bst_pop(stack);
        // Print the node
        bst_print_node(tree);
        // Go right
        bst_leftmost_inorder(tree->right, stack);
    }

    // Free the memory for the stac
    free(stack);
}

/*
 * Helper function for iterative postorder.
 *
 * It goes along the left branch to the leftmost node of the subtree and stores
 * the nodes in a stack of nodes. In the stack of boolean values, it stores the 
 * information that the node was visited for the first time.
 *
 * Implement the function iteratively using node and boolean stacks without using
 * your own helper functions.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {

    // While the subtree is not empty
    while (tree != NULL) {
        // Store the node in the stack
        stack_bst_push(to_visit, tree);
        // Store true, indicating we've visited the node for the first time
        stack_bool_push(first_visit, true);
        // Go left
        tree = tree->left;
    }
}

/*
 * Postorder traversal of the tree.
 *
 * For the currently processed node, call the bst_print_node function on it.
 *
 * Implement the function iteratively using the bst_leftmost_postorder function and
 * stacks of nodes and boolean values without using your own helper functions.
 */
void bst_postorder(bst_node_t *tree) {
    // Allocate and initialize the node stack
    stack_bst_t *node_stack = malloc(sizeof(stack_bst_t));
    if (node_stack == NULL) {
        return;
    }

    // Allocate and initialize the boolean stack
    stack_bst_init(node_stack);
    stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
    if (bool_stack == NULL) {
        return;
    }
    stack_bool_init(bool_stack);

    bool isFromLeft;

    // Go as left as possible
    bst_leftmost_postorder(tree, node_stack, bool_stack);

    // While the node stack is not empty
    while (!stack_bst_empty(node_stack)) {
        // Check the node on top
        tree = stack_bst_top(node_stack);
        isFromLeft = stack_bool_top(bool_stack);
        stack_bool_pop(bool_stack);
        // Were we on the left?
        if (isFromLeft) {
            // We're supposed to go to the right
            stack_bool_push(bool_stack, false);
            bst_leftmost_postorder(tree->right, node_stack, bool_stack);
        }
        else {// We were already on the right
            // Remove the node from the stack and write it out
            stack_bst_pop(node_stack);
            bst_print_node(tree);
        }
    }

    // Free the memory for the stack
    free(node_stack);
    free(bool_stack);
}

/* End of btree/iter/btree.c */
