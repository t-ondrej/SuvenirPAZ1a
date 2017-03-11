#include <stdio.h>
#include <stdlib.h>

#include "zoznamsuvenirov.h"
#include "suvenir.h"
#include "userinterface.h"
#include "stringutils.h"

#define NAZOV_SUBORU "suveniry.txt"
#define POCET_MOZNOSTI 15
#define tabulka_pocet_stlpcov 6
#define VYSTUP_OFFSET 2
#define LAVA_BUNKA_OFFSET 1
#define DLZKA_RIADKA_MAX 100
#define MAX(a,b) ((a < b) ?  (b) : (a))

static const char* banner[] = {  
"  ____                        _            _                 ",
" / ___| _   ___   _____ _ __ (_)_ __   ___| |__   ___  _ __  ",
" \\___ \\| | | \\ \\ / / _ \\ '_ \\| | '__| / __| '_ \\ / _ \\| '_ \\ ",
"  ___) | |_| |\\ V /  __/ | | | | |    \\__ \\ | | | (_) | |_) |",
" |____/ \\__,_| \\_/ \\___|_| |_|_|_|    |___/_| |_|\\___/| .__/ ",
"                                                      |_|    "
};

static const int banner_rows = sizeof banner / sizeof banner[0];

static const char* moznosti[] = { "Ukoncit program", "Zobrazit moznosti",
		"Vypisat vsetky suveniry", "Pridat suvenir", "Vypisat umelcov",
		"Vypisat najlacnejsi suvenir v predaji", "Vypisat suveniry danej kategorie",
		"Urobit inventuru", "Predat suvenir", "Vypisat vyplaty",
		"Vypisat viacdodavatelskych suvenirov", "Co vyrabat?",
		"Najrychlejsie vyrobeny suvenir", "Kedy ist na dovolenku","Zoradit podla datumu predaja" };

static const char* hlavicka[] = {  "Datum vyroby", "Nazov", "Kategoria", 
                "Autor", "Cena","Datum predaja" };

void zobraz_banner()
{
    for (int i = 0; i < banner_rows; i++)
	printf("%s\n", banner[i]);

    printf("\n");
}

void zobraz_moznosti() 
{
    for (int i = 0; i < POCET_MOZNOSTI; i++) 
        printf(" %d. %s\n", i + 1, moznosti[i]);      
        
    printf("\n");
}

void spusti_program()
{
    zobraz_banner();
    zobraz_moznosti();
    
    list_suveniry *list = zo_suboru(NAZOV_SUBORU);
    
    while (1) 
    {
        printf("Zadajte cislo moznosti, ktoru chcete vybrat: ");
        
        int cislo_moznosti = vyziadaj_vstup();       
                
        if (cislo_moznosti == 0)
        {
            printf("Neplatne cislo moznosti.\n"); 
            continue;
        }
        
        vyber_moznost(cislo_moznosti, list);
    }
}

