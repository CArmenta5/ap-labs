#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

/*Funciones de ayuda*/
int strCompare(char *origin, char *check){
    int i = 0;
    if(mystrlen(origin) != mystrlen(check)){
        return -1;
    }
    for(; origin[i] != '\0'; i++){
        if (origin[i] != check[i]){
            return -1;
        }
    }
    return 0;

}

int main(int argc, char **argv) {

    char *operacion;
    if(argc != 4){
        printf("No se han mandado la cantidad de parametros necesarios.\n");
        printf("-add/-find \"<<cadena>>\" \"<<cadena>>\".\n");
        return 0;
    }
    if(strCompare(argv[1],"-add") == 0 || strCompare(argv[1],"-find") == 0){
        operacion = argv[1];
    }else{
        printf("No se mando la operacion correcta.\n");
        printf("-add/-find \"<<cadena>>\" \"<<cadena>>\".\n");
        return 0;
    }
    if (strCompare(operacion,"-add") == 0){
        printf("Initial Lenght      : %d\n", mystrlen(argv[2]));
        char* new = mystradd(argv[2], argv[3]);
        printf("New String          : %s\n", new);
        printf("New length          : %d\n", mystrlen(new));
    }else{
        printf("['%s'] string was found at [%d] position\n", argv[3], mystrfind(argv[2], argv[3]));
    }
    return 0;
}
