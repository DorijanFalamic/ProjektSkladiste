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
    system("cls");  /* Ako je uslov iz prethodne linije istinit (tj. ako je program na Windows-u), ova linija se izvršava. 
                                        Komanda cls se koristi u Windows konzoli za čišćenje ekrana.*/
#else
    system("clear");
#endif
}

const char* kategorijaUString(Kategorija k) {
    switch (k) {
    case VOCE:
        return "voce";
    case POVRCE:                //sprema ako napisemo voce u VOCE i povrce u POVRCE
        return "povrce";
    default:
        return "nepoznato";
    }
}

Kategorija stringUKategoriju(const char* s) {
    if (strcmp(s, "voce") == 0 || strcmp(s, "Voce") == 0)
        return VOCE;                                    // provjerava da li su dva strinka jednaka i ako je vraca POVRCE ili VOCE 
    else if (strcmp(s, "povrce") == 0 || strcmp(s, "Povrce") == 0)
        return POVRCE;
    else
        return VOCE;
}

// Provjera postoji li vec uneseni ID
int postojiId(int id) {
    for (size_t i = 0; i < skladiste.brojProizvoda; i++) {
        if (skladiste.niz[i].id == id) {//provjerrava da li ima isti ID na zadanom indeksu pocevsi od nule
            return 1;
        }
    }
    return 0;                  //ako petlja ne pronadje ID vraca se 0 sto oznacava da ne postoji proizvod sa unesenim ID-om
}

