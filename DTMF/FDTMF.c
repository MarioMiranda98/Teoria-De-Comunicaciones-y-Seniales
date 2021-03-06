#include "FDTMF.h"

void preparacionDTMF(char *archivo) {
    FILE *dArchivo = NULL;
    short* signal = NULL;
    int numeroMuestras;
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
    signalDFT = malloc(sizeof(double) * numeroMuestras);
    muestras = malloc(sizeof(short) * numeroMuestras);

    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    cerrarArchivo(dArchivo);

    double duracion = (double) (((double) numeroMuestras) / (double) fMuestreo);
    printf("Duracion:%lf\n", duracion);
    signalDFT = tdf(numeroMuestras, signal, signalDFT);

    for(int i = 0; i < numeroMuestras; i++) {
        muestras[i] = signalDFT[i];
        if(absoluto(muestras[i]) > 5000)
            printf("%d: %d\n", i, muestras[i]);
    }
  
    dtmf(muestras, duracion);

    free(c);
    free(signal);
    free(signalDFT);
}

void dtmf(short *muestras, double duracion) {
    int frecuenciasDTMF[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    int posicion[8];
    int j, k;
    j = 0;
    
    char teclado[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    short f;

    for(int i = 0; i < 8; i++) {
        int pos = (int)(frecuenciasDTMF[i] * duracion);
        printf("%d, %d\n", pos, (pos + 1));
        f = absoluto(muestras[pos]);

        if(f > 500) 
            posicion[i] = 1;
        else 
            posicion[i] = 0;
    }

    for(int i = 0; i < 4; i++) {
        if(posicion[i]) {
            if(posicion[4]) {
                printf("%c", teclado[i][0]);
            }
            
            if(posicion[5]) {
                printf("%c", teclado[i][1]);
            }

            if(posicion[6]) {
                printf("%c", teclado[i][2]);
            }

            if(posicion[7]) {
                printf("%c", teclado[i][3]);
            }
        }
    }

    printf("\n");
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

    return signalTDF;
}

short absoluto(short n) {
    if(n < 0)
        n *= (-1);
    
    return n;
}