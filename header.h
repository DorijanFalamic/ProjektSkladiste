#define _CRT_SECURE_NO_WARNINGS
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 50
#define MAX_KATEGORIJA 50
#define MAX_PROIZVODI 100

typedef enum { VOCE, POVRCE } Kategorija; // stavka 11 sa obaveznih koncepta

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

extern Skladiste skladiste; // stavka 8 sa obaveznih koncepta

// Funkcije
void dodajProizvod();
void prikaziProizvode();
void pretraziProizvod();
void obrisiProizvod();
void urediProizvod();
void ucitajProizvode();
void sacuvajProizvode();
void sortirajProizvode();
void* binarnaPretraga(const void* kljuc, const void* element);

#endif
