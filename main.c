/*
	Resic hlavolamu Loydova patnactka.
	Verze 1.0
	
	Modul main.c
	Tento medul je hlavnim modulem celeho programu, komponuje vsechny casti analyzatoru doromady.
	Zpracovava se zde vstup od uzivatel, ktery se dale predava pomoci metod.
	
	Dialekt : ANSI C
    Kompilator: ANSI C-compatible
    
    Copyright (c) Jitka Furbacherova, 2016
    KIV/PC

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "strom.h"
#include "logika.h"
#include <string.h>


/* ______________________________________________________________________________________________

    int over_parametry(char *vstup)
    
    Vstupni parametrem teto funkce je prvni parametr predany na prikazove radce. Tato funkce overi,
	zda jsou vstupni parametry zadany korekne, v pripade ze narazi na chybu, vrati hodnotu teto 
	chyby. V opacnem pripade zavola funkci over_resitelnost(pole, ocekavany_pocet_cisel, pocet_radku) 
	a vrati hodnotu teto funkce.
   ______________________________________________________________________________________________
*/

int over_parametry(char *vstup){
	int i,j; /* pomocne promenne vyuzivane jako indexy*/
	double pocet_radku; 
	int ocekavany_pocet_cisel=1; 
	int overeni_parametru;
	int *pole;
	int *pom;
	int rad; /*rad nacteneho cisla*/
	int pocet_stredniku=0;
	
	/*V pripade ze neni predan zadny vstup, skonci chybou*/
	if (vstup == NULL){
		printf("ERR#1: Missing argument");
		printf("\n Welcome to the program 15 puzzle solver. \n");
		printf ("\n Just enter one argument please. \n For example: solve15.exe \"8 5 11 4; 2 15 7 9; 13 _ 3 6; 12 14 15 10\"");
		return 1;
	}

	/*urcim ocekavany_pocet_cisel podle poctu mezer ve vstupu*/
	for (i = 0; i < strlen(vstup); i++){
		if (vstup[i] == ' '){
			ocekavany_pocet_cisel++;
		}
		
	}
	
	pole = (int *) malloc(ocekavany_pocet_cisel * sizeof(int)); /* pole o velikosti poctu zadanych cisel, do neho prevedu vstup na cisla*/
	pom = (int *) malloc(ocekavany_pocet_cisel * sizeof(int)); /* pole o velikosti poctu zadanzch cisel, pomoci ktereho kontroluji vyskyt vsech cisel*/

	if ((pole==NULL)||(pom==NULL)){
		printf("ERR#4: Non-existent solution!");
		free(pom);
		free(pole);
		return 4;
	}

	/*nejpve vynuluju pocet vyskytu*/
	for (i = 0; i < ocekavany_pocet_cisel; i++){
		pom[i] = 0;
	}
	
	pocet_radku = sqrt(ocekavany_pocet_cisel);
	if (pocet_radku < 3){
		printf("ERR#3: Field too small!");
		return 3;
	}
	
	/*Pokud se druha mocnina poctu radku prevedena na int nerovna ocekavanemu poctu cisel,
	 nebyl zadan spravny pocet cisel, protoze se nejedna o mocninou 2. */
	if (((int)pocet_radku*pocet_radku != ocekavany_pocet_cisel)){
		printf("ERR#2: Malformed input!");
		return 2;
	}
	
	j = 0;/*posouva me v celem vstupu*/
	i = 0; /*kolikate cislo zpracovavam  */
	/*Prevod vstupniho parametru na ciselne pole*/
	while( j < strlen(vstup)){
		/* dokud neprekrocim delku vstupu a nenanarazim na mezeru*/
		while (( j < strlen(vstup)) &&(vstup[j] != ' ')){
			/* postupuje znak po znaku a zkouma jestli se jedna o cislo*/
			if (vstup[j]=='_'){
				pole[i]=0;
				pom[0]=1;
				j++;
			}
			else if (isdigit(vstup[j])){  
					pole[i] = atoi(&vstup[j]);
					pom[pole[i]]++; /*zvysim pocet nalezeni daneho cisla o jedna*/
					rad=pole[i];
					while (rad>0){ /*posunu index pohzbujici se ve vstupu o pocet cifer nacteneho cisla*/
						rad=rad/10;
						j++;
					}
			}
			else if (vstup[j] == ';'){
				pocet_stredniku++;
				j++;
			}
			else { /*pokud je nacten nespecifikovany znak, vrati chybu*/
				printf ("ERR#2: Malformed input!");
				return 2;
			}
		}
		i++; 
		j++;
	}			

	/*pokud nebyl nacten spravny pocet stredniku vrati chybu*/
	if (pocet_stredniku+1!=pocet_radku){
		printf ("ERR#2: Malformed input!");
	}
	
	/*kontrola, ze jsou ve vstupou vsechna cisla prave jednou*/
	for (i = 0 ; i < ocekavany_pocet_cisel; i++){
		if (pom[i] != 1){
			printf("\n ERR#2: Malformed input!");
			return 2;
		}
	}
	free(pom);
	
	/*V pripade ze nenastala zadna chyba zavolam funkci overeni resitelnosti*/
	overeni_parametru = over_resitelnost(pole, ocekavany_pocet_cisel,(int) pocet_radku);
	return overeni_parametru;
	}	

