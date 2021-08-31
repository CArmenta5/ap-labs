#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    long res = values[0];
    printf("%d ", values[0]);
	switch (operator) {
        case 1:
            for(int i = 1; i < nValues; i++){
                printf("+ %d ", values[i]);
                res += values[i];
            }
            break;
        case 2:
            for(int i = 1; i < nValues; i++){
                printf("- %d ", values[i]);
                res -= values[i];
            }
            break;
        case 3:
            for(int i = 1; i < nValues; i++){
                printf("* %d ", values[i]);
                res *= values[i];
            }
            break;
        }
    printf("= ");
    return res;
}

int designateNumber(char *operator){
    if(strcmp(operator, "add") == 0){
        return 1;
    }else if(strcmp(operator, "sub") == 0){
        return 2;
    }else if(strcmp(operator, "mult") == 0){
        return 3;
    }else{
        return -1;
    }
}

int main(int argc, char **argv) {
    // https://www.codesdope.com/c-dynamic-memory/
    // at least ./calculator.o <<operator>> <<number>> <<number>> 
    if(argc <= 3){
        printf("No se mandaron los suficientes parametros para el programa.\n");
        return -1;
    }
    char *word;
    word = argv[1];
    int operator = designateNumber(word);

    if(operator == -1){
        printf("No se mandó ningun operador disponible.\n");
        return -1;
    }
    
    int len = argc-2;
    int *numbers = (int*) malloc(len * sizeof(int));   /* allocating memory dynamically */
    
    for(int i = 0; i < len; i++){
        char *number = argv[2 + i];

        // checks if the parameter is a integer
        for(int j = 0; number[j] != 0; j++){
            if(!isdigit(number[j])){
                printf("Se ha mandando un arreglo que no contiene puros enteros.\n");
                return -1;
            }
        }
        numbers[i] = atoi(number);
    }

    printf("%ld \n", calc(operator, len, numbers));
    free(numbers);
   
    return 0;
}
