/**
 * @file btree/rec/btree.c
 * @brief Binary Search Tree - Recursive Variant
 * @details This file implements a binary search tree (BST) where insertion, search, 
 *          and traversal operations are performed using recursion. The recursive
 *          approach provides a concise and elegant solution, but may not be suitable
 *          for trees with a very deep structure or large datasets due to potential
 *          stack overflow issues. The functionality provided includes initialization,
 *          insertion, search, deletion, disposal, and various forms of tree traversal.
 * 
 *          The key functions implemented in this file include:
 *          - bst_init: Initializes the BST.
 *          - bst_insert: Inserts a new node or updates an existing node in the BST.
 *          - bst_search: Searches for a node in the BST.
 *          - bst_delete: Deletes a node from the BST.
 *          - bst_dispose: Frees the entire BST.
 *          - bst_preorder: Performs a preorder tree traversal.
 *          - bst_inorder: Performs an inorder tree traversal.
 *          - bst_postorder: Performs a postorder tree traversal.
 *          - bst_replace_by_rightmost: Helper function for node deletion.
 *          - bst_print_node: Helper function to print a node's key and value.
 * 
 *          Each of these functions is documented with appropriate preconditions,
 *          postconditions, and usage notes.
 * 
 * @note While this code is designed for educational purposes and task-specific
 *       implementations, it can be adapted and integrated into larger projects.
 * 
 * @code
 * // Example of using BST functions
 * bst_node_t *tree = NULL;
 * bst_init(&tree);
 * bst_insert(&tree, 'a', 42);
 * int value;
 * if (bst_search(tree, 'a', &value)) {
 *     printf("Value: %d\n", value);
 * }
 * bst_delete(&tree, 'a');
 * bst_dispose(&tree);
 * @endcode
 * 
 * @see btree.h for type definitions and function prototypes.
 * 
 * @see https://github.com/Jekwwer/IAL-Project02-2021 for the project repository.
 *
 * @author Evgenii Shiliaev
 * @date December 2021 (solution)
 *       November 2023 (code comments updated)
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes a binary search tree to an empty state.
 * 
 * @details This function sets the root of the binary search tree to NULL, marking
 *          the tree as empty and ready for node insertions or other operations.
 * 
 * @param tree A double pointer to the binary search tree that will be initialized.
 * 
 * @pre The double pointer 'tree' should point to a variable that will hold the root
 *      of the binary search tree. The variable pointed to by 'tree' should not
 *      already be initialized, as this function will overwrite it without freeing
 *      existing nodes.
 * 
 * @post After this function is called, the binary search tree is initialized to an
 *       empty state with the root pointer set to NULL.
 * 
 * @note This function is typically the first operation performed on a binary search
 *       tree variable.
 * 
 * @code
 * bst_node_t *my_tree;
 * bst_init(&my_tree); // Now 'my_tree' is initialized to an empty tree
 * @endcode
 * 
 * @todo In the future, consider enhancing the function to automatically free any
 *       existing nodes if 'tree' is already initialized to prevent memory leaks.
 * 
 * @warning If 'tree' already points to an initialized binary search tree, this operation
 *          will lead to memory leaks unless the nodes are freed beforehand.
 * 
 * @return This function does not return a value.
 */
void bst_init(bst_node_t **tree) {
    // NULL check
    if (tree != NULL) {
        *tree = NULL;
    }
}

