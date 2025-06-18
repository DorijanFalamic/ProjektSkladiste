#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"

#define DATOTEKA "skladiste.txt"

Skladiste skladiste = { NULL, 0 };

// Funkcija za ciscenje ekrana (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

const char* kategorijaUString(Kategorija k) {
    switch (k) {
    case VOCE:
        return "voce";
    case POVRCE:
        return "povrce";
    default:
        return "nepoznato";
    }
}

Kategorija stringUKategoriju(const char* s) {
    if (strcmp(s, "voce") == 0 || strcmp(s, "Voce") == 0)
        return VOCE;
    else if (strcmp(s, "povrce") == 0 || strcmp(s, "Povrce") == 0)
        return POVRCE;
    else
        return VOCE;
}

// Provjera postoji li vec uneseni ID
int postojiId(int id) {
    for (size_t i = 0; i < skladiste.brojProizvoda; i++) {
        if (skladiste.niz[i].id == id) {
            return 1;
        }
    }
    return 0;
}

void dodajProizvod() {
    if (skladiste.niz == NULL) {
        printf("Greska: Memorija za proizvode nije alocirana!\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    if (skladiste.brojProizvoda >= MAX_PROIZVODI) {
        printf("Dosegnut maksimalan broj proizvoda.\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    Proizvod novi;
    int noviId;
    do {
        printf("Unesite ID: ");
        scanf("%d", &noviId);
        if (postojiId(noviId)) {
            printf("Proizvod s tim ID-em vec postoji! Unesite drugi ID.\n");
        }
    } while (postojiId(noviId));
    novi.id = noviId;
    printf("Unesite naziv: ");
    scanf(" %49[^\n]", novi.naziv);
    char katStr[20];
    printf("Unesite kategoriju (voce/povrce): ");
    scanf(" %19[^\n]", katStr);
    novi.kategorija = stringUKategoriju(katStr);
    printf("Unesite kolicinu (kg): ");
    scanf("%f", &novi.kolicina);
    printf("Unesite cijenu (po kg): ");
    scanf("%f", &novi.cijena);

    skladiste.niz[skladiste.brojProizvoda++] = novi;
    sacuvajProizvode();
    printf("Proizvod dodan i spremljen.\n");
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void prikaziProizvode() {
    static int brojPoziva = 0;
    brojPoziva++;
    printf("Funkcija prikaziProizvode pozvana %d puta\n", brojPoziva);

    FILE* fp = fopen(DATOTEKA, "r");
    if (!fp) {
        printf("Datoteka ne postoji ili je prazna.\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    Proizvod p;
    printf("\n--- Lista proizvoda ---\n");
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {
        printf("ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, kategorijaUString(p.kategorija), p.kolicina, p.cijena);
    }
    fclose(fp);
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

// Podizbornik za pretragu
void pretraziProizvod() {
    int podizbor;
    do {
        clearScreen();
        printf("\n--- Podizbornik pretrage ---\n");
        printf("1. Pretraga po nazivu\n");
        printf("2. Pretraga po ID-u (bsearch)\n");
        printf("0. Povratak na glavni izbornik\n");
        printf("Odabir: ");
        scanf("%d", &podizbor);

        switch (podizbor) {
        case 1:
            pretraziProizvodPoNazivu();
            break;
        case 2:
            pretraziProizvodBsearch();
            break;
        case 0:
            printf("Povratak na glavni izbornik.\n");
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    } while (podizbor != 0);
}

void pretraziProizvodPoNazivu() {
    char trazeni[MAX_NAZIV];
    printf("Unesite naziv za pretragu: ");
    scanf(" %49[^\n]", trazeni);
    FILE* fp = fopen(DATOTEKA, "r");
    if (!fp) {
        printf("Datoteka ne postoji.\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    Proizvod p;
    int found = 0;
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {
        if (strcmp(p.naziv, trazeni) == 0) {
            printf("Pronaden: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, kategorijaUString(p.kategorija), p.kolicina, p.cijena);
            found = 1;
        }
    }
    if (!found) printf("Proizvod nije pronaden.\n");
    fclose(fp);
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

// bsearch primjer (koncept 24)
int usporediProizvodId(const void* a, const void* b) {
    return ((Proizvod*)a)->id - ((Proizvod*)b)->id;
}

void pretraziProizvodBsearch() {
    if (skladiste.brojProizvoda == 0) {
        printf("Nema proizvoda.\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    int trazeniId;
    printf("Unesite ID za pretragu: ");
    scanf("%d", &trazeniId);
    qsort(skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvodId);
    Proizvod kljuc;
    kljuc.id = trazeniId;
    Proizvod* pronadjen = bsearch(&kljuc, skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvodId);
    if (pronadjen) {
        printf("Pronaden: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", pronadjen->id, pronadjen->naziv, kategorijaUString(pronadjen->kategorija), pronadjen->kolicina, pronadjen->cijena);
    }
    else {
        printf("Proizvod nije pronaden.\n");
    }
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void obrisiProizvod() {
    int idZaBrisanje;
    printf("Unesite ID proizvoda za brisanje: ");
    scanf("%d", &idZaBrisanje);

    // Brisanje iz memorije
    int found = 0;
    for (size_t i = 0; i < skladiste.brojProizvoda; i++) {
        if (skladiste.niz[i].id == idZaBrisanje) {
            for (size_t j = i; j < skladiste.brojProizvoda - 1; j++) {
                skladiste.niz[j] = skladiste.niz[j + 1];
            }
            skladiste.brojProizvoda--;
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Proizvod s ID %d nije pronaden.\n", idZaBrisanje);
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    sacuvajProizvode();
    printf("Proizvod obrisan.\n");
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void urediProizvod() {
    int idZaUrediti;
    printf("Unesite ID proizvoda za uredivanje: ");
    scanf("%d", &idZaUrediti);
    int found = 0;
    for (size_t i = 0; i < skladiste.brojProizvoda; i++) {
        if (skladiste.niz[i].id == idZaUrediti) {
            char kategorijaStr[30];
            printf("Unesite nove podatke za proizvod:\n");
            printf("Naziv: ");
            scanf(" %49[^\n]", skladiste.niz[i].naziv);
            printf("Kategorija (voce/povrce): ");
            scanf(" %29[^\n]", kategorijaStr);
            skladiste.niz[i].kategorija = stringUKategoriju(kategorijaStr);
            printf("Kolicina: ");
            scanf("%f", &skladiste.niz[i].kolicina);
            printf("Cijena: ");
            scanf("%f", &skladiste.niz[i].cijena);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Proizvod s ID %d nije pronaden.\n", idZaUrediti);
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    sacuvajProizvode();
    printf("Proizvod uspjesno azuriran.\n");
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void ucitajProizvode() {
    skladiste.niz = (Proizvod*)malloc(sizeof(Proizvod) * MAX_PROIZVODI);
    skladiste.brojProizvoda = 0;
    FILE* fp = fopen(DATOTEKA, "r");
    if (!fp) {
        printf("Datoteka ne postoji, nastavljamo s praznim skladistem.\n");
        return;
    }
    while (skladiste.brojProizvoda < MAX_PROIZVODI) {
        Proizvod p;
        int kategorijaInt;
        int rezultat = fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, &kategorijaInt, &p.kolicina, &p.cijena);
        if (rezultat != 5) {
            break;
        }
        p.kategorija = (Kategorija)kategorijaInt;
        skladiste.niz[skladiste.brojProizvoda++] = p;
    }
    fclose(fp);
}

// Rekurzivna funkcija za spremanje proizvoda (koncept 25)
void sacuvajProizvodeRekurzivno(FILE* fp, size_t index) {
    if (index >= skladiste.brojProizvoda) {
        return;
    }
    Proizvod p = skladiste.niz[index];
    fprintf(fp, "%d,%s,%d,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);
    sacuvajProizvodeRekurzivno(fp, index + 1);
}

void sacuvajProizvode() {
    FILE* fp = fopen(DATOTEKA, "w");
    if (!fp) {
        perror("Greska pri upisu u datoteku");
        return;
    }
    sacuvajProizvodeRekurzivno(fp, 0);
    fclose(fp);
}

int usporediProizvode(const void* a, const void* b) {
    const Proizvod* pa = (const Proizvod*)a;
    const Proizvod* pb = (const Proizvod*)b;
    return strcmp(pa->naziv, pb->naziv);
}

void sortirajProizvode() {
    if (skladiste.brojProizvoda > 1) {
        qsort(skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvode);
        printf("Proizvodi sortirani po nazivu.\n");
    }
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

// Funkcija s fseek/ftell (koncept 20)
void prikaziVelicinuDatoteke() {
    FILE* fp = fopen(DATOTEKA, "r");
    if (!fp) {
        printf("Datoteka ne postoji.\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    fseek(fp, 0, SEEK_END);
    long velicina = ftell(fp);
    fclose(fp);
    printf("Velicina datoteke: %ld bajtova\n", velicina);
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}
