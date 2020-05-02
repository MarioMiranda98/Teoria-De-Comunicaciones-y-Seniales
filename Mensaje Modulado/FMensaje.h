#ifndef __FMENSAJE__
#define __FMENSAJE__

#include "Funciones.h"
#include <math.h>
#define PI 3.1415926535

void preparacionMensaje(char *);
char obtenerCaracter(short *, int);
short *obtenerSegmento(short *, short *, int, int);
double *tdf(int, short *, double *);

#endif // !__FMENSAJE__