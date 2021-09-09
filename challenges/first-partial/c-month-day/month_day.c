#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Chear año 
static char daytab[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

static char *name[] = {
       "January", "February", "March",
       "April", "May", "June",
       "July", "August", "September",
       "October", "November", "December"
    };
    
int leap(int year, int yearday){
    if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0){
        if(yearday > 366 || yearday == 0){
            printf("Se mandó un día fuera de los limites del año.\n");
            return -1;
        }
        return 1;
    }else{
        if(yearday > 365 || yearday == 0){
            printf("Se mandó un día fuera de los limites del año.\n");
            return -1;
        }
        return 0;
    }
}

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
    //Check if the number is a bisiesto
    int bisiesto = leap(year, yearday);
    if(bisiesto != -1){
        int acum = 0;
        int res = 0;
        
        for(*pmonth = 0; *pmonth < 12; *pmonth = *pmonth +1 ){
            if(daytab[bisiesto][*pmonth] + acum >= yearday ){
                *pday = daytab[bisiesto][*pmonth] - ((daytab[bisiesto][*pmonth] + acum) - yearday);
                break;
            }else{
                acum += daytab[bisiesto][*pmonth];
            }
        }

        printf("%s %d, %d\n", name[*pmonth], *pday, year);
    }
  
}

int main(int argc, char **argv) {
    if(argc != 3){
        printf("No se mandaron los datos correctamente.\n");
        return -1;
    }
    for(int i = 0; i < 2; i++){
        char *number = argv[1 + i];
        for(int j = 0; number[j] != 0; j++){
            if(!isdigit(number[j])){
                printf("Se han mandado dos parametros que no son enteros.\n");
                return -1;
            }
        }
    }
    int year = atoi(argv[1]);
    int yearday = atoi(argv[2]);
    int month = 0;
    int *pmonth = &month;
    int day = 0;
    int *pday = &day;

    month_day(year, yearday, pmonth, pday);

    
    return 0;
}
