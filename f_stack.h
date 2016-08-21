#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef MemoryError
#define MemoryError() do{                                           \
    printf("Memory Error. File: %s Line: %d", __FILE__, __LINE__);  \
    exit(0);                                                        \
}while(0);
#endif

typedef struct Stack_s{
    void **top;
    void **bottom;
    int capacity;
} Stack_t;

#define StackNew(s,_size) do{                                       \
    (s) = malloc(sizeof(Stack_t));                                  \
    if((s) == NULL)                                                 \
        MemoryError();                                              \
    (s)->capacity = _size;                                          \
    (s)->top = (s)->bottom = malloc(sizeof(void*) * _size);         \
    if((s)->top == NULL)                                            \
        MemoryError();                                              \
} while(0);

static inline int StackLen( Stack_t *s )
{
    return ((uintptr_t)s->top - (uintptr_t)s->bottom) / sizeof(void *);
}

static inline void StackPush( Stack_t *s, void *data )
{
    if(StackLen(s) != s->capacity)
        *s->top++ = data;
}

static inline void *StackPeek( Stack_t *s )
{
    return *(s->top - 1);
}

static inline void *StackPop( Stack_t *s )
{
    return (s->top == s->bottom) ? NULL: *(--s->top);
}
#endif
