#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include <limits.h>

#include "zoznamsuvenirov.h"
#include "suvenir.h"
#include "datum.h"
#include "stringutils.h"

#define ARRAY_CAPACITY_INCREMENT 10
#define DLZKA_RIADKA_MAX 100
#define DELIMITER "|"

list_suveniry* list_init()
{
    list_suveniry* suveniry = malloc(sizeof(list_suveniry));
    suveniry->kapacita = 0;
    suveniry->velkost = 0;
    suveniry->suveniry = NULL;
    
    return suveniry;
}

void list_free(list_suveniry* zoznam) 
{
    int i;
    
    for (i = 0; i < zoznam->velkost; i++) 
        free(zoznam->suveniry[i]);

    free(zoznam->suveniry);
    zoznam->suveniry = NULL;

    zoznam->velkost = 0;
    zoznam->kapacita = 0;
}

void list_pridaj(list_suveniry* zoznam, Suvenir* suvenir)
{

    if (zoznam->velkost <= zoznam->kapacita)
    {
        zoznam->kapacita += ARRAY_CAPACITY_INCREMENT;
        zoznam->suveniry = realloc(zoznam->suveniry, sizeof(Suvenir) * zoznam->kapacita);
    }

    zoznam->suveniry[zoznam->velkost] = suvenir;
    zoznam->velkost++;
}

list_suveniry* zo_suboru(char* nazov_suboru) 
{
    FILE* subor = fopen(nazov_suboru, "r");
    
    if (subor == NULL) 
    {
        fprintf(stderr, "Nepodarilo sa otvorit %s.\n", nazov_suboru);
        exit(EXIT_FAILURE);
    }

    list_suveniry* suveniry = list_init();
    
    char* uloha_string = malloc(sizeof(char) * DLZKA_RIADKA_MAX);

    while (fgets(uloha_string, DLZKA_RIADKA_MAX, subor) != NULL)
    { 
        Suvenir *suvenir = zo_stringu(uloha_string);
        list_pridaj(suveniry, suvenir);
    }
    
    free(uloha_string);
    fclose(subor);
    
    return suveniry;
}

void uloz(char* nazov_suboru, list_suveniry* zs) 
{
   FILE* subor = fopen(nazov_suboru, "w");
   
   if (subor == NULL) 
   {
        fprintf(stderr, "Nepodarilo sa otvorit %s.\n", nazov_suboru);
        exit(EXIT_FAILURE);
    }
   
   for(int i = 0; i < zs->velkost; i++) 
   {
       char* string_suvenir = to_string_suvenir(zs->suveniry[i]);
       fputs(string_suvenir, subor);
   }
   
   fclose(subor);
}

