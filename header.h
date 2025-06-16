#define _CRT_SECURE_NO_WARNINGS // Isključivanje upozorenja vezanih za sigurnost CRT funkcija
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h> // Standardni ulaz/izlaz
#include <stdlib.h> // Standardna biblioteka, koristi se za malloc i free
#include <string.h> // Za rad sa stringovima

#define MAX_NAZIV 50 // Maksimalna duljina naziva proizvoda
#define MAX_KATEGORIJA 50 // Maksimalna duljina naziva kategorije (trenutno nedefinirano u funkcijama)
#define MAX_PROIZVODI 100 // Maksimalan broj proizvoda u skladištu

typedef enum { VOCE, POVRCE } Kategorija; // Definicija tipizirane varijable za kategorije stavka 11 sa obaveznih koncepata

typedef struct { // Struktura za proizvod
    int id; // Jedinstveni identifikator proizvoda
    char naziv[MAX_NAZIV]; // Naziv proizvoda
    Kategorija kategorija; // Kategorija proizvoda (voće ili povrće)
    float kolicina; // Količina proizvoda u kilogramima
    float cijena; // Cijena po kilogramu
} Proizvod;

typedef struct { // Struktura skladišta
    Proizvod* niz; // Pokazivač na niz proizvoda u memoriji
    size_t brojProizvoda; // Broj trenutno pohranjenih proizvoda
} Skladiste;

extern Skladiste skladiste; // Deklaracija globalne varijable skladiste za korištenje u drugim datotekama

// Deklaracije funkcija koje se koriste u programu
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
