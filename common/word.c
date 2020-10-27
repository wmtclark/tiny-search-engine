/* word.c 
* 
* fulfills the word.h functions, see word.h for more
*
* Thomas Clark Spring 2020 for CS 50 
*/

#include <stdio.h>
#include <ctype.h>

/***************** normalizeString ******************/
//converts a string to lower case
void normalizeString(char *str){
    //consulted with stackoverflow to find out about tolower
    for(int i = 0; str[i];i++){ //loop through and lower every character
        str[i] = tolower(str[i]);
    }
}
