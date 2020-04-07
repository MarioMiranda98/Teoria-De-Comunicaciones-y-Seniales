#include "FIFFT.h"

void preparacionIFFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    short* signal = NULL;
    short* canal1 = NULL;
    short* canal2 = NULL;
    Cabecera *c = NULL;
    char *pie = NULL;
    int numeroMuestras = 0;
    int numeroMuestrasCanales = 0;
    int bytesPie = 0;
    short *muestras = NULL;
    float *signalIFFT = NULL;
    float *xr = NULL;
    float *xi = NULL;

    c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    c = obtenerCabecera(c, dArchivo);
    mostrarCabecera(c, archivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    numeroMuestrasCanales = (numeroMuestras / 2);
    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;

    signal = malloc(sizeof(short) * numeroMuestras);
    canal1 = malloc(sizeof(short) * numeroMuestrasCanales);
    canal2 = malloc(sizeof(short) * numeroMuestrasCanales);
    xr = malloc(sizeof(float) * numeroMuestrasCanales);
    xi = malloc(sizeof(float) * numeroMuestrasCanales);
    pie = malloc(sizeof(char) * bytesPie);

    signal = muestrearSenial(dArchivo, numeroMuestras, signal);
    canal1 = obtenerCanal1(signal, canal1, numeroMuestrasCanales);
    canal2 = obtenerCanal2(signal, canal2, numeroMuestrasCanales);

    pie = obtenerPie(dArchivo, pie, bytesPie);

    cerrarArchivo(dArchivo);
  
    for(int i = 0; i < numeroMuestrasCanales; i++) {
        xr[i] = canal1[i];
        xi[i] = canal2[i];
    }

    signalIFFT = malloc(sizeof(float) * numeroMuestras);
    muestras = malloc(sizeof(short) * numeroMuestras);

    signalIFFT = ifft(xr, xi, numeroMuestrasCanales, signalIFFT);

    for(int i = 0; i < numeroMuestras; i++) {
        muestras[i] = signalIFFT[i];
    }

    dArchivoSalida = abrirArchivo(archivoSalida, 2);

    fwrite(c, 44, 1, (FILE *) dArchivoSalida);
    fwrite(muestras, c -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
    colocarPie(dArchivoSalida, pie, bytesPie);

    cerrarArchivo(dArchivoSalida);

    free(c);
    free(signal);
    free(canal1);
    free(canal2);
    free(pie);
    free(signalIFFT);
    free(muestras);
    free(xr);
    free(xi);
}

float *ifft(float *xr, float *xi, int numeroMuestras, float *signalIFFT) {
    int m, n, i, j, k, j1;
    float arg, s, c, tempr, tempi, w;
    
    m = log((float) numeroMuestras) / log(2.0);

    for(i = 0; i < numeroMuestras; ++i) {
        j = 0;
        for(k = 0; k < m; ++k) 
            j = (j << 1) | (1 & (i >> k));

        if(j < i) {
            swap(&xr[i], &xr[j]);
            swap(&xi[i], &xi[j]);
        }
    }

    for(i = 0; i < m; i++) {
        n = w = pow(2.0, (float) i);
        w = PI / n;

        k = 0;

        while(k < (numeroMuestras - 1)) {
            for(j = 0; j < n; j++) {
                arg = -j * w;
                c = cos(arg);
                s = sin(arg);
                j1 = k + j;
                tempr = xr[j1 + n] * c - xi[j1 + n] * s;
                tempi = xi[j1 + n] * c + xr[j1 + n] * s;
                xr[j1 + n] = xr[j1] + tempr;
                xi[j1 + n] = xi[j1] + tempi;
                xr[j1] = xr[j1] - tempr;
                xi[j1] = xi[j1] - tempi; 
            }
            k += 2 * n;
        }
    }


    for(i = 0; i < numeroMuestras; i++) {
        signalIFFT[2 * i] = xr[i];
        signalIFFT[(2 * i) + 1] = xi[i];
    }

    return signalIFFT;
}

void swap(float *a, float *b) {
    float aux;
    
    aux = *a;
    *a = *b;
    *b = aux; 
}
