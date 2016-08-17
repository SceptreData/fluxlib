/*
 * FluxLib Array.h
 * For Convenient/Generic Array functions.
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

f_array_t *f_ArrNew( size_t size );
f_array_t *f_ArrCustomNew(const f_icd *icd );
void f_ArrReserve( f_array_t *arr, int num );
void f_ArrClear( f_array_t *arr );
void f_ArrFree( f_array_t *arr );
void f_ArrResize( f_array_t *arr, size_t num );

char *f_ArrGet( f_array_t *arr, size_t i);
void f_ArrAppend( f_array_t *arr,const void *item);
void f_ArrInsert( f_array_t *arr, void *item, size_t i);
void f_ArrPush( f_array_t *arr, void *item);
char *f_ArrPop( f_array_t *arr );

int f_ArrEltIdx( f_array_t *arr, void *elt );
void *f_ArrFirst( f_array_t *arr );
void *f_ArrLast( f_array_t *arr );
void *f_ArrNext( f_array_t *arr, void *p );
void *f_ArrPrev( f_array_t *arr, void *p );
void f_ArrEraseN( f_array_t *arr, int idx, int n );
f_array_t *f_ArrCopy( f_array_t *arr );

void f_ArrSort( f_array_t *arr, int (*cmp)());
void *f_ArraySearch( f_array_t *arr, void *value,int (*cmp)());
bool f_ArrContains( f_array_t *arr, void *value, bool (*cmp)(const void *, const void *));

int f_ArrLen( f_array_t*arr );
bool f_ArrIsEmpty( f_array_t*arr );
bool f_ArrIsFull( f_array_t *arr);

void f_ArrStrcpy( void *dst, const void *src );
void f_ArrStrDestroy( void *item );
static const f_icd f_icd_str = {sizeof(char*), NULL, f_ArrStrcpy, f_ArrStrDestroy};

#endif

