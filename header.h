#define _CRT_SECURE_NO_WARNINGS
#ifndef HEADER_H
#define HEADER_H

#define MAX_NAZIV 50
#define MAX_KATEGORIJA 30

typedef struct {
	int id;
	char naziv[MAX_NAZIV];
	char kategorija[MAX_KATEGORIJA];
	float kolicina;
	float cijena;
} Proizvod;

// Funkcije
void dodajProizvod();
void prikaziProizvode();
void pretraziProizvod();
void obrisiProizvod();

#endif
