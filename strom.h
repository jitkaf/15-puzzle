/*
	Resic hlavolamu Loydova patnactka.
	Verze 1.0
	
	Dialekt : ANSI C
    Kompilator: ANSI C-compatible
    
    Copyright (c) Jitka Furbacherova, 2016
    KIV/PC

*/

#ifndef STROM_H
#define STROM_H


/* ____________________________________________________________________________
 
	Definice symbolu.
   ____________________________________________________________________________
*/

#define ZAKLADATEL   0
#define LEVO         1
#define NAHORU       2
#define PRAVO        3
#define DOLU         4




/*
typedef
struct pole{
int *pole;	
}pole;
*/

/* ____________________________________________________________________________
 
	Struktury a datove typy.
   ____________________________________________________________________________
*/
typedef
struct uzel {
	int ohodnoceni;
	int mira_zamichani;
	int *pole;
	int id;
	int dira;
	int smer; /*0- koren, 1-levo, 2-nahoru, 3-pravo , 4 - dolu*/
	struct uzel *leva;
	struct uzel *prava;
	struct uzel *horni;
	struct uzel *dolni;
	struct uzel *rodic;
	struct uzel *predchozi; 
	struct uzel *dalsi;
	struct uzel *xtyuzel; 
	struct uzel *vrchol_fronty;
	int patro;
} uzel;

/*
typedef
struct fronta{
	struct uzel *uzel;
	struct uzel *dalsi;
}fronta;*/


/* ____________________________________________________________________________
 
   Prototypy funkci.
   ____________________________________________________________________________
*/
int vypocti_ohodnoceni(int *vstup, int pocet_cisel,int pocet_radku);
uzel *tvor_strom(int argc, int* vstup, uzel* rodic, int smer,uzel * predek, uzel * koren, int pocet_radku);
void pridej_leva(uzel *uzlik, int argc, int *vstup, uzel * predek,uzel * koren, int pocet_radku);
void pridej_prava(uzel *uzlik, int argc, int *vstup,uzel * predek,uzel * koren,int pocet_radku );
void pridej_nahoru(uzel *uzlik, int argc, int *vstup,uzel * predek,uzel * koren,  int pocet_radku);
void pridej_dolu(uzel *uzlik, int argc, int *vstup,uzel * predek,uzel * koren,  int pocet_radku);
void volni_strom(uzel *uzlik);


#endif