void dodajProizvod() {
    if (skladiste.niz == NULL) {
        printf("Greska: Memorija za proizvode nije alocirana!\n");
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();            // koristimo da bi stisnuli enter i nastavili
        return;
    }
    if (skladiste.brojProizvoda >= MAX_PROIZVODI) {                //ako je broj proizvoda presao limit skladista(max_proizvodi) onda printef-aj dole
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
        if (postojiId(noviId)) {                   // ako id vec postoji petlja ce se ponovno izvrsiti (koristimo da vidimo jel vec postoji id)
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

    skladiste.niz[skladiste.brojProizvoda++] = novi;  //dodaje novi proizvod na sljedece slobodno mjesto u "skladistu"
    sacuvajProizvode();
    printf("Proizvod dodan i spremljen.\n");
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void prikaziProizvode() {
    static int brojPoziva = 0;
    brojPoziva++;                                                                  //Svaki put kada se funkcija pozove, brojPoziva se povećava za 1   |
    printf("Funkcija prikaziProizvode pozvana %d puta\n", brojPoziva);    //         <-----------------------------------------i onda ovdje ispisuje  | 

    FILE* fp = fopen(DATOTEKA, "r");  //pokusava otvoriti datoteku definisano skladiste.txt (definirano gore u 9toj liniji),ako ne uspioje fajl pojnter ce biti null
    if (!fp) {                                  //ako nije ispisuje nam grešku koju smo printf-ali dolje u liniji |
        printf("Datoteka ne postoji ili je prazna.\n");   // <----------------------------------------------------|              
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    Proizvod p;            //čuvanje informacija u promjenjivom tipu
    printf("\n--- Lista proizvoda ---\n");
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) {  //ako ucita sve uspjesno printf-a ovo iz petlje dole|
        printf("ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, kategorijaUString(p.kategorija), p.kolicina, p.cijena); //<-----------------------------------|
    }
    fclose(fp);
    printf("Pritisni Enter za nastavak...");  // ispisuje poruku i koristi getchar da nastavili |
    getchar();          //<---------------------------------------------------------------------|
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
                //---------------------------ovo je podizbornik za pretrazivanje proizvoda koji u switch caseu poziva te tri funkcije
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
    char trazeni[MAX_NAZIV];                             // max_naziv je definirano u headeru (50 mjesta [51 zato sto se broji i 0])
    printf("Unesite naziv za pretragu: ");
    scanf(" %49[^\n]", trazeni);
    FILE* fp = fopen(DATOTEKA, "r");            // ovo je fajl pojnter na datoteku i otvara ju u rezimu citanja te ispod provjerava ako ne postoji ispisuje poruku da ne postoji   |
    if (!fp) {//           <-------------------------------------------------------------------------------------------------------------------------------------------------------|
        printf("Datoteka ne postoji.\n");//   <------------------------------------------------------------------------------------------------------------------------------------|
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    Proizvod p;        // promjenjiv tip koji cuva informacije
    int found = 0;             // sstavlja pronadjene na 0 koristi se za pracenje da li je pronadjenneo
    while (fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, (int*)&p.kategorija, &p.kolicina, &p.cijena) == 5) { //za čitanje podataka iz datoteke
        if (strcmp(p.naziv, trazeni) == 0) {        //  Ako su nazivi jednaki, to znači da je proizvod pronađen.
            //koristi funkciju strcmp za poređenje naziva proizvoda (p.naziv) sa traženim nazivom (trazeni)
            printf("Pronaden: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", p.id, p.naziv, kategorijaUString(p.kategorija), p.kolicina, p.cijena);
            found = 1;
        }
    }
    if (!found) printf("Proizvod nije pronaden.\n");              //ako nije onda  |
    fclose(fp);                  // zatvori <--------------------------------------|
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
        printf("Nema proizvoda.\n");                                  //akol je 0 nema proizvoda pa printfa to
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    int trazeniId;
    printf("Unesite ID za pretragu: ");
    scanf("%d", &trazeniId);                                      // unos IDa sa tipkovnice
    qsort(skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvodId);// ova linija pomocu qsorta sortira proizvode pomocu id tako da iz func usporediproizvode definise na koji se nacin usporeduju
    Proizvod kljuc;                   //koristi se za pretragu
    kljuc.id = trazeniId;
    Proizvod* pronadjen = bsearch(&kljuc, skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvodId);  //bsearch trazi proizvod koji se poklama sa IDom ('kljuc') i vraca pokazivac na proizvod ili null
    if (pronadjen) {
        printf("Pronaden: ID: %d | %s | %s | %.2f kg | %.2f kn/kg\n", pronadjen->id, pronadjen->naziv, kategorijaUString(pronadjen->kategorija), pronadjen->kolicina, pronadjen->cijena);
    }
    else {
        printf("Proizvod nije pronaden.\n");                    // ako je u bsearchu vracen null ispisuje ovo
    }
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void obrisiProizvod() {
    int idZaBrisanje;          // inicijalizira id koji korisnik zeli da obrise i cuva ga
    printf("Unesite ID proizvoda za brisanje: ");
    scanf("%d", &idZaBrisanje);

    // Brisanje iz memorije
    int found = 0;
    for (size_t i = 0; i < skladiste.brojProizvoda; i++) {
        if (skladiste.niz[i].id == idZaBrisanje) {
            for (size_t j = i; j < skladiste.brojProizvoda - 1; j++) {
                skladiste.niz[j] = skladiste.niz[j + 1];
            }                 /*znaci ovaj dio koda pretrazuje niz proizvoda da
                              pronadje id koji je unesen te pomice proizvode([j + 1]) uljevo brisuci proizvod, n
                              akon toga se skladiste smanjuje za jedan i petlja se prekida(zato sto se found ispod postavlha na 1*/
            skladiste.brojProizvoda--;
            found = 1;
            break;
        }
    }
    if (!found) {                                        // ako nijefound onda proizvod nije pronadjen (logično zhaha)
        printf("Proizvod s ID %d nije pronaden.\n", idZaBrisanje);
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    sacuvajProizvode();             // poziva funkciju da sacuva ostale i ispisuje dole da je taj sa trazenim idom obrisan
    printf("Proizvod obrisan.\n");
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void urediProizvod() {
    int idZaUrediti;          //inicijalizacija
    printf("Unesite ID proizvoda za uredivanje: ");
    scanf("%d", &idZaUrediti);              //unos sa tipkovinice inicijalizaicje gore
    int found = 0;         // found koristimo da znamo kasnije jer pronadjen taj id 
    for (size_t i = 0; i < skladiste.brojProizvoda; i++) {      
        if (skladiste.niz[i].id == idZaUrediti) {              // koristi se za pronalazak ida koji je unesen da bi se uredio jel
            char kategorijaStr[30];// ovaj dio koda se izvrsava kada se pronadje taj id i unosimo onda nove podatke tj ove ispod |
            printf("Unesite nove podatke za proizvod:\n");  //   <---------------------------------------------------------------|
            printf("Naziv: ");                 //                <---------------------------------------------------------------|
            scanf(" %49[^\n]", skladiste.niz[i].naziv);//        <---------------------------------------------------------------|
            printf("Kategorija (voce/povrce): ");//              <---------------------------------------------------------------|
            scanf(" %29[^\n]", kategorijaStr);//                 <---------------------------------------------------------------|
            skladiste.niz[i].kategorija = stringUKategoriju(kategorijaStr);//<---------------------------------------------------|
            printf("Kolicina: ");//               <------------------------------------------------------------------------------|                           
            scanf("%f", &skladiste.niz[i].kolicina);//           <---------------------------------------------------------------|
            printf("Cijena: ");//                                <---------------------------------------------------------------|
            scanf("%f", &skladiste.niz[i].cijena);//             <---------------------------------------------------------------|
            found = 1;                              // znaci da je proizvoda pronadjen zato sto je found postavljeno na 1 te izlazi iz petlje pomocu break; |
            break;                  // <--------------------------------------------------------------------------------------------------------------------|
        }
    }
    if (!found) {
        printf("Proizvod s ID %d nije pronaden.\n", idZaUrediti);               // klasika ako nijefound onda ispisi ovo unutar petlje
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    sacuvajProizvode();           // poziva funkciju da sacuva ostale i ispisuje dole da je taj sa trazenim idom azuriran
    printf("Proizvod uspjesno azuriran.\n");                              
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

void ucitajProizvode() {
    skladiste.niz = (Proizvod*)malloc(sizeof(Proizvod) * MAX_PROIZVODI);// alocira memoriju skladiste.niz koristeci malloc,velicina je odredjena MAX_PROIZVODI u headeru   |
                                                                        //sizeof odredjuje koliko memorije je potrebno za jedan proizvod                                   |
    skladiste.brojProizvoda = 0;        //0 zato sto nema proizvoda
    FILE* fp = fopen(DATOTEKA, "r");       // otvara skladiste.txt za citanje ako ne postoji onda vracan null
    if (!fp) {                 // ako nije otvoreno uspjesno ispisuje gresku
        printf("Datoteka ne postoji, nastavljamo s praznim skladistem.\n");
        return;
    }
    while (skladiste.brojProizvoda < MAX_PROIZVODI) {          //ucitava iz MAX_PROIZVODI
        Proizvod p;
        int kategorijaInt;         // za cuvanje informacija i onda se kasnije ucitava u fscanfu
        int rezultat = fscanf(fp, "%d,%49[^,],%d,%f,%f\n", &p.id, p.naziv, &kategorijaInt, &p.kolicina, &p.cijena);
        if (rezultat != 5) {// ako ne bude 5 proizovda ucitanih onda izlazi iz petlje pomocu breakea
            break;
        }
        p.kategorija = (Kategorija)kategorijaInt; // dodjeljujemo p.kategorija odgovarajuci tip kategorija 
        skladiste.niz[skladiste.brojProizvoda++] = p;
    }
    fclose(fp);  // nakon sto je sve ucitano zatvara datoteku
}

// Rekurzivna funkcija za spremanje proizvoda (koncept 25)
void sacuvajProizvodeRekurzivno(FILE* fp, size_t index) {
    if (index >= skladiste.brojProizvoda) {  // ako je index veci ili jedna broju proizvoda u sklad. , ako je zavrsava se i svi su proizvodi sacuvani
        return;                 
    }
    Proizvod p = skladiste.niz[index];   // trazimo proizvod na trenutnom indeksu u nizu prozivoda iz skladista i cuva ga u p
    fprintf(fp, "%d,%s,%d,%.2f,%.2f\n", p.id, p.naziv, p.kategorija, p.kolicina, p.cijena);  // pomocu printfa pise podatke o proizvodu
    sacuvajProizvodeRekurzivno(fp, index + 1);  // omogucava da predje na sljedeci proizvodu u nizu , tu smo ju pozvali unutar iste funkcije i to ju cini rekurzivnomo
}

void sacuvajProizvode() {
    FILE* fp = fopen(DATOTEKA, "w");
    if (!fp) {                           //otvara skladiste urezimu za pisanje i ako se krivo upise ispise gresku 
        perror("Greska pri upisu u datoteku");  // ispisuje opis greske (errno) i dodaje dvotocku i razmak
        return;
    }
    sacuvajProizvodeRekurzivno(fp, 0);  // rekurzivno cuva proizvode iz fajla poceci od nula pa nadalje
    fclose(fp);  // i kada je sve sacuvano closea datoteku
}

int usporediProizvode(const void* a, const void* b) {  // funkcija proprima a i b koji su pokazivaci
    const Proizvod* pa = (const Proizvod*)a;//        |  ove dvije linije pretvaraju pokazivace a i b na tip proizvoda sto omogucuje pristup strukturi proizvod putem pokazivaca pa i pb
    const Proizvod* pb = (const Proizvod*)b;//        |  ove dvije linije pretvaraju pokazivace a i b na tip proizvoda sto omogucuje pristup strukturi proizvod putem pokazivaca pa i pb
    return strcmp(pa->naziv, pb->naziv);       // usporedjuje nazive i ako su veci ili manji od pb i pa vraca vrijednost
}

void sortirajProizvode() {
    if (skladiste.brojProizvoda > 1) {    // sluzi da provjerimo jel ima 1 ili 0 proizvoda i ako ima nema potrebe za sortiranjem
        qsort(skladiste.niz, skladiste.brojProizvoda, sizeof(Proizvod), usporediProizvode);//ako je zadovoljen uvjet gore poziva se ovo i sortira po nazivu,koristi skladiste.brojProizvoda da na koliko treb a sortirati,sizeof za velicinu
        printf("Proizvodi sortirani po nazivu.\n");
    }
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}

// Funkcija s fseek/ftell (koncept 20)
void prikaziVelicinuDatoteke() {
    FILE* fp = fopen(DATOTEKA, "r");    // otvara datoteku u rezimu citanja
    if (!fp) {
        printf("Datoteka ne postoji.\n");            // ako ne moze onda ispisuje gresku
        printf("Pritisni Enter za nastavak...");
        getchar();
        getchar();
        return;
    }
    fseek(fp, 0, SEEK_END);      // pomice sa 0 na kraj datoteke sto kaznije omogucuje da ispisemo maksimalnu velicinu
    long velicina = ftell(fp);   // u ovom slucaju ftell vraca poziciju i predstavlja ukupnu velicinu i zato koristimo long inicijalizacij
    fclose(fp);
    printf("Velicina datoteke: %ld bajtova\n", velicina);             // zatvara sa fclose i ispisuje koliko ima u datoteci bajtova
    printf("Pritisni Enter za nastavak...");
    getchar();
    getchar();
}
