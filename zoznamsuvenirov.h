#ifndef ZOZNAMSUVENIROV_H
#define ZOZNAMSUVENIROV_H

#include "suvenir.h"

typedef struct
{
    int velkost;
    int kapacita;
    Suvenir** suveniry;
} list_suveniry;

list_suveniry* list_init();
void list_free(list_suveniry* ls);
void list_pridaj(list_suveniry* suveniry, Suvenir* suvenir);

list_suveniry* zo_suboru(char* nazov_suboru);
void uloz(char *nazov_suboru, list_suveniry* ls);
char** zoznam_umelcov(list_suveniry* ls);
Suvenir* najlacnejsi_v_predaji(list_suveniry* ls);
list_suveniry* zoznam_podla_kategorie(char *kategoria, list_suveniry* ls);
void inventura(list_suveniry* ls, char** nazvy, int* pocet_nepredanych_kusov);
void predaj(list_suveniry* ls, char* nazov, Datum* datum_predaja);
void vyplata(list_suveniry* ls, char** umelci, int* vyplaty, int mesiac, int rok);
char** viacdodavatelske_vyrobky(list_suveniry* ls);
int nachadza_sa_v_zozname(char** zoznam, char* string);
char** co_vyrabat(list_suveniry* ls, char* umelec);
Suvenir* najrychlejsie_vyrobeny_vyrobok(list_suveniry* ls, char* autor);
int abs(int hodnota);
int kedy_na_dovolenku(list_suveniry* ls, int rok);
void zorad_podla_datumu_predaja(list_suveniry* ls);
char*** zoznam_do_tabulky(list_suveniry* list);

#endif /* ZOZNAMSUVENIROV_H */

