#include <malloc.h>
#include <string.h>

#include "suvenir.h"
#include "stringutils.h"
#include "datum.h"

#define DELIMITER "|"
#define DLZKA_RIADKA_MAX 100

Suvenir* vytvor_nepredany_suvenir(Datum* datum_vyroby, char* nazov, char* kategoria, char* autor, double cena)
{
    Suvenir* vysledok = malloc(sizeof(Suvenir));
    
    if (vysledok == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    Suvenir suvenir = {  cena, datum_vyroby, nazov, kategoria, autor, nezadane()};
    *vysledok = suvenir;

    return vysledok;
}

Suvenir vytvor_predany_suvenir(Datum* datum_vyroby, char* nazov, char* kategoria, char* autor, double cena, Datum* datum_predaja)
{
    Suvenir* vysledok = malloc(sizeof(Suvenir));
    
    if (vysledok == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    Suvenir suvenir = { cena, datum_vyroby, nazov, kategoria, autor, datum_predaja };
    *vysledok = suvenir;

    return suvenir;
}

Suvenir* zo_stringu(char* string)
{
    Suvenir* vysledok = malloc(sizeof(Suvenir));
    char* string_vysledny = strduplicate(string);
    
    char** token = malloc(sizeof(char*) * 6);
    
    if (vysledok == NULL || token == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }

    token[0] = strtok(string_vysledny,"|");
    
    for(int i = 1; i < 6; i++)
        token[i] = strtok(NULL, "|");    
    
    vysledok->datumVyroby = datum_from_string(token[0]);    
    vysledok->nazov = token[1];
    vysledok->kategoria = token[2];
    vysledok->autor = token[3];
    vysledok->cena = atof(token[4]);

    
    if(token[5] == NULL)   
        vysledok->datumPredaja = nezadane();   
    else   
        vysledok->datumPredaja = datum_from_string(token[5]);
       
    free(token);
    
    return vysledok;
}

char* to_string_suvenir(Suvenir *suvenir)
{
    char* cena_string = double_to_string(suvenir->cena);

    char* string = malloc(sizeof(char) * DLZKA_RIADKA_MAX);
    
    if (string == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    trimwhitespace(datum_to_string(suvenir->datumVyroby));
    trimwhitespace(suvenir->nazov); 
    trimwhitespace(suvenir->kategoria); 
    trimwhitespace(suvenir->autor); 
    trimwhitespace(cena_string);
    trimwhitespace(datum_to_string(suvenir->datumPredaja));
    
    sprintf(string, "%s|%s|%s|%s|%s|%s\n", datum_to_string(suvenir->datumVyroby), 
            suvenir->nazov, suvenir->kategoria, suvenir->autor, cena_string, 
            datum_to_string(suvenir->datumPredaja));

    free(cena_string);
    
    return string;
}

int je_predany(Suvenir *suvenir)
{
    if (suvenir->datumPredaja->den == -1) {
        return -1;
    } 
    else {
        return 1;
    }
}

int compare_to(Suvenir *suvenir1, Suvenir *suvenir2)
{
    if (suvenir1->datumPredaja->rok == -1) 
    {
	if (suvenir2->datumPredaja->rok == -1)
            return 0;
	else
            return -1;               
    } 
    else if (suvenir2->datumPredaja->rok == -1)
	return 1;
    
    long rozdiel_datumov = rozdiel_hodin_datumov(suvenir1->datumPredaja, suvenir2->datumPredaja);
    
    if (rozdiel_datumov > 0) 
	return -1;
    else if (rozdiel_datumov < 0)
	return 1;
    else 
    {
	rozdiel_datumov = rozdiel_hodin_datumov(suvenir1->datumVyroby, suvenir2->datumVyroby);
        
	if (rozdiel_datumov > 0)
            return -1;
	else if (rozdiel_datumov < 0)
            return 1;
	else
            return 0;
    }
}

