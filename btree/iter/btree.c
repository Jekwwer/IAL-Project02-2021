/**
 * @file btree/iter/btree.c
 * @brief Binary Search Tree - Iterative Version
 * @details Implements a binary search tree (BST) using iterative methods for
 *          insertion, search, and traversal. This approach avoids recursion and
 *          uses stacks to manage state, which can be more memory efficient for
 *          deep trees. The key functions implemented here include:
 *          - bst_init: Initializes the BST to an empty state.
 *          - bst_insert: Inserts a new node with a key and value, or updates an existing node.
 *          - bst_search: Searches for a node by key and retrieves its value if found.
 *          - bst_delete: Removes a node by key from the BST.
 *          - bst_dispose: Frees all memory used by the BST.
 *          - bst_preorder: Iteratively performs a preorder traversal of the BST.
 *          - bst_inorder: Iteratively performs an inorder traversal of the BST.
 *          - bst_postorder: Iteratively performs a postorder traversal of the BST.
 *          - bst_replace_by_rightmost: Replaces a node with the rightmost node of a subtree.
 *          - bst_print_node: Prints the key and value of a BST node.
 * 
 *          Each function is documented to detail its operation, usage, and any
 *          necessary precautions.
 * 
 * @note This implementation is suitable for educational purposes and practical
 *       use in environments where recursion is not ideal.
 * 
 * @code
 * // Usage example of BST iterative functions
 * bst_node_t *tree = NULL;
 * bst_init(&tree); // Initialize the tree
 * bst_insert(&tree, 'a', 1); // Insert nodes
 * bst_insert(&tree, 'b', 2);
 * int value;
 * if (bst_search(tree, 'a', &value)) {
 *     printf("Found node 'a' with value: %d\n", value);
 * }
 * bst_delete(&tree, 'a'); // Delete a node
 * bst_dispose(&tree); // Dispose of the tree
 * @endcode
 * 
 * @see btree.h for BST structure and function prototypes.
 * @see stack.h and stack.c for stack operations used in this iterative BST implementation.
 * 
 * @see https://github.com/Jekwwer/IAL-Project02-2021 for the project repository and additional documentation.
 * 
 * @author Evgenii Shiliaev
 * @date December 2021 (solution)
 *       November 2023 (code comments updated)
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes a binary search tree to an empty state.
 * 
 * @details Sets the root of the binary search tree to NULL, indicating an empty tree.
 *          This action prepares the tree for operations such as insertions and searches.
 * 
 * @param tree A double pointer to the root of the binary search tree which will be initialized.
 * 
 * @pre The double pointer 'tree' should not be NULL and should point to a valid memory address.
 * 
 * @post The binary search tree pointed to by 'tree' is set to an empty state with the root
 *       pointer being NULL.
 * 
 * @note It is assumed that 'tree' is not pointing to an already initialized tree. If it is,
 *       memory leaks can occur due to lost references to allocated nodes.
 * 
 * @code
 * bst_node_t *my_tree = NULL;
 * bst_init(&my_tree); // Now 'my_tree' is initialized to an empty tree
 * @endcode
 * 
 * @todo Ensure that future versions of this function handle already initialized trees to prevent
 *       memory leaks.
 * 
 * @warning Calling this function on a binary search tree that has already been initialized
 *          without first freeing all allocated nodes will result in memory leaks.
 * 
 * @return This function does not return a value.
 */
void bst_init(bst_node_t **tree) {

    // NULL check
    if (tree != NULL) {
        (*tree) = NULL;
    }
}

