/* pagedir.h 

pagedir contains functions useful for writing a webpage to a directory

Thomas Clark Spring 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
/*checks the existence of a directory by opening a file and inputting a 
.program file into the directory*/
bool checkDirectory(char *directoryname, char *program);
/*writes a given webpage into the given directoryname*/
void writePage(char *directoryname, char *URL, const int depth, char *HTML, int id);
/*writes a given filename, from a directoryname and an id*/
char *fnameFromId(char *directoryname, int id);
/* checks if a given directory is from a crawler directory. first has to be checked w/checkDirectory*/
bool isCrawlerDirectory(const char *directory);