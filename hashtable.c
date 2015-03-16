//
//  hashtable.c
//  nimo
//
//  Created by 易国磐 on 15-3-15.
//  Copyright (c) 2015年 易国磐. All rights reserved.
//

#include <limits.h>
#include <stdlib.h>

#include "hashtable.h"
#include "assert.h"

#include "mem.h"

hashtable create_hashtable(int allocs, int (*cmp)(void const* x, void const *y), int (*hash)(void *p))
{
    hashtable table = NM_CALLOC(1, sizeof(*table) + allocs * sizeof(struct buckets));
    assert(table);
    assert(allocs >= 0);
    int k = 0;
    while (allocs) {
        k++;
        allocs >>=2;
    }
    
    allocs = 2 << k;
    
    table->allocs  = allocs;
    table->elems   = 0;
    table->hash    = hash?hash:hashfunc;
    table->cmp     = cmp?cmp:cmpfunc;
    table->keydup  = NULL;
    table->valdup  = NULL;
    table->keyfree = NULL;
    table->valfree = NULL;
    
    table->bucket = (buckets *)(table + sizeof(*table));
    int i;
    for (i = 0; i < allocs; i++) {
        table->bucket[i] = NULL;
    }
    
    return table;
}

void *hashtable_get(hashtable table, void *key)
{
    assert(table);
    assert(key);
    assert(table->allocs > 0);
    
    int i = (table->hash)(key) % table->allocs;
    buckets p = table->bucket[i];
    for (; p; p = p->next) {
        if (!(*table->cmp)(p->key, key)) {
            return p->val;
        }
    }
    
    return NULL;
}

static buckets buckets_new(hashtable table, void *key, void *val)
{
    buckets bucket = NM_CALLOC(1, sizeof(*bucket));
    assert(bucket);
    table_set_key(table, bucket, key);
    table_set_val(table, bucket, val);
    bucket->next = NULL;
    return bucket;
}

int hashtable_set(hashtable table, void *key, void *value)
{
    assert(table);
    assert(key);
    assert(table->allocs > 0);
    int i = (table->hash)(key) % table->allocs;
    buckets p;
    for (p = table->bucket[i]; p; p = p->next) {
        if ((*table->cmp)(p->key, key)) {
            p->val = value;
            return HASHTABLE_SUCCESS;
        }
    }
    buckets newp = buckets_new(table, key, value);
    if (table->bucket[i]) {
        table->bucket[i]->next = newp;
        table->bucket[i] = newp;
    } else {
        table->bucket[i] = newp;
    }
    table->elems++;
    return HASHTABLE_SUCCESS;
}

int hashtable_del(hashtable table, void *key)
{
    assert(table);
    assert(key);
    int i = (*table->hash)(key) % table->elems;
    buckets p,q = NULL;
    for (p = table->bucket[i]; p; p = p->next) {
        if ((*table->cmp)(p->key, key)) {
            if (q && q!=0x0) q->next = p->next;
            if (table->keyfree) table->keyfree(p->key);
            if (table->valfree) table->valfree(p->val);
            table->elems--;
            NM_FREE(p);
            return HASHTABLE_SUCCESS;
        } else {
            q = p;
        }
    }
    
    return HASHTABLE_FAILURE;
}

void hashtable_free(hashtable *table)
{
    assert(table && *table);
    int i = 0;
    buckets p,q = NULL;
    while (i < (*table)->allocs) {
        for (p = (*table)->bucket[i]; p; p = q) {
            if ((*table)->keyfree) (*table)->keyfree(p->key);
            if ((*table)->valfree) (*table)->valfree(p->val);
            q = p->next;
            NM_FREE(p);
        }
        i++;
    }
    NM_FREE(*table);
}

void hashtable_map(hashtable table, void (*map)(void const*key, void *val))
{
    assert(table);
    int i = 0;
    buckets p;
    while (i < (table)->allocs) {
        for (p = (table)->bucket[i]; p; p = p->next) {
            map(p->key, p->val);
        }
        i++;
    }
}