/**
 * @brief Searches for a node with the specified key in the binary search tree.
 * 
 * @details Performs an iterative search for a node by its key. If found, returns true
 *          and updates the value parameter with the node's value. If not found, returns
 *          false without changing the value parameter.
 * 
 * @param tree The root node of the binary search tree to search within.
 * @param key The key of the node to be searched for.
 * @param value Pointer to an integer where the value of the found node is to be stored.
 * 
 * @pre The binary search tree should be properly initialized and populated with nodes.
 *      The 'value' pointer must point to a valid integer variable.
 * 
 * @post If a node with the specified key is found, 'value' is updated with the node's value.
 *       Otherwise, 'value' is not modified.
 * 
 * @note This function uses an iterative approach instead of recursion to search the tree.
 * 
 * @code
 * bst_node_t *tree = ... // assume tree is previously populated
 * int node_value;
 * if (bst_search(tree, 'd', &node_value)) {
 *     printf("Node found with value: %d\n", node_value);
 * } else {
 *     printf("Node not found\n");
 * }
 * @endcode
 * 
 * @warning The function assumes the tree is not NULL and is a properly constructed binary search tree.
 *          Passing a NULL tree or an improperly constructed tree may lead to undefined behavior.
 * 
 * @retval true A node with the specified key is found.
 * @retval false No node with the specified key is found.
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

/**
 * @brief Inserts a new node or updates an existing node in the binary search tree.
 * 
 * @details This function will insert a new node with the given key and value or update 
 *          the value of an existing node with the same key. It iteratively traverses 
 *          the tree to maintain the binary search property during insertion.
 * 
 * @param tree A double pointer to the root of the binary search tree.
 * @param key The key of the node to be inserted or updated.
 * @param value The value to be associated with the given key.
 * 
 * @pre 'tree' should not be NULL and should point to a valid binary search tree root pointer.
 * 
 * @post The tree will contain a node with the given key and value. If the key already existed, 
 *       its value is updated; otherwise, a new node is inserted.
 * 
 * @note No helper functions are called; all operations are performed within this function.
 * 
 * @code
 * bst_node_t *tree = NULL;
 * bst_insert(&tree, 'a', 1); // Inserts a new node
 * bst_insert(&tree, 'b', 2); // Inserts another new node
 * bst_insert(&tree, 'a', 3); // Updates the value of the existing node with key 'a'
 * @endcode
 * 
 * @todo Implement error handling for memory allocation failure during new node creation.
 * 
 * @warning If a NULL pointer is passed as 'tree', the function will return early without 
 *          performing the insertion. If memory allocation fails, the function will exit 
 *          without creating a new node.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Replaces the target node with its rightmost descendant in the binary search tree.
 * 
 * @details Locates the rightmost descendant of the subtree pointed to by 'tree' and 
 *          swaps its key and value with those of the 'target' node. The rightmost 
 *          node is then deleted from the tree, ensuring that the BST properties are 
 *          preserved and that memory is properly freed.
 * 
 * @param target Node whose value and key will be replaced.
 * @param tree Double pointer to the root of the subtree from which the rightmost node will be found.
 * 
 * @pre 'target' must not be NULL and should point to a valid node within the BST.
 *      'tree' must not be NULL and should point to the root of a valid subtree.
 * 
 * @post The 'target' node's key and value are replaced with those of the rightmost node 
 *       in the subtree. The rightmost node is removed from the tree.
 * 
 * @note This function is a utility function typically used during the deletion of a node
 *       in a BST when the node has two children.
 * 
 * @code
 * // Example of usage within bst_delete function:
 * if (node_to_delete->left != NULL && node_to_delete->right != NULL) {
 *     bst_replace_by_rightmost(node_to_delete, &node_to_delete->left);
 * }
 * @endcode
 * 
 * @todo Implement checks to safely handle cases where 'tree' is NULL or the subtree is empty.
 * 
 * @warning The function assumes that 'target' and the subtree rooted at 'tree' are non-NULL.
 *          Incorrect usage can lead to undefined behavior or memory leaks.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Removes a node with the specified key from the binary search tree.
 * 
 * @details Searches for a node by key and removes it, preserving the BST properties.
 *          Handles nodes with different configurations of subtrees and uses a helper
 *          function for two-subtree nodes. Frees the memory occupied by the removed node.
 * 
 * @param tree A double pointer to the root of the binary search tree.
 * @param key The key of the node to be removed.
 * 
 * @pre 'tree' must not be NULL and should point to a valid binary search tree. The tree
 *      should be properly constructed to ensure that the BST properties are maintained.
 * 
 * @post The node with the specified key is removed from the tree. If the node had one
 *       or no children, it is simply removed and the tree is reconnected. If the node
 *       had two children, it is replaced with the rightmost node of its left subtree.
 * 
 * @note If the specified key does not exist in the tree, the tree remains unchanged.
 * 
 * @code
 * bst_node_t *tree = ... // assume tree is previously populated
 * bst_delete(&tree, 'd'); // Removes the node with key 'd', if it exists
 * @endcode
 * 
 * @todo Handle the case where the 'tree' is NULL more gracefully, potentially by
 *       returning an error code.
 * 
 * @warning If 'tree' is NULL or improperly constructed, the behavior is undefined. The function
 *          assumes that 'bst_replace_by_rightmost' is implemented and works correctly.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Deletes the entire binary search tree, releasing all allocated resources.
 * 
 * @details Uses an iterative process with a stack to traverse and delete all nodes in the
 *          binary search tree. After the operation, the tree's root pointer is set to NULL,
 *          indicating that the tree is empty.
 * 
 * @param tree A double pointer to the root of the binary search tree to be disposed of.
 * 
 * @pre 'tree' must not be NULL and should point to a valid binary search tree. The stack
 *      used for the iterative process must be properly initialized before use.
 * 
 * @post After this function is called, all nodes of the binary search tree are deleted and
 *       the memory is freed. The root pointer 'tree' is set to NULL.
 * 
 * @note The function uses a non-recursive method to traverse the tree, relying on an auxiliary
 *       stack data structure for node management.
 * 
 * @code
 * bst_node_t *tree = ... // assume tree is previously populated
 * bst_dispose(&tree); // Deletes the entire tree and sets `tree` to NULL
 * @endcode
 * 
 * @todo Consider enhancing the function to handle NULL 'tree' pointers more gracefully.
 * 
 * @warning The function assumes that the provided stack operations are correctly implemented.
 *          Failure to properly use the stack can lead to incomplete tree disposal and memory leaks.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Traverses the leftmost branch of a subtree, visiting nodes in preorder.
 * 
 * @details As part of an iterative preorder traversal, this function visits nodes
 *          starting from a subtree root down to the leftmost leaf, pushing visited
 *          nodes onto a stack for later processing and using `bst_print_node` to
 *          print each visited node.
 * 
 * @param tree The root node of the subtree to start the traversal from.
 * @param to_visit Stack data structure to store nodes during the traversal.
 * 
 * @pre 'to_visit' must be initialized prior to calling this function. 'tree' should
 *      be the root node of a subtree or NULL if the subtree is empty.
 * 
 * @post Nodes along the leftmost branch of the subtree are visited in preorder and 
 *       pushed onto the 'to_visit' stack. The 'tree' pointer remains unchanged.
 * 
 * @note This function is used internally within an iterative preorder traversal
 *       routine and is not designed for standalone use.
 * 
 * @code
 * // Usage within an iterative preorder traversal function
 * stack_bst_t *to_visit = ...; // assume stack is already created and initialized
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_leftmost_preorder(tree, to_visit);
 * // Continue with preorder traversal...
 * @endcode
 * 
 * @todo Update the function to handle memory allocation failures more gracefully, potentially
 *       by returning an error code.
 * 
 * @warning This function assumes the existence of `bst_print_node` for printing node values
 *          and that the `to_visit` stack is properly managed externally.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Conducts a preorder traversal of a binary search tree iteratively.
 * 
 * @details Employs a stack to visit each node of the binary search tree in preorder fashion
 *          without using recursion. The `bst_print_node` function is invoked for each node to
 *          process its value, and `bst_leftmost_preorder` is used to reach the leftmost node.
 * 
 * @param tree Pointer to the root node of the binary search tree to traverse.
 * 
 * @pre 'tree' must be the root of a valid binary search tree or NULL if the tree is empty.
 * 
 * @post All nodes of the binary search tree have been visited in preorder, and their values
 *       have been processed by `bst_print_node`.
 * 
 * @note This function requires `bst_print_node` to be defined elsewhere for node processing.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_preorder(tree); // Performs the preorder traversal of the tree
 * @endcode
 * 
 * @todo Update the function to handle memory allocation failures more gracefully, potentially
 *       by returning an error code.
 * 
 * @warning Assumes that `bst_print_node` and `bst_leftmost_preorder` are correctly implemented.
 *          Failure in memory allocation for the stack will result in early termination of the function.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Moves down to the leftmost node of a subtree, storing nodes for later inorder traversal.
 * 
 * @details This function is used within an iterative inorder traversal to navigate from the root
 *          of a subtree to its leftmost node, storing all intermediate nodes in a stack.
 *          This sets up the necessary state for continuing an inorder traversal from the given subtree.
 * 
 * @param tree The root node of the subtree to start the traversal from.
 * @param to_visit Stack data structure to store nodes during the traversal.
 * 
 * @pre 'to_visit' must be initialized prior to calling this function. 'tree' should
 *      be the root node of a subtree or NULL if the subtree is empty.
 * 
 * @post Nodes down to the leftmost of the subtree are pushed onto the 'to_visit' stack
 *       and are ready for inorder traversal.
 * 
 * @note This function is a utility function designed to be used as part of an iterative inorder
 *       traversal algorithm and is not meant for standalone use.
 * 
 * @code
 * // Usage within an iterative inorder traversal function
 * stack_bst_t *to_visit = ...; // assume stack is already created and initialized
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_leftmost_inorder(tree, to_visit);
 * // Continue with inorder traversal...
 * @endcode
 * 
 * @todo Update the function to handle memory allocation failures more gracefully, potentially
 *       by returning an error code.
 * 
 * @warning It is assumed that 'to_visit' is a properly initialized stack. Using this function without
 *          a valid 'to_visit' stack or outside of an inorder traversal routine may lead to undefined behavior.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Conducts an inorder traversal of a binary search tree iteratively.
 * 
 * @details Employs a stack to visit each node of the binary search tree in inorder fashion,
 *          without using recursion. The `bst_print_node` function is invoked for each node to
 *          process its value, and `bst_leftmost_inorder` is used to reach the leftmost node.
 * 
 * @param tree Pointer to the root node of the binary search tree to traverse.
 * 
 * @pre 'tree' must be the root of a valid binary search tree or NULL if the tree is empty.
 * 
 * @post All nodes of the binary search tree have been visited in inorder, and their values
 *       have been processed by `bst_print_node`.
 * 
 * @note This function requires `bst_print_node` to be defined elsewhere for node processing.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_inorder(tree); // Performs the inorder traversal of the tree
 * @endcode
 * 
 * @todo Update the function to handle memory allocation failures for the stack more gracefully,
 *       potentially by returning an error code.
 * 
 * @warning Assumes that `bst_print_node` and `bst_leftmost_inorder` are correctly implemented.
 *          Failure in memory allocation for the stack will result in early termination of the function.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Moves down to the leftmost node of a subtree, storing nodes for later postorder traversal.
 * 
 * @details This function is used within an iterative postorder traversal to navigate from the root
 *          of a subtree to its leftmost node, pushing all intermediate nodes onto a stack and marking
 *          each with a first-time visit flag on a separate boolean stack. This prepares for a 
 *          controlled traversal back up the tree, allowing nodes to be visited in the correct order.
 * 
 * @param tree The root node of the subtree to start the traversal from.
 * @param to_visit Stack data structure to store nodes during the traversal.
 * @param first_visit Stack of boolean values to track the first-time visitation of nodes.
 * 
 * @pre 'to_visit' and 'first_visit' must be initialized prior to calling this function. 'tree' should
 *      be the root node of a subtree or NULL if the subtree is empty.
 * 
 * @post Nodes down to the leftmost of the subtree are pushed onto the 'to_visit' stack with a first-time
 *       visitation status marked on the 'first_visit' stack.
 * 
 * @note This function is a utility function designed to be used as part of an iterative postorder
 *       traversal algorithm and is not meant for standalone use.
 * 
 * @code
 * // Usage within an iterative postorder traversal function
 * stack_bst_t *to_visit = ...; // assume node stack is already created and initialized
 * stack_bool_t *first_visit = ...; // assume boolean stack is already created and initialized
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_leftmost_postorder(tree, to_visit, first_visit);
 * // Continue with postorder traversal...
 * @endcode
 * 
 * @warning It is assumed that 'to_visit' is a properly initialized node stack and 'first_visit' is a
 *          properly initialized boolean stack. Using this function without valid 'to_visit' and 'first_visit'
 *          stacks or outside of a postorder traversal routine may lead to undefined behavior.
 * 
 * @return This function does not return a value.
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

/**
 * @brief Executes an iterative postorder traversal of a binary search tree.
 * 
 * @details Utilizes a non-recursive approach to traverse the tree in postorder, processing
 *          each node's value with `bst_print_node`. It leverages two stacks: one to store
 *          nodes and another to keep track of whether a node has been visited before,
 *          using the `bst_leftmost_postorder` helper function for initial traversal.
 * 
 * @param tree Pointer to the root node of the binary search tree to traverse.
 * 
 * @pre 'tree' should be the root of a binary search tree or NULL if the tree is empty.
 * 
 * @post The nodes of the tree have been visited and processed in postorder sequence.
 * 
 * @note The function relies on `bst_print_node` to be defined and used for processing
 *       each node during traversal.
 * 
 * @code
 * bst_node_t *tree = ...; // assume tree is previously populated
 * bst_postorder(tree); // Performs the postorder traversal of the tree
 * @endcode
 * 
 * @todo Address potential memory allocation failures for both stacks with proper error handling.
 * 
 * @warning Assumes successful memory allocation for the node and boolean stacks. If allocation
 *          fails, the traversal is aborted prematurely.
 * 
 * @return This function does not return a value.
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
