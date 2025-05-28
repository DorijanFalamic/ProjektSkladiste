#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header.h"

int main() {
    int izbor;

    do {
        printf("\n--- Skladiste voca i povrca ---\n");
        printf("1. Dodaj novi proizvod\n");
        printf("2. Prikazi sve proizvode\n");
        printf("3. Pretrazi proizvod\n");
        printf("4. Obrisi proizvod\n");
        printf("5. Uredi proizvod\n");
        printf("6. Izlaz\n");

        printf("Odabir: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            dodajProizvod();
            break;
        case 2:
            prikaziProizvode();
            break;
        case 3:
            pretraziProizvod();
            break;
        case 4:
            obrisiProizvod();
            break;
        case 5: 
            UrediProizvod();
            break;
        case 6:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznata opcija. Pokusajte ponovno.\n");
        }
    } while (izbor != 6); 

    return 0;
}
