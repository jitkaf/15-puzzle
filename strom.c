/*
	Resic hlavolamu Loydova patnactka.
	Verze 1.0
	
	Modul strom.c
	V tomto modulu tvorim cely stavovy strom reseni.
	
	Dialekt : ANSI C
    Kompilator: ANSI C-compatible
    
    Copyright (c) Jitka Furbacherova, 2016
    KIV/PC

*/

#include <stdio.h>
#include <stdlib.h>
#include "strom.h"
#include <string.h>
#include <math.h>

/* ______________________________________________________________________________________________

    Globalni promenne.
   ______________________________________________________________________________________________
*/
int global_id = 0; /*pocet uzlu, ktere byly vytvoreny*/
int pocet = 1; /*pocet uzlu cekajicich na expandovani*/


/* ______________________________________________________________________________________________
 
   int vypocti_ohodnoceni(int *vstup,int ohodnoceni_rodice, int argc, int pocet_radku, int patro)
   
   Navratova hodnota teto funkce je pouzita jako ohodnoceni tvoreneho uzlu. Ohodnoceni pocitam
   pomoci Manhatnovi vzdalenosti a take sleduji, jak moc jsou prvky jiz serazene (vysvetleno
   v dokumentaci). Pri vypoctu prochazim pole zadane v argumentu funkce prvek po prvku.
   
   V pripade ze by bylo pole serazene, byl by prvek 1 na indexu 0, prvek 2 na indexu 1,...,
    prvek 0 predstavujici diru by byl na poslednim miste.
   ______________________________________________________________________________________________
*/
int vypocti_ohodnoceni(int *vstup, int pocet_cisel, int pocet_radku){
	int ohodnoceni;
	int mira_zamichani=0;
	int i;
	int sloupecek; /*sloupecek ve kterem se prvek vyskytuje*/
	int radka; /*radka ve kterem se prvek vyskytuje*/
	int patri_sloupecek; /*sloupecek ve kterem se prvek ma vyskytovat*/
	int patri_radka; /*radka ve kterem se prvek ma vyskytovat*/
 	for (i = 0; i < pocet_cisel; i++){
		radka=i/pocet_radku+1;
		sloupecek=(i)%pocet_radku+1;
		if (vstup[i] != 0){	
			patri_radka = (vstup[i] - 1)/pocet_radku+1;
			patri_sloupecek = (vstup[i] - 1)%pocet_radku+1;
		}
		else {
			patri_radka = pocet_radku;
			patri_sloupecek = pocet_radku;
		}
		if (vstup[i] != 0){ 
			if (sloupecek < pocet_radku){
				if ((vstup[i+1] != 0) && (vstup[i]+1 != vstup[i+1])){
					mira_zamichani++;
				}
			}
			else {
				if (radka < pocet_radku){
					if ((vstup[i+pocet_radku] != 0) && (vstup[i]+1 != vstup[i+pocet_radku]) && (vstup[i+1] != 0) && (vstup[i]+1 != vstup[i+1])){ /*- blbe*/
						mira_zamichani++;
					}
				}
			}
		}
		
		mira_zamichani = mira_zamichani + abs(sloupecek - patri_sloupecek) + abs(radka - patri_radka);		
	}
	
	ohodnoceni = mira_zamichani;
	return ohodnoceni;
}