/**
 * @brief Searches for a node with the specified key in a binary search tree recursively.
 * 
 * @details This function performs a recursive search to find a node with the given key.
 *          If found, it returns true and updates the provided pointer with the node's value.
 *          If not found, it returns false, leaving the provided value pointer unchanged.
 * 
 * @param tree The root node of the binary search tree to search within.
 * @param key The key of the node to search for.
 * @param value Pointer to an integer where the value of the found node will be written.
 * 
 * @pre 'tree' should be a valid binary search tree or NULL if the tree is empty. 'value' should
 *      point to a valid integer variable if 'tree' is not NULL.
 * 
 * @post If a node with the specified key is found, 'value' is updated with the node's value.
 *       Otherwise, 'value' remains unchanged.
 * 
 * @note The function uses recursion to traverse the tree, making it simpler and more readable,
 *       but potentially less efficient on large trees due to stack space usage.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * int value;
 * if (bst_search(tree, 'a', &value)) {
 *     printf("Node found with value: %d\n", value);
 * } else {
 *     printf("Node not found\n");
 * }
 * @endcode
 * 
 * @warning This recursive implementation may not be as efficient as an iterative approach
 *          for very large trees due to stack space usage.
 * 
 * @retval true A node with the specified key was found and 'value' was updated.
 * @retval false No node with the specified key was found and 'value' was not updated.
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

/**
 * @brief Recursively adds or updates a node in the binary search tree.
 * 
 * @details If a node with the provided key exists, its value is updated; otherwise, 
 *          a new node with the specified key and value is inserted. The function maintains 
 *          the BST property during insertion and performs its operations recursively.
 * 
 * @param tree A double pointer to the root of the binary search tree.
 * @param key The unique identifier for the node.
 * @param value The associated value for the node.
 * 
 * @pre The double pointer 'tree' should not be NULL and should reference the root pointer
 *      of a binary search tree (which may be NULL if the tree is empty).
 * 
 * @post After execution, 'tree' points to a binary search tree that contains a node with 
 *       the specified key and value. If that key was already present, the corresponding 
 *       node's value is updated.
 * 
 * @note The function is self-contained and does not rely on external helper functions.
 * 
 * @code
 * bst_node_t *tree = NULL;
 * bst_insert(&tree, 'a', 1); // Inserts 'a' or updates it if it already exists
 * bst_insert(&tree, 'b', 2); // Inserts 'b'
 * bst_insert(&tree, 'a', 3); // Updates 'a' with a new value of 3
 * @endcode
 * 
 * @todo Handle memory allocation failure when creating a new node.
 * 
 * @warning If 'tree' is NULL, the function will exit without action. If memory allocation 
 *          for a new node fails, the function silently fails without inserting.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Recursively replaces the target node with the rightmost node's values in the BST.
 * 
 * @details Searches for the rightmost node within a given subtree and updates the target node
 *          with its key and value. The rightmost node is then removed from the tree. This operation
 *          is integral to the node deletion process in a binary search tree, specifically when 
 *          deleting nodes with two children.
 * 
 * @param target Pointer to the node within the binary search tree that will be replaced.
 * @param tree Double pointer to the root node of the subtree containing the rightmost node.
 * 
 * @pre 'target' must point to a valid node within the tree, and 'tree' must point to the root
 *      of a non-empty subtree within the same binary search tree.
 * 
 * @post The 'target' node's key and value are replaced with those from the rightmost node in 
 *       the subtree. The rightmost node is then deleted, and its memory is freed.
 * 
 * @warning Assumes 'tree' points to a valid, non-empty subtree. Using it on an empty subtree
 *          or passing a NULL pointer for 'tree' will result in undefined behavior. The function
 *          also assumes that 'bst_delete' is available and correctly implemented to remove the 
 *          rightmost node.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Removes a node with the specified key from the binary search tree recursively.
 * 
 * @details Recursively navigates the binary search tree to find and remove the node with the
 *          target key. The node's subtree(s) are reattached appropriately to maintain BST
 *          properties, and the `bst_replace_by_rightmost` function is used when removing a
 *          node with two children.
 * 
 * @param tree A double pointer to the root of the binary search tree.
 * @param key The key of the node to be removed.
 * 
 * @pre 'tree' must not be NULL and should reference the root pointer of a binary search tree.
 *      The tree should be constructed properly to maintain BST properties.
 * 
 * @post The node with the specified key is removed. If the node had one or no children, it is
 *       simply removed and the tree is reconnected. If the node had two children, it is
 *       replaced with the rightmost node of its left subtree, and that node is removed.
 * 
 * @note Assumes that the `bst_replace_by_rightmost` function is correctly implemented and can
 *       be used to facilitate the removal of nodes with two children.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_delete(&tree, 'd'); // Removes the node with key 'd', if it exists
 * @endcode
 * 
 * @warning Assumes that `tree` points to a valid binary search tree. Incorrect use could
 *          lead to memory leaks or dangling pointers.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Eliminates all nodes from a binary search tree, deallocating memory.
 * 
 * @details Proceeds through the binary search tree, releasing all nodes and setting the 
 *          tree to a null state, akin to being freshly initialized. The function is recursive,
 *          eliminating subtrees before their parent nodes, ensuring a complete and safe disposal.
 * 
 * @param tree A double pointer to the root of the binary search tree to be disposed of.
 * 
 * @pre 'tree' must not be NULL and should reference the root pointer of a binary search tree.
 * 
 * @post After the function call, 'tree' is set to NULL, indicating that the binary search tree
 *       is empty and all memory has been freed.
 * 
 * @note This function is self-contained and requires no external helper functions.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_dispose(&tree); // The tree is now empty and 'tree' is NULL
 * @endcode
 * 
 * @warning The function expects 'tree' to be a valid pointer to a binary search tree root pointer.
 *          If 'tree' is NULL or the referenced pointer is not the start of a binary search tree,
 *          the function's behavior is undefined.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Traverses a binary search tree in preorder and processes nodes.
 * 
 * @details Applies the preorder traversal method (root, then left subtree, then right subtree)
 *          to visit all nodes in the binary search tree. Each node is processed using the
 *          `bst_print_node` function during the visit.
 * 
 * @param tree The root node of the binary search tree to traverse.
 * 
 * @pre 'tree' can be either a valid binary search tree node or NULL if the tree or subtree is empty.
 * 
 * @post The entire binary search tree has been traversed in preorder, with each node being processed.
 * 
 * @note This function uses a simple recursive approach to visit nodes in the proper order for preorder traversal.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_preorder(tree); // The tree is traversed in preorder, processing each node
 * @endcode
 * 
 * @todo Ensure that `bst_print_node` is robust against potential future changes to the node structure.
 * 
 * @warning The function assumes that `bst_print_node` is correctly implemented and that 'tree'
 *          is the root of a binary search tree or NULL.
 * 
 * @return This function does not return a value.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_print_node(tree);
        bst_preorder(tree->left);
        bst_preorder(tree->right);
    }
}

/**
 * @brief Conducts an inorder traversal of a binary search tree and processes nodes.
 * 
 * @details Implements an inorder traversal (left subtree, root, right subtree) recursively,
 *          processing each node with the `bst_print_node` function. This approach ensures that
 *          nodes are visited in ascending key order for a binary search tree.
 * 
 * @param tree The root node of the binary search tree to traverse.
 * 
 * @pre 'tree' should be a valid binary search tree node or NULL if the subtree is empty.
 * 
 * @post The binary search tree has been traversed in inorder, with each node being processed.
 * 
 * @note Relies on the `bst_print_node` function to handle the action performed on each visited node.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_inorder(tree); // The tree is traversed in inorder, processing each node
 * @endcode
 * 
 * @todo Ensure that `bst_print_node` is robust against potential future changes to the node structure.
 * 
 * @warning It is expected that `bst_print_node` is defined and that 'tree' points to a valid node.
 *          If `bst_print_node` is not implemented, or if 'tree' is not a proper binary search tree node,
 *          the behavior is undefined.
 * 
 * @return This function does not return a value.
 */
void bst_inorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_inorder(tree->left);
        bst_print_node(tree);
        bst_inorder(tree->right);
    }
}

/**
 * @brief Navigates through a binary search tree in postorder and processes nodes.
 * 
 * @details Uses a postorder traversal (visiting left subtree, then right subtree, then root)
 *          to recursively visit all nodes in a binary search tree. Each node is processed
 *          using the `bst_print_node` function upon visitation.
 * 
 * @param tree The root node of the binary search tree to traverse.
 * 
 * @pre 'tree' can be either the root of a binary search tree or NULL if the tree or subtree is empty.
 * 
 * @post The entire binary search tree has been traversed in postorder, with each node being processed.
 * 
 * @note Assumes the `bst_print_node` function is defined and used for processing each node.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_postorder(tree); // The tree is traversed in postorder, processing each node
 * @endcode
 * 
 * @todo Ensure that `bst_print_node` is robust against potential future changes to the node structure.
 * 
 * @warning The function presumes `bst_print_node` is properly implemented and that 'tree'
 *          is either a valid binary search tree node or NULL.
 * 
 * @return This function does not return a value.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_postorder(tree->left);
        bst_postorder(tree->right);
        bst_print_node(tree);
    }
}

/* End of btree/rec/btree.c */