/* ______________________________________________________________________________________________

    int over_resitelnost(int *pole, int ocekavany_pocet_cisel, int pocet_radku)
    
    V teto funkci overuji resitelnost zadaneho stavu hlavolamu. Pokud je hlavolam resitelny, 
    je navratovou hodnotou 0, v opacnem pripade 4.
   ______________________________________________________________________________________________
*/
int over_resitelnost(int *pole, int ocekavany_pocet_cisel, int pocet_radku){
	int i,j;
	int pocet_inverzi = 0;
	int prazdne_misto;
	int dira;
	int hlavni;
	
	/*nejprve spoctu pocet inverzi - viz dokumentace */
	for (i = 0; i < ocekavany_pocet_cisel; i++){
		if (pole[i] != 0){
			for (j = i+1; j < ocekavany_pocet_cisel; j++){
				if ((pole[j] != 0) && (pole[i] > pole[j])){
					pocet_inverzi++;
				}
			}
		}
		else{
			prazdne_misto=(ocekavany_pocet_cisel-i-1)/(pocet_radku)+1;  
			dira=i;
		}
	}

	/*pokud je pocet invrzi lichy a pocer radku hlavolamu tako, pak neni hlavolam resitelny*/
	if ((pocet_radku % 2 == 1) && (pocet_inverzi % 2 == 1)){
		/*neplati matice licha inverze sude - nejde resir*/
		printf("ERR#4: Non-existent solution!");
		return 4;
	}
	else if (pocet_radku % 2 == 0){
		/*pokud je pocet radku, pocet inverzi a radkem s vyskytem prazdneho mista(od spodu) sudy, hlavolam neni resitelny*/
		if ((pocet_inverzi % 2 == 0) && (prazdne_misto % 2 == 0)){
			printf("ERR#4: Non-existent solution!");
			return 4;
		}
	 	/*pokud je pocet radku sudy a pocet inverzi a radek s vyskytem prazdneho mista lichy, hlavolam neni resitelny*/
		else if ((pocet_inverzi % 2 == 1) && (prazdne_misto == 1)){
			printf("ERR#4: Non-existent solution!");
			return 4;
		}
	}

	/*pokud funkce dojde az sem, zadany stav hlavolamu ma reseni a je zavolana hlavni smycka*/
	hlavni=hlavni_smycka(pole, ocekavany_pocet_cisel, pocet_radku, dira);
	return hlavni;
}


/* ______________________________________________________________________________________________

    int over_resitelnost(int *pole, int ocekavany_pocet_cisel, int pocet_radku)
    
    V teto funkci overuji resitelnost zadaneho stavu hlavolamu. Pokud je hlavolam resitelny,
	je navratovou hodnotou 0, v opacnem pripade 4.
   ______________________________________________________________________________________________
*/
int hlavni_smycka(int *pole, int ocekavany_pocet_cisel,int pocet_radku, int dira){
	int i;
	uzel *koren;
	uzel *prosly;
	uzel *uzlik;
	int hotovo = -1; /* nabyde hodnoty jedna, pokud je nalezeno reseni*/
	
	koren = tvor_strom(ocekavany_pocet_cisel,pole,NULL,0,NULL,NULL, pocet_radku) ;
	koren->vrchol_fronty = koren;
	
	uzlik = koren;
	prosly = koren;

	/*dokud neni nalezeno reseni*/
	while (hotovo == -1){
		/*pokud existuje vrchol fronty, nastavim uzlik prave na nej*/
		if (koren->vrchol_fronty != NULL){
				uzlik = koren->vrchol_fronty;
		}
		else {
			printf("ERR#6: Cannot continue!");
			return 6;
		}

		/*nalezeno diru*/
		for (i = 0; i < ocekavany_pocet_cisel; i++){   
			if( uzlik->pole[i] == 0){
				dira = i;
			}
		}
		
		uzlik->dira = dira;
		hotovo = expanduj(pocet_radku,ocekavany_pocet_cisel, uzlik,koren);
	}

	free(pole);
	uvolni_strom(koren);
	return hotovo;
}


/* ______________________________________________________________________________________________
 
    Main programu
   ______________________________________________________________________________________________
*/
int main(int argc, char *argv[]) {
	/* V pripade, ze je predan vice nez jeden parametr, progra vypise chybu a skonci.*/
	if (argv[2] != NULL){
		printf("ERR#7: To many arguments!");
		printf("\n Welcome to the program 15 puzzle solver. \n");
		printf ("\n Just enter one argument please. \n For example: solve15.exe \"8 5 11 4; 2 15 7 9; 13 _ 3 6; 12 14 15 10\"");
		return 7;
	}
	
	int overeni = over_parametry(argv[1]);
	return overeni;
	
	return 0;
}



