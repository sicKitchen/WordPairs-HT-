//
//  ht.c
//  wordpairs
//
//  Created by Spencer Kitchen on 1/20/16.
//  Copyright © 2016 Spencer Kitchen. All rights reserved.
//

#include "ht.h"
#include "crc64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------
Ht* htNew(int size){
    Ht *newHT = NULL;                   // initilize new hashtable
    
    if((newHT =(Ht *)malloc(sizeof(Ht))) == NULL){    // give it space the size of Ht structure
        fprintf(stderr, "could not allocate memory\n");
        exit(2);
    }
    if((newHT->table = (KVpair**)malloc(sizeof(KVpair*) * size)) == NULL){  // give space to table part of Ht
        fprintf(stderr, "could not allocate memory\n");
        exit(2);
    }
    for (int i=0; i<size; i++) {
        newHT->table[i] = NULL;         // initialize
        newHT->count = 0;               // nothing has been inserted
    }
    newHT->size = size;
    
    return newHT;
}
//------------------------------------------------------
void htInsert(Ht *hashTable, char *key, void *value){
    
    long double size = hashTable->size;
    long double count = hashTable->count;
    long double loadFactor = (count / size);
    //printf("loadFactor: %Lf\n", loadFactor);
    
    int bucket = htHashValue( hashTable, key);
    
    KVpair *current = NULL;        // initilize for transversing LL
    KVpair *prev = NULL;
    
    current = hashTable->table[bucket]; // set current of LL to first slot in bucket
    
    //transverse LL
    while (current != NULL && strcmp(key, current->key) != 0) {
        prev = current;
        current = current->next;
    }
    
    KVpair *newKV = NULL;                       // make new kvpair
    if((newKV = (KVpair *)malloc(sizeof(KVpair))) == NULL){
        fprintf(stderr, "could not allocate memory\n");
        exit(2);
    }
    // set mem for newkv
    newKV->key = key;
    newKV->next = NULL;
    newKV->value = value; // assume value is correct, handled in main.c
        
    // insert at begining of LL
    if (current == hashTable->table[bucket]) {  // current is equal to start if bucket LL
        newKV->next = current;                  // newKV points to NULL
        hashTable->table[bucket] = newKV;       // start of bucket points to newKV
        //printf("insert at begining\n");
            
    // insert at end of LL
    } else {
        prev->next = newKV;                 // prev is on last element of LL,
        //printf("insert at end\n");        // set prev->next to new KVpair inserted
    }
    
    // test if load factor has got to high
    if (loadFactor > .7f) {
        resizeHt(hashTable);
    }
}

//-----------------------------------------------------
// get hash number
int htHashValue( Ht *hashTable, char *key){
    int hashValue = 0;
    hashValue = crc64(key) % hashTable->size;
    return hashValue;
}
//----------------------------------------------------
// searchs for match in hash table and returns value, value is a void* so have to cast
// search to value type.
//
void* htSearch(Ht *hashTable, char *key){
    int bucket = htHashValue(hashTable, key);
    KVpair *current;                                    // to hold where we are in LL
    current = hashTable->table[bucket];
    while (current != NULL && current->key != NULL) {
        if (strcmp(key, current->key) != 0) {           // not the same
            current = current->next;                    // transverse LL
        }
        else if (strcmp(key, current->key) == 0) {      // they match
            return current->value;
        }
    }
    return NULL;                                        // didnt find a match
}
//-----------------------------------------------------
// Applies whatever the function passed into it does to every node in the LL
// when creating functions for htApply you can only pass in a 'key' and a
// 'Value'. Function only is scope of single node as apply handles transversing
void htApply(Ht *hashTable, void (* func)(char *key,void *value)){
    KVpair *current = NULL;
    for (int bucket=0; bucket < hashTable->size; bucket++) {   //loop through hashtable
        current = hashTable->table[bucket];
        if (current != NULL) {                  // if not empty
            while (current != NULL) {           // loop through LL
                func(current->key, (int*)current->value);   // function gets key,value passed in
                current = current->next;                    // tansverse to next element
            }
        }
    }
}
//----------------------------------------------------------

void htDelete(Ht *hashTable, void (* func)(KVpair *current)){
    KVpair *current = NULL;
    KVpair *temp = NULL;
    
    for (int bucket=0; bucket < hashTable->size; bucket++) {   //loop through hashtable
        current = hashTable->table[bucket];
        while (current != NULL) {           // loop through LL
            if (current->next != NULL) {
                temp = current->next;       // save the next pointer
                current->next = NULL;       // disconnect current->next
                func(current);              // free function
                current = temp;             // set current to next pointer
            } else {
                func(current);
                current = NULL;
                
            }
        }
    }
}
//----------------------------------------------------

void resizeHt(Ht* hashTable){
    Ht *biggerHt = htNew(hashTable->size * 3);
    //printf("hashTable: %d      biggerHt: %d\n", hashTable->size, biggerHt->size);
    
    KVpair *current = NULL;
    for (int bucket=0; bucket < hashTable->size; bucket++) {   //loop through hashtable
        current = hashTable->table[bucket];
        if (current != NULL) {                  // if not empty
            while (current != NULL) {           // loop through LL
                biggerHt->count = hashTable->count;             // count stays the same
                htInsert(biggerHt, current->key, current->value); // insert into bigger table
                current->key = NULL;                        // disconnect key & value
                current->value = NULL;
                current = current->next;                    // tansverse to next element
            }
        }
    }
    
    KVpair **temp1 = hashTable->table;          // holds small table
    hashTable->table = biggerHt->table;         // sets small table to bigger table
    hashTable->size = biggerHt->size;           // transfer size from big table to small table
    biggerHt->table = temp1;
    biggerHt->size = (biggerHt->size / 3);
    
    htDelete(biggerHt, _freeKVpair);
    free(biggerHt->table);
    free(biggerHt);
}

//------------------------------------------------------
void _freeKVpair(KVpair *current){
    if ((current->value == NULL) || (current->key == NULL)) {
        // empty KVpair
        free(current);
    
    // full KVpair
    } else {
        free(current->value);
        free(current->key);
        free(current);
    }
}












