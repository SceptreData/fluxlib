#ifndef F_VEC2_H
#define F_VEC2_H
/*  f_vec2.h
 *  David Bergeron 2016
 *  Small home-cooked 2d vector library. Return value passed to the function
 *  as a paramater to ensure minimum overhead.
 */
#include <stdio.h>
#include <stdbool.h>  
#include <math.h>

typedef struct vec2_s{
    float x, y;
} vec2_t;

static inline void Vec2Clone( vec2_t *clone, vec2_t *old_v )
{
    *clone = *old_v;
}

static inline void Vec2Add( vec2_t *r, vec2_t *a, vec2_t *b)
{
    r->x = a->x + b->x;
    r->y = a->y + b->y;
}

static inline void Vec2Sub( vec2_t *r, vec2_t *a, vec2_t *b)
{
    r->x = a->x - b->x;
    r->y = a->y - b->y;
}

static inline void Vec2Scale( vec2_t *r, vec2_t *v, float scale)
{
    r->x = v->x * scale;
    r->y = v->y * scale;
}

static inline void Vec2Div( vec2_t *r, vec2_t *v, float div )
{
    r->x = v->x / div;
    r->y = v->y / div;
}

static inline float Vec2Dot( vec2_t *a, vec2_t *b)
{
    return (a->x * b->x) + (a->y * b->y);
}

static inline float Vec2Cross( vec2_t *a, vec2_t *b)
{
    return (a->x * b->y) - (a->y * b->x);
}

static inline bool Vec2Equal( vec2_t *a, vec2_t *b )
{
    return (a->x == b->x && a->y == b->y);
}

static inline float Vec2Len( vec2_t *v )
{
    return sqrtf(v->x * v->x + v->y * v->y);
}

static inline float Vec2Len2( vec2_t *v )
{
    return (v->x * v->x) + (v->y * v->y);
}

static inline float Vec2Dist( vec2_t *a, vec2_t *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    return sqrtf((dx * dx) + (dy * dy));
}

static inline float Vec2Dist2( vec2_t *a, vec2_t *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    return ((dx * dx) + (dy * dy));
}

static inline void Vec2Normalize(vec2_t *r, vec2_t *v )
{
    float len = Vec2Len(v);
    if( len > 0 ){
        r->x = v->x / len;
        r->y = v->y / len;
    }
}

static inline void Vec2Rotate( vec2_t *ret, vec2_t *vec, float angle )
{
    float cos_val = cosf(angle);
    float sin_val = sinf(angle);
    ret->x = cos_val * vec->x - sin_val * vec->y;
    ret->y = sin_val * vec->x + cos_val * vec->y;
}

static inline float Vec2AngleTo( vec2_t *a, vec2_t *b )
{
    if( b == NULL){
        return atan2f(a->y, a->x);
    }
    return atan2f(a->y, a->x) - atan2f(b->y, b->x);
}

static inline void Vec2Perpendicular( vec2_t *r, vec2_t *v )
{
    r->x = -(v->y);
    r->y = v->x;
}

static inline void Vec2String( char *buf, vec2_t *vec )
{
    sprintf( buf, "(%.2f, %.2f)", vec->x, vec->y);
}

static inline void Vec2Print( const vec2_t *v )
{
    printf("(%.2f, %.2f)\n", v->x, v->y);
}
#endif
