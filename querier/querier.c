/* querier.c
*
* takes in user queries and returns results from a preset indexer and 
*
* Thomas clark CS50 Spring 2020
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "index.h"
#include "counters.h"
#include "file.h"
#include "hashtable.h"
#include "pagedir.h"

/*********** local structures ********************/
typedef struct result {
    int docID;
    int score;
    char *URL;
} result_t;

/*********** local function headers *************/
int parser(char* input, char **query);
void querier(char **query, int queryLength, char * pageDirectory, hashtable_t *index);
bool queryValidate(char **query, int queryLength);
void countersCopy(counters_t *dest,counters_t *source);
void countersCopyHelper(void *dest,const int key, const int count);

/************ merging functions and helpers *****/
counters_t * andMerge(counters_t *currentQuery,counters_t *newQuery);
counters_t * orMerge(counters_t *totalQuery,counters_t *currentQuery);
void andMergeHelper(void *merge, const int key, const int count);
void andMergeHelperPrep(void *currentQuery, const int key, const int count);
void orMergeHelper(void *merge, const int key, const int count);

/************** results functions ******************/
void resultsCounter(void * numResults, const int key, const int count);
void resultsArrayConstructor(void *resultsArray, const int key, const int count);
result_t *newResult(int docID, int score);
void resultsAddURL(result_t **resultsArray, int numResults, char *pageDirectory);
void resultsPrint(result_t **resultsArray,int numResults);
void resultsDelete(result_t **resultsArray,int numResults);



/*********************** main ***********************/
int main(int argc, char *argv[]){
    //first check command line for errors 
    FILE *fp;
    //check the command line for appropriate number of arguments 
    if (argc != 3){
        fprintf(stderr,"Error: incorrect number of arguments, see README for usage\n");
        return 1;
    }
    //allocate for the pageDirectory name, 3 extra chars in case the / is needed
    char *pageDirectory = malloc(sizeof(char)*(strlen(argv[1])+3));
    strcpy(pageDirectory,argv[1]);                                       //copy the argument over into pagedirectory to prevent overflow on adding /
    //check if the directory exists, and add / if necessary
    if(!checkDirectory(pageDirectory,".querier")){
        fprintf(stderr,"Error in finding the pageDirectory\n");
        free(pageDirectory);
        return 2;
    }
    //check if its a crawler directory
    if(!isCrawlerDirectory(pageDirectory)){
        printf("Error: %s is not a crawler directory\n",pageDirectory);
        free(pageDirectory);
        return 3;
    }
    //ensure that the file is writeable
    if( (fp = fopen(argv[2],"r")) == NULL ){
        fprintf(stderr,"error in opening indexFile");
        free(pageDirectory);
        return 4;
    }


    //run the querier program

    //define the index 
    hashtable_t *index = indexFromFile(fp);
    fclose(fp);

    //define vars to put into querier
    char *input; 
    char **query;
    int sizeOfQuery = 0;

    printf("Query? ");                                                          //prompt user input 
    while((input = freadlinep(stdin))!=NULL){                                   //read user input
        printf("\nQuery: %s \n",input);                                         //print the query to be parsed
        query = malloc(sizeof(char*)*(strlen(input)/2+1));                      //allocate 1/2 the stringlength of the input 
        sizeOfQuery = parser(input,query);                                      //sets query to an array of input length and returns size
        if(sizeOfQuery<=0){                                                     //on no items or error 
            printf("Query? ");
            free(query);                                                        //clear memory and reset 
            free(input);
            continue;
        }
        querier(query, sizeOfQuery,pageDirectory,index);                        //run the query
        printf("Query? ");                                                      //prompt new input
        free(query);                                                            //clean memory
        free(input);
    }
    free(pageDirectory); //free the space we allocated for pageDirectory above
    indexDelete(index); //delete the index
    return 0;
}


/******************** parser *******************/
// Takes a string as an input and a blank array of words (pre-allocated)
// changes the blank array of words into an array of all the words in 
// the input string 
// returns the size of the array or -1 on error
int parser(char* input, char **query){
    int i = 0;
    int j = 0;
    bool flag = true;

    while(flag){

        if(input[i]=='\0'){                                                             //end on EOL
            flag = false;
        }

        else if(isalpha(input[i])){                                                     //start when we run into a letter
            query[j] = &input[i];                                                       //track the start of the word 
            while(!isspace(input[i])&&input[i]!='\0'){                                  //run until a space or EOL
                if(!isalpha(input[i])){                                                 //error check for extra characters 
                    fprintf(stdout,"Error: '%c' is an invalid character\n",input[i]);
                    return(-1);
                }
                input[i]=tolower(input[i]);                                             //lowercase the letter
                i++;                                                                    //continue through the string
            }
            if(input[i]=='\0'){                                                         //check for end of line 
                flag = false;                                                           //exit loop on end of line
            }
            input[i]='\0';                                                              //crush the space with a null terminator
            i++;                                                                        //continue scrolling through the string 
            j++;                                                                        //continue scrolling through the query array 
        }

        else if(isspace(input[i])){                                                     //skip spaces that are not \0
            i++;
        }

        else{                                                                           //throw error on incorrect characters
            fprintf(stdout,"Error: '%c' is an invalid character\n",input[i]);
            return -1;
        }

    }
    return j;                                                                           //return size of query array
}

