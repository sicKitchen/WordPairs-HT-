//
//  ht.h
//  wordpairs
//
//  Created by Spencer Kitchen on 1/20/16.
//  Copyright © 2016 Spencer Kitchen. All rights reserved.
//

#ifndef ht_h
#define ht_h

#include <stdio.h>
//--------------------------------------------------------------------
typedef struct _kvPair
{
    char* key;      // holds character string
    void* next;     // points to new entry in bucket
    void* value;    // holds hash key
}KVpair;

typedef struct _ht
{
    int count;      // how many entries into table
    int size;       // default size to create hashtable
    KVpair **table; // pointer to pointer to table made up of KVpairs
}Ht;
//--------------------------------------------------------------------
Ht* htNew(int size);
int htHashValue( Ht *hashTable, char *key);
void htInsert(Ht *hashTable, char *key, void *value);
void* htSearch(Ht *hashTable, char *key);
void htApply(Ht *hashTable, void (* func)(char *key,void *value));
void htDelete(Ht *hashTable, void (* func)(KVpair *current));
void resizeHt(Ht* hashTable);
void _freeKVpair(KVpair *current);


#endif /* ht_h */

