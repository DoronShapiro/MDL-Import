#include <string.h>
#include <ctype.h>

void strip_trailing_whitespace(char* string){
    
    //first remove beginning whitespace
    char* newstring;
    int i;
    
    newstring = string;
    for(i=0; i<strlen(string); i++)
        if(!isspace(string[i])){
            newstring = &(string[i]);
            break;
        }
    strcpy(string,newstring);
    for(i=strlen(string)-1; i>0; i--)
        if(isspace(string[i]))
            string[i] = '\0';
        else
            break;
    
}