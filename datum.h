#ifndef DATUM_H
#define DATUM_H

typedef struct
{
    int den;
    int mesiac;
    int rok;
} Datum;

Datum* datum_from_string(char* s);
char* datum_to_string(Datum* datum);
int porovnaj_datumy(Datum* datum1, Datum* datum2);
long datumu_v_hodinach(Datum* datum) ;
long rozdiel_hodin_datumov(Datum* datum1, Datum* datum2);
Datum* nezadane();
        
#endif /* DATUM_H */

