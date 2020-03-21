#ifndef __FIFFT__
#define __FIFFT__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

struct Complejo {
    double real;
    double imaginario;
};

typedef struct Complejo Complejo;

void preparacionIFFT(char *, char *);
double *ifft(short *, short *, Complejo **, Complejo **, int, int, int, double *);

#endif // !__FIFFT__