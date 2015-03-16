//
//  hashtable.h
//  nimo
//
//  Created by 易国磐 on 15-3-15.
//  Copyright (c) 2015年 易国磐. All rights reserved.
//

#ifndef __nimo__hashtable__
#define __nimo__hashtable__

#include <stdio.h>

typedef struct hashtable *hashtable;
typedef struct buckets *buckets;

struct buckets
{
    void *key;
    void *val;
    buckets next;
};

struct hashtable
{
    int (*cmp)(void const* x, void const *y);
    int (*hash)(void *p);
    void *(*keydup)(const void *key);
    void *(*valdup)(const void *val);
    void (*keyfree)(void *key);
    void (*valfree)(void *key);
    int elems;
    int allocs;
    buckets *bucket;
};

#define HASHTABLE_SUCCESS 0
#define HASHTABLE_FAILURE -1

hashtable create_hashtable(int size, int (*cmp)(void const* x, void const *y), int (*hash)(void *p));
void *hashtable_get(hashtable table, void *key);
int hashtable_set(hashtable table, void *key, void *value);
int hashtable_del(hashtable table, void *key);
void hashtable_free(hashtable *table);
void hashtable_map(hashtable table, void (*map)(void const* key, void* val));
void** hashtable2array(hashtable table, void *end);

#define set_table_keydup(x,dup)  (x)->keydup = dup;
#define set_table_valdup(x,dup)  (x)->valdup = dup;
#define set_table_keyfree(x,fre) (x)->keyfree = fre;
#define set_table_valfree(x,fre) (x)->valfree = fre;

#define table_set_key(ht,b,k) do{\
if(ht->keydup) (b)->key = (ht)->keydup(k);\
else (b)->key = k;\
}while(0)

#define table_set_val(ht,b,v) do{\
if((ht)->keydup) (b)->val = (ht)->valdup(v);\
else (b)->val = v;\
}while(0)

#define set_table_cmp(x, c) (x)->cmp = c;
#define set_table_hash(x, h) (x)->hash = h;


static inline int cmpfunc(const void *x, const void *y)
{
    return x == y;
}

static inline int hashfunc(void *k)
{
    return (int) k >> 2;
}

#endif /* defined(__nimo__hashtable__) */
