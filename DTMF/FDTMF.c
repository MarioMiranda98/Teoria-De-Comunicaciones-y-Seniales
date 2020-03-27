#include "FDTMF.h"

void preparacionDTMF(char *archivo) {
    FILE *dArchivo = NULL;
    short* signal = NULL;
    int numeroMuestras;
    double duracion;
    int fMuestreo;
    double *signalDFT = NULL;
    Cabecera *c = NULL;
    short *muestras = NULL;

    dArchivo = abrirArchivo(archivo, 1);

    c = malloc(sizeof(Cabecera));
    c = obtenerCabecera(c, dArchivo);
    numeroMuestras = numeroDeMuestras(c->tamSubBloque2, c->bpm);
    fMuestreo = c->frecuenciaMuestreo;

    signal = malloc(sizeof(short) * numeroMuestras);
    signalDFT = malloc(sizeof(double) * numeroMuestras * 2);
    muestras = malloc(sizeof(short) * numeroMuestras * 2);

    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    cerrarArchivo(dArchivo);

    signalDFT = tdf(numeroMuestras, signal, signalDFT);
    duracion = (double)((double)fMuestreo / (double)numeroMuestras);

    for(int i = 0; i < numeroMuestras * 2; i++) 
        muestras[i] = signalDFT[i];

  
    dtmf(muestras, duracion);

    free(c);
    free(signal);
    free(signalDFT);
}

void dtmf(short *muestras, double duracion) {
    int frecuenciasDTMF[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633 };
    int posicion[2];
    int j, k;
    j = k = 0;
    
    char teclado[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    short f;

    for(int i = 0; i < 8; i++) {
        int pos = (int)(frecuenciasDTMF[i] / duracion);
        f = absoluto(muestras[2 * pos]);

        if(f > 7000) {
            posicion[j++] = k;
        }

        f = absoluto(muestras[2 * pos + 1]);
        if(f > 7000) {
            posicion[j++] = k;
        }

        //printf("%d\n", f);

        if(i == 3)
            k = 0;
        else 
            k++;
    }

    //printf("[%d][%d]\n", posicion[0], posicion[1]);

    printf("%c\n", teclado[posicion[0]][posicion[1]]);
}

double *tdf(int numeroMuestras, short* signal, double* signalTDF) {
    for (int k = 0; k < numeroMuestras; k++) {
        signalTDF[2 * k] = 0;
        signalTDF[(2 * k) + 1] = 0;
        for (int n = 0; n < numeroMuestras; n++) {
            signalTDF[2 * k] += signal[n] * cos((2 * PI * k * n) / numeroMuestras);
            signalTDF[(2 * k) + 1] += (signal[n] * sin((2 * PI* k * n) / numeroMuestras)) * (-1);
        }
        
        signalTDF[2 * k] /= numeroMuestras;   
        signalTDF[(2 * k) + 1] /= numeroMuestras;
    }

    return signalTDF;
}

short absoluto(short n) {
    if(n < 0)
        n *= (-1);
    
    return n;
}