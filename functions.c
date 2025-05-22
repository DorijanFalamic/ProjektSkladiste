#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "header.h"

#define DATOTEKA "skladiste.txt"

// Dodavanje proizvoda: dodaje novi red u datoteku
void dodajProizvod() {
	FILE* fp = fopen(DATOTEKA, "a");  // otvaramo za dodavanje na kraj
	if (!fp) {
		printf("Ne mogu otvoriti datoteku za pisanje.\n");
		return;
	}

	Proizvod novi;
	printf("Unesite ID: ");
	scanf("%d", &novi.id);
	printf("Unesite naziv: ");
	scanf(" %49[^\n]", novi.naziv);
	printf("Unesite kategoriju (voce/povrce): ");
	scanf(" %29[^\n]", novi.kategorija);
	printf("Unesite kolicinu (kg): ");
	scanf("%f", &novi.kolicina);
	printf("Unesite cijenu (po kg): ");
	scanf("%f", &novi.cijena);

	fprintf(fp, "%d,%s,%s,%.2f,%.2f\n", novi.id, novi.naziv, novi.kategorija, novi.kolicina, novi.cijena);
	fclose(fp);
	printf("Proizvod dodan.\n");
}

// Prikaz proizvoda: čita iz datoteke i ispisuje sve redove
void prikaziProizvode() {
	FILE* fp = fopen(DATOTEKA, "r");
	if (!fp) {
		printf("Datoteka ne postoji ili je prazna.\n");
		return;
	}

	Proizvod p;
	printf("\n--- Lista proizvoda ---\n");
	while (fscanf(fp, "%d,%49[^,],%29[^,],%f,%f\n", &p.id, p.naziv, p.kategorija, &p.kolicina, &p.cijena) == 5) {
		printf("ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);
	}
	fclose(fp);
}

// Pretraživanje proizvoda po nazivu
void pretraziProizvod() {
	char trazeni[MAX_NAZIV];
	printf("Unesite naziv za pretragu: ");
	scanf(" %49[^\n]", trazeni);

	FILE* fp = fopen(DATOTEKA, "r");
	if (!fp) {
		printf("Datoteka ne postoji.\n");
		return;
	}

	Proizvod p;
	int found = 0;
	while (fscanf(fp, "%d,%49[^,],%29[^,],%f,%f\n", &p.id, p.naziv, p.kategorija, &p.kolicina, &p.cijena) == 5) {
		if (strcmp(p.naziv, trazeni) == 0) {
			printf("Pronađen: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);
			found = 1;
			break;
		}
	}
	if (!found) printf("Proizvod nije pronaden.\n");
	fclose(fp);
}

// Brisanje proizvoda po ID-u: čita sve proizvode, preskače onaj s ID-om za brisanje i zapisuje ponovo
void obrisiProizvod() {
	int idZaBrisanje;
	printf("Unesite ID proizvoda za brisanje: ");
	scanf("%d", &idZaBrisanje);

	FILE* fp = fopen(DATOTEKA, "r");
	if (!fp) {
		printf("Datoteka ne postoji.\n");
		return;
	}

	FILE* temp = fopen("temp.txt", "w");
	if (!temp) {
		printf("Ne mogu kreirati privremenu datoteku.\n");
		fclose(fp);
		return;
	}

	Proizvod p;
	int found = 0;
	while (fscanf(fp, "%d,%49[^,],%29[^,],%f,%f\n", &p.id, p.naziv, p.kategorija, &p.kolicina, &p.cijena) == 5) {
		if (p.id == idZaBrisanje) {
			found = 1;  // preskače zapis za brisanje
			continue;
		}
		fprintf(temp, "%d,%s,%s,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);
	}

	fclose(fp);
	fclose(temp);

	if (!found) {
		printf("Proizvod s ID %d nije pronaden.\n", idZaBrisanje);
		remove("temp.txt");
		return;
	}

	remove(DATOTEKA);
	rename("temp.txt", DATOTEKA);
	printf("Proizvod obrisan.\n");
}
