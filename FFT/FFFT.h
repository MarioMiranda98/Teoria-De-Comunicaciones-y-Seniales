#ifndef __FFFT__
#define __FFFT__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

struct Complejo {
    double real;
    double imaginario;
};

typedef struct Complejo Complejo;

void preparacionFFT(char *, char *);
int esPotenciaDos(int);
Cabecera *modificarCabecera(Cabecera *, int, int);
short *rellenar(short *, int, int);
double *fft(short *, int, int, Complejo **, Complejo **, double *, int);

#endif // !__FFFT__