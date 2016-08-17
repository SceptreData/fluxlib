#include "f_array.h"

f_array_t *f_ArrCustomNew( const f_icd *icd )
{
    f_array_t *arr = malloc(sizeof(f_array_t));
    if ( arr == NULL ) oom();
    arr->next_idx = 0;
    arr->size = 0;
    arr->icd = *icd;
    arr->data = NULL;
    return arr;
}

f_array_t *f_ArrNew( size_t size )
{
    f_icd icd = {size, NULL, NULL, NULL};
    f_array_t *new = f_ArrCustomNew( &icd );
    return new;
}

/*
 *  f_ArrReserve():
 *  Ensures that num amount of extra slots are available.
 *  If not, doubles the size of the allocated memory array until num slots are free.
 */
void f_ArrReserve( f_array_t *arr, int num )
{
    if ( (arr->next_idx + num) > arr->size ){
        char *tmpArr;
        while ( (arr->next_idx + num) > arr->size ){
            if ( arr->size )
                arr->size = 2 * arr->size;
            else
                arr->size = 8;
        }
        tmpArr = (char *) realloc(arr->data, arr->size * arr->icd.size );
        if (tmpArr == NULL) oom();
        arr->data = tmpArr;
    }
}

/*
 *   If our arr has a custom destructor function, call it on each element.
 *   Then, free the arr memory array.
 */
void f_ArrClear( f_array_t *arr )
{
    if (arr->size){
        if (arr->icd.dtor){
            unsigned cur;
            for ( cur = 0; cur < arr->next_idx; cur++) {
                arr->icd.dtor(f_ArrGet(arr, cur));
            }
        }
        free(arr->data);
    }
    arr->size = 0;
}

void f_ArrFree( f_array_t *arr )
{
    f_ArrClear( arr );
    free(arr);
}

/*
 *   if new size is smaller than current array:
 *       invoke destructor on extra elements.
 *   else:
 *       extend array
 *       call init function if appropriate
 *       else:
 *           memset new elements to 0. 
 *   NOTE: sets current arrIndex to last element meaning Append/Pop operations 
 *         will grab zero'd out elements if the resize has caused an extend.
 *         If you just want to extend memory, use f_ArrReserve.
 *         Most useful when you want to invoke init function on large number of elements.
 */
void f_ArrResize( f_array_t *arr, size_t num )
{
    size_t cur;
    if ( arr->next_idx > num ){
        if (arr->icd.dtor) {
            for ( cur = num; cur < arr->next_idx; cur++){
                arr->icd.dtor(f_ArrGet(arr,cur));
            }
        }
    }
    else if ( arr->next_idx < (size_t)num ){
        f_ArrReserve(arr, num - arr->next_idx);
        if ( arr->icd.init ){
            for ( cur = arr->next_idx; cur < num; cur++){
                arr->icd.init(f_ArrGet(arr, cur));
            }
        } else {
            memset(f_ArrGet(arr, arr->next_idx), 0, arr->icd.size * (num - arr->next_idx));
        }
    }
    arr->next_idx = num;
}

/*
 * f_ArrGet()
 * Returns pointer to item memory at desired index.
 * Make sure to cast to desired type.
 */
char *f_ArrGet( f_array_t *arr, size_t idx )
{
    if ( idx < arr->next_idx )
        return (char *)(arr->data + (arr->icd.size * idx));
    else
        return NULL;
}

void f_ArrInsert( f_array_t *arr, void *item, size_t idx)
{
    if (idx > arr->next_idx){
        f_ArrResize(arr, idx);
    }

    f_ArrReserve(arr, 1);
    if (idx < arr->next_idx){
        char *old_slot = f_ArrGet(arr, idx + 1);
        char *new_slot = f_ArrGet(arr, idx);
        size_t memblock_size = (arr->next_idx - idx) * arr->icd.size;
        memmove(old_slot, new_slot, memblock_size);
    }
    if (arr->icd.copy){
        arr->icd.copy(f_ArrGet(arr, idx), item);
    } else {
        memcpy(f_ArrGet(arr, idx), item, arr->icd.size);
    }
    arr->next_idx++;
}

void f_ArrPush( f_array_t *arr, void *item )
{
    f_ArrInsert(arr, item, 0);
}

void f_ArrAppend( f_array_t *arr, const void *item)
{
        f_ArrReserve(arr, 1);
        if( arr->icd.copy ){
            arr->icd.copy(f_ArrGet(arr, arr->next_idx++), item);
        } else {
            char *mem_slot = f_ArrGet(arr, arr->next_idx);
            memcpy(mem_slot, item, arr->icd.size);
            arr->next_idx++;
        }
}
/*
 * f_Arr_pop:
    returns pointer to last element in arr, then shrinks arr by 1.
    Important: If custom destructor function is used, PoP will return NULL.
    If you need to use the pointer to the last value, make sure to use 
    f_ArrGet first.
 */