/************************ querier *********************/
// checks a query against the given documents and prints the results 
void querier(char **query, int queryLength, char * pageDirectory, hashtable_t *index){
    //initialize variables
    counters_t *currentQuery,*totalQuery;
    result_t **resultsArray;
    int numResults = 0;

    if(queryValidate(query,queryLength)){                                               //check that its a valid survey
    currentQuery = counters_new();
    countersCopy(currentQuery, hashtable_find(index,query[0]));                         //start with the first word in the index
    totalQuery = NULL;                                                                  //set total to NULL to ensure a running total
        for(int i = 1;i<queryLength;i++){                                               //loop through the query 
            if(strcmp(query[i],"or")==0){                                               //on OR
                totalQuery = orMerge(totalQuery,currentQuery);                          //orMerge into the total
                currentQuery = counters_new();
                countersCopy(currentQuery, hashtable_find(index,query[i+1]));           //reset currentQuery to the item after OR
                i++;                                                                    //skip the word currentQuery just read in line 179
            }
            else if(strcmp(query[i],"and")!=0){                                         //ignore the word AND    
                currentQuery = andMerge(currentQuery,hashtable_find(index,query[i]));   //on anything else merge into the current query
            }
        }
        //this ensures that a query merges if it had at least 1 OR in it 
        //otherwise it is redundant, as OR with NULL just returns the non-null element
        totalQuery = orMerge(totalQuery,currentQuery); //end up with a total query

        //count the number of return items in the query
        counters_iterate(totalQuery,&numResults,resultsCounter);

        //allocate for an array of results 
        resultsArray = malloc(sizeof(result_t*)*numResults+1);
        for(int i = 0;i<numResults;i++){
            resultsArray[i] = NULL;
        }

        //add the results to the array
        counters_iterate(totalQuery,resultsArray,resultsArrayConstructor);
        counters_delete(totalQuery);
        //Add URL to all results
        resultsAddURL(resultsArray,numResults,pageDirectory);
        //print results
        resultsPrint(resultsArray,numResults);
        resultsDelete(resultsArray,numResults);
        
    }
}

/********************* queryValidate ****************/
//checks that a query has the proper synatax 
bool queryValidate(char **query, int queryLength){
    int count = 0;

    //check the first and last word 
    if((strcmp(query[0],"and")==0)||(strcmp(query[queryLength-1],"and")==0)){
        printf("Error: the string cannot begin or end in \'and\'\n");
        return false;
    }
    if((strcmp(query[0],"or")==0)||(strcmp(query[queryLength-1],"or")==0)){
        printf("Error: the string cannot begin or end in \'or\'\n");
        return false;
    }
    //check for consecutive and/or 
    for(int i=0;i<queryLength-1;i++){
        if(strcmp(query[i],"or")==0||strcmp(query[i],"and")==0){
            count++;
            if(count == 2){
                printf("Error: \'and\' and \'or\' cannot be consecutive\n");
                return false;
            }
        }
        else count = 0;
    }
    return true;
}


/************************ andMerge *******************/
/*takes the intersect of two counters*/
counters_t * andMerge(counters_t *currentQuery,counters_t *newQuery){
    counters_t *merge = counters_new();
    countersCopy(merge,newQuery);                                               //copy newQuery into merge, free merge later
    counters_iterate(merge,currentQuery,andMergeHelperPrep);                    //loop through merge to check for items not in currentQuery
    counters_iterate(currentQuery, merge, andMergeHelper);                      //loop through currentQuery and copy the lower values to merge
    counters_delete(currentQuery);                                              //free the space for currentQuery
    return(merge);
}

/********************** andMergeHelper ****************/
/*sets the counter in merge to the minimum of it and the counter it is merging with*/
void andMergeHelper(void *merge, const int key, const int count){
    if(counters_get(merge,key)>count){                                           //if the count in merge is greater than in currentQuery
        counters_set(merge,key,count);                                           //set the count to currentQuery
    }
}

