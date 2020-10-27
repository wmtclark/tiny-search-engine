/* 
 * counters.c
 *
 * Thomas Clark 2020
 * loosely modeled on bag.c Dartmouth CS50 20S
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode {
  int key;		      
  int count;         
  struct counternode *next;	      // link to next node
} counternode_t;

/**************** global types ****************/
typedef struct counters {
  struct counternode *head;	      // head of the list of items in counters
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
/*create a new counternode*/
static counternode_t *counternode_new(const int key);

/**************** counters_new ****************/
/*create and return a new counters structure*/
counters_t* counters_new(void)
{
  //allocate a new counters strucutre
  counters_t *counters = count_malloc(sizeof(counters_t));

  if (counters == NULL) {
    return NULL; // error allocating counter
  } else { //initialize the new head to NULL
    counters->head = NULL; 
    return counters; 
  }
}

/**************** counters_add ****************/
/*create a new counternode, or increment the node for a given key*/
int counters_add(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    //check to see if a counter exists 
    int current_count = counters_get(ctrs,key); 

    if(current_count != 0) {                      //if counter already exists 
      counters_set(ctrs,key,current_count+1);     //set the value to one greater 
      return(current_count+1);                    //return the counter
    }
    else{
      counternode_t *new = counternode_new(key);  //create a new counter
      if (new != NULL) {
        // add it to the head of the list
        new->next = ctrs->head;
        ctrs->head = new;
      }
    }
  }

#ifdef MEMTEST
  count_report(stdout, "After counter add");
#endif
return (0);
}


/**************** counternode_new ****************/
/* Allocate and initialize a counternode */
// the 'static' modifier means this function is not visible 
// outside this file
static counternode_t* counternode_new(int key)
{
  counternode_t *node = count_malloc(sizeof(counternode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = key;                      //set the key
    node->count = 1;                      //start with count of one 
    node->next = NULL;                    //point to NULL
    return node;
  }
}


/**************** counters_get ****************/
/* Return current value of counter associated with the given key.*/
int counters_get(counters_t *ctrs, const int key){
  if(ctrs == NULL || ctrs->head == NULL){ //if no set or an empty set
    return 0;
  } else{
    //loop through the set until a counter is found with the given key
    counternode_t *counter = ctrs->head;
    while(counter !=NULL){
      if(counter->key == key){
        return (counter->count);          //return the value for given ken
      }
      counter = counter -> next;
    }
    return 0;                             //0 if nothing is found 
  } 
}



/**************** counters_set ****************/
/* Set the current value of counter associated with the given key.*/
bool counters_set(counters_t *ctrs, const int key, const int count){
  if(ctrs == NULL || key < 0 || count < 0){ //check inputs 
    return false;
  } else{ 
    if(counters_get(ctrs,key)!=0){          //if the node already exists for key
      counternode_t *counter = ctrs->head;
      while(counter !=NULL){                //find the node 
        if(counter->key == key){
          counter -> count = count;         //replace the count
          return true;                      //exit true 
        }
        counter = counter-> next;
      }
    } else {                                //if the node does not exist 
      counternode_t *new = counternode_new(key); //create a nwe node
      if (new != NULL) {
      //set the new count to given
        new -> count = count;
      // add it to the head of the list
        new->next = ctrs->head;
        ctrs->head = new;
        return true; //exit 
      }
    }
  }
   return false; //return false on error 
}

/**************** counters_print ****************/
/* Print all counters; provide the output file.*/
void counters_print(counters_t *ctrs, FILE *fp){
  if(fp != NULL){
    if(ctrs != NULL){
      fputc('{',fp);
      counternode_t *counter = ctrs->head;
      //error check an extra comma by manually entering the first input
      fprintf(fp,"%d=%d",counter->key,counter->count);
      counter = counter -> next;

      while(counter != NULL){                     //loop through the list
      if(counter != NULL){                        //print a comma and key value pair
        fputc(',',fp); 
        fprintf(fp,"%d=%d",counter->key,counter->count); 
      }
      counter = counter -> next;                  //next item in the list
    }
    fputc('}',fp);                                //close the bracket
    } else {
    fputs("(null)",fp);                           //print null for NULL counters_t 
    }
  }
}

/**************** counters_iterate ****************/
/* Iterate over all counters in the set.*/
void counters_iterate(counters_t *ctrs, void *arg, 
                      void (*itemfunc)(void *arg, 
                                       const int key, const int count)){
  if(ctrs != NULL && itemfunc != NULL){         //for non-null conditions
    for(counternode_t* node = ctrs->head;node != NULL; node = node->next){
      (*itemfunc)(arg,node->key,node->count);   //call the function on each item
    }
  }
}
            
/**************** counters_delete ****************/
/* Delete the whole counterset.*/
void counters_delete(counters_t *ctrs){
  if (ctrs!=NULL){ 
    for(counternode_t *node = ctrs->head;node!=NULL;){ //scan through ctrs
      counternode_t *next = node -> next;  //set a new pointer (next) to the next item in the list
      count_free(node); //free current item
      node = next; //make current item next item
    }
    count_free(ctrs); //free the larger structure 
  }

  #ifdef MEMTEST
  count_report(stdout, "After counter delete");
  #endif

}                        

