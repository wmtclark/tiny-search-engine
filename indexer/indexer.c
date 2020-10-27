/* indexer.c
*
* creates an index from a given directory and writes it to output
*
* Thomas clark CS50 Spring 2020
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashtable.h"
#include "webpage.h"
#include "memory.h"
#include "pagedir.h"
#include "counters.h"
#include "word.h"
#include "index.h"
#include "file.h"

/*************** local functions *******************/
//reads an index from the crawler directors pageDirectory and passes it to fp
void indexer (char *pageDirectory, FILE *fp);
//creates the index as a hashtable 
hashtable_t *indexMake( char *pageDirectory);
//creates a webpage from a file in a crawler directory
webpage_t *webpage_get(FILE *fp);
/******************* main ******************/ 

//parses command line and calls indexer 
int main (int argc, char* argv[]){
    FILE *fp;

    //check the command line for appropriate number of arguments 
    if (argc != 3){
        fprintf(stderr,"Error: incorrect number of arguments, see README for usage\n");
        return 1;
    }
    else{
        //allocate for the pageDirectory name, 3 extra chars in case the / is needed
        char *pageDirectory = malloc(sizeof(char)*(strlen(argv[1])+3));
        strcpy(pageDirectory,argv[1]); //copy the argument over into pagedirectory to prevent overflow on adding /
        
        //check if the directory exists, and add / if necessary
        if(!checkDirectory(pageDirectory,".indexer")){
            fprintf(stderr,"Error in finding the directory\n");
            return 2;
        }
        //check if its a crawler directory
        if(!isCrawlerDirectory(pageDirectory)){
        printf("Error: pageDirectory is not a crawler directory\n");
        return 3;
        }
        //ensure that the file is writeable
        if( (fp = fopen(argv[2],"w")) == NULL ){
        fprintf(stderr,"error in opening indexFile");
        return 4;
        }
        //call indexer on correct arguments
        indexer(pageDirectory,fp);
        free(pageDirectory); //free the space we allocated for pageDirectory above
    }
    return 0; 
}

/******************* indexer *******************/ 
// creates and populates the index, then saves it to the file 
// returns 0 on sucess, error codes on error
void indexer (char *pageDirectory, FILE *fp){
    hashtable_t *index = indexMake(pageDirectory); //make the index 
    indexSave(index, fp);//save the index to a file 
    fclose(fp); //close the file 
    indexDelete(index);//delete the index 
}

/****************** indexMake ********************/
//creates a new hashtable of counters for the index
hashtable_t *indexMake(char *pageDirectory){
    //declare variables
    hashtable_t *new = hashtable_new(500); 
    int id = 1;
    char *filename = fnameFromId(pageDirectory,id);
    FILE *fp;
    int pos = 0;
    char *word;
    webpage_t* currentPage;

    //loop through the directory, and insert each word into the index
    while( (fp = fopen(filename,"r")) != NULL){
        //turn the tile into a webpage type 
        currentPage = webpage_get(fp);
        //loop through each word on the HTML of the webpage
        while( (word = webpage_getNextWord(currentPage, &pos)) != NULL) {     
            if(strlen(word)>3){ //eliminate short strings 
                normalizeString(word); //lowercase each word 
                //if the counterset exists, add to, otherwise make a new counterset
                if(counters_add(hashtable_find(new, word),id) == 0){
                    counters_t *ctrs = counters_new();
                    counters_add(ctrs,id);
                    hashtable_insert(new, word, ctrs);
                }
            }
            free(word);                             //hashtable copies keys, so free each word
        }
        id++;                                       //move to the next page
        pos = 0;                                    //reset position in file 
        webpage_delete(currentPage);                //clear the current page
        free(filename);                             //clear the filename
        fclose(fp);                                 //close the file 
        filename = fnameFromId(pageDirectory, id);  //set the nextfilename
    }
    free(filename); //free the last filename on error of existence
    return(new);
}

/***************** webpage_get *****************/
webpage_t *webpage_get(FILE *fp){
    char *URL = freadlinep(fp); //read the first line (URL)
    int depth;
    fscanf(fp,"%d\n",&depth);   //scan in the depth
    char* HTML = freadfilep(fp); //read the rest of the page
    webpage_t *new = webpage_new(URL,depth,HTML); //make a webpage

    return(new);
}
