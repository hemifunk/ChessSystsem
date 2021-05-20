#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>

/**
 * FreeFunc is a function used to free an object stored as void*.
 */
typedef void (*FreeFunc)(void* element);

/**
 * CopyFunc is a function used to copy an object stored as void*.
 */
typedef void* (*CopyFunc)(void* element);

/**
 * Node is a pointer to data container used to store a single element in a linked list.
 */
typedef struct Node_t* Node;

/**
 * List is a pointer to a linked list.
 */
typedef struct List_t* List;

/**
 * listCreate creates a new empty, generic linked list, of length 0.
 * 
 * @param copy_func - a function used to copy this list's objects.
 * @param free_func - a function used to free this list's objects.
 * @returns - The new list. Returns null if any errors occur.
 */
List listCreate(CopyFunc copy_func, FreeFunc free_func);

/**
 * listDestroy frees the memory used by a list and the objects it stores.
 * @param list - the list to free.
 */
void listDestroy(List list);

/**
 * listGetPtr gets a pointer to the variable holding a pointer to a generic element inside a list.
 * @param list - the list the get the pointer from.
 * @param index - the index of the object to het the pointer from.
 * @returns - returns a pointer of type void** to the variable storing the pointer of the requested generic object. Returns null if any errors occur.
 */
void** listGetPtr(List list, int index);

/**
 * listGet returns a pointer to a generic element inside a list.
 * @param list - the list the get the pointer from.
 * @param index - the index of the object to het the pointer from.
 * @returns - returns a pointer of type void*, which points to the requested generic object. Returns null if any errors occur.
 */
void* listGet(List list, int index);

/**
 * listAdd adds a new generic element to a list.
 * @param list - the list to add an element into.
 * @param value - a pointer to the generic element to be added into the list.
 * @returns - true if added successfully, false otherwise.
 */
bool listAdd(List list, void* value);

/**
 * listCopy makes a deep copy of the provided list.
 * @param list - the list to make a copy of.
 * @returns - a deep copy of the provided list. Returns null if any errors occur.
 */
List listCopy(List list);

/**
 * listRemove removes an element in a specific index from a list.
 * @param list - the list to remove the element from.
 * @param index - the index of the element to be removed.
 * @returns - true if removed successfully, false otherwise.
 */
bool listRemove(List list, int index);

/**
 * listSize returns the size of a given list.
 * @param list - the list the get the size of.
 * @returns - the size of a given list. returns 0 if any errors occur.
 */
int listSize(List list);

/**
 * listClear frees all the elements stored in the list, and sets it size to 0.
 * @param list - the list to clear.
 */
void listClear(List list);

#endif
