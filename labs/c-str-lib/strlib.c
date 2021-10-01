#include <stdlib.h>

int mystrlen(char *str){
    int length = 0;
    for(int i = 0; str[i] != '\0' ;i++){
        length++;
    }
    return length;
}

char *mystradd(char *origin, char *addition){
    int lenOrigin = mystrlen(origin);
    int lenAddition = mystrlen(addition);

    // Create a new var to avoid override the params
    char *newStr = (char *)malloc(mystrlen(origin) + 1);
   
    int i = 0;
    for(; i < lenOrigin; i++){
        newStr[i] = origin[i];
    }
    for(i = 0; i < lenAddition; i++){
        newStr[lenOrigin + i] = addition[i];
    }

    newStr[lenOrigin + i] = '\0';

    return newStr;
}

int mystrfind(char *origin, char *substr){
    int j, i, prev;
    for(i = 0; origin[i] != '\0'; i++){
        prev = i;
        for(j = 0; substr[j] != '\0'; j++){
            if (origin[i] == substr[j] && origin[i] != '\0'){
                i++;
            }
            else{
                if(j>0){
                    i--;
                }
                break;
            }
        }
        if(substr[j] == '\0'){
            return prev;
        }
    }
    return -1;
}

