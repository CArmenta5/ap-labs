#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINES 5000
void *docptr[MAXLINES];

void quicksort(void *lineptr[], int left, int right,
               int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
               int (*comp)(void *, void *));

// Number compare function
int  numcmp(const char *a, const char *b){
	double n1, n2; 
	n1 = atof(a); 
    n2 = atof(b);
	if(n1 == n2){
		return 0;
	}else if(n1 > n2){
		return 1;
	}else{
		return -1;
	}
} 

int main(int argc, char **argv) {
	// Compare value 1 - numbers | 0 - strings
	int cmp = 0;
		
	if(argc == 5 || argc == 6){

		// Checks the flag to assign the compare value
		if (strcmp(*++argv, "-n") == 0) {
     		cmp = 1;
    	}else{
			*--argv;
		}
		
		/*Read file*/
		FILE *fp;
		char *line = NULL;
		size_t len = 0;
    	ssize_t read;

		fp = fopen(*++argv,"r");
		if(fp == NULL){
			printf("Favor de mandar un archivo para evaluar.\n");
			return 0;
		}
		int cont = 0;
		while((read = getline(&line, &len, fp)) != -1){
			docptr[cont] = malloc(strlen(line)+1);
			if(line != NULL){	
				if(cont > 4999){
					printf("Se ha llegado al máximo de contenido.\n");
					return 0;
				}
				strcpy(docptr[cont], line);
				
				cont++;
			}
		}
		fclose(fp);
		if(line)
			free(line);
		if(cont == 0){
			printf("El del archivo está vacio.\n");
			return 0;
		}
		char* sort;
		strcpy(sort, *++argv);
		if(strcmp(sort, "-quicksort") == 0){
			printf("sorting numbers.txt file with quicksort\n");
			quicksort(docptr, 0, cont-1, (int (*)(void *, void *))(cmp ? numcmp : strcmp));
		}else if(strcmp(sort, "-mergesort") == 0){
			printf("sorting numbers.txt file with mergesort\n");
			mergesort(docptr, 0, cont-1, (int (*)(void *, void *))(cmp ? numcmp : strcmp));
		}else{
			printf("Mande una forma de ordenamiento adecuada.\n");
			return 0;
		}
		// Check the output flag 
		if(strcmp(*++argv, "-o") == 0){
			fp = fopen(*++argv, "w");
			if(fp == NULL){
				printf("Favor de mandar un archivo para la salida.\n");
				return 0;
			}
			for(int i =0; i< cont;i++){
				fprintf(fp, "%s",(char*)  docptr[i]);
			}
			printf("Results file can be found at ./%s\n",*argv--);

			fclose(fp);
		}else{
			printf("No se mandó la bandera -o para la salida del archivo.\n");
			return 0;
		}
	}else{
		printf("No se mandó la cantidad correcta de parametros para el programa.\n");
		return 0;
	}
}