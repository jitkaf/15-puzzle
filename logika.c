/*
	Resic hlavolamu Loydova patnactka.
	Verze 1.0
	
	Modul logika.c
	V tomto modulu se odehrava logika programu. Rozhoduje se zde, zda jde uzel expandovat
	do jednotlivych smeru a zda uz je treba uzel vubec jeste expandovat nebo jiz bylo
	reseni nalezeno.
	Pojmy vlevo/vpravo.. se vzdy vztahuji k pohybu prazdneho mista.
	
	Dialekt : ANSI C
    Kompilator: ANSI C-compatible
    
    KIV/PC

*/

#include <stdio.h>
#include <stdlib.h>
#include "strom.h"
#include "logika.h"
#include <string.h>

/* _____________________________________________________________________________________________________

    int expanduj(int pocet_radku, int pocet_zadanych_cisel, struct uzel *uzel, struct uzel *koren)
    
    Vstupni parametrem teto funkce je pocet radku zadanho hlavolamu, pocet zadanych cisel,
    uzel ktery bude expandovan a koren celeho stromu reseni.
    
    Kazdy uzel ma v sobe ulozeno, zda je levym/pravy/hornim/spodnim synem sveho otce,
    pred expandovanim v kazdem smeru otestuji nejprve, zda bych nevytvorilo zpatky rodice.
    Postupne expanduji dany uzel do vsech smeru, pouze v pripade ze bych nalezla pri nekterem
    expandovani reseni, je zavolana funkce vypis_postup(..) a jako navratova hodnota vracena jednicka
	V tomto pripade expandovani dale nepokracuje - reseni je nelezeno.
   _____________________________________________________________________________________________________
*/
int expanduj( int pocet_radku, int pocet_zadanych_cisel, uzel *uzlik, uzel* koren){
	int prazdno = uzlik->dira;
	uzel *predek = uzlik; /* uzel, ktery znaci naposledy vyvtoreny uzel pred tim co budu tvorit ted*/
	/*kdyz by to prislo serazany*/
	if (uzlik->mira_zamichani == 0){  
			return 0;
	}
		
	/*VLEVO - Pokud neni uzlik pravym nasledovnikem rodice a je mozny posun prazdneho mista doleva, zavolam fci vlevo*/
	if ((uzlik->smer != 3) && (prazdno-1 >= (prazdno / pocet_radku) * (pocet_radku))){
		predek=vlevo(uzlik, predek, koren, pocet_radku, pocet_zadanych_cisel);
		if (predek==NULL){
			return 4;
		}
		if (uzlik->leva->mira_zamichani == 0){
			vypis_postup(uzlik->leva);
			return 0;
		}
	}

	/*VPRAVO - Pokud neni uzlik levym nasledovnikem rodice a je mozny posun prazdneho mista doprava, zavolam fci vpravo*/
	if ((uzlik->smer != 1) && (prazdno+1 < ((prazdno + pocet_radku) / pocet_radku) * (pocet_radku))){
		predek=vpravo(uzlik, predek, koren, pocet_radku, pocet_zadanych_cisel);
		if (predek==NULL){
			return 4;
		}
		if (uzlik->prava->mira_zamichani == 0){
			vypis_postup(uzlik->prava);
			return 0;
		}	
	}
		
	/*NAHORU - Pokud neni uzlik dolnim nasledovnikem rodice a je mozny posun prazdneho mista dolu, zavolam fci nahoru*/
	if ((uzlik->smer != 4) && (prazdno - pocet_radku > 0)){
		predek=nahoru(uzlik, predek, koren, pocet_radku, pocet_zadanych_cisel);
		if (predek==NULL){
			return 4;
		}
		if (uzlik->horni->mira_zamichani == 0){
			vypis_postup(uzlik->horni);
			return 0;
		}
	}
	
	/*DOLU - Pokud neni uzlik hornim nasledovnikem rodice a je mozny posun prazdneho mista nahoru, zavolam fci dolu*/
	if ((uzlik->smer != 2) && (prazdno + pocet_radku < pocet_zadanych_cisel)){
		predek=dolu( uzlik, predek, koren, pocet_radku, pocet_zadanych_cisel);
		if (predek==NULL){
			return 4;
		}
		if (uzlik->dolni->mira_zamichani == 0){
			vypis_postup(uzlik->dolni);
			return 0;
		}
	}
	
	return -1;
}