char *f_ArrPop(f_array_t *arr)
{
    if (arr->icd.dtor){
        arr->icd.dtor( f_ArrGet( arr, arr->next_idx - 1));
        arr->next_idx--;
        return NULL;
    } else {
        char *p = f_ArrGet(arr, arr->next_idx - 1);
        arr->next_idx--;
        return p;
    }
}

int f_ArrEltIdx( f_array_t *arr, void *elt)
{
    if ( (char*)elt >= (char *)arr->data )
        return ((char*)elt - (char*)arr->data)/(size_t)arr->icd.size;
    else
        return -1;
}

void *f_ArrFirst( f_array_t *arr )
{
    return (void *)f_ArrGet(arr, 0);
}

void *f_ArrLast( f_array_t *arr )
{
    return (void*)f_ArrGet(arr, arr->next_idx - 1);
}

void *f_ArrNext( f_array_t *arr, void *p )
{
    if (p == NULL)
        return f_ArrFirst(arr);
    else{
        if (arr->next_idx > f_ArrEltIdx(arr, p) + 1)
            return f_ArrGet(arr, f_ArrEltIdx(arr, p) + 1);
        else
            return NULL;
    }
}
void *f_ArrPrev( f_array_t *arr, void *p )
{
    if (p == NULL){
        return f_ArrLast(arr);
    }

    int elt_idx = f_ArrEltIdx(arr, p);
    if (elt_idx > 0){
        return f_ArrGet(arr, elt_idx - 1);
    } else {
        return NULL;
    }
}

void f_ArrEraseN( f_array_t *arr, int pos, int len )
{
    if( arr->icd.dtor ){
        size_t j;
        for( j = 0; j < len; j++){
            arr->icd.dtor(f_ArrGet(arr, j + pos));
        }
    }
    if (arr->next_idx > (pos + len)){
        char *dst_block = f_ArrGet(arr, pos);
        char *src_block = f_ArrGet(arr, pos + len);
        int block_size = arr->next_idx - (pos +len) * (arr->icd.size);
        memmove(dst_block, src_block, block_size);
    }
    arr->next_idx -= len;
}

f_array_t *f_ArrCopy( f_array_t *arr )
{
    f_array_t *new_arr = f_ArrCustomNew( &(arr->icd) );
    if ( new_arr == NULL ){
        oom();
    }

    size_t memblock_size = (arr->size * arr->icd.size );
    new_arr->data = malloc( memblock_size );
    if (new_arr->data == NULL){
        oom();
    }

    memcpy(new_arr->data, arr->data, memblock_size);
    new_arr->next_idx = arr->next_idx;
    new_arr->size = arr->size;

    return new_arr;
}

void f_ArrSort( f_array_t *arr, int (*cmp)() )
{
    qsort( arr->data, arr->next_idx, arr->icd.size, cmp );
}

void *f_ArrSearch( f_array_t *arr, void *value, int (*cmp)() )
{
    return bsearch( value, arr->data, arr->next_idx, arr->icd.size, cmp );
}
/*
 * Iterate through arr using *next.
 * O(n) time vs O(log_n) for search.
 */

bool f_ArrContains( f_array_t *arr, void *item, bool (*cmp)(const void *, const  void *))
{
    void *p = NULL;
    while ( (p = f_ArrNext(arr, p)) != NULL ){
        if ( cmp( p, item ) == true ){
            return true;
        }
    }

    return false;
}

int f_ArrLen( f_array_t *arr)
{
    return (int)arr->next_idx;
}

bool f_ArrIsEmpty( f_array_t *arr)
{
    return (arr->next_idx == 0);
}

bool f_ArrIsFull( f_array_t *arr)
{
    return (arr->next_idx >= arr->size);
}

/*
   custom string copy/destruct functions.
 * f_Arr_strcpy()
    cast src/dst to char** (string), copy string using strdup.
   f_Arr_strdst()
 */
void f_ArrStrcpy( void *dst, const void *src )
{
    char **slot = (char**)dst;
    char **str  = (char**)src;
    if ( *str == NULL )
        *slot = NULL;
    else
        *slot = strdup(*str);
}

void f_ArrStrDestroy( void *str )
{
    char **_str = (char**)str;
    if(*_str){
        free(*_str);
    }
}
