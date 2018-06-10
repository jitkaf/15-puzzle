/*
	Resic hlavolamu Loydova patnactka.
	Verze 1.0
	
	Dialekt : ANSI C
    Kompilator: ANSI C-compatible
    
    Copyright (c) Jitka Furbacherova, 2016
    KIV/PC

*/

#ifndef PROCHAZENI_H
#define PROCHAZENI_H


/* ____________________________________________________________________________
 
   Prototypy funkci.
   ____________________________________________________________________________
*/
int expanduj( int pocet_radku, int pocet_zadanych_cisel, uzel* uzlik, uzel *koren);
uzel *vlevo(uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel);
uzel *vpravo(uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel);
uzel *nahoru( uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel);
uzel *dolu(uzel *uzlik, uzel *predek,uzel *koren, int pocet_radku, int  pocet_zadanych_cisel);
void vypis_postup(uzel *uzlik);

#endif
