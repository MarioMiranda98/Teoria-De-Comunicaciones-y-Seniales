#include "FIFFT.h"

void preparacionIFFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    short* signal = NULL;
    short* canal1 = NULL;
    short* canal2 = NULL;
    Cabecera *c = NULL;
    char *pie = NULL;
    Complejo **X = NULL;
    Complejo **Y = NULL;
    int numeroMuestras = 0;
    int numeroMuestrasCanales = 0;
    int bytesPie = 0;
    const int filas = 32;
    int columnas = 0;
    short *muestras = NULL;
    double *signalIFFT = NULL;

    c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    c = obtenerCabecera(c, dArchivo);
    mostrarCabecera(c, archivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    numeroMuestrasCanales = (numeroMuestras / 2);
    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    columnas = numeroMuestrasCanales / filas;

    signal = malloc(sizeof(short) * numeroMuestras);
    canal1 = malloc(sizeof(short) * numeroMuestrasCanales);
    canal2 = malloc(sizeof(short) * numeroMuestrasCanales);
    pie = malloc(sizeof(char) * bytesPie);

    signal = muestrearSenial(dArchivo, numeroMuestras, signal);
    canal1 = obtenerCanal1(signal, canal1, numeroMuestrasCanales);
    canal2 = obtenerCanal2(signal, canal2, numeroMuestrasCanales);

    pie = obtenerPie(dArchivo, pie, bytesPie);

    cerrarArchivo(dArchivo);

    X = malloc(sizeof(Complejo *) * filas);
    Y = malloc(sizeof(Complejo *) * filas);

    for(int i = 0; i < filas; i++) {
        X[i] = malloc(sizeof(Complejo) * columnas);
        Y[i] = malloc(sizeof(Complejo) * columnas);
    }

    signalIFFT = malloc(sizeof(double) * numeroMuestras);
    muestras = malloc(sizeof(short) * numeroMuestras);

    signalIFFT = ifft(canal1, canal2, X, Y, filas, columnas, numeroMuestras, signalIFFT);

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
    free(X);
    free(Y);
    free(signalIFFT);
    free(muestras);
}

double *ifft(short *canal1, short *canal2, Complejo **X, Complejo **Y, int filas, int columnas, int numeroMuestras, double *signalIFFT) {
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            Y[i][j].real = 0;
            Y[i][j].imaginario = 0;

            for(int k = 0; k < columnas; k++) {
                Y[i][j].real += canal1[(filas * k) + i] * cos((2 * PI * j * k) / columnas) - (canal2[(filas * k) + i] * sin((2 * PI * j * k) / columnas));
                Y[i][j].imaginario += (canal1[(filas * k) + i] * sin((2 * PI * j * k) / columnas)) + (canal2[(filas * k) + i] * cos((2 * PI * j * k) / columnas));
            }

            Y[i][j].real = ((Y[i][j].real * cos((2 * PI * i * j) / numeroMuestras)) - (Y[i][j].imaginario * sin((2 * PI * i * j) / numeroMuestras)));
            Y[i][j].imaginario = ((Y[i][j].imaginario * cos((2 * PI * i * j) / numeroMuestras)) + (Y[i][j].real * sin((2 * PI * i * j) / numeroMuestras)));
        }
    }

    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            X[i][j].real = 0;
            X[i][j].imaginario = 0;
            for(int k = 0; k < filas; k++) {
                X[i][j].real += ((Y[k][j].real * cos((2 * PI * i * k) / filas)) - (Y[k][j].imaginario * sin((2 * PI * i * k) / filas)));
                X[i][j].imaginario += ((Y[k][j].imaginario * cos((2 * PI * i * k) / filas)) + (Y[k][j].real * sin((2 * PI * i * k) / filas)));
            }

            signalIFFT[(2 * (i * columnas + j))] = X[i][j].real;
            signalIFFT[(2 * (i * columnas + j)) + 1] = X[i][j].imaginario;
        } 
    }

    return signalIFFT;
}