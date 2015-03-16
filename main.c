//
// Created by 易国磐 on 15-3-14.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hashtable.h"
#include "mem.h"

typedef struct {
    int uid;
    
} data;

void dump1(void **ptr)
{
    printf("ptr = %d\n", *((int *)*ptr));
}

void dump(void *ptr)
{
    data *dat = ((data *)ptr);
    printf("ptr = %d \n", dat->uid);
}
int cmp(void const *ptr1, void const *ptr2)
{
    return *(int *)ptr1 - *(int *)ptr2;
}

void print(void const*key, void *val)
{
    printf("table-get = %s,%s\n", (char *)key, (char *)val);
}

void *keydup(const void *key)
{
    char *k = (char *)key;
    return strdup(k);
}

void *valdup(const void *val)
{
    char *v = (char *)val;
    return strdup(v);
}

int scmp(const void *x, const void *y)
{
    return strcmp((char *)x,(char *)y);
}
int main(void)
{
    hashtable table = create_hashtable(16, NULL, NULL);
    set_table_keydup(table, keydup);
    set_table_valdup(table, valdup);
    set_table_keyfree(table, free);
    set_table_valfree(table, free);
    set_table_cmp(table, scmp);
    
    hashtable_set(table, "hehe", "hello");
    hashtable_set(table, "hehe1", "hello2");
    hashtable_map(table, print);
    printf("table-get = %s\n", hashtable_get(table, "hehe"));
    hashtable_free(&table);
//    data dat;
//    dat.uid = 10;
//    
//    array_t array = array_new(sizeof(data), 3);
//    array_set(array, 0, &dat);
//    dat.uid = 11;
//    array_set_multi(array, &dat, &dat, NULL);
//    dat.uid = 9;
//    array_set_multi(array, &dat, NULL);
//    dat.uid = 7;
//    array_set_multi(array, &dat, NULL);
//
//    array_map(array, dump);
//    printf("after sort.\n");
//    
//    array_sort(array, cmp);
//    array_map(array, dump);
//
//    printf("allocs = %d\n", array_allocs(array));
//    array_t a1 = array_copy(array, NULL);
//    printf("dump a1.\n");
//    array_map(a1, dump);
//    
//    array_t a2 = array_new(sizeof(data), 4);
//    data d1;
//    d1.uid = 2;
//    array_set_multi(a2, &d1, NULL);
//    d1.uid = 13;
//    array_push(a2, &d1);
//    printf("dump a2.\n");
//
//    array_map(a2, dump);
//    
//    printf("merge.\n");
//
//    array_t a3 = array_merge(a1, a2);
//    array_sort(a3, cmp);
//    data ret;
//    array_pop(a3, &ret);
//    printf("read = %d\n", ret.uid);
//    array_map(a3, dump);
//    int i = array_sum(a3);
//    //array_walk(a3, &i, sum);
//    printf("i's value = %d.\n", i);
//
//    array_free(&a2);
//    array_free(&a3);
//    array_free(&array);
//    array_free(&a1);

    printf("helloworld");
    exit(0);
}