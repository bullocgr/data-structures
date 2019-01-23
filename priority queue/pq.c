/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Grace Bullock
 * Email: bullocgr@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
  struct dynarray* array;
};

struct element {
  void* value;
  int priority;
};


void move_up(struct pq* pq);
void swap(struct pq* pq, int parent, int child);
void bubble_down(struct pq* pq);

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pq = malloc(sizeof(struct pq));
  pq->array = dynarray_create();
  return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  dynarray_free(pq->array);
  free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  // printf("The size is: %d", dynarray_size(pq->array));
  if(dynarray_size(pq->array) == 0) {
    return 1;
  }
  return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  struct element* new_element = malloc(sizeof(struct element));
  new_element->value = value;
  new_element->priority = priority;
  dynarray_insert(pq->array, -1, new_element);
  move_up(pq);
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct element* element = dynarray_get(pq->array, 0);
  void* value = element->value;
  return value;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  struct element* element = dynarray_get(pq->array, 0);
  int priority = element->priority;
  return priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  struct element* remove = dynarray_get(pq->array, 0);
  void* removed_val = remove->value;
  dynarray_set(pq->array, 0, dynarray_get(pq->array, -1));
  dynarray_remove(pq->array, -1);
  free(remove);
  bubble_down(pq);
  return remove->value;
}

void move_up(struct pq* pq) {
  int child = dynarray_size(pq->array)-1;
  int parent = (child-1)/2;
  struct element* temp_child = dynarray_get(pq->array, child);
  struct element* temp_parent = dynarray_get(pq->array, parent);
  int priority_child = temp_child->priority;
  int priority_parent = temp_parent->priority;

  while(child != 0 && priority_child < priority_parent) {
    swap(pq, parent, child);
    child = parent;
    parent = (parent-1)/2;
    temp_parent = dynarray_get(pq->array, parent);
    priority_parent = temp_parent->priority;
  }
}

void swap(struct pq* pq, int parent, int child) {
  struct element* temp_parent = dynarray_get(pq->array, parent);
  struct element* temp_child = dynarray_get(pq->array, child);
  dynarray_set(pq->array, parent, temp_child);
  dynarray_set(pq->array, child, temp_parent);
}

void bubble_down(struct pq* pq) {
  int left;
  int right;
  int left_priority = 65;
  int right_priority = 65;
  struct element* left_child;
  struct element* right_child;
  struct element* parent;
  int size = dynarray_size(pq->array) - 1;
  int index = 0;


  while(2*index+1 < size) {
    left = (2*index) + 1;
    right = (2*index) + 2;
    parent = dynarray_get(pq->array, index);
    if(left < size) {
      right_child = dynarray_get(pq->array, right);
      right_priority = right_child->priority;
    }
    if(left <= size) {
      left_child = dynarray_get(pq->array, left);
      left_priority = left_child->priority;
      // printf("This is the prio left: %d\n", left_priority);
      // printf("This is the right prio: %d\n", right_priority);

    }
    if(left_priority < parent->priority && left_priority <= right_priority) {
      // printf("This is hitting the left swap");
      swap(pq, left, index);
      index = left;
    }
    else if(right_priority < parent->priority && right_priority <= left_priority) {
      swap(pq, right, index);
      index = right;
    }
    else {
      index = size;
    }
  }
}