#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Stack_s
{
    void **top;
    void **bottom;
} Stack_t;

Stack_t *NewStack( size_t size );

void *PeekStack( Stack_t *s );

void *PopStack( Stack_t *s );

int LenStack( Stack_t *s );

#endif
