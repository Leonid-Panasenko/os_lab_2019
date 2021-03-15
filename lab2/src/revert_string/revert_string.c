#include "revert_string.h"
#include <string.h>
#include <stdio.h>

void Swap(char *left, char *right)
{
	char l = *left;
    *left = *right;
    *right = l;
}

void RevertString(char *str)
{
    int i;
    int l = strlen(str);
    char* newstr = "";

	for(i = 0; i <= l/2; i++)
    {                                                                 
        Swap(str+i, str+l-i-1);
    }
}
