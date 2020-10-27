/*crawler.c 
*
*implements and runs crawler module 
*crawls through a given set of pages and prints them to a directory
*
*Thomas Clark Dartmouth College Spring 2020
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "bag.h"
#include "webpage.h"
#include "memory.h"
#include "pagedir.h"

/*************** local functions *******************/
void crawler (const char *seedURL, char *pageDirectory, int maxDepth);
void URLdelete(void *URL);

/******************* main driver ******************/ 

int main (int argc, char* argv[]){
    // declare variables
    int depth;
    int maxdepth = 3;

    //check the command line 
    if (argc != 4){
        fprintf(stderr,"Error: incorrect number of arguments, see README for usage");
        return 1;
    }
    else{
        //check the last argument is an integer of an appropriate size
        sscanf(argv[3],"%d",&depth);
        if(depth<0 || depth>maxdepth){
            depth = maxdepth;
            fprintf(stderr,"depth less than 0 or over maxdepth, it is set to 3");
        }
        
        //check the directory exists 
        if(!checkDirectory(argv[2],".crawler")){
            fprintf(stderr,"Error in finding the directory");
            return 2;
        }


        // Call Crawler 
        crawler(argv[1],argv[2],depth);
    }

    return 0; 
}



/********************** crawler **************************/
// crawls from the seedURL to the max depth, written any found pages to pageDirectory
void crawler (const char *seedURL, char *pageDirectory, int maxDepth){

    bag_t *bag = bag_new(); //instantiate a bag for pages
    hashtable_t *seen = hashtable_new(50); //instantiate a hashtable 
    char *str = "s";

    //allocate a string for seedURL so that webpage_delete will not throw an error
    char *first_url = malloc(sizeof(char)*(strlen(seedURL)+1)); 
    strcpy(first_url,seedURL);

   
    
    NormalizeURL(first_url); //normalize the first url to prevent revisiting later
    webpage_t *page = webpage_new(first_url,0,NULL); //create a webpage at seedURL
    bag_insert(bag,page);  //insert the first page into a bag
    hashtable_insert(seen,first_url,str); //insert the url into a hashtable

    webpage_t *current_page;
    char *current_url;
    int pos = 0;
    int id = 1;

    //while loop runs for every page crawler finds in the given depth
    while((current_page = bag_extract(bag)) != NULL){
        if(!webpage_fetch(current_page)){  //fetch the HTML for the page 
            webpage_delete(current_page); //if there is no HTML, clear the memory
        }
        else{  
            if(webpage_getDepth(current_page)<=maxDepth){ //check depth of page
                // write the given page into a directory
                writePage(pageDirectory,webpage_getURL(current_page),webpage_getDepth(current_page),webpage_getHTML(current_page),id);
                id++; 
                //find every URL on the current page
                while((current_url = webpage_getNextURL(current_page,&pos))!=NULL){
                    //normalize and check that it is internal
                    printf("Checking URL: %s\n",current_url);
                    if(IsInternalURL(current_url)){
                        //check that the URL is not already seen
                        if(hashtable_insert(seen,current_url,str)){
                            //if not seen, create a page and insert into bag
                            printf("\tInserting URL: %s\n",current_url);
                            webpage_t *next_page = webpage_new(current_url,webpage_getDepth(current_page)+1,NULL);
                            bag_insert(bag,next_page);
                        }
                        else free(current_url); //free the URL on fail to hash
                    }
                    else {free(current_url);} //free URL on fail 
                }
            }
            webpage_delete(current_page); //free the webpage
            pos = 0; //reset the position from the URL scan
        }
    }
    bag_delete(bag,webpage_delete); //clear the bag
    hashtable_delete(seen,URLdelete); //clear the table (and any remainig URL's)
}


/*********************** URLdelete ********************/ 
//necessary to allow hashtable_delete to run, this function ironically enough has no function
void URLdelete(void *URL){}
