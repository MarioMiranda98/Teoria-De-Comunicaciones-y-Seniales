#include "FFFT.h"

void preparacionFFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    short* signal = NULL;
    double* signalFFT = NULL;
    Cabecera *c = NULL;
    int numeroMuestras = 0;
    int numeroMuestrasNuevo = 0;
    char* pie = NULL;
    int bytesPie = 0;

    c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    c = obtenerCabecera(c, dArchivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);

    if(!esPotenciaDos(numeroMuestras)) {
        int l = log2(numeroMuestras);
        l += 1;

        numeroMuestrasNuevo = pow(2, l);

        c = modificarCabecera(c, numeroMuestras, numeroMuestrasNuevo);

        signal = malloc(sizeof(short) * numeroMuestrasNuevo);
        signal = muestrearSenial(dArchivo, numeroMuestrasNuevo, signal);

        signal = rellenar(signal, numeroMuestras, numeroMuestrasNuevo);
    } else {
        signal = malloc(sizeof(short) * numeroMuestras);
        signal = muestrearSenial(dArchivo, numeroMuestras, signal);
    }

    pie = obtenerPie(dArchivo, pie, bytesPie);
    cerrarArchivo(dArchivo);

    free(c);
    free(signal);
    free(signalFFT);
    free(pie);
}

double *fft(int filas, int columnas, Complejo **X, Complejo **Y) {

}

int esPotenciaDos(int numeroMuestras) { //1 Si es potencia de dos, 0 si no lo es
    return ((numeroMuestras != 0) && ((numeroMuestras & (numeroMuestras - 1)) == 0)) ? 1 : 0;
}

Cabecera *modificarCabecera(Cabecera *c, int numeroMuestras, int numeroMuestrasNuevo) {
    int m = numeroMuestras - numeroMuestrasNuevo;
    
    c -> tamArchivo += (m * 2);
    c -> tamSubBloque2 += (m * 2);

    return c;
}

short *rellenar(short *signal, int numeroMuestras, int numeroMuestrasNuevo) {   
    for(int i = numeroMuestras; i < numeroMuestrasNuevo; i++)
        signal[i] = 0;

    return signal;
}