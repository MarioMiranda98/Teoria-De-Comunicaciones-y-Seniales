#ifndef __FUNCIONES__
#define __FUNCIONES__

#include "Cabecera.h"
#include <stdio.h>
#include <stdlib.h>

FILE* abrirArchivo(char *, int);
void cerrarArchivo(FILE *);
//void manejarCabecera(Cabecera *, FILE *, FILE *);
//void manejarPie(Cabecera *, FILE *, FILE *);
Cabecera *obtenerCabecera(Cabecera *, FILE *);
short *muestrearSenial(FILE *, int, short *);
char *obtenerPie(FILE *, char*, int);
void colocarPie(FILE *, char *, int);
short *obtenerCanal1(short *, short *, int);
short *obtenerCanal2(short *, short *, int);
int numeroDeMuestras(int, int);

#endif //!__FUNCIONES__