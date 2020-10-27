/* 
 * set.c
 *
 * Thomas Clark 2020
 * loosely modeled on bag.c Dartmouth CS50 20S
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  char* key;		              //key value
  void* item;                 //item value 
  struct setnode *next;	      // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;	      // head of the list of items in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
/*create a new setnode*/
static setnode_t *setnode_new(char* key);

/**************** set_new ****************/
/*create and return a new set structure*/
set_t *set_new(void)
{
  //allocate a new set strucutre
  set_t *set = count_malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL; // error allocating counter
  } else { //initialize the new head to NULL
    set->head = NULL; 
    return set; 
  }
}

/**************** set_insert ****************/
/*create a new setnode, or increment the node for a given key*/
bool set_insert(set_t *set, const char *key, void *item)
{
  if ((set != NULL && key != NULL) && item !=NULL) {
    //check to see if a key is already in the set
    if(set_find(set,key) == NULL){
        char *newkey = count_malloc(sizeof(key));     //allocate for a new key
        strcpy(newkey,key);                           //copy the new key in 

        setnode_t *new = setnode_new(newkey);         //create a new setnode

        if (new == NULL) {                            // if we fail to allocate for a new node
            return false;
        } else{
        new -> item = item;                           //place the item in the node
        new->next = set->head;                        // add it to the head of the list
        set->head = new;
        return true;
        }
    }
  }

#ifdef MEMTEST
  count_report(stdout, "After counter add");
#endif
return false;
}


/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
// the 'static' modifier means this function is not visible 
// outside this file
static setnode_t* setnode_new(char* key)
{
  setnode_t *node = count_malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = key;   //point to key
    node->item = NULL; //point to NULL 
    node->next = NULL; //point to NULL
    return node;
  }
}


/**************** set_find ****************/
/* Return current value of counter associated with the given key.*/
void *set_find(set_t *set, const char *key){
  if(set == NULL || set->head == NULL || key == NULL){ //if no set or an empty set
    return NULL;
  } else{
    //loop through the set until a counter is found with the given key
    setnode_t *node = set->head;
    while(node !=NULL){
      if(strcmp(node->key, key)==0){
        return (node->item); //return the value for given key
      }
      node = node -> next;
    }
    return NULL; //0 if nothing is found 
  } 
}




/**************** set_print ****************/
/* Print all set; provide the output file.*/
void set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item)){
  if (fp != NULL) { 
    if (set != NULL) {                              //if set exists 
      if(set->head != NULL && itemprint != NULL){   //if the head and itemprint are not NULL
        fputc('{', fp);                             //open bracket
        setnode_t *node = set->head;                //start at head to remove extra comma 
        (*itemprint)(fp, node->key,node->item); 
        node = node->next;
        for (; node != NULL; node = node->next) {   //print each item after a comma
            if (itemprint != NULL) { 
            fputc(',', fp);
            (*itemprint)(fp, node->key,node->item); 
            }
        }
      fputc('}', fp);                               //print the close file marker 
      }
    } else {
      fputs("(null)", fp);                          //print null if set does not exist 
    }
  }
}

/**************** set_iterate ****************/
/* Iterate over all set in the set.*/
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item)){
  if(set != NULL && itemfunc != NULL){ //for non-null conditions
    for(setnode_t* node = set->head;node != NULL; node = node->next){
      (*itemfunc)(arg,node->key,node->item); //call the function on each item
    }
  }
}
            
/**************** set_delete ****************/
/* Delete the whole setet.*/
void set_delete(set_t *set, void (*itemdelete)(void *item)){
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL; ) {
        count_free(node->key);
        if (itemdelete != NULL) {		        // if possible...
            (*itemdelete)(node->item);	    // delete node's item
        }
        setnode_t *next = node->next;	      // remember what comes next
        count_free(node);			              // free the node
        node = next;			                  // and move on to next
    }

    count_free(set);
  }

#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}                        

