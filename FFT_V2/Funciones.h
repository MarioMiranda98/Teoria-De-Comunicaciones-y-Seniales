#ifndef __FUNCIONES__
#define __FUNCIONES__

#include "Cabecera.h"
#include <stdio.h>
#include <stdlib.h>

FILE* abrirArchivo(char *, int);
void cerrarArchivo(FILE *);
//void manejarCabecera(Cabecera *, FILE *, FILE *);
//void manejarPie(Cabecera *, FILE *, FILE *);

///////Modificacion de funciones////////////////////
Cabecera *obtenerCabecera(Cabecera *, FILE *);
short *muestrearSenial(FILE *, int, short *);
char *obtenerPie(FILE *, char*, int);
///////////////////////////////////////////////////

///////Funciones para TDF//////////////////
void cabeceraStereo(Cabecera *, FILE *);
void colocarPie(FILE *, char *, int);
///////////////////////////////////////////

int numeroDeMuestras(int, int);

#endif //!__FUNCIONES__