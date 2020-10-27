/* index.h - header file in common module 
 * 
 * Contains functions to save and load indexes from files 
 *
 * Thomas Clark, Spring 2020 Dartmouth CS50
 */

#include <stdio.h>
#include <stdlib.h>
#include "counters.h"
#include "hashtable.h"
#include "file.h"

 /************** indexSave *****************/ 
 //saves an index to a file 
void indexSave(hashtable_t *index,FILE *fp);

/**************** indexFromFile ****************/
//takes a valid file pointer and returns an index
hashtable_t *indexFromFile(FILE *fp);

/*************** indexDelete ********************/
//deletes an index
void indexDelete(hashtable_t* index);