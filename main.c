#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header.h"




int main() {
    ucitajProizvode(); // Učitavanje u memoriju

    int izbor;
    do {
        printf("\n--- Skladiste voca i povrca ---\n");
        printf("1. Dodaj novi proizvod\n");
        printf("2. Prikazi sve proizvode\n");
        printf("3. Pretrazi proizvod\n");
        printf("4. Obrisi proizvod\n");
        printf("5. Uredi proizvod\n");
        printf("6. Sortiraj proizvode\n");
        printf("7. Izlaz\n");

        printf("Odabir: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1: dodajProizvod(); break;
        case 2: prikaziProizvode(); break;
        case 3: pretraziProizvod(); break;
        case 4: obrisiProizvod(); break;
        case 5: urediProizvod(); break;
        case 6:
            sortirajProizvode();
            sacuvajProizvode();  // Spremi sortirane podatke u datoteku
            printf("Sortirano i spremljeno u datoteku.\n");
            break;
        case 7: printf("Izlaz iz programa.\n"); break;
        default: printf("Nepoznata opcija.\n");
        }

    } while (izbor != 7);

    sacuvajProizvode(); // spremi na kraju
    free(skladiste.niz); // oslobodi memoriju - stavka 18
    return 0;
}
