# WordPairs

Wordpairs is a program targeting Linux platforms which reads words from one or more files and prints out a list of the most frequently occurring sequential pairs of words and the number of times they occurred in decreasing order of occurrences.

## Details

A hash table is used to store and count occurences of sequences of words. The hash table keeps track of how full it is and grows itself to a larger size (more buckets) as needed.

* `Separate Chaining` is used when creating the hash table buckets.
* The standard librabry `qsort()` is used to sort hash table.
* `getWord.c` is used to read successive words from file descriptor.
* `crc64.c` is used as hash function to hash strings.

## Interface

```txt
wordpairs <-count> fileName1 <fileName2> <fileName3> ...
```

Where:

* `count` is the integer number of word pairs to print out and `fileNameN` are pathnames from which to read words.
* If no `count` argument is specified, ALL word pairs are printed to `stdout`.
* (tokens enclosed in angular brackets are optional).

## Archive

```txt
WordPairs-HT-
    ├── Makefile            : Build program
    ├── README.md           : Readme for Program
    ├── crc64.c             : Hash algorithm - Used in hash table
    ├── crc64.h             : Hash algorithm header
    ├── getWord.c           : Returns characters until 1 word is assembled
    ├── getWord.h           : header for getWord
    ├── ht.c                : Hash table source
    ├── ht.h                : Hash table header
    ├── main.c              : Reads terminal input and interfaces with HT
    ├── mobydick.txt        : Big test input
    ├── testfile.txt        : Small test input
    └── testfile2.txt       : Small test input
```

## Build & Compile

* Clone repo and open WordPairs-HT- in the terminal
* To compile:

```txt
make
```

* To remove enter in terminal

```txt
make clean
```

## Usage

***Comparing 2 test files:***

```txt
./wordpairs testfile.txt testfile2.txt
```

Output:

```txt
         4 the lazy
         4 over the
         4 the quick
         4 jumps over
         2 quick onyx
         2 goblin jumps
         2 dozen liquor
         2 quartz judge
         2 pack my
         2 quick brown
         2 sphinx of
         2 my vow
         2 my box
         2 brown fox
         2 onyx goblin
         2 liquor jugs
         2 of black
         2 with five
         2 lazy dog
         2 five dozen
         2 lazy dwarf
         2 black quartz
         2 fox jumps
         2 box with
         2 judge my
         1 dwarf pack
         1 vow pack
         1 jugs sphinx
         1 jugs the
         1 dog the
         1 dog sphinx
```

***Top 10 Word Pairs in Moby Dick:***

```txt
./wordpairs -10 mobydick.txt
```

Output:

```txt
       959 of the
       595 in the
       348 to the
       196 of his
       188 to be
       188 from the
       179 of a
       179 and the
       176 on the
       167 it was
```

## Built With

* C99
* GNU Make
* Tested on OSX & Ubuntu

## Author

* **Spencer Kitchen**
* Email: <sckitchen.dev@gmail.com>
