#ifndef __FIFFT__
#define __FIFFT__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

void preparacionIFFT(char *, char *);
float *ifft(float *xr, float *xi, int numeroMuestras, float *signalIFFT);
void swap(float *a, float *b);

#endif // !__FIFFT__