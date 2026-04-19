#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* copyString(char** str1, char* str2)
{
    if (str2 == NULL) {
        *str1 = NULL;
        return NULL;
    }
    
    *str1 = (char*)malloc(strlen(str2) + 1);
    if (*str1 == NULL) {
        return NULL;
    }
    
    return strcpy(*str1, str2);
}