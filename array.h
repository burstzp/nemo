//
// Created by 易国磐 on 15-3-14.
//

#ifndef _NIMO_ARRAY_H_
#define _NIMO_ARRAY_H_
typedef struct array_t *array_t;
struct array_t {
    void *ptr;
    int size;
    int allocs;
    int length;
} ;

array_t array_new(int size, int elems);
void array_set_multi(array_t array, void *, ...);
void array_set(array_t array, int idx, void *ptr);
void* array_get(array_t array, int idx);
void array_push(array_t array, void *ptr);
void array_pop(array_t array, void *ret);
int array_sum(array_t array);
void array_map(array_t array, void (*map)(void *ptr));
void array_walk(array_t array, void *ret, void (*walk)(void *ptr, void *ret));
void array_sort(array_t array, int (*cmp)(void const *ptr1, void const *ptr2));
int array_length(array_t array);
void array_free(array_t *array);
int array_allocs(array_t array);
array_t array_copy(array_t array, void *(*cpy)(void *ptr));
array_t array_merge(array_t array1, array_t array2);

#endif //_NIMO_ARRAY_H_
