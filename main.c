//
//  main.c
//  wordpairs
//
//  Created by Spencer Kitchen on 1/18/16.
//  Copyright © 2016 Spencer Kitchen. All rights reserved.
//

// ===== exit codes ========
// 1: something to do with opening or closing files
// 2: something to do with allocating memory
//
//---------------------------------------------
#include <stdio.h>
#include "ht.h"
#include <stdlib.h>
#include "getWord.h"
#include <string.h>

#define HT_START 111
//- Struct & Global ---------------------------
typedef struct _MYarray
{
    char* key;          // holds character string
    void* value;        // holds hash key
}Array;

int position = 0;       // for transversing array
Array *array = NULL;    // initilize array

//- declarations -------------------------------
// _name means function is designed to be passed into another function
//
//
void mainInsert(Ht* hashTable, char* key);  // sets value and inserts key
void printHashTable(Ht* hashTable);         // prints whole hashtable
void printArray (Ht *hashTable ,int count); // prints out a certain length of array
void makeNsortArray (Ht *hashTable);        // makes array and sorts it
int _cmpfunc (const void *a,const void *b); // used for qsort compare
void _prettyPrint(char *key, void *val);    // pass into apply to print Ht
void _makeArray(char *key, void* value);    // pass into apply to make array from ht
//===============================================================
int main(int argc, const char * argv[]) {
    
    Ht *MainHashTable = htNew(HT_START);    // initilize hash table
    FILE *fp;                               // initilize file pointer
    char* temp;                             // hold getWord
    int argIter = 1;    // for looping through argv[], start at 1 because 0 is address
    int count=0;        // holds variable -count from command line
    
    // check at least one argument was passed in
    if (argv[1] == NULL) {
        fprintf(stderr, "You did not pass in any files\n");
        exit(1);
    }
    
    // check if first argument is count
    if (argv[1][0] == '-') {
        sscanf(argv[1], "-%d", &count);     // "-%d" will ignore '-' when reading
        //printf("count: %d\n", count);
        argIter++;                          // since count is present move argIter forward +1
    }
    
    while (argIter < argc){
        
        // open file
        if((fp = fopen(argv[argIter], "r")) == NULL) {
            fprintf(stderr, "Could not find File\n");
            exit(1);
        }
        
        temp = getNextWord(fp);             // get first word
        
        char* word1 = NULL;
        char* word2 = NULL;
        char* wordPair = NULL;
        int wordLength;
        
        while (temp != NULL) {
            wordLength = strlen(temp);
            //printf("wordlength: %d\n", wordLength);
            word1=temp;
            temp = getNextWord(fp);
            word2=temp;
            if (word2 == NULL) {
                free(word1);
                break;
            }
            wordLength = wordLength + strlen(temp)+2 ;  // white space plus null term
            //printf("wordlength: %d\n", wordLength);
            //printf("word1: %s, word2: %s\n", word1, word2);
            if((wordPair = (char*) malloc(wordLength)) == NULL){ //set memory to worldlength
                fprintf(stderr, "could not allocate memory\n");
                exit(2);
            }
            strcpy(wordPair, word1);    // copy word one
            free(word1);
            strcat(wordPair, " ");                      // add space
            strcat(wordPair, word2);                    // add word two
            //printf("wordpair: %s\n", wordPair);
            mainInsert(MainHashTable, wordPair);        // insert wordpair into table
        }
        fclose(fp);
        free(temp);
        free(word2);
        argIter++;
    }
    
    makeNsortArray(MainHashTable);
    //printHashTable(MainHashTable);
    printArray(MainHashTable, count);
    
    //----- lets get frees in order --------------
    free(array);
    htDelete(MainHashTable, _freeKVpair);
    free(MainHashTable->table);
    free(MainHashTable);
    return 0;
}

//=======================================================
//- functions -------------------------------
// Sets value to 1 if not in table or increases value if already in hashtable
// Inserts key and value into hashtable.
//
void mainInsert(Ht* hashTable, char* key) {
    int *occurance = (int *)htSearch(hashTable, key);       // check if value is already in Ht
    if (occurance == NULL) {                                // Not in table, need to set value
        if((occurance = (int *)malloc(sizeof(int))) == NULL){
            fprintf(stderr, "could not allocate memory\n");
            exit(2);
        }
        (*occurance) = 1;                                   // always first entry
        hashTable->count++;                                 // increment count since we insert
        htInsert(hashTable, key, occurance);
    } else {
        (*occurance)++;                                     // is in list already, increase value
        free(key);                                          // dont need key anymore
    }
}

//------------------------------------------------
// will print out array up to count passed into function
void printArray (Ht *hashTable ,int count){
    // if -count flag has not been been specified, count = number of inserts 
    if (count == 0 || count > hashTable->count) {   // check if count is greater then woprpairs
        count = hashTable->count;
    }
    
    for (int i = 0; i < count ; i++) {
        printf("%10d %s\n", *((int*)array[i].value), array[i].key);
    }
}
//-----------------------------------------------
void makeNsortArray (Ht *hashTable){
    if((array = calloc(hashTable->count, sizeof(Array))) == NULL){    // set memory for array
        fprintf(stderr, "could not allocate memory\n");
        exit(2);
    }
    htApply(hashTable, _makeArray);     // applies _makeArray to each element in Ht
    
    qsort(array, hashTable->count, sizeof(Array), _cmpfunc); // sort array (biggest first)
}

//-------------------------------------------------
// prints out all elements of hashtable
void printHashTable(Ht* hashTable){
    htApply(hashTable, _prettyPrint);
}
//-------------------------------------------------
// code to be passed into apply, will happen on each member of hash table
void _prettyPrint(char *key, void *value)
{
    printf("Key: %s  |  Value: %d\n", key, *((int*)value));
}

//--------------------------------------------------
// code to be passed into apply, will happen on each member of hash table
void _makeArray(char *key, void *value){
    array[position].key = key;
    array[position].value = value;
    position++;
}
//---------------------------------------------------
// Help from http://stackoverflow.com/questions/9827560/how-to-sort-struct-using-qsort-in-c-programming

int _cmpfunc (const void *a,const void *b){
    int _a = *((int*)((Array*)a)->value);
    int _b = *((int*)((Array*)b)->value);
    
    //printf("_a: %d, _b: %d \n", _a, _b);
    
    if (_a > _b) return -1;
    if (_a == _b)return 0;
    return 1;
    
}
//--------------------------------------------------