char** zoznam_umelcov(list_suveniry* ls) 
{
    char** zoznam = malloc(sizeof(char*) * (ls->velkost + 1));
    
    if (zoznam == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    int pridat, idx = 0;
    
    for(int i = 0; i < ls->kapacita; i++) 
    {
        pridat = 1;
        
        for(int j = 0; j < idx; j++)
        {
            if(strcmp(ls->suveniry[i]->autor, zoznam[j]) == 0) 
                pridat = 0;
        }
        
        if (pridat) 
        {
            zoznam[idx] = ls->suveniry[i]->autor;
            idx++;
        }
    }
    
    zoznam[idx] = NULL;
    
    return zoznam;
}

Suvenir* najlacnejsi_v_predaji(list_suveniry* ls) 
{
    Suvenir* suvenir = malloc(sizeof(Suvenir));
    
    if (suvenir == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    double najlacnejsia_cena = INT_MAX;
    
    for(int i = 0; i < ls->velkost; i++)
    {
        if (ls->suveniry[i]->datumPredaja->den > -1 && ls->suveniry[i]->cena < najlacnejsia_cena)
        {
            najlacnejsia_cena = ls->suveniry[i]->cena;
            suvenir = ls->suveniry[i];
        }
    }
    
    return suvenir;
}

list_suveniry* zoznam_podla_kategorie(char* kategoria, list_suveniry* ls)
{
    list_suveniry* vysledok = list_init();
    
    for(int i = 0; i < ls->velkost; i++)
    {
        if (strcmp(ls->suveniry[i]->kategoria, kategoria) == 0)
            list_pridaj(vysledok, ls->suveniry[i]);
    }
    
    return vysledok;
}

void inventura(list_suveniry* ls, char** nazvy, int* pocet_nepredanych_kusov)
{
    int pridat, idx = 0;
    
    for(int i = 0; i < ls->velkost; i++)
    {
        pridat = 1;
        
        for(int j = 0; j < idx; j++)
        {
            if (strcmp(ls->suveniry[i]->nazov, nazvy[j]) == 0)
                pridat = 0;
            
            if(ls->suveniry[i]->datumPredaja->den == -1 && strcmp(ls->suveniry[i]->nazov, nazvy[j]) == 0)          
                pocet_nepredanych_kusov[j]++;         
        }
        
        if (pridat == 1)
        {
            nazvy[idx] = ls->suveniry[i]->nazov;
            pocet_nepredanych_kusov[idx] = ls->suveniry[i]->datumPredaja->den == -1 ? 1 : 0;
            idx++;
        }
    }
    
    nazvy[idx] = NULL;
}

void predaj(list_suveniry* ls, char* nazov, Datum* datum_predaja) 
{
    Suvenir* najstarsi_suvenir = NULL;
    Datum* najstarsi_datum = nezadane();
    trimwhitespace(nazov);
  //  trimwhitespace(datum_predaja);
    
    for(int i = 0; i < ls->velkost; i++)
    {
        if (strcmp(nazov, ls->suveniry[i]->nazov) == 0) 
        {
            if (je_predany(ls->suveniry[i]) == 1) { continue; }
        
            if (porovnaj_datumy(najstarsi_datum, ls->suveniry[i]->datumVyroby) == 1)
            {
                najstarsi_suvenir = ls->suveniry[i];
                najstarsi_datum = ls->suveniry[i]->datumVyroby;
            }       
        }
    }
    
    if(najstarsi_suvenir != NULL) 
        najstarsi_suvenir->datumPredaja = datum_predaja;
    
    free(nazov);
    free(najstarsi_datum);
    
}

void vyplata(list_suveniry* ls, char** umelci, int* vyplaty, int mesiac, int rok) 
{  
    for (int i = 0; i < ls->velkost; i++) {
        if (umelci[i] == NULL) 
            break;     
        
        vyplaty[i] = 0;
        
        for (int j = 0; j < ls->velkost; j++) 
        {
            if(ls->suveniry[j]->datumPredaja->mesiac == mesiac 
                    && ls->suveniry[j]->datumPredaja->rok == rok) 
            {
                if(strcmp(ls->suveniry[j]->autor, umelci[i]) == 0) 
                {
                    vyplaty[i] += ls->suveniry[j]->cena * 0.7;
                }
            }
        }
    }
}

char** viacdodavatelske_vyrobky(list_suveniry* ls) 
{
    char** zoznam_vyrobkov = malloc(sizeof(char*) * ls->velkost);
    
    if (zoznam_vyrobkov == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    int idx = 0;
    zoznam_vyrobkov[idx] = NULL;
    
    for(int i = 0; i < ls->velkost; i++) 
    {
        for(int j = i + 1; j < ls->velkost; j++) 
        {
            if (strcmp(ls->suveniry[i]->nazov, ls->suveniry[j]->nazov) == 0
                    && strcmp(ls->suveniry[i]->autor, ls->suveniry[j]->autor) != 0
                        && nachadza_sa_v_zozname(zoznam_vyrobkov, ls->suveniry[i]->nazov) == -1) 
            {
                zoznam_vyrobkov[idx] = ls->suveniry[i]->nazov;
                idx++;
                zoznam_vyrobkov[idx] = NULL;
            }
        }
    }
        
    return zoznam_vyrobkov;
}

int nachadza_sa_v_zozname(char **zoznam, char *string) 
{
    int nachadza_sa = -1, i = 0;
    
    while (zoznam[i] != NULL) {
        if(strcmp(zoznam[i], string) == 0) {
            nachadza_sa = 1;
            break;
        }
        i++;
    }

    return nachadza_sa;
}

char** co_vyrabat(list_suveniry* ls, char* umelec) 
{
    char** zoznam_vyrobkov = malloc(sizeof(char*));
    
    if (zoznam_vyrobkov == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    int idx = 0;
    
    zoznam_vyrobkov[0] = NULL;
    
    for(int i = 0; i < ls->velkost; i++) 
    {
        if(strcmp(ls->suveniry[i]->autor, umelec) == 0 && 
                ls->suveniry[i]->datumPredaja->rok > -1) 
        {       
            if(nachadza_sa_v_zozname(zoznam_vyrobkov, ls->suveniry[i]->nazov) == -1) 
            {
                zoznam_vyrobkov[idx] = ls->suveniry[i]->nazov;
                idx++;
                realloc(zoznam_vyrobkov, (sizeof(char*) * (idx + 1)));
                zoznam_vyrobkov[idx] = NULL;
            }
        }
    }
    
    if(idx <= 2) 
    {
        zoznam_vyrobkov[idx] = "nieco nove";
        zoznam_vyrobkov[idx + 1] = NULL;
    }
    
    return zoznam_vyrobkov;
}

Suvenir* najrychlejsie_vyrobeny_vyrobok(list_suveniry* ls, char* autor) 
{
    Suvenir* suvenir = malloc(sizeof(Suvenir));
    Suvenir* vysledok = malloc(sizeof(Suvenir));
    
    if (suvenir == NULL || vysledok == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    long najkratsi_rozdiel_hodin = INT_MAX;
    
    suvenir = NULL;
    
    for(int i = 0; i < ls->velkost; i++) 
    {
        if (suvenir == NULL && strcmp(ls->suveniry[i]->autor, autor) == 0) 
        {
            suvenir = ls->suveniry[i];
            continue;
        }

        if (strcmp(ls->suveniry[i]->autor, autor) == 0) 
        {
            if(abs(rozdiel_hodin_datumov(suvenir->datumVyroby, ls->suveniry[i]->datumVyroby)) < najkratsi_rozdiel_hodin) 
            {
                vysledok = ls->suveniry[i];
                najkratsi_rozdiel_hodin = abs(rozdiel_hodin_datumov(suvenir->datumVyroby, ls->suveniry[i]->datumVyroby));
            }
            
            suvenir = ls->suveniry[i];
        }     
    }
    
    return vysledok;
}

int abs(int hodnota)
{
    return hodnota < 0 ? hodnota * (-1) : hodnota;
}

int kedy_na_dovolenku(list_suveniry* ls, int rok) 
{
    int najmensia_trzba = INT_MAX;
    int den_od = 1;
    
    for (int i = 1; i < 335; i++) 
    {
        double trzba = 0;
        
        for (int j = 0; j < ls->velkost; j++) 
        {
            if (ls->suveniry[j]->datumPredaja->rok == rok) 
            {
                Datum* datum = ls->suveniry[j]->datumPredaja;
                int pocet_dni = (datum->mesiac - 1) * 30 + datum->den;
                
                if (pocet_dni >= i && pocet_dni <= (i + 30)) 
                {
                    trzba += ls->suveniry[j]->cena;
                }
            }
        }
        
        if (trzba < najmensia_trzba) 
        {
            den_od = i;
            najmensia_trzba = trzba;
        }
    }
    
    return den_od;
}

void zorad_podla_datumu_predaja(list_suveniry* ls) 
{
    Suvenir* tmp = malloc(sizeof(Suvenir));  
    
    if (tmp == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    int max_idx = 0;
      
    if (ls->velkost <= 1) 
        return;   
    
    for(int i = 0; i < ls->velkost; i++) 
    {
        max_idx = i;
        for(int j = i + 1; j < ls->velkost; j++) 
        {
            if (compare_to(ls->suveniry[j], ls->suveniry[max_idx]) == -1) 
                max_idx = j;          
        }
        
        tmp = ls->suveniry[i];
        ls->suveniry[i] = ls->suveniry[max_idx];
        ls->suveniry[max_idx] = tmp;
    }  
}

char*** zoznam_do_tabulky(list_suveniry* list)
{
    char*** vysledok = malloc(sizeof(char**) * list->velkost);

    for (int i = 0; i < list->velkost; i++)
    {
        Suvenir* suvenir = list->suveniry[i];

        vysledok[i] = malloc(sizeof(char*) * DLZKA_RIADKA_MAX);

        vysledok[i][0] = datum_to_string(suvenir->datumVyroby);
        vysledok[i][1] = strduplicate(suvenir->nazov);
        vysledok[i][2] = strduplicate(suvenir->kategoria);
        vysledok[i][3] = strduplicate(suvenir->autor);
        vysledok[i][4] = double_to_string(suvenir->cena);
        vysledok[i][5] = datum_to_string(suvenir->datumPredaja);
    }

    return vysledok;
}