int vyziadaj_vstup() 
{
    char* str = malloc(sizeof(char) * 5);
        
    if (str == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
        
    fgets(str, 5, stdin);
    
    return atoi(str);
}

void vyber_moznost(int cislo_moznosti, list_suveniry* list) 
{       
    switch (cislo_moznosti) {
	case 1:
        	return;
        case 2:
        	zobraz_moznosti();
		break;
	case 3:
		vypis_suvenirov(list);
		break;
	case 4:
		nacitaj_a_pridaj_suvenir(list);
		break;
	case 5:
		vypis_umelcov(list);
		break;
	case 6:
		vypis_najlacnejsi_v_predaji(list);
		break;
	case 7:
		vypis_podla_kategorie(list);
		break;
	case 8:
		vypis_inventuru(list);
		break;
	case 9:
		vypis_predaj(list);
		break;
	case 10:
		vypis_vyplaty(list);
		break;
	case 11:
		vypis_viacdodavatelske_suveniry(list);
		break;
	case 12:
		vypis_co_vyrabat(list);
		break;
	case 13:
		vypis_najrychlejsie_vyrobeny_suvenir(list);
		break;
	case 14:
		vypis_kedy_na_dovolenku(list);
               	break;
	case 15:
		vypis_zoradenie_podla_datumu_predaja(list);
		break;
    }   
}

void vypis_suvenirov(list_suveniry* ls)
{
    vypis_list(ls);
}

void nacitaj_a_pridaj_suvenir(list_suveniry* ls)
{
    Suvenir* suvenir = malloc(sizeof(Suvenir));   
    char* string = malloc(sizeof(char) * 100);
    
    if (suvenir == NULL || string == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte nazov:");
    fgets(string, 100, stdin);
    suvenir->nazov = strduplicate(string);
    
    printf("Zadajte datum vyroby: ");
    fgets(string, 100, stdin);
    suvenir->datumVyroby = datum_from_string(string);
    
    printf("Zadajte kategoriu: ");
    fgets(string, 100, stdin);
    suvenir->kategoria = strduplicate(string);
    
    printf("Zadajte autora: ");
    fgets(string, 100, stdin);
    suvenir->autor = strduplicate(string);
    
    printf("Zadajte cenu: ");
    fgets(string, 100, stdin);
    suvenir->cena = atof(string);
    
    printf("Zadajte datum predaja: ");
    fgets(string, 100, stdin);
    suvenir->datumPredaja = datum_from_string(string);
    
    free(string);
    
    list_pridaj(ls, suvenir);
    uloz(NAZOV_SUBORU, ls);
}

void vypis_umelcov(list_suveniry* ls)
{
    char** umelci = zoznam_umelcov(ls);
    int i = 0;
    
    printf("Zoznam umelcov: ");
    
    while (umelci[i] != NULL) 
    {
        printf("%i. %s \n", i + 1, umelci[i]);      
        i++;
    }
    
    for (int j = 0; j < i; j++) 
	free(umelci[j]);
       
    free(umelci);
}

void vypis_najlacnejsi_v_predaji(list_suveniry* ls)
{
    Suvenir* suvenir = najlacnejsi_v_predaji(ls);
    char* string = to_string_suvenir(suvenir);
    printf("Najlacnejsi suvenir: %s", string);
    
    free(suvenir);
    free(string);
}

void vypis_podla_kategorie(list_suveniry* ls) 
{
    char* kategoria = malloc(sizeof(char) * 100);
    
    if (kategoria == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte kategoriu: ");
    fgets(kategoria, 100, stdin);
    
    trimwhitespace(kategoria);
    
    list_suveniry *zoznam = zoznam_podla_kategorie(kategoria, ls);
    vypis_suvenirov(zoznam);
    
    free(kategoria);
    list_free(zoznam);
}

void vypis_inventuru(list_suveniry* ls)
{
    char** nazvy_suvenirov = malloc(sizeof(char*) * ls->velkost);
    int* pocet_nepredanych_kusov = malloc(sizeof(int) * ls->velkost);
    
    if (nazvy_suvenirov == NULL || pocet_nepredanych_kusov == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    inventura(ls, nazvy_suvenirov, pocet_nepredanych_kusov);
    
    for (int i = 0; i < ls->velkost; i++) 
    {
        if (nazvy_suvenirov[i] == NULL) 
            break;
        
	printf("Nazov suveniru: %s\t Pocet nepredanych kusov: %d\n", nazvy_suvenirov[i], pocet_nepredanych_kusov[i]);   
        free(nazvy_suvenirov[i]);
    }
    
    free(pocet_nepredanych_kusov);
    free(nazvy_suvenirov);
}

void vypis_predaj(list_suveniry* ls) 
{
    char* nazov_suveniru = malloc(sizeof(char) * 100);
    
    if (nazov_suveniru == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte nazov suveniru, ktory chcete kupit: ");
    fgets(nazov_suveniru, 100, stdin);
    
    char* datum_predaja = malloc(sizeof(char) * 100);
    
    if (datum_predaja == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte dnesny datum: ");
    fgets(datum_predaja, 100, stdin);
    Datum *datum = datum_from_string(datum_predaja);
    
    predaj(ls, nazov_suveniru, datum);
    uloz(NAZOV_SUBORU, ls);
}

void vypis_vyplaty(list_suveniry* ls)
{
    char* string = malloc(sizeof(char) * 100);
    
    if (string == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte, v ktorom roku: ");
    fgets(string, 100, stdin);
    
    int rok = atoi(string);
    
    printf("Zadajte, v ktorom mesiaci: ");
    fgets(string, 100, stdin);
    
    int mesiac = atoi(string);
    
    char** umelci = zoznam_umelcov(ls);
    int* vyplaty = malloc(sizeof(int) * 100);
    
    if (vyplaty == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    vyplata(ls, umelci, vyplaty, mesiac, rok);
    
    int i = 0;
    
    for (i; i < ls->velkost; i++) 
    {
        if (umelci[i] == NULL) 
            break;
        
	printf("Meno umelca: %s\t Vyska vyplati: %d\n", umelci[i], vyplaty[i]);
   } 
    
    free(string);
    
    for (int j = 0; j < i; j++) 
        free(umelci[j]);   
    
    free(umelci);
    free(vyplaty);
}

void vypis_viacdodavatelske_suveniry(list_suveniry* ls)
{
    char** suveniry = viacdodavatelske_vyrobky(ls);
    int i = 0;
    
    while (suveniry[i] != NULL)
    {
        printf("%i. %s \n", i + 1, suveniry[i]);
        free(suveniry[i]);
        i++;
    }
    
    free(suveniry);
}

void vypis_co_vyrabat(list_suveniry* ls)
{
    char *umelec = malloc(sizeof(char) * 100);
    
    if (umelec == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte meno umelca: ");
    fgets(umelec, 100, stdin);
    
    trimwhitespace(umelec);
    char **vyrobky = co_vyrabat(ls, umelec);
    
    int i = 0;
    
    while (vyrobky[i] != NULL)
    {
        printf("%i. vyrobok: %s \n", i + 1, vyrobky[i]);
        free(vyrobky[i]);
        i++;
    }
    
    free(umelec);
    free(vyrobky);
}

void vypis_najrychlejsie_vyrobeny_suvenir(list_suveniry* ls)
{
    char* umelec = malloc(sizeof(char) * 100);
    
    if (umelec == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte meno umelca: ");
    fgets(umelec, 100, stdin);
    
    trimwhitespace(umelec);
    
    Suvenir *suvenir = najrychlejsie_vyrobeny_vyrobok(ls, umelec);
    
    char *suvenir_string = to_string_suvenir(suvenir);
    printf("Najrychlejsie vyrobeny suvenir je: %s", suvenir_string);
    
    free(umelec);
    free(suvenir);
    free(suvenir_string);
}

void vypis_kedy_na_dovolenku(list_suveniry* ls) 
{
    char* rok_string = malloc(sizeof(char) * 5);
    
    if (rok_string == NULL)
    {
        fprintf(stderr, "Nedostatok pamäte.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Zadajte rok: ");
    fgets(rok_string, 4, stdin);
    
    trimwhitespace(rok_string);
    int rok = atoi(rok_string);
    
    int den_od = kedy_na_dovolenku(ls, rok);
    
    int mesiac = den_od / 30, den = den_od % 30;
    mesiac = mesiac == 0 ? 1 : mesiac;
    
    printf("Najmensie trzby boli v obdobi od %i.%i do %i.%i \n", den, mesiac, den, mesiac + 1);
    
    printf("\n");
    
    free(rok_string);
    
}

void vypis_zoradenie_podla_datumu_predaja(list_suveniry* ls)
{
    zorad_podla_datumu_predaja(ls);
    vypis_suvenirov(ls);
}

void vypis_list(list_suveniry* list)
{
    if (list->velkost > 0)
    {
        char*** table = zoznam_do_tabulky(list);
        vypis_tabulku(table, hlavicka, list->velkost, tabulka_pocet_stlpcov);
        tabulka_free(table, list->velkost, tabulka_pocet_stlpcov);
    }
    else
        vypis_string_s_offsetom(VYSTUP_OFFSET, "Ziadne polozky k zobrazeniu.\n");
}

void vypis_tabulku(char*** tabulka, const char** hlavicka, int pocet_riadkov, int pocet_stlpcov)
{
    int* ohranicenia = vypocitaj_ohranicenia(tabulka, hlavicka, pocet_riadkov, pocet_stlpcov);

    vypis_okraj_tabulky(ohranicenia, pocet_stlpcov);
    vypis_riadok_tabulky((char**) hlavicka, ohranicenia, pocet_stlpcov);
    vypis_okraj_tabulky(ohranicenia, pocet_stlpcov);

    for (int i = 0; i < pocet_riadkov; i++)
        vypis_riadok_tabulky(tabulka[i], ohranicenia, pocet_stlpcov);

    vypis_okraj_tabulky(ohranicenia, pocet_stlpcov);
    free(ohranicenia);
}

void tabulka_free(char*** tabulka, int pocet_riadkov, int pocet_stlpcov)
{
    for (int i = 0; i < pocet_riadkov; i++)
    {
        for (int j = 0; j < pocet_stlpcov; j++) 
            free(tabulka[i][j]);
        
        free(tabulka[i]);
    }

    free(tabulka);
}

void vypis_string_s_offsetom(int lavy_offset, char* string)
{
    printf("%-*s", lavy_offset, "");
    printf("%s", string);
}

void vypis_okraj_tabulky(int* ohranicenia, int pocet_stlpcov)
{
    vypis_string_s_offsetom(VYSTUP_OFFSET, "");
    char* okraj = malloc(sizeof(char) * DLZKA_RIADKA_MAX);
    int idx = 0;

    okraj[idx++] = '+';

    for (int i = 0; i < pocet_stlpcov; i++)
    {
        for (int j = 0; j < ohranicenia[i] + LAVA_BUNKA_OFFSET; j++)
            okraj[idx++] = '-';

	okraj[idx++] = '+';
    }

    okraj[idx++] = '\n';
    okraj[idx] = '\0';
    printf(okraj);
    free(okraj);
}

void vypis_riadok_tabulky(char** polozky, int* ohranicenia, int pocet_stlpcov)
{
    vypis_string_s_offsetom(VYSTUP_OFFSET, "");
    printf("|");

    for (int i = 0; i < pocet_stlpcov; i++)
    {
        vypis_string_s_offsetom(LAVA_BUNKA_OFFSET, "");
        printf("%-*s|", ohranicenia[i], polozky[i]);
    }

    printf("\n");
}

int* vypocitaj_ohranicenia(char*** tabulka, const char* hlavicka[], int pocet_riadkov, int pocet_stlpcov)
{
    int* vysledok = malloc(sizeof(int) * pocet_stlpcov);

    for (int i = 0; i < pocet_stlpcov; i++)
        vysledok[i] = 0;

    for (int i = 0; i < pocet_riadkov; i++)
        for (int j = 0; j < pocet_stlpcov; j++)
            vysledok[j] = MAX(strlen(tabulka[i][j]), vysledok[j]);
    
    for (int i = 0; i < pocet_stlpcov; i++)
    {
        vysledok[i] = MAX(strlen(hlavicka[i]), vysledok[i]);
        vysledok[i] += LAVA_BUNKA_OFFSET;
    }

    return vysledok;
}