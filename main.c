#define _CRT_SECURE_NO_WARNINGS // Onemogućuje sigurnosna upozorenja
#include <stdio.h> // Učitava standardnu ulazno-izlaznu biblioteku
#include "header.h" // Učitava vlastiti header sa strukturama i prototipovima funkcija

int main() {
    ucitajProizvode(); // Učitava proizvode iz datoteke u memoriju prije pokretanja programa

    int izbor; // Varijabla za spremanje korisničkog izbora u meniju
    do {
        // Ispis glavnog menija
        printf("\n--- Skladiste voca i povrca ---\n");
        printf("1. Dodaj novi proizvod\n");
        printf("2. Prikazi sve proizvode\n");
        printf("3. Pretrazi proizvod\n");
        printf("4. Obrisi proizvod\n");
        printf("5. Uredi proizvod\n");
        printf("6. Sortiraj proizvode\n");
        printf("7. Izlaz\n");

        printf("Odabir: "); // Traži unos opcije
        scanf("%d", &izbor); // Čita odabir korisnika

        switch (izbor) { // Odabir funkcije na osnovu korisnikovog unosa
        case 1: dodajProizvod(); break; // Dodaj novi proizvod
        case 2: prikaziProizvode(); break; // Prikaži sve proizvode
        case 3: pretraziProizvod(); break; // Pretraži proizvod po nazivu
        case 4: obrisiProizvod(); break; // Obriši proizvod po ID-u
        case 5: urediProizvod(); break; // Uredi postojeći proizvod
        case 6:
            sortirajProizvode(); // Sortiraj proizvode po nazivu
            sacuvajProizvode();  // Spremi sortirane proizvode u datoteku
            printf("Sortirano i spremljeno u datoteku.\n"); // Obavijest korisniku
            break;
        case 7: printf("Izlaz iz programa.\n"); break; // Izlaz iz programa
        default: printf("Nepoznata opcija.\n"); // Poruka o nepoznatoj opciji
        }

    } while (izbor != 7); // Petlja se vrti dok korisnik ne izabere izlaz (7) 

    sacuvajProizvode(); // Sprema proizvode u datoteku prije izlaska iz programa
    free(skladiste.niz); // Oslobađa memoriju alociranu za proizvode
    return 0; // Završava program sa statusom 0
}
