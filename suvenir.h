#ifndef SUVENIR_H
#define SUVENIR_H

#include "datum.h"

typedef struct suvenir
{
    double cena;
    Datum* datumVyroby;
    char* nazov;
    char* kategoria;
    char* autor;
    Datum* datumPredaja;
} Suvenir;

Suvenir* vytvor_nepredany_suvenir(Datum* datum_vyroby, char* nazov, char* kategoria, char* autor, double cena);
Suvenir vytvor_predany_suvenir(Datum* datum_vyroby, char* nazov, char* kategoria, char* autor, double cena, Datum* datum_predaja);
Suvenir* zo_stringu(char* string);
char* to_string_suvenir(Suvenir* suvenir);
int je_predany(Suvenir* suvenir);
int compare_to(Suvenir* suvenir1, Suvenir* suvenir2);

#endif /* SUVENIR_H */

