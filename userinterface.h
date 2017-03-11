#ifndef USERINTERFACE_H
#define USERINTERFACE_H

void zobraz_banner();
void zobraz_moznosti();
void spusti_program();
int vyziadaj_vstup();
void vyber_moznost(int cislo_moznosti, list_suveniry* list);
void vypis_suvenirov(list_suveniry* ls);
void nacitaj_a_pridaj_suvenir(list_suveniry* ls);
void vypis_umelcov(list_suveniry* ls);
void vypis_najlacnejsi_v_predaji(list_suveniry* ls);
void vypis_podla_kategorie(list_suveniry* ls);
void vypis_inventuru(list_suveniry* ls);
void vypis_predaj(list_suveniry* ls);
void vypis_vyplaty(list_suveniry* ls);
void vypis_viacdodavatelske_suveniry(list_suveniry* ls);
void vypis_co_vyrabat(list_suveniry* ls);
void vypis_najrychlejsie_vyrobeny_suvenir(list_suveniry* ls);
void vypis_kedy_na_dovolenku(list_suveniry* ls);
void vypis_zoradenie_podla_datumu_predaja(list_suveniry* ls);

void vypis_list(list_suveniry* list);
void vypis_tabulku(char*** tabulka, const char** hlavicka, int pocet_riadkov, int pocet_stlpcov);
void tabulka_free(char*** tabulka, int pocet_riadkov, int pocet_stlpcov);
void vypis_string_s_offsetom(int lavy_offset, char* string);
void vypis_okraj_tabulky(int* ohranicenia, int pocet_stlpcov);
void vypis_riadok_tabulky(char** polozky, int* ohranicenia, int pocet_stlpcov);
int* vypocitaj_ohranicenia(char*** tabulka, const char** hlavicka, int pocet_riadkov, int pocet_stlpcov);

#endif /* USERINTERFACE_H */

