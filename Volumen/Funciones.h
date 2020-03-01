#ifndef __FUNCIONES__
#define __FUNCIONES__

#include "Cabecera.h"
#include <stdio.h>
#include <stdlib.h>

FILE* abrirArchivo(char *, int);
void cerrarArchivo(FILE *);
void manejarCabecera(Cabecera *, FILE *, FILE *);
void manejarPie(Cabecera *, FILE *, FILE *);
int numeroDeMuestras(int, int);

#endif //!__FUNCIONES__