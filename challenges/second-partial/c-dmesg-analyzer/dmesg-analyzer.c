#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPORT_FILE "report.txt"
#define HASHSIZE 5000

static struct nlist *hashtab[HASHSIZE];  /* pointer table */

void analizeLog(char *logFile, char *report);


int main(int argc, char **argv) {

    if (argc < 2) {
	    printf("Usage:./dmesg-analizer logfile.txt\n");
	    return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}
/*BEGIN AUX (mystradd, mystrfind, substr)*/
char* mystradd(const char *origin, const char *addition){
    char *result = malloc(strlen(origin) + strlen(addition) + 1); 
    strcpy(result, origin);
    strcat(result, addition);
    return result;
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
char * mystrsub(char *origin, int start, int end){
    char* res = malloc((end - start) + 2);
    int i;
    for(i = 0; start < end; i++){
        res[i] = origin[start];
        start++;
    }
    res[i+1]='\0';
    return res;
}
/*END AUX*/
/*BEGIN Table Lookup*/
struct nlist {       
    struct nlist *next;   
    char *name;           
    char *defn;           
};

unsigned hash(char *s){
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s){
    struct nlist *np;
    for (np = hashtab[hash(s)];  np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;     
    return NULL;           
}

char *mystrdup(const char *s){
    char *r = NULL;
    if((r = malloc(strlen(s) + 1)) != NULL)
        memcpy(r, s, strlen(s) + 1);
  return r;
}

struct nlist *install(char *name, char *defn){
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { 
        np = (struct nlist *) malloc(sizeof(*np) + 1);
        if (np == NULL || (np->name = mystrdup(name)) == NULL){
            return NULL;}
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else   {   
        //free((void *) np->defn);   /*free previous defn */
        np->defn = mystradd(np->defn, defn);
        return NULL;
    } 
    if ((np->defn = mystrdup(defn)) == NULL)
        return NULL;
    return np;
}
/*END Table Lookup*/

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    
    char *time;
    char *entry;
    char *description;
    int init = 10;

    char** keys = malloc(init * sizeof(*keys));
    
    char *line = NULL;
    FILE *fp;
    size_t len = 0;
    ssize_t read;


    fp = fopen(logFile,"r");
    if(fp == NULL){
		printf("Favor de mandar un archivo para evaluar.\n");
        return;
    }
        
    keys[0] = "General:";

    int cont = 1;
    while((read = getline(&line, &len, fp)) != -1){       
        if(cont == init){
                init = init*2;
                char** tmp = realloc(keys, init * sizeof(*keys));
                keys = tmp;
        }
        int pos = mystrfind(line,"]");
        time = mystrsub(line, 0, pos + 1);
        time = mystradd("\t", time);
        int pos2 = mystrfind(line,": ");
        if(pos2 == -1){
            entry = "General:";
        }else{
            entry = mystrsub(line, pos+2, pos2+1);
        }
        description = mystrsub(line, pos2 + 1, strlen(line));  
        if(lookup(entry) == NULL){
            keys[cont] = entry;
            cont++;
        }
        if(pos2 ==-1){
            install(entry, mystradd("\t", description));
        }else{
            install(entry, mystradd(time, description));

        }
    }
    fclose(fp);
    if(line)
        free(line);

    int fd = open(report, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("Problemas al intentar abrir el archivo %s", report);
        return;
    }
    struct nlist *np;

    for(int i  = 0; i < cont;i++){
        char *text= malloc(sizeof(char) * strlen(keys[i])+1);
        memcpy(text, keys[i], strlen(keys[i])+1);    
        write(fd, keys[i], strlen(keys[i]));
        write(fd, "\n", strlen("\n"));
        for (np = hashtab[hash(keys[i])];  np != NULL; np = np->next){
            char *log = malloc(sizeof(char) * strlen(np->defn)+1);
            memcpy(log, np->defn, strlen(np->defn)+1);
            write(fd, log, strlen(log));

        }
    }
    if(close(fd) < 0){
        printf("Problemas al intentar cerrar el archivo %s", report);
        return;
    }
    printf("Report is generated at: [%s]\n", report);
}