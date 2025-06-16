#define _CRT_SECURE_NO_WARNINGS // Onemogućuje upozorenja za korištenje nesigurnih funkcija poput scanf
#include <stdio.h> // Učitava standardnu ulazno-izlaznu biblioteku
#include <string.h> // Učitava funkcije za rad sa stringovima
#include "header.h" // Učitava vlastiti zaglavni fajl sa definicijama struktura i prototipovima funkcija

#define DATOTEKA "skladiste.txt" // Definira naziv datoteke za pohranu podataka o proizvodima

Skladiste skladiste = { NULL, 0 }; // Globalna varijabla koja drži niz proizvoda i njihov broj; inicijalno NULL i 0

// Pomoćna funkcija koja prevodi enum Kategorija u string, zbog krive inizijalizacije izbacivalo mi je exception thrown
const char* kategorijaUString(Kategorija k) {
    switch (k) {
    case VOCE: return "voce"; // Ako je kategorija VOCE, vraća string "voce"
    case POVRCE: return "povrce"; // Ako je kategorija POVRCE, vraća string "povrce"
    default: return "nepoznato"; // U ostalim slučajevima vraća "nepoznato"
    }
}

// Pomoćna funkcija koja prevodi string u enum Kategorija, zbog krive inizijalizacije izbacivalo mi je exception thrown
Kategorija stringUKategoriju(const char* s) {
    if (strcmp(s, "voce") == 0 || strcmp(s, "Voce") == 0)
        return VOCE; // Ako je string "voce" (ili s velikim V), vraća VOCE
    else if (strcmp(s, "povrce") == 0 || strcmp(s, "Povrce") == 0)
        return POVRCE; // Ako je string "povrce" (ili s velikim P), vraća POVRCE
    else
        return VOCE; // Zadana kategorija u slučaju neprepoznatog stringa je VOCE
}

// Funkcija za dodavanje novog proizvoda u skladište
void dodajProizvod() {
    if (skladiste.niz == NULL) { // Provjerava je li memorija za proizvode alocirana
        printf("Greska: Memorija za proizvode nije alocirana!\n"); // Ispisuje poruku o grešci
        return; // Izlazi iz funkcije
    }

    if (skladiste.brojProizvoda >= MAX_PROIZVODI) { // Provjerava je li dosegnut maksimalni broj proizvoda
        printf("Dosegnut maksimalan broj proizvoda.\n"); // Ispisuje poruku upozorenja
        return; // Izlazi iz funkcije
    }

    Proizvod novi; // Deklarira novu varijablu za proizvod
    printf("Unesite ID: "); // Traži od korisnika unos ID-a proizvoda
    scanf("%d", &novi.id); // Čita uneseni ID sa standardnog ulaza

    printf("Unesite naziv: "); // Traži unos naziva proizvoda
    scanf(" %49[^\n]", novi.naziv); // Čita naziv (do 49 znakova, uključujući praznine)

    char katStr[20]; // Deklarira string za unos kategorije
    printf("Unesite kategoriju (voce/povrce): "); // Traži unos kategorije proizvoda
    scanf(" %19[^\n]", katStr); // Čita unesenu kategoriju
    if (strcmp(katStr, "voce") == 0) // Ako je uneseno "voce"
        novi.kategorija = VOCE; // Postavlja kategoriju na VOCE
    else
        novi.kategorija = POVRCE; // Inače postavlja na POVRCE

    printf("Unesite kolicinu (kg): "); // Traži unos količine u kg
    scanf("%f", &novi.kolicina); // Čita količinu

    printf("Unesite cijenu (po kg): "); // Traži unos cijene po kg
    scanf("%f", &novi.cijena); // Čita cijenu

    // Dodaje novog proizvoda u niz u memoriji
    skladiste.niz[skladiste.brojProizvoda++] = novi; // Spremi proizvod i povećaj broj proizvoda

    // Sprema sve proizvode iz memorije u datoteku
    sacuvajProizvode();

    printf("Proizvod dodan i spremljen.\n"); // Potvrda korisniku
}

