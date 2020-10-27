/*indextest.c contains the driver and functions for indextest
*
* takes an index from a file and writes it to a second file 
*
* Thomas Clark Spring 2020, Dartmouth CS50
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashtable.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"
#include "counters.h"
#include "webpage.h"

/*************** function headers *************/
//reads the oldIndexFile and prints the index into newIndexFile. The two files should be identical except order
void indextest(FILE *oldIndexFile, FILE* newIndexFile);

/******************* main ******************/ 

//parses command line and calls indexer 
int main (int argc, char* argv[]){
    // declare variables
    FILE *fp, *fp2;
    //check the command line 
    if (argc != 3){
        fprintf(stderr,"Error: incorrect number of arguments, see README for usage\n");
        return 1;
    }
    //check if the first file exists 
    if((fp = fopen(argv[1],"r"))==NULL){
        fprintf(stderr,"Error: index file does not exist\n");
        return 2;
    }
    //check that the second file is writeable
    if((fp2 = fopen(argv[2],"w"))==NULL){
        fprintf(stderr,"Error: writing file does not exist\n");
        return 3;
    }
    //call indextest
    indextest(fp,fp2);
    return 0; 
}

/***************** indexTest *******************/
//drives the index test
void indextest(FILE *oldIndexFile, FILE* newIndexFile){
    hashtable_t *index = indexFromFile(oldIndexFile); //read in an index from a file 
    fclose(oldIndexFile);                             //close the file 
    indexSave(index,newIndexFile);                    //save the index to a new file
    fclose(newIndexFile);                             //close the new file 
    indexDelete(index);                               //clear memory
}

