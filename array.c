//
// Created by 易国磐 on 15-3-14.
//
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "array.h"
#include "mem.h"

array_t array_new(int size, int allocs)
{
    assert(size && allocs);
    array_t array = NM_CALLOC(1, sizeof(*array) + size * allocs);
    assert(array);
    array->size  = size;
    array->allocs = allocs;
    array->length = 0;
    array->ptr   = (void *)(array + sizeof(*array));
    return array;
}

void array_set(array_t array, int idx, void *ptr)
{
    assert(array);
    assert(array->ptr);
    if (idx < array->allocs) {
        memcpy(array->ptr + idx * array->size, ptr, array->size);
    } else {
        int allocs = array->allocs * 2;
        assert(idx < allocs);
        array->allocs = allocs;
        
        array = NM_REALLOC(array, allocs * array->size + sizeof(*array));//
        assert(array);
        array->ptr = (void *)(array + sizeof(*array));
        memcpy(array->ptr + idx * array->size, ptr, array->size);
    }
    array->length++;
}

void array_set_multi(array_t array, void *x, ...)
{
    assert(array);
    assert(array->ptr);
    
    va_list ap;
    va_start(ap, x);
    int i = array->length > 0 ? array->length: 0;
    
    for (; x != NULL && x; x = va_arg(ap, void *)) {
        array_set(array, i, x);
        i++;
    }
    va_end(ap);
}
void* array_get(array_t array, int idx)
{
    assert(array);
    assert(array->ptr && idx < array->allocs);
    return array->ptr + idx * array->size;
}

void array_push(array_t array, void *ptr)
{
    array_set(array, array->length, ptr);
}

void array_pop(array_t array, void *ret)
{
    array_t p = array_get(array, array->length - 1);
    if (ret && ret != 0x0) {
        memcpy(ret, p, p->size);
    }
    
    NM_FREE(p);
}

void array_shift(array_t array, void *ptr)
{
    
}

void* array_unshift(array_t array)
{
    return NULL;
}

void array_map(array_t array, void (*map)(void *ptr))
{
    assert(array);
    assert(array->ptr);
    int j;
    void *p = NULL;
    for (j = 0; j < array->length; ++j) {
        p = array->ptr + j * array->size;
        if (p && p!= 0x0) map(p);
    }
}

void array_walk(array_t array, void *ret, void (*walk)(void *ptr, void *ret))
{
    assert(array);
    assert(array->ptr);
    int j;
    void *p = NULL;
    for (j = 0; j < array->length; ++j) {
        p = array->ptr + j * array->size;
        if (p && p!= 0x0) walk(p, ret);
    }
}


void array_sort(array_t array, int (*cmp)(void const *ptr1, void const *ptr2))
{
    assert(array);
    assert(array->ptr);
    qsort(array->ptr, array->length, array->size, cmp);
}

int array_length(array_t array)
{
    return array->length;
}

int array_allocs(array_t array)
{
    return array->allocs;
}

array_t array_copy(array_t array, void *(*ptr_dup)(void *ptr))
{
    assert(array);
    assert(array->ptr);

    array_t array_dup = array_new(array->size, array->length);
    array_dup->length = array->length;
    
    memcpy(array_dup->ptr, array->ptr, array->length * array->size);
    return array_dup;
}

array_t array_merge(array_t array1, array_t array2)
{
    assert(array1 && array2);
    assert(array1->ptr && array2->ptr);
    assert(array1->size == array2->size);
    
    int length = array1->length + array2->length;
    array_t array = array_new(array1->size, length);
    array->length = length;
    memcpy(array->ptr, array1->ptr, array1->length * array->size);
    memcpy(array->ptr + array1->length * array->size, array2->ptr, array2->length * array->size);
    return array;
}

void array_free(array_t *array)
{
    NM_FREE(*array);
}