/* _____________________________________________________________________________________________________
 
    uzel* vlevo(uzel *uzlik, uzel *predek, uzel *koren, int pocet_radku, int  pocet_zadanych_cisel)
    
    Ze zadanych parametru je spocitan index prazdneho misto tvoreneho uzlu, take je tomuto uzlu
    vytvoreno pole s udavajici aktualni stav hlavolamu.
    Do uzlu predek ulozi aktualni vytvoreni uzel nebot v pripade tvoreni dalsiho uzlu, bude prave tento
    uzel predkem - naposledy vytvorenym uzlem.
   _____________________________________________________________________________________________________
*/
uzel* vlevo(uzel *uzlik, uzel *predek, uzel *koren, int pocet_radku, int  pocet_zadanych_cisel){
	int index_levy = uzlik->dira-1;
	int* pomocne_pole = (int *) malloc(pocet_zadanych_cisel*sizeof(int));
	memcpy(pomocne_pole,uzlik->pole,pocet_zadanych_cisel*sizeof(int) );
	pomocne_pole[index_levy] = 0;
	pomocne_pole[uzlik->dira] = uzlik->pole[index_levy];
	pridej_leva(uzlik, pocet_zadanych_cisel,pomocne_pole,predek, koren,  pocet_radku);
	predek = uzlik->leva;
	free(pomocne_pole);
		
	return predek;
}

/* _____________________________________________________________________________________________________
 
    uzel* vpravo(uzel *uzlik, uzel *predek, uzel *koren, int pocet_radku, int  pocet_zadanych_cisel)
    
    Ze zadanych parametru je spocitan index prazdneho misto tvoreneho uzlu, take je tomuto uzlu
    vytvoreno pole s udavajici aktualni stav hlavolamu.
    Do uzlu predek ulozi aktualni vytvoreni uzel nebot v pripade tvoreni dalsiho uzlu, bude prave tento
    uzel predkem - naposledy vytvorenym uzlem.
   _____________________________________________________________________________________________________
*/
uzel *vpravo(uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel){
	int index_pravy = uzlik->dira+1;
	int* pomocne_pole = (int *) malloc(pocet_zadanych_cisel*sizeof(int));
	memcpy(pomocne_pole,uzlik->pole, pocet_zadanych_cisel*sizeof(int));
	pomocne_pole[index_pravy] = 0;
	pomocne_pole[uzlik->dira] = uzlik->pole[index_pravy];
	pridej_prava(uzlik,pocet_zadanych_cisel,pomocne_pole,predek,koren, pocet_radku);
	predek = uzlik->prava;
	free(pomocne_pole);	
	
	return predek;	
}


/* _____________________________________________________________________________________________________
 
    uzel* vlevo(uzel *uzlik, uzel *predek, uzel *koren, int pocet_radku, int  pocet_zadanych_cisel)
    
    Ze zadanych parametru je spocitan index prazdneho misto tvoreneho uzlu, take je tomuto uzlu
    vytvoreno pole s udavajici aktualni stav hlavolamu.
    Do uzlu predek ulozi aktualni vytvoreni uzel nebot v pripade tvoreni dalsiho uzlu, bude prave tento
    uzel predkem - naposledy vytvorenym uzlem.
   _____________________________________________________________________________________________________
*/
uzel *nahoru(uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel){
	int index_horni = uzlik->dira - pocet_radku;
	int* pomocne_pole = (int *) malloc(pocet_zadanych_cisel*sizeof(int));
	memcpy(pomocne_pole, uzlik->pole, pocet_zadanych_cisel*sizeof(int));
	pomocne_pole[index_horni] = 0;
	pomocne_pole[uzlik->dira] = uzlik->pole[index_horni];
	pridej_nahoru(uzlik, pocet_zadanych_cisel,pomocne_pole,predek,koren, pocet_radku);
	predek = uzlik->horni;
	free(pomocne_pole);
	
	return predek;
}