/* _______________________________________________________________________________________________________________
 
    uzel *tvor_strom(int argc, int* vstup, uzel *rodic,int smer, uzel *predek, uzel *koren, int pocet_radku)
    
    V teto funkci je vytvoren uzel ze zadanych parametru. Kazdemu uzlu jsou nastaveny vsechny nasledovnici na NULL.
	Korenu funkce je zde nastaven ukazatel na vrchol fronty, ktery udava, ktery uzel ma byt dale expandovan.
    
   _______________________________________________________________________________________________________________
*/
uzel *tvor_strom(int argc, int* vstup, uzel *rodic,int smer, uzel *predek, uzel *koren, int pocet_radku) {
	uzel *pomocnik;
	uzel *uzlik = (uzel *) malloc(sizeof(uzel));  
	
	if (uzlik==NULL){
		printf("ERR#4: Non-existent solution!");
		return NULL;
	}
	  
	uzlik->pole = (int *)malloc(argc*sizeof(int));
	
	if (uzlik->pole==NULL){
		printf("ERR#4: Non-existent solution!");
		return NULL;
	}
	
	uzlik->id = global_id; 
	global_id++;
	memcpy(uzlik->pole,vstup, argc * sizeof(int));
	

	/* pokud pridavam prvni uzel nastavim patro na 0 a urcim ohodnoceni*/	
	if (rodic == NULL){
		uzlik->patro = 0;
	}
	/*pridava prvni dite rodice*/
	else if (predek == rodic){
		uzlik->patro = rodic->patro+1;
	}
	/*pridavam jine nez prvni dite rodice*/
	else {
		uzlik->patro = predek->patro;
	}

	uzlik->mira_zamichani = vypocti_ohodnoceni(vstup, argc, pocet_radku);
	uzlik->ohodnoceni =	uzlik->mira_zamichani+uzlik->patro;
	uzlik->smer = smer;
	uzlik->leva = NULL;
	uzlik->prava = NULL;
	uzlik->horni = NULL;
	uzlik->dolni = NULL;
	uzlik->dalsi = NULL;
	uzlik->predchozi = NULL;
	uzlik->xtyuzel = NULL; 
	uzlik->rodic=rodic;

	/*pokud vkladam nezakladajici koren*/
	if (uzlik->id != 0){
		/*pokud vkladam prvni dite je nutno odstranit rodice*/
		if (predek->id == rodic->id){
			pocet--; /*snizim pocet prvku cekajicich na expanzi*/
			/*pokud se jedna o vrchol fronty musim vrchol posunout*/
			if (koren->vrchol_fronty->id == predek->id){ /*pokud je ten vrchol sam*/
				if (koren->vrchol_fronty->dalsi == NULL){
					koren->vrchol_fronty = uzlik;
					uzlik->dalsi = NULL;
					uzlik->predchozi = NULL;
					return uzlik;
				}
				else {
					koren->vrchol_fronty = koren->vrchol_fronty->dalsi;
					koren->vrchol_fronty->predchozi=NULL;
				}
			}
			/*pokud je na konci  - uz vim ze neni jediny*/
			else if (predek->dalsi == NULL){ 
				predek->predchozi->dalsi = NULL;
			}
			/*je uvnitr*/
			else {
				predek->dalsi->predchozi = predek->predchozi;
				predek->predchozi->dalsi = predek->dalsi;
			}
		}	

		/*Pokud je prvek dostatecny dobry nebo je jediny, zaradim ho do cekatelu na expandovani */
		if ((koren == NULL) || (koren->xtyuzel == NULL) || (uzlik->mira_zamichani < koren->xtyuzel->mira_zamichani + pocet_radku) || (pocet < pow(pocet_radku,pocet_radku))){
			pocet++; /*poet prvku cekajici na expandovani*/
			if (koren->xtyuzel == NULL){
				koren->xtyuzel = uzlik;
			}
			else if	((koren->xtyuzel->ohodnoceni < uzlik->ohodnoceni) && (pocet < pow(pocet_radku,pocet_radku))){
				koren->xtyuzel = uzlik;
			}
			else if	(pocet	>=	pow(pocet_radku,pocet_radku)){
				koren->xtyuzel = koren->xtyuzel->predchozi;	
			}
			if (koren->vrchol_fronty->dalsi == NULL){/*pokud je tam jen jeden prvek*/
				if (koren->vrchol_fronty->ohodnoceni <= uzlik->ohodnoceni){/*pokud je vrchol lepsi pridam za nej*/
					koren->vrchol_fronty->dalsi = uzlik;
					uzlik->predchozi = koren->vrchol_fronty;
				}
				else {/*pridam pred*/
					uzlik->dalsi = koren->vrchol_fronty;
					koren->vrchol_fronty = uzlik;
					koren->vrchol_fronty->dalsi = uzlik->dalsi;
				}
			}
			else { /*pokud pridavam do vic prvku*/
				if (koren->vrchol_fronty->ohodnoceni > uzlik->ohodnoceni){ /*pokud je novej prvek nejlepsi prijde na start*/
					uzlik->dalsi = koren->vrchol_fronty;
					koren->vrchol_fronty = uzlik;
				}
				else { /* uz vim ze existuje .dalsi prvek a ze nepridavam na start*/
					 pomocnik = koren->vrchol_fronty;
					while ((pomocnik->dalsi != NULL) && (pomocnik->dalsi->ohodnoceni < uzlik->ohodnoceni)){
						pomocnik=pomocnik->dalsi;
					}		
					uzlik->dalsi = pomocnik->dalsi;
					pomocnik->dalsi = uzlik;
					uzlik->predchozi = pomocnik;
					if (uzlik->dalsi != NULL){
						uzlik->dalsi->predchozi =uzlik;
					}
				}
			}
		}
    }
	return uzlik;
}


