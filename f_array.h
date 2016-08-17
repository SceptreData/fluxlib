/*
 * FluxLib Array.h
 * For Convenient/Generic Array functions.
 * Support for custom Constructor/Destructor functions
 * David Bergeron c2016
 */

#ifndef F_ARRAY_H
#define F_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define oom() do{                                                             \
    printf("Memory Error. File: %s Line: %d", __FILE__, __LINE__);            \
    exit(0);                                                                  \
}while(0);

typedef void (init_f)(void *func);
typedef void (ctor_f)(void *dst, const void *src);
typedef void (dtor_f)(void *func);

typedef struct f_icd {
    size_t size;
    init_f *init;
    ctor_f *copy;
    dtor_f *dtor;
} f_icd;

typedef struct f_array{
    size_t next_idx;  // i: index of next available slot.
    size_t size;  //  current number of allocated slots.
    f_icd icd; // init/construct/destructor functions
    char *data; // m slots of icd->size*
} f_array_t;

#define F_ARRAY_NEW(type) f_array_new(sizeof(type))

/*
 * f_ArrNew()
 * returns an empty array. Array slot size is equal to 
 * size passed to function,
 */

f_array_t *f_ArrNew( size_t size );

/*
 * f_ArrCustomNew()
 * Returns empty Array, but allows you to specify custom constructor/destructor
 * functions. Refer to struct f_icd above.
 */

f_array_t *f_ArrCustomNew(const f_icd *icd );

/*
 *  f_ArrReserve():
 *  Ensures that num amount of extra slots are available.
 *  If not, doubles the size of the allocated memory array until num slots are free.
 */

void f_ArrReserve( f_array_t *arr, int num );

/* f_ArrClear()
 * Zeroes out an array. Does not free.
 * If an array has a custom destructor function it will be applied to each
 * element in the array.
 */

void f_ArrClear( f_array_t *arr );

/* f_ArrFree()
 * Frees your f_array_t item.
 */

void f_ArrFree( f_array_t *arr );

/* f_ArrResize()
 *  Resizes the array to given num of slots.
 *  If new size is smaller than existing array, will destroy extra
 *  objects if a destructor function is provided.
 *  NOTE: Sets arr->next_idx to the end of the resized array. If you are just
 *  looking to extend the array's memory use f_ArrReserve().
 */

void f_ArrResize( f_array_t *arr, size_t num );

/* f_ArrGet()
 *  returns a pointer(char *) to the provided index of our array.
 *  If using this function, don;t forget to cast the return value to the 
 *  desired type.
 */

char *f_ArrGet( f_array_t *arr, size_t idx);

/* f_ArrInsert()
 * f_ArrAppend()
 * f_ArrPush()
 *  Functions for adding elements to your array.
 *  Insert adds elt to given index.
 *  Append adds elt to end of array.
 *  Push adds elt to front of array
 */

void f_ArrAppend( f_array_t *arr,const void *item);
void f_ArrInsert( f_array_t *arr, void *item, size_t i);
void f_ArrPush( f_array_t *arr, void *item);

/*
 * f_ArrPop()
 *  Pops the last value from the provided array
 *  NOTE:
 *  if no custom destructor is used, will return a pointer to popped elt.
 *  if Custom Destructor is used, will return NULL.
 */ 

char *f_ArrPop( f_array_t *arr );

/* f_ArrEltIdx()
 *  Returns the Index of a given Array Element.
 *  Used by f_ArrNext() and f_ArrPrev()
 */

int f_ArrEltIdx( f_array_t *arr, void *elt );

/* f_ArrFirst(), f_ArrLast()
 *returns pointer to First or Last element of Array respectively.
 */

void *f_ArrFirst( f_array_t *arr );
void *f_ArrLast( f_array_t *arr );

/* f_ArrNext(), f_ArrPrev()
 *  Functions for iterating through your array.
 *  Returns NULL if provided element is last/first in list.
 */

void *f_ArrNext( f_array_t *arr, void *p );
void *f_ArrPrev( f_array_t *arr, void *p );

/* f_ArrEraseN()
 *   Erases n elements from provided index.
 *   if elements exist beyond the given boundary, they are moved so the list
 *   wont contain empty values.
 */

void f_ArrEraseN( f_array_t *arr, int idx, int n );

/* f_ArrCopy()
 *  returns a new array. Copies provided array and all data into the new array.
 */

f_array_t *f_ArrCopy( f_array_t *arr );

/* f_ArrSort()
 *  Sort your array with provided comparision function.
 */

void f_ArrSort( f_array_t *arr, int (*cmp)());

/* f_ArraySearch()
 *  performs a binary search on your array using the given comparison function.
 */

void *f_ArraySearch( f_array_t *arr, void *value,int (*cmp)());

/* f_ArrContains()
 *  Searches your array for a given value.
 *  Differs from f_ArraySearch in that it iterates over the elements instead of using
 *  a binary search. ( O(n) time vs O(log_n) )
 */

bool f_ArrContains( f_array_t *arr, void *value, bool (*cmp)(const void *, const void *));

/* f_ArrLen()
 *  returns current number of elements in the array.
 */

size_t f_ArrLen( f_array_t*arr );

/* f_ArrIsEmpty()
 *  returns true if array has zero elements.
*/

bool f_ArrIsEmpty( f_array_t*arr );

/* f_ArrIsFull()
 *  returns true if array currently has the max number of elements.
 */

bool f_ArrIsFull( f_array_t *arr);

/* f_ArrStrCpy(), f_ArrStrDestroy()
 *  Custom string constructor/destructor functions for arrays of strings.
 */

void f_ArrStrcpy( void *dst, const void *src );
void f_ArrStrDestroy( void *item );

/* f_icd_str
 *  a prebuilt icd (init, constructor, destructor) to allow you to build an
 *  array of strings.
 */

static const f_icd f_icd_str = {sizeof(char*), NULL, f_ArrStrcpy, f_ArrStrDestroy};

#endif
