/*
 * Header file for auxiliary stacks.
 * Do not modify this file.
 */
#ifndef IAL_BTREE_ITER_STACK_H
#define IAL_BTREE_ITER_STACK_H

#include "../btree.h"

// Maximum stack size
#define MAXSTACK 30

/*
 * Macro generating declarations for a stack of type T with the name infix TNAME.
 * For TNAME="bst" working with type T="bst_node_t*":
 *   Data type stack_bst_t
 *   Functions void stack_bst_init(stack_bst_t *stack)
 *           void stack_bst_push(stack_bst_t *stack, bst_node_t *item)
 *           bst_node_t *stack_bst_pop(stack_bst_t *stack)
 *           bst_node_t *stack_bst_top(stack_bst_t *stack)
 *           bool stack_bst_empty(stack_bst_t *stack)
 * And equivalent for TNAME="bool", T="bool".
 */
#define STACKDEC(T, TNAME)                                                     \
  typedef struct {                                                             \
    T items[MAXSTACK];                                                         \
    int top;                                                                   \
  } stack_##TNAME##_t;                                                         \
                                                                               \
  void stack_##TNAME##_init(stack_##TNAME##_t *stack);                         \
  void stack_##TNAME##_push(stack_##TNAME##_t *stack, T item);                 \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack);                             \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack);                             \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack);

STACKDEC(bst_node_t *, bst)
STACKDEC(bool, bool)

#endif

/* End of btree/iter/stack.h */
