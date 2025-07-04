#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "header.h"

int main() {
    ucitajProizvode();
    int izbor;
    do {
        clearScreen();
        printf("--- Skladiste voca i povrca ---\n");
        printf("1. Dodaj novi proizvod\n");
        printf("2. Prikazi sve proizvode\n");
        printf("3. Pretrazi proizvod\n");
        printf("4. Obrisi proizvod\n");
        printf("5. Uredi proizvod\n");
        printf("6. Sortiraj proizvode po nazivu\n");
        printf("7. Prikazi velicinu datoteke\n");
        printf("8. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &izbor);

        switch ((IzbornikOpcije)izbor) {
        case DODAJ:
            dodajProizvod();
            break;
        case PRIKAZI:
            prikaziProizvode();
            break;
        case PRETRAZI:
            pretraziProizvod();
            break;
        case OBRISI:
            obrisiProizvod();
            break;
        case UREDI:
            urediProizvod();
            break;
        case SORTIRAJ:
            sortirajProizvode();
            sacuvajProizvode();
            printf("Pritisni Enter za nastavak...");
            getchar();
            getchar();
            break;
        case VELICINA:
            prikaziVelicinuDatoteke();
            break;
        case IZLAZ:
            printf("Izlaz iz programa.\n");
            break;
        default: printf("Nepoznata opcija.\n");
            printf("Pritisni Enter za nastavak...");
            getchar();
            getchar();
        }
    } while (izbor != IZLAZ); // izvrsava se sve dok je razlicito od IZLAZ , zato sto kad popriki tu vrijednoost onda sacuva proizvode pomocu funkcije,oslobada memoriju
    sacuvajProizvode();//to je funkcija za sacuvanje
    free(skladiste.niz);//za oslobadanje ako alocirana memorija nije osoblodjena i nije potrebna vise
    skladiste.niz = NULL;  // postavlja vrijednost null da ne bi doslo do problema gdje pokazivac i dalje pokazuje na prethodno oslobodenu memoriju
    skladiste.brojProizvoda = 0; //stavlja brojPro. na 0, koristno za ponovno pokretanje
    return 0;
}