// Funkcija za prikaz svih proizvoda iz datoteke
void prikaziProizvode() {
    FILE* fp = fopen(DATOTEKA, "r"); // Otvara datoteku za čitanje
    if (!fp) { // Ako datoteka ne postoji ili nije moguće otvoriti
        printf("Datoteka ne postoji ili je prazna.\n"); // Ispisuje poruku
        return; // Izlazi iz funkcije
    }

    Proizvod p; // Deklarira varijablu za učitavanje proizvoda
    printf("\n--- Lista proizvoda ---\n"); // Ispis naslova liste
    // Čita red po red iz datoteke dok postoji zapis i ispisuje podatke o proizvodu
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {
        // Ispis proizvoda formatiran
        printf("ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, kategorijaUString(p.kategorija), p.kolicina, p.cijena);
    }
    fclose(fp); // Zatvara datoteku
}

// Funkcija za pretraživanje proizvoda prema nazivu
void pretraziProizvod() {
    char trazeni[MAX_NAZIV]; // Mjesto za unos traženog naziva
    printf("Unesite naziv za pretragu: "); // Ispis upita za unos naziva
    scanf(" %49[^\n]", trazeni); // Čita naziv za pretragu

    FILE* fp = fopen(DATOTEKA, "r"); // Otvara datoteku za čitanje
    if (!fp) { // Provjera uspješnog otvaranja
        printf("Datoteka ne postoji.\n"); // Poruka o nepostojanju datoteke
        return; // Izlazak iz funkcije
    }

    Proizvod p; // Deklaracija varijable za proizvod
    int found = 0; // Zastavica za pronalazak proizvoda
    // Čita po jedan zapis iz datoteke dok postoji
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {
        // Ako naziv proizvoda odgovara traženom nazivu
        if (strcmp(p.naziv, trazeni) == 0) {
            // Ispis pronađenog proizvoda
            printf("Pronaden: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, kategorijaUString(p.kategorija), p.kolicina, p.cijena);
            found = 1; // Postavlja zastavicu na 1
            break; // Izlazi iz petlje nakon pronalaska
        }
    }
    if (!found) printf("Proizvod nije pronaden.\n"); // Ako nema pronalaska, ispisuje poruku
    fclose(fp); // Zatvara datoteku
}

// Funkcija za brisanje proizvoda prema ID-u
void obrisiProizvod() {
    int idZaBrisanje; // Varijabla za ID proizvoda koji treba obrisati
    printf("Unesite ID proizvoda za brisanje: "); // Traži unos ID-a za brisanje
    scanf("%d", &idZaBrisanje); // Čita uneseni ID

    FILE* fp = fopen(DATOTEKA, "r"); // Otvara originalnu datoteku za čitanje
    if (!fp) { // Provjerava je li otvorena uspješno
        printf("Datoteka ne postoji.\n"); // Poruka o grešci
        return; // Izlazi iz funkcije
    }

    FILE* temp = fopen("temp.txt", "w"); // Otvara privremenu datoteku za pisanje
    if (!temp) { // Provjera uspješnosti otvaranja
        printf("Ne mogu kreirati privremenu datoteku.\n"); // Poruka o grešci
        fclose(fp); // Zatvara originalnu datoteku
        return; // Izlazi iz funkcije
    }

    Proizvod p; // Varijabla za učitavanje proizvoda
    int found = 0; // Zastavica ako je proizvod pronađen
    // Petlja za čitanje i prepisivanje svih proizvoda osim onog za brisanje
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {
        if (p.id == idZaBrisanje) { // Ako je ovo proizvod za brisanje
            found = 1; // Postavlja zastavicu da je pronađeno
            continue; // Preskače zapis i ne upisuje u temp datoteku
        }
        // Inače prepisuje zapis u privremenu datoteku
        fprintf(temp, "%d,%s,%d,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);
    }

    fclose(fp); // Zatvara originalnu datoteku
    fclose(temp); // Zatvara privremenu datoteku

    if (!found) { // Ako proizvod nije pronađen
        printf("Proizvod s ID %d nije pronaden.\n", idZaBrisanje); // Ispisuje poruku
        remove("temp.txt"); // Briše privremenu datoteku jer nema promjene
        return; // Izlazi iz funkcije
    }

    remove(DATOTEKA); // Briše originalnu datoteku
    rename("temp.txt", DATOTEKA); // Preimenuje privremenu u originalnu
    printf("Proizvod obrisan.\n"); // Potvrda brisanja
}

// Funkcija za uređivanje proizvoda po ID-u
void urediProizvod() {
    int idZaUrediti; // ID proizvoda za uređivanje
    printf("Unesite ID proizvoda za uredivanje: "); // Zahtjeva unos ID-a
    scanf("%d", &idZaUrediti); // Čita ID

    FILE* fp = fopen(DATOTEKA, "r"); // Otvara originalnu datoteku za čitanje
    if (!fp) { // Provjerava uspješnost otvaranja
        printf("Datoteka ne postoji.\n"); // Poruka o grešci
        return; // Izlazi iz funkcije
    }

    FILE* temp = fopen("temp.txt", "w"); // Otvara privremenu datoteku za pisanje
    if (!temp) { // Provjerava uspješnost otvaranja
        printf("Ne mogu kreirati privremenu datoteku.\n"); // Poruka o grešci
        fclose(fp); // Zatvara originalnu datoteku
        return; // Izlazi iz funkcije
    }

    Proizvod p; // Varijabla za proizvod
    int found = 0; // Zastavica ako je proizvod pronađen
    // Petlja za čitanje proizvoda i uređivanje onog sa traženim ID
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {
        if (p.id == idZaUrediti) { // Ako je proizvod za uređivanje
            found = 1; // Postavlja zastavicu
            printf("Unesite nove podatke za proizvod:\n"); // Traži unos novih podataka

            char kategorijaStr[30]; // Privremeni string za unos kategorije

            printf("Naziv: "); // Zahtjeva novi naziv
            scanf(" %49[^\n]", p.naziv); // Čita novi naziv

            printf("Kategorija (voce/povrce): "); // Zahtjeva novu kategoriju
            scanf(" %29[^\n]", kategorijaStr); // Čita kategoriju
            p.kategorija = stringUKategoriju(kategorijaStr); // Pretvara string u enum

            printf("Kolicina: "); // Zahtjeva novu količinu
            scanf("%f", &p.kolicina); // Čita količinu

            printf("Cena: "); // Zahtjeva novu cijenu
            scanf("%f", &p.cijena); // Čita cijenu
        }
        // Upisuje proizvod (novi ili nepromijenjeni) u privremenu datoteku
        fprintf(temp, "%d,%s,%d,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);
    }

    fclose(fp); // Zatvara originalnu datoteku
    fclose(temp); // Zatvara privremenu datoteku

    if (!found) { // Ako proizvod nije pronađen
        printf("Proizvod s ID %d nije pronden.\n", idZaUrediti); // Ispisuje poruku
        remove("temp.txt"); // Briše privremenu datoteku
        return; // Izlazi iz funkcije
    }

    remove(DATOTEKA); // Briše originalnu datoteku
    rename("temp.txt", DATOTEKA); // Preimenuje privremenu u originalnu
    printf("Proizvod uspjesno azuriran.\n"); // Potvrda uređivanja
}

// Učitava proizvode iz datoteke u memoriju
void ucitajProizvode() {
    // Alocira memoriju za maksimalan broj proizvoda
    skladiste.niz = (Proizvod*)malloc(sizeof(Proizvod) * MAX_PROIZVODI);
    skladiste.brojProizvoda = 0; // Resetira brojač

    FILE* fp = fopen(DATOTEKA, "r"); // Otvara datoteku za čitanje
    if (!fp) { // Ako datoteka ne postoji
        printf("Datoteka ne postoji, nastavljamo s praznim skladistem.\n"); // Obavještava korisnika
        return; // Izlazi jer nema podataka za učitati
    }

    // Čita datoteku red po red dok ne dosegne kraj ili maksimalan broj proizvoda
    while (skladiste.brojProizvoda < MAX_PROIZVODI) {
        Proizvod p; // Privremena varijabla za proizvod
        int kategorijaInt; // Integer za privremeni učitak kategorije
        int rezultat = fscanf(fp, "%d,%49[^,],%d,%f,%f\n",
            &p.id, p.naziv, &kategorijaInt, &p.kolicina, &p.cijena); // Čita jedan zapis iz datoteke
        if (rezultat != 5) // Ako nije uspješno pročitao svih 5 polja
            break; // Izlazi iz petlje

        p.kategorija = (Kategorija)kategorijaInt; // Pretvara int u enum
        // Sprema proizvod u niz i povećava broj proizvoda
        skladiste.niz[skladiste.brojProizvoda++] = p;
    }

    fclose(fp); // Zatvara datoteku
}

// Pomoćna rekurzivna funkcija za čuvanje proizvoda
void sacuvajProizvodeRekurzivno(FILE* fp, size_t index) {
    if (index >= skladiste.brojProizvoda) {
        return; // Osnovni slučaj: ako je indeks veći ili jednak broju proizvoda, izlazimo
    }

    Proizvod p = skladiste.niz[index]; // Dohvaća proizvod iz niza
    fprintf(fp, "%d,%s,%d,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena); // Ispisuje u datoteku

    // Rekurzivni poziv za sledeći proizvod
    sacuvajProizvodeRekurzivno(fp, index + 1);
}

// Funkcija koja otvara datoteku i pokreće rekurzivno čuvanje proizvoda
void sacuvajProizvode() {
    FILE* fp = fopen(DATOTEKA, "w"); // Otvara datoteku za pisanje (prepisuje postojeću)
    if (!fp) { // Ako nije moguće otvoriti datoteku
        perror("Greska pri upisu u datoteku"); // Ispisuje sistemsku poruku o grešci
        return; // Izlazi iz funkcije
    }

    // Pokreće rekurzivnu funkciju za čuvanje proizvoda
    sacuvajProizvodeRekurzivno(fp, 0);

    fclose(fp); // Zatvara datoteku
}

// Funkcija za usporedbu proizvoda po nazivu za sort funkciju
int usporediProizvode(const void* a, const void* b) {
    const Proizvod* pa = (const Proizvod*)a; // Pretvara na tip Proizvod
    const Proizvod* pb = (const Proizvod*)b; // Pretvara na tip Proizvod
    return strcmp(pa->naziv, pb->naziv); // Vraća rezultat usporedbe naziva ASCII poretkom
}

// Sortira proizvode po nazivu abecednim redom
void sortirajProizvode() {
    if (skladiste.brojProizvoda > 1) { // Ako ima barem dva proizvoda
        qsort(skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvode); // Sortira niz proizvoda koristeći qsort i funkciju usporediProizvode
        printf("Proizvodi sortirani po nazivu.\n"); // Obavještava korisnika o izvršenoj sorti
    }
}
