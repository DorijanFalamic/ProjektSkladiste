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
	FILE* fp = fopen(DATOTEKA, "r");       // Otvara datoteku sa imenom definisanim u DATOTEKA u režimu čitanja
	if (!fp) {  // Provjerava da li je datoteka uspešno otvorena , Kada se koristi !fp, to znači "NIJE fp", što se može interpretirati kao "da li je fp NULL?"
		printf("Datoteka ne postoji ili je prazna.\n");             // Ispisuje poruku o grešci ako datoteka nije otvorena
		return;            // Izlazi iz funkcije ako datoteka nije otvorena
	}

	Proizvod p;
	printf("\n--- Lista proizvoda ---\n");
	while (fscanf(fp, "%d,%49[^,],%29[^,],%f,%f\n", &p.id, p.naziv, p.kategorija, &p.kolicina, &p.cijena) == 5) {
		// Format za fscanf: očekuje ID (int), naziv (string do 49 karaktera), kategoriju (string do 29 karaktera), količinu (float) i cenu (float)
		printf("ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);// Ispisuje podatke o proizvodu u formatiranom obliku
	}                                                                 
	fclose(fp);   // Zatvara datoteku nakon što su svi podaci pročitani
}

// Pretraživanje proizvoda po nazivu
void pretraziProizvod() { // Definicija funkcije koja pretražuje proizvod po nazivu
	char trazeni[MAX_NAZIV];      // Deklaracija niza karaktera za čuvanje naziva proizvoda koji se pretražuje,najvise se moze 50 zato sto je tako deklarirano u headeru
	printf("Unesite naziv za pretragu: ");         // Ispisuje poruku korisniku da unese naziv proizvoda za pretragu
	scanf(" %49[^\n]", trazeni);            // Čita korisnikov unos do 49 karaktera (ne uključujući novi red) i smešta ga u niz trazeni
	FILE* fp = fopen(DATOTEKA, "r");          // Otvara datoteku sa imenom definiranim u DATOTEKA u režimu čitanja
	if (!fp) {					// Provjerava da li je datoteka uspešno otvorena
		printf("Datoteka ne postoji.\n"); 
		return;					// Izlazi iz funkcije ako datoteka nije otvorena
	}
	Proizvod p;        //p tip Proizvod koja će čuvati podatke o jednom proizvodu
	int found = 0;					// Inicijalizuje promejenjivu found na 0, koja će se koristiti za praćenje da li je proizvod pronađen
	while (fscanf(fp, "%d,%49[^,],%29[^,],%f,%f\n", &p.id, p.naziv, p.kategorija, &p.kolicina, &p.cijena) == 5) { // Čita podatke iz datoteke i smešta ih u p
		// Format za fscanf: očekuje ID (int), naziv (string do 49 karaktera), kategoriju (string do 29 karaktera), količinu (float) i cenu (float)
		if (strcmp(p.naziv, trazeni) == 0) { //Ako je uslov istinit (tj. ako su nazivi proizvoda jednaki), izvršava se kod unutar if bloka, koji ispisuje informacije o pronađenom proizvodu.  
			printf("Pronađen: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena); // Ispisuje podatke o pronađenom proizvodu
			found = 1;					// Postavlja found na 1 nakon ifa oznacavajuci da je proizvod pronađen
			break;			// Izlazi iz petlje jer je proizvod pronađen
		}
	}
	if (!found) printf("Proizvod nije pronaden.\n");					// Ako proizvod nije pronađen, ispisuje poruku o tome
	fclose(fp);									// Zatvara datoteku nakon što su svi podaci pročitani
}

// Brisanje proizvoda po ID-u: čita sve proizvode, preskače onaj s ID-om za brisanje i zapisuje ponovo
void obrisiProizvod() {				
	int idZaBrisanje;// Deklaracija promenljive za čuvanje ID-a proizvoda koji se briše
	printf("Unesite ID proizvoda za brisanje: "); // Ispisuje poruku korisniku da unese ID proizvoda za brisanje
	scanf("%d", &idZaBrisanje); // Čita korisnikov unos ID-a proizvoda
	FILE* fp = fopen(DATOTEKA, "r"); // Otvara datoteku za citanje
	if (!fp) {						// Proverava da li je datoteka uspešno otvorena
		printf("Datoteka ne postoji.\n");				// Ispisuje poruku o grešci ako datoteka nije otvorena
		return;					// Izlazi iz funkcije ako datoteka nije otvorena
	}
	FILE* temp = fopen("temp.txt", "w"); // Otvara privremenu datoteku "temp.txt" u režimu pisanja
	if (!temp) { 
		printf("Ne mogu kreirati privremenu datoteku.\n");					// Ispisuje poruku o grešci zbog neuspijelog kreiranja datoteke
		fclose(fp);							// Zatvara originalnu datoteku
		return;						// Izlazi iz funkcije ako privremena datoteka nije otvorena
	}
	Proizvod p; 
	int found = 0; // Inicijalizuje promenljivu found na 0, koja će se koristiti za praćenje da li je proizvod pronađen
	while (fscanf(fp, "%d,%49[^,],%29[^,],%f,%f\n", &p.id, p.naziv, p.kategorija, &p.kolicina, &p.cijena) == 5) { // Čita podatke iz datoteke i smešta ih u p (ovaj kod proizvoda kako bih cuvao sve u jednom)
												//ID (int), naziv , kategoriju , količinu  i cenu  
		if (p.id == idZaBrisanje) { // Proverava da li je ID proizvoda jednak ID-u koji korisnik želi da obriše
			found = 1;  // Postavlja found na 1, označavajući da je proizvod pronađen
			continue; // Preskoči trenutni zapis (ne upisuje ga u privremenu datoteku)
		}
		fprintf(temp, "%d,%s,%s,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena); // Upisuje podatke o proizvodu u privremenu datoteku
	}
	fclose(fp); // Zatvara originalnu datoteku
	fclose(temp); // Zatvara privremenu datoteku
	if (!found) { // Proverava da li proizvod nije pronađen
		printf("Proizvod s ID %d nije pronaden.\n", idZaBrisanje); // Ispisuje poruku o tome da proizvod nije pronađen
		remove("temp.txt"); // Briše privremenu datoteku jer nije bilo brisanja
		return; // Izlazi iz funkcije
	}
	remove(DATOTEKA); // Briše originalnu datoteku
