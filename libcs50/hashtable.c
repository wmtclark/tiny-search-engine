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
#include "hashtable.h"
#include "jhash.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct slot {                                   //defines a slot in the table as a set      
  set_t *set;    
} slot_t;


/**************** global types ****************/
typedef struct hashtable { 
  slot_t **slots;                                       //defines an array of slot_t pointers 
  int num_slots;                                        //defines the number of slots
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/*creates the array of sets that is the table */
//static ensures that this function cannot be changed 
static slot_t **new_slots(const int num_slots); 

/**************** set_new ****************/
/*create and return a new set structure*/
hashtable_t *hashtable_new(const int num_slots){
  //allocate a new set strucutre
  if(num_slots <= 0){                                   //error check for negative number of slots 
      fprintf(stderr,"Error: Numer of slots must be >= 0");
      return NULL;
  }
  hashtable_t *ht = count_malloc(sizeof(hashtable_t)); //allocate for the structure

  if (ht == NULL) {
    return NULL;                                        // error allocating ht
  } else { 
                                                        //instantiate the ht for given num_slots 
    ht -> slots = new_slots(num_slots);  
    ht -> num_slots = num_slots;
    return ht; 
  }
}

/************** new_slots ********************/
/*create a new slots array for a hashtable */
static slot_t **new_slots(const int num_slots){
    slot_t **slots = count_malloc(sizeof(slot_t)*num_slots);//allocate memory to store the pointer to the slots 
    for(int i = 0; i<num_slots; i++){                       //scan through the slots 
        slots[i] = count_malloc(sizeof(slot_t));            //create memory for the slot
        slots[i]->set = set_new();                          //instantiate a new set in the slot 
    }
    return(slots);
}

/****************** hashtable_insert *************/
/*Insert item, identified by key (string), into the given hashtable */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){
    if(ht == NULL || key == NULL){ 
        return false;
    }
    if(hashtable_find(ht,key) == NULL){                     //if the key does not exist 
        long hash = JenkinsHash(key,ht->num_slots);         //find the hash for a given key 
        if(!set_insert(ht->slots[hash]->set,key,item)){     //attempt insert
            return false;                                   //false on fail 
        }
        return true;                                        //true on success 
        }
    return false;
#ifdef MEMTEST
  count_report(stdout, "After counter add");
#endif
}

/************* hashtable_find *******************/
/* Return the item associated with the given key.*/
void *hashtable_find(hashtable_t *ht, const char *key){
    if(ht == NULL || key == NULL){
        return NULL;
    } else {
        long hash = JenkinsHash(key, ht->num_slots);        //hash can find the correct slot 
        return(set_find(ht->slots[hash]->set,key));         //use set_find to check the appropriate slot
    }
}

/*************** hashtable_print ***************/
/* Print the whole table; provide the output file and func to print each item.*/
void hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item)){
    if(fp != NULL){                                         //check null pointer
        if(ht!=NULL){                                       //check null table 
            for(int i=0;i<ht->num_slots;i++){               //loop through the slots 
                set_print(ht->slots[i]->set,fp,itemprint);  //use set_print to print a slot 
                fputc('\n',fp);                             //new line 
            }
        } else{
        fputs("(null)",fp);                                 //null on null ht
        }
    }
}

/****************** hashtable_iterate ************/
/* Iterate over all items in the table; in undefined order.*/
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) ){
    if(ht !=NULL && itemfunc !=NULL){
        for(int i = 0;i<ht->num_slots;i++){                 //loop through slots 
            set_iterate(ht->slots[i]->set,arg,itemfunc);    //loop through set at each slot 
        }
    }
}

/************** hashtable_delete **************/
/* Delete hashtable, calling a delete function on each item.*/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ){
    if(ht !=NULL && itemdelete != NULL){                    //check to ensure deletion is possible
        for(int i =0;i<ht->num_slots;i++){                  //loop through each slot 
            set_delete(ht->slots[i]->set,itemdelete);       //delete the set 
            count_free(ht->slots[i]);                       //delete the slot 
        }
        count_free(ht->slots);                              //free the slots pointer
        count_free(ht);                                     //free the table 
    }
}

                 

