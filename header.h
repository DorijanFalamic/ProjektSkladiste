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
#define MAKS(a, b) ((a) > (b) ? (a) : (b)) //definiramo makro kako bi nam sluzio za pronalazenje maks vrijednosti nekog broja

// Inline funkcija (koncept 9)
static inline float minFloat(float a, float b) { return a < b ? a : b; } // inline zato sto saljemo direktno u fuknciju umjedsto na standardni poziv
                                                   //ako je a manje od b, funkcija vraća a. a u suprotno, vraća b.
// Enum za kategoriju proizvoda
typedef enum {
    VOCE,
    POVRCE
} Kategorija;

// Enum za izbornik (koncept 11)
typedef enum {// enum znaci da se radi o numeraciji i sve unutar se odnosi na ovo izvan[IzbornikOpcije]
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

extern Skladiste skladiste;  // Mozemo rec da varijabla extern govori programu da varijabla postoji negdje drugdje 

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
