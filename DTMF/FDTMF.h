#ifndef __FDTMF__
#define __FDTMF__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

void preparacionDTMF(char *);
double *tdf(int numeroMuestras, short* signal, double *);
void dtmf(short *, int, int);
short absoluto(short);

#endif // !__FDTMF__
