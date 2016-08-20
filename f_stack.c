#include "f_stack.h"

Stack_t *NewStack( size_t size )
{
    Stack_t *s = malloc(sizeof(Stack_t));
    if( s == NULL)
        exit(1);
    s->top = s->bottom = malloc( sizeof(void *) * size );
    return s;
}

void PushStack( Stack_t *s, void *data )
{
    *s->top++ = data;
}

void *PeekStack( Stack_t *s )
{
    return *(s->top - 1);
}

void *PopStack( Stack_t *s )
{
    if( s->top == s->bottom )
        return NULL;
    return *(--s->top);
}
// alternatively return (s->top == s->bottom ) ? *(--s->top): NULL;

int LenStack( Stack_t *s )
{
    return ((uintptr_t)s->top - (uintptr_t)s->bottom) / sizeof(void *);
}
