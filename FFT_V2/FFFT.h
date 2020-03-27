#ifndef __FFFT__
#define __FFFT__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

void preparacionFFT(char *, char *);
int esPotenciaDos(int);
Cabecera *modificarCabecera(Cabecera *, int, int);
short *rellenar(short *, int, int);
float *fft(float *xr, float *xi, int numeroMuestras, float *signalFFT);
void swap(float *a, float *b);

#endif // !__FFFT__