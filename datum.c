#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "datum.h"
#include "stringutils.h"

#define DELIMITER "."

Datum* datum_from_string(char* s)
{
    char* string = strduplicate(s);
    
    if (strlen(string) < 6) 
        return nezadane();
    
    Datum* datum = malloc(sizeof(Datum));
    
    char* token = strtok(string, DELIMITER);
    datum->den = atoi(token);
    
    token = strtok(NULL, DELIMITER);
    datum->mesiac = atoi(token);
    
    token = strtok(NULL, DELIMITER);
    datum->rok = atoi(token);
    
    free(string);
    return datum;
}

char* datum_to_string(Datum* datum)
{
    char* string = malloc(sizeof(char) * 10);
    
    if (string == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    sprintf(string, "%d.%d.%d", datum->den, datum->mesiac, datum->rok);
    
    return string;
}

int porovnaj_datumy(Datum* datum1, Datum* datum2)
{
    if(datum1->rok == -1)
        return 1;
    
    if(datum2->rok == -1)
        return -1;
    
    if(datum1->rok < datum2->rok)
        return -1;
    
    if(datum1->rok > datum2->rok)
        return 1;
    
    if(datum1->mesiac < datum2->mesiac)
        return -1;
    
    if(datum1->mesiac > datum2->mesiac)
        return 1;
    
    if(datum1->den < datum2->den)
        return -1;
    
    if(datum1->den > datum2->den)
        return 1;
    
    return 0;
}

long datum_v_hodinach(Datum* datum) 
{
    long vysledok = 0;
    
    vysledok += datum->rok * 8760;
    vysledok += (datum->mesiac - 1) * 720;
    vysledok += datum->den * 24;
    
    return vysledok;
}

long rozdiel_hodin_datumov(Datum* datum1, Datum* datum2)
{
    long vysledok = datum_v_hodinach(datum1) - datum_v_hodinach(datum2);
    
    return vysledok;
}

Datum* nezadane()
{
    Datum *datum = malloc(sizeof(Datum));
    
    if (datum == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    datum->den = -1;
    datum->mesiac = -1;
    datum->rok = -1;
    
    return datum;
}