/********************** andMergeHelperPrep ***************/
/*sets all counters in merge, but not in item1, to 0*/
void andMergeHelperPrep(void *currentQuery, const int key, const int count){
    if(counters_get(currentQuery,key) == 0){                                     //if it exists in merge, but not currentQuery
        counters_set(currentQuery,key,0);                                         //insert a value of 0 in currentQuery
    }
}

/*********************** orMerge *********************/
/*takes the sum of two counters*/
counters_t *orMerge(counters_t *totalQuery,counters_t *currentQuery){
    counters_t *merge = counters_new();                                         //create a merge to return 

    if(totalQuery==NULL){                                                       //if there is nothing in the running total
        countersCopy(merge,currentQuery);                                       //copy the current total into merge
        counters_delete(currentQuery);                                          //delete the current total pointer
        return(merge);                                                          //return the current total now stored in merge
    }
    
    countersCopy(merge,currentQuery);                                           //copy one counters into merge
    counters_iterate(totalQuery, merge, orMergeHelper);                         //iterate over the set and add the values at each docID
    counters_delete(totalQuery);                                                //remove the total pointer
    counters_delete(currentQuery);                                              //remove the current pointer
    return(merge);                                                              //return the result
}

/*********************** orMergeHelper ***************/
//used by orMerge to add the values of the two sets at each docID
void orMergeHelper(void *merge, const int key, const int count){
    counters_set(merge, key, count+counters_get(merge,key));
}

/********************* countersCopy *******************/
/*copies source into destination
assumes dest has been created outside of this function */
void countersCopy(counters_t *dest,counters_t *source){
    counters_iterate(source,dest,countersCopyHelper);
}

/******************** countersCopyHelper ***************/
/*sets the count at dest to the current key and count values
used by iterate in countersCopy*/
void countersCopyHelper(void *dest, const int key, const int count){
    counters_set(dest,key,count);
}

/*********************** newResult ******************/
//returns a result pointer with the given values and NULL URL
result_t *newResult(int docID, int score){
    result_t *result = malloc(sizeof(result_t));

    if(result == NULL){
        return NULL; //out of memory
    }
    else{ //create a new results object
        result->docID = docID;
        result->score = score;
        result->URL = NULL;
        return result;
    }

}

/********************** resultsCounter *********************/
//helper for iterate. returns the number of nonzero items in a counter
void resultsCounter(void *numResults, const int key, const int count){
    int *num = (int *)numResults;
    if(count!=0){
        *num = *num + 1;
    }
}

/****************** resultsArrayConstructor ***************/
//construct a results array at the pointer results array in descending order
void resultsArrayConstructor(void *resultsArray, const int key, const int count){
    int i = 0;
    int j = 0;
    result_t **array = (result_t**)resultsArray;
    if(count>0){
        //loop through array until we hit the end or a value less than current count
        while(array[i]!= NULL && array[i]->score > count){
            i++;
        }
        if(array[i]!=NULL){                                                         //if there is an item where we want to insert
            for( j = 0; array[j]!=NULL;j++);                                        //scroll to the end of the array
            for(;j>i;j--){                                                          //copy each item to the right until we hit the insertion point
                array[j] = array[j-1];
            }
            array[i]= newResult(key,count);                                         //insert the value 
        }
        else array[i] = newResult(key,count);                                       //insert the value 
    }
}

/****************** resultsAddURL ******************/
//adds a URL to every item in the results array. Error checks that each file exists
void resultsAddURL(result_t **resultsArray, int numResults, char *pageDirectory){
    FILE *fp;
    char *filename;

    for(int i = 0;i<numResults;i++){                                                //loop through the array
        filename = fnameFromId(pageDirectory,resultsArray[i]->docID);               //create the filename for an ID 
        if((fp = fopen(filename,"r"))!= NULL){                                      //if readable file 
            resultsArray[i]->URL = freadlinep(fp);                                  //the URL is the first line
            fclose(fp); 
        }
        else{ //error 
            fprintf(stderr,"Error in reading output file with docID %d",resultsArray[i]->docID);
        }
        free(filename);
    } 
}  

/******************* resultsPrint **************/
//prints results by looping through the array
void resultsPrint(result_t **resultsArray,int numResults){
    printf("\n");
    printf("Matches %d documents (ranked):\n",numResults);
    for(int i = 0;i<numResults;i++){
        fprintf(stdout,"Score:% 5d docID:% 5d URL:%s\n",resultsArray[i]->score,resultsArray[i]->docID,resultsArray[i]->URL);
    }
    printf("-------------------------------------------\n");
}

/***************** resultsDelete *****************/
//clears results
void resultsDelete(result_t **resultsArray,int numResults){
    for(int i =0;i<numResults;i++){
        free(resultsArray[i]->URL);
        free(resultsArray[i]);
    }
    free(resultsArray);
}