/* _____________________________________________________________________________________________________
 
    uzel* vlevo(uzel *uzlik, uzel *predek, uzel *koren, int pocet_radku, int  pocet_zadanych_cisel)
    
    Ze zadanych parametru je spocitan index prazdneho misto tvoreneho uzlu, take je tomuto uzlu
    vytvoreno pole s udavajici aktualni stav hlavolamu.
    Do uzlu predek ulozi aktualni vytvoreni uzel nebot v pripade tvoreni dalsiho uzlu, bude prave tento
    uzel predkem - naposledy vytvorenym uzlem.
   _____________________________________________________________________________________________________
*/
uzel *dolu(uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel){
	int index_dolni = uzlik->dira + pocet_radku;
	int* pomocne_pole = (int *) malloc(pocet_zadanych_cisel*sizeof(int));
	memcpy(pomocne_pole, uzlik->pole, pocet_zadanych_cisel*sizeof(int));
	pomocne_pole[index_dolni] = 0;
	pomocne_pole[uzlik->dira] =uzlik->pole[index_dolni];
	pridej_dolu(uzlik, pocet_zadanych_cisel,pomocne_pole,predek,koren, pocet_radku);	
	free(pomocne_pole);
	
	return predek;	
	
}

/* ______________________________________________________________________________________________
 
    vypis_postup(uzel *uzlik)
    
    Paramezrem teto funkce je uzel, ktery predstavuje reseni hlavolamu. Jedna se o list stromu,
    kazdy list ma ukazatel na rodice. Pomoci tohoto ukazatele projdu posloupnost uzlu vedoucich
    od reseni az k zadani a nasledne pozpatku vypisu postup.
   ______________________________________________________________________________________________
*/
void vypis_postup(uzel *uzlik){
	int pocet_kroku=uzlik->patro;
	int *postup=(int *) malloc(pocet_kroku*sizeof(int));
	int *pozice= (int *) malloc(pocet_kroku*sizeof(int));
	
	/*s nejvetsi pravdepodobnosti nenastane*/
	if ((postup==NULL)||(pozice==NULL)){
		printf("ERR#4: Non-existent solution!");
		free(pozice);
		free(postup);
		exit(4);
	}
	
	int i=pocet_kroku;
	uzel *pomocnik=uzlik;  
	
	for (i; i > 0; i--){
		
		postup[i] = pomocnik->smer; /*z aktualniho prochazeneho uzlu urcim jeho smer*/
		pozice[i]= pomocnik->rodic->pole[pomocnik->dira]; /*v miste kde mel rodic diru se nachazi kamen, kterym bylo posunuto */
		pomocnik = pomocnik->rodic;
	}
	pozice[pocet_kroku]=uzlik->pole[uzlik->rodic->dira];

	for (i=1; i <= pocet_kroku; i++){
		/* hodnota 1 predstavuje posun prazdneho mista doleva - kamen se tedy posune doprava*/
		if (postup[i] == LEVO){
			printf ("%d: [%d] RIGHT \n", i, pozice[i]);
		} 
		/* hodnota 2 predstavuje posun prazdneho mista nahoru - kamen se tedy posune dolu*/
		else if (postup[i] == NAHORU){
			printf ("%d: [%d] DOWN \n", i, pozice[i]);
		}
		/* hodnota 3 predstavuje posun prazdneho mista doprava - kamen se tedy posune doleva*/
		else if (postup[i] == PRAVO){
			printf ("%d: [%d] LEFT \n", i,pozice[i]);
		}
		/* hodnota 4 predstavuje posun prazdneho mista dolu - kamen se tedy posune nahoru*/
		else if (postup[i] == DOLU){
			printf ("%d: [%d] UP \n", i, pozice[i]);
		}
		else {
			printf ("\n divna vec %d  -  %d\n", i, postup[i]);
		}
	}
	
	free(pozice);
	free(postup);

	printf("GOAL");
}