/* ______________________________________________________________________________________________

	void pridej_leva(uzel *koren, int argc, int *vstup,uzel * predek,uzel * koren, int pocet_radku)
 
    V teto funkci overim, ze novy uzel pridavam na spravne misto - ze jiz neni obsazene. 
    Tato funkce slouzi spise pro ladici ucely.
   ______________________________________________________________________________________________
*/
void pridej_leva(uzel *uzlik, int argc, int *vstup, uzel * predek, uzel * koren, int pocet_radku) {
	if (uzlik == NULL) {
		return;
	}
	
	if (uzlik->leva == NULL) {
		uzlik->leva = tvor_strom(argc, vstup, uzlik, LEVO, predek, koren, pocet_radku);
		return;
	} else {
		printf("ERR#6: Cannot continue!");
		exit(6);
	}
}


/* ______________________________________________________________________________________________
 	
 	void pridej_prava(uzel *koren, int argc, int *vstup,uzel * predek,uzel * koren, int pocet_radku)
 
    V teto funkci overim, ze novy uzel pridavam na spravne misto - ze jiz neni obsazene. 
    Tato funkce slouzi spise pro ladici ucely.
   ______________________________________________________________________________________________
*/
void pridej_prava(uzel *uzlik, int argc, int *vstup, uzel * predek, uzel * koren, int pocet_radku) {
	if (uzlik == NULL) {
		return;
	}

	if (uzlik->prava == NULL) {
		uzlik->prava = tvor_strom(argc, vstup, uzlik, PRAVO, predek, koren, pocet_radku);
			return;
	} else {
		printf("ERR#6: Cannot continue!");
		exit(6);
	}
}


/* ______________________________________________________________________________________________

	void pridej_nahoru(uzel *koren, int argc, int *vstup, uzel * predek,uzel * koren, int pocet_radku)
 
    V teto funkci overim, ze novy uzel pridavam na spravne misto - ze jiz neni obsazene. 
    Tato funkce slouzi spise pro ladici ucely.
   ______________________________________________________________________________________________
*/
void pridej_nahoru(uzel *uzlik, int argc, int *vstup, uzel * predek,uzel * koren, int pocet_radku) {
	if(uzlik == NULL) {
		return;
	}

	if (uzlik->horni == NULL) {
		uzlik->horni = tvor_strom(argc, vstup, uzlik, NAHORU, predek, koren, pocet_radku);
		return;
	} else {
		printf("ERR#6: Cannot continue!");
		exit(6);
	}
}


/* ______________________________________________________________________________________________
	
	void pridej_dolu(uzel *koren, int argc, int *vstup,uzel * predek,uzel * koren,  int pocet_radku)
 
    V teto funkci overim, ze novy uzel pridavam na spravne misto - ze jiz neni obsazene. 
    Tato funkce slouzi spise pro ladici ucely.
   ______________________________________________________________________________________________
*/
void pridej_dolu(uzel *uzlik, int argc, int *vstup,uzel * predek,uzel * koren,  int pocet_radku) {
	if(uzlik == NULL) {
		return;
	}
	
	if (uzlik->dolni == NULL) {
		uzlik->dolni = tvor_strom(argc, vstup, uzlik, DOLU, predek, koren, pocet_radku);
		return;
	} else {
		printf("ERR#6: Cannot continue!");
		exit(6);
	}
}



/* ______________________________________________________________________________________________
 
    void uvolni_strom(uzel *uzlik)
    
    V teto funkci projdu rekurzivne vsechny uzly stromu stavoveho prostoru a uvolnim pamet.
   ______________________________________________________________________________________________
*/
void uvolni_strom(uzel *uzlik) {
	/*printf ("kuk");*/
	
	if (uzlik->leva != NULL) {
		uvolni_strom(uzlik->leva);
	}
	
	if (uzlik->prava != NULL) {
		uvolni_strom(uzlik->prava);
	
	}
	if (uzlik->horni != NULL) {
		uvolni_strom(uzlik->horni);
	}
	
	if (uzlik->dolni != NULL) {
		uvolni_strom(uzlik->dolni);
	}
	
	free(uzlik->pole);
	free(uzlik);
}









