#ifndef __FUNCIONES__
#define __FUNCIONES__

#include "Cabecera.h"
#include <stdio.h>
#include <stdlib.h>

FILE* abrirArchivo(char *, int);
void cerrarArchivo(FILE *);
Cabecera *obtenerCabecera(Cabecera *, FILE *);
short *muestrearSenial(FILE *, int, short *);
int numeroDeMuestras(int, int);

#endif //!__FUNCIONES__