# Querier CS50: Implementation Spec

## Overview

The querier takes the index into a hashtable of countersets, that pairs a word to document ID and the number of times it appears. This index is then searched through, word by word, from an array of strings taken from the input. The results are added into a counterset that tracks IDs and scores, which is then converted into a structure called `result` to be printed.

## Pseudocode

### Querier

Querier takes two command line arguments, a pageDirectory and an indexFileName. It first parses the command line, ensuring that the directory exists, is readable, and is a crawler directory. To do this, it checks to see if a ".crawler" file exists in the pageDirectory. It also attempts to open a readable file at the indexFileName path. Once the command line is parsed, *parser* is called.

The parser converts a single input string into an array of words. It loops through the string, placing a pointer at the beginning of a word, then scrolling through the word (ensuring that all letters are alphabetical) until it hits the end of a word, where it inserts a `/0` to ensure that the word is considered a string. It then scans until it finds the next word or hits the end of the string. 

Once *parser* returns an array of words, *querier* is called being passed the index which is created by indexFromFile.

*Querier* calls *queryValidate* which uses `strcmp` to ensure that there are no consecutive and/or, and that the query does not begin in and/or. 

*Querier* then does the following:

1. Scan through the array of words

    1. get the values for pageID and count for a word from the index

    2. merge the word into the current string with and UNLESS the previous word was OR

        1. AndMerge creates a new countersset, then copies the first of the two items to be merged with `countersCopy`. It then uses `counters_iterate` to loop through the second countersset and takes the lower value of the two sets at each pageID.

    3. IF the previous word was or, start a new string that is ORmerged into the result at the end of the input or when the next or is read

        1. OrMerge creates a new countersset, then copies the first of the two items to be merged with `countersCopy`. It then uses `counters_iterate` to loop through the second countersset and takes the sum of the values of the two sets at each pageID and makes it the new value at that pageID.

2. An array of structures called `results`, with a score, pageID, and URL is created in descending order by scainnig through the countersset and adding each element to the array. 

3. Assign each element in the array to the URL associated with that ID by looping through the array and grabbbing the first line of the document associated with a given ID 

4. print the array to the screen

5. Free the array and all structures it contains using counters_delete, and a custom-made struct-delete function. 


## Function prototypes, parameters, and explanations

### querier

the querier is defined above

```c
/*********** local structures ********************/
//used for storing results
typedef struct result {
    int docID;
    int score;
    char *URL;
} result_t;

/*********** local function headers *************/
//returns the size of an array of words stored at query parsed from input, -1 on error
int parser(char* input, char **query);
//drives the querier algorithm for a given query, index, and Directory
void querier(char **query, int queryLength, char * pageDirectory, hashtable_t *index);
//true on valid query, false on invalid query
bool queryVaidate(char **query, int queryLength);
//uses counters_iterate to copy dest into source. Dest needs to be prealloced
void countersCopy(counters_t *dest,counters_t *source);
void countersCopyHelper(void *dest,const int key, const int count);

/************ merging functions and helpers *****/
//returns a newly allocated counters, the interset of the two other counters
counters_t * andMerge(counters_t *currentQuery,counters_t *newQuery);
//returns a newly allocated counters, the union of the two counters
counters_t * orMerge(counters_t *totalQuery,counters_t *currentQuery);
//below are helper headers used by counters_iterate
void andMergeHelper(void *merge, const int key, const int count);
void andMergeHelperPrep(void *currentQuery, const int key, const int count);
void orMergeHelper(void *merge, const int key, const int count);

/************** results functions ******************/
//used by counters_iterate to count the number of results
void resultsCounter(void * numResults, const int key, const int count);
//construct a results array at the pointer results array in descending order
void resultsArrayConstructor(void *resultsArray, const int key, const int count);
//returns a result pointer with the given values and NULL URL
result_t *newResult(int docID, int score);
//adds a URL to every item in the results array. Error checks that each file exists
void resultsAddURL(result_t **resultsArray, int numResults, char *pageDirectory);
//prints results 
void resultsPrint(result_t **resultsArray,int numResults);
//clears results array
void resultsDelete(result_t **resultsArray,int numResults);
```

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
//reads the remainder of a file from a fp
char *freadfilep(FILE *fp);
```

### common.a functions

common.a contains shared functions referenced in the above pseudocode used by both indextest and querier.

```c
/*********** from index.h ****************/
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

The only global structures used were `hashtable`, which is an array of sets, and `counters`, which is a LIFO linked list of counters objects, each containing a key and an integer count.

Locally we created a `results_t` structure which stores a score, pageID, and URL.

## Error handling

### querier errors

1 - the command line does not have the two arguments necessary for the querier program

2- the pagedirectory one is attempting to read from  does not exist.

3- the pagedirectory is not a crawler directory

4- the file to be written to is invalid
