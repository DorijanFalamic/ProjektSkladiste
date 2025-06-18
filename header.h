#define _CRT_SECURE_NO_WARNINGS

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 50
#define MAX_KATEGORIJA 50
#define MAX_PROIZVODI 100

// Makro funkcija (koncept 9)
#define MAKS(a, b) ((a) > (b) ? (a) : (b))

// Inline funkcija (koncept 9)
static inline float minFloat(float a, float b) { return a < b ? a : b; }

// Enum za kategoriju proizvoda
typedef enum {
    VOCE,
    POVRCE
} Kategorija;

// Enum za izbornik (koncept 11)
typedef enum {
    DODAJ = 1,
    PRIKAZI,
    PRETRAZI,
    OBRISI,
    UREDI,
    SORTIRAJ,
    VELICINA,
    IZLAZ
} IzbornikOpcije;

typedef struct {
    int id;
    char naziv[MAX_NAZIV];
    Kategorija kategorija;
    float kolicina;
    float cijena;
} Proizvod;

typedef struct {
    Proizvod* niz;
    size_t brojProizvoda;
} Skladiste;

extern Skladiste skladiste;

// Deklaracije funkcija
void dodajProizvod();
void prikaziProizvode();
void pretraziProizvod();
void pretraziProizvodPoNazivu();
void pretraziProizvodBsearch();
void obrisiProizvod();
void urediProizvod();
void ucitajProizvode();
void sacuvajProizvode();
void sortirajProizvode();
void prikaziVelicinuDatoteke();
int usporediProizvodId(const void* a, const void* b);
void clearScreen();
int postojiId(int id);

#endif
