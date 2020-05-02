#include "FMensaje.h"

void preparacionMensaje(char *archivo) {
    FILE *dArchivo = NULL;
    short* signal = NULL;
    short* segmento = NULL;
    Cabecera *c = NULL;
    const double INTERVALO = 32;
    int numeroMuestras = 0;
    int totalSegementos = 0;
    int inicio = 0;
    int final = 0;

    c = malloc(sizeof(Cabecera));
    dArchivo = abrirArchivo(archivo, 1);

    c = obtenerCabecera(c, dArchivo);
    numeroMuestras = numeroDeMuestras(c->tamSubBloque2, c->bpm);
    totalSegementos = numeroMuestras / INTERVALO;
    
    signal = malloc(sizeof(short) * numeroMuestras);
    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    cerrarArchivo(dArchivo);

    segmento = malloc(sizeof(short) * INTERVALO);

    final = INTERVALO;
    for(int i = 0; i < totalSegementos; i++) {
        segmento = obtenerSegmento(segmento, signal, inicio, final);
        
        printf("%c", obtenerCaracter(segmento, INTERVALO));

        inicio = final;
        final += (INTERVALO);
    }

    printf("\n");

    free(c);
    free(signal);
    free(segmento);
}

char obtenerCaracter(short *segmento, int intervalo) {
    double *signalTDF = malloc(sizeof(double) * intervalo);
    char caracter = 0x00;
    char a;

    signalTDF = tdf(intervalo, segmento, signalTDF);

    for(int i = 1, j = 0; i < intervalo / 2; i += 2, j++) {
       //printf("%lf\n", signalTDF[i]);

       if(signalTDF[i] > 2000) {
           a = pow(2, 7 - j);
           caracter |= a;
       }
    }
    
    free(signalTDF);
    return caracter;
}

short *obtenerSegmento(short *segmento, short *signal, int inicio, int final) {
    for(int i = inicio, j = 0; i < final; i++)
        segmento[j++] = signal[i];

    return segmento;
}

double *tdf(int numeroMuestras, short* signal, double* signalTDF) {
    double *xr = malloc(sizeof(double) * numeroMuestras);
    double *xi = malloc(sizeof(double) * numeroMuestras);
    for (int k = 0; k < numeroMuestras; k++) {
        xr[k] = 0;
        xi[k] = 0;
        for (int n = 0; n < numeroMuestras; n++) {
            xr[k] += signal[n] * cos((2 * PI * k * n) / numeroMuestras);
            xi[k] += (signal[n] * sin((2 * PI* k * n) / numeroMuestras)) * (-1);
        }
        
        xr[k] /= numeroMuestras;
        xi[k] /= numeroMuestras;
        signalTDF[k] = sqrt((pow(xr[k], 2) + pow(xi[k], 2)));
    }

    free(xr);
    free(xi);
    return signalTDF;
}