/* pageddir.c 
*
* contains code to write webpages to a directory 
*
* Thomas Clark May 2020 for Dartmouth CS 50 
*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/************* header ********************/
bool checkDirectory(char *directoryname, char *program);
void writePage(char *directoryname, char *URL, const int depth, char *HTML, int id);
char *fnameFromId(char *directoryname, int id);
bool isCrawlerDirectory(const char *directory);
/*********** functions **************/

/*checks for the existence of a directory 
returns true if the directoryname is a directory that can be written to
false otherwise 
*/
bool checkDirectory(char *directoryname, char *program){
    FILE *myfile;
    //add a / to the directory name if not present
    if(directoryname[strlen(directoryname)-1] != '/'){ 
        strcat(directoryname,"/");
    }
    //create a file in the directory with the name program
    char *filename = malloc(sizeof(char)*(strlen(directoryname)+strlen(program)+1));
    strcpy(filename,directoryname); 
    strcat(filename,program);

    //open a file to see if the directory is writeable 
    if((myfile = fopen(filename,"w")) != NULL){ 
        //return true on writeable
        fprintf(myfile,"opened from %s",program);
        fclose(myfile);
        free(filename);
        return true;
    }
    else{
        //return false on unwriteable
        free(filename);
        return false;
    }
}

/*writes the elements of a webpage to a given file*/
void writePage(char *directoryname, char *URL, const int depth, char *HTML, int id){
    FILE *fp;
    //create a unique ID for the filename
    char *filename = fnameFromId(directoryname,id);
    //write the page to the file 
    if((fp = fopen(filename,"w"))!= NULL){
        fprintf(fp,"%s\n%d\n%s",URL,depth,HTML);
        fclose(fp);
    }
    else{
        fprintf(stderr,"Error in writing to file with ID %d",id);
    }
    free(filename);
}


//create the file with the dirname/id format
char *fnameFromId(char *directoryname, int id){
    //allocate for the size of the filename
    char *filename = malloc(sizeof(char)*(strlen(directoryname)+2)+sizeof(char)*(int)(ceil(log10(id))));
    //print the filename to a string 
    sprintf(filename,"%s%d",directoryname,id);
    return(filename);
}



bool isCrawlerDirectory(const char *directory){
    FILE *myfile;
    //create a file in the directory with the name program
    char *filename = malloc(sizeof(char)*(strlen(directory)+strlen(".crawler ")+1));
    strcpy(filename,directory); 
    strcat(filename,".crawler");

    //open a file to see if the directory is writeable 
    if((myfile = fopen(filename,"r")) != NULL){ 
        //return true on writeable
        fclose(myfile);
        free(filename);
        return true;
    }
    else{
        //return false on unwriteable
        free(filename);
        return false;
    }
}