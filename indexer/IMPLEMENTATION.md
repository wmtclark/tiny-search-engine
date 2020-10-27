# Indexer CS50: Implementation Spec

## Overview

The indexer reads the input file into a hashtable, which pairs words (*key*) to counters (*value*). Each counter tracks the ID and number of mentions of a word in a given ID. This hashtable is therefore functionally the index. This index is then written into the output file.

## Pseudocode

### Indexer

Indexer takes two command line arguments, a pageDirectory and an output indexFileName. It first parses the command line, ensuring that the directory exists, is readable, and is a crawler directory. To do this, it checks to see if a ".crawler" file exists in the pageDirectory. It also attempts to open a writeable file at the indexFileName path. Once the command line is parsed, *indexer* is called. *Indexer* in turn calls a function *indexMake* which loops through the directory as follows:

1. create a filename at pageDirectory/id, and read the file into a webpage using *webpage_get*

    1. use *findNextWord* to find the next word in the file

        1. if the word is more than three letters
            1. convert the word to lowercase with *normalizeString* - which simply calls toLower on each character.

            2. Check if there is a counter for the word by calling counters_add on that hashtable at that word and ID. On 0 there is nothing in the hashtable at that spot.

                1. If counters_add returns 0, the hashatable does not contain a counter at that word.
                2. create a new counters object
                3. insert it into the hashtable

One the hashtable is constructed, it is returned to *indexer* in order to be passed to *indexSave*. *indexSave* takes the index and writes each line to a file using the hashtable_iterate method, printing a word and each associated counter to a line in the given file.

Finally, the index is deleted by calling *indexDelete* which uses the hashtable_delete method and calls counters_delete for each row, effectively clearing out all of the memory used to store hashtable.

### Indextest

Indextest takes two commandline arguments, an indexFile and a newIndexFile. It first uses a *main* function to check that the file exist and are read/writeable. It then passes the files as pointers to the *indexTest* method. This method calls *readFromFile*, a method from the index.h library, which goes through a file, and scans the content into a hashtable of words and counters, an index. The nuance to this index is that because we can read the number of lines from a file, using *lines_in_file*, the hashtable will have one row per word. This index is passed back to the *indexTest* and then saved with *indexSave* as detailed above. Memory is finally freed with *hashtable_delete* as above.

## Function prototypes, parameters, and explanations

### indexer

the indexer is defined above

```c
//reads an index from the crawler directors pageDirectory and passes it to fp
void indexer (char *pageDirectory, FILE *fp);
//creates the index as a hashtable
hashtable_t *indexMake( char *pageDirectory);
//creates a webpage from a file in a crawler directory
webpage_t *webpage_get(FILE *fp);
```

### indextest

Indextest is outlined above

```c
//reads the oldIndexFile and prints the index into newIndexFile. The two files should be identical except order
void indextest(FILE *oldIndexFile, FILE* newIndexFile);
```

### hashtable

functions from hashtable to create, write, and delete a hashtable

```c
/* Create a new (empty) hashtable. */
hashtable_t *hashtable_new(const int num_slots);

/* Insert item, identified by key (string), into the given hashtable.*/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item);

/* Iterate over all items in the table; in undefined order.*/
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) );
/* Delete hashtable, calling a delete function on each item.*/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) );
```

### counters

functions from counters to create, write, and delete a counters.

```c
/* Create a new (empty) counters set; return NULL if error. */
counters_t *counters_new(void);

/*Increment the counter indicated by key.*/
int counters_add(counters_t *ctrs, const int key);

/* Set the current value of counter associated with the given key.*/
bool counters_set(counters_t *ctrs, const int key, const int count);

/*Iterate over all counters in the set and execuse a function on each counter*/
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count));

/*Delete the whole counterset*/
void counters_delete(counters_t *ctrs);
```

### file

file.h is a library that contains functions used to determine the number of words in a page, and one used to read the remainder of a page (used by webpage_get).

```c
//the number of lines in a file, given a file pointer
int lines_in_file(FILE *fp);
//reads the remainder of a file from a fp
char *freadfilep(FILE *fp);
```

### common.a functions

common.a contains shared functions referenced in the above pseudocode used by both indextest and indexer.

```c
/*********** from index.h ****************/
 //saves an index to a file
void indexSave(hashtable_t *index,FILE *fp);

//takes a valid file pointer and returns an index
hashtable_t *indexFromFile(FILE *fp);

//deletes an index
void indexDelete(hashtable_t* index);

/************* from word.h **************/
//converts a string to lower case
void normalizeString(char *str);

/************* from pagedir.h **********/
/*checks the existence of a directory by opening a file and inputting a
.program file into the directory*/
bool checkDirectory(char *directoryname, char *program);
/*writes a given webpage into the given directoryname*/
char *fnameFromId(char *directoryname, int id);
/* checks if a given directory is from a crawler directory. first has to be checked w/checkDirectory*/
bool isCrawlerDirectory(const char *directory);
```

## Data structures

The only structures used were `hashtable`, which is an array of sets, and `counters`, which is a LIFO linked list of counters objects, each containing a key and an integer count.

## Error handling

### indexer errors

1 - the command line does not have the two arguments necessary for the indexer program

2- the pagedirectory one is attempting to read from  does not exist.

3- the pagedirectory is not a crawler directory

4- the file to be written to is invalid

### indextest errors

1-  the command line does not have the two arguments necessary for the indexer program

2 - the indexfile to be read does not exist

3- the indexfile to be written to does not exist
