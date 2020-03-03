#ifndef __FIDFT__
#define __FIDFT__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

void preparacionIDFT(char *, char *);
double *idft(short *, short *, double *, int);

#endif // !__FIDFT__