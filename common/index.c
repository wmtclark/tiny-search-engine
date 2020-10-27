/* index.=c - c file in common module 
 * 
 * Contains functions to save and load indexes from files 
 *
 * Thomas Clark, Spring 2020 Dartmouth CS50
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "file.h"
#include "counters.h"

/************** global function headers **************/
void indexSave(hashtable_t * index, FILE *fp);
void indexDelete(hashtable_t* index);
hashtable_t *indexFromFile(FILE *fp);

/*************** local function headers **************/
void indexSaveHelper(void *arg, const char *key, void *item);
void indexSaveCounterPrint(void *arg, const int key, const int count);
hashtable_t *indexFromFile(FILE *fp);
void indexDeleteHelper(void *item);


/****************** indexSave ******************/
//saves an index to a given file
void indexSave(hashtable_t * index, FILE *fp){
    hashtable_iterate(index, fp, indexSaveHelper); //iterate through the hsahtable, printing each line
}

/*************** indexDelete ********************/
//deletes an index
void indexDelete(hashtable_t* index){
    hashtable_delete(index,indexDeleteHelper); //use delete helper to delete the hashtable
}

/**************** indexFromFile ****************/
//takes a valid file pointer and returns an index
hashtable_t *indexFromFile(FILE *fp){
    int numWords = lines_in_file(fp); //count the number of words
    hashtable_t *index = hashtable_new(numWords); //create a table where each row is a word 
    //create variables for the loop 
    char *word;
    int id, count;
    counters_t *ctrs;

    for(int i = 0;i<numWords;i++){ //loop through each line of the file 
        word = freadwordp(fp);     //read a word 
        ctrs = counters_new();     //create a counterset for that word 
        while(fscanf(fp,"%d %d ",&id,&count)== 2){ //for each time a word is seen
            counters_set(ctrs, id, count);  //set the counter to the appropriate value
            hashtable_insert(index,word,ctrs); //insert it into the index
        }
        free(word); //hashtable copies keys, so we can free word here 
    }
    return index;
}

/************** local functions ***************/

/***************** indexSaveHelper ************/
//iterates through each counter from save
void indexSaveHelper(void *arg, const char *key, void *item){
    FILE *fp = arg;
    fprintf(fp,"%s ",key); //print a word to the file
    counters_iterate(item, fp, indexSaveCounterPrint); //print every counter
    fprintf(fp,"\n");
}

/***************** indexSaveCounterPrint ************/
//prints each counter from save
void indexSaveCounterPrint(void *arg, const int key, const int count){
    FILE *fp = arg;
    fprintf(fp,"%d %d ",key, count); //print the counter pairs 
}

/***************** indexDeleteHelper ******************/
//deletes the counter arguments, used by hashtable_delete
void indexDeleteHelper(void *item){
    counters_delete(item); //delete the counterset 
}