#include "FFFT.h"

void preparacionFFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    short* signal = NULL;
    double* signalFFT = NULL;
    Cabecera *c = NULL;
    Complejo **X = NULL;
    Complejo **Y = NULL;
    int numeroMuestras = 0;
    int numeroMuestrasNuevo = 0;
    char* pie = NULL;
    int bytesPie = 0;
    const int filas = 32;
    int columnas = 0;
    short* muestras = NULL;
    int no = 0;

    c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    c = obtenerCabecera(c, dArchivo);
    mostrarCabecera(c, archivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);

    if(!esPotenciaDos(numeroMuestras)) {
        int l = log2(numeroMuestras);
        l += 1;

        numeroMuestrasNuevo = pow(2, l);

        c = modificarCabecera(c, numeroMuestras, numeroMuestrasNuevo);

        signal = malloc(sizeof(short) * numeroMuestrasNuevo);
        signal = muestrearSenial(dArchivo, numeroMuestras, signal);

        signal = rellenar(signal, numeroMuestras, numeroMuestrasNuevo);

        columnas = numeroMuestrasNuevo / filas;
        signalFFT = malloc(sizeof(double) * (numeroMuestrasNuevo * 2));
        muestras = malloc(sizeof(short) * (numeroMuestrasNuevo * 2));
        no = numeroMuestrasNuevo;
    } else {
        signal = malloc(sizeof(short) * numeroMuestras);
        signal = muestrearSenial(dArchivo, numeroMuestras, signal);
        
        columnas = numeroMuestras / filas;
        signalFFT = malloc(sizeof(double) * (numeroMuestras * 2));
        muestras = malloc(sizeof(short) * (numeroMuestras * 2));
        no = numeroMuestras;
    }

    pie = obtenerPie(dArchivo, pie, bytesPie);
    cerrarArchivo(dArchivo);

    X = malloc(sizeof(Complejo *) * filas);
    Y = malloc(sizeof(Complejo *) * filas);

    for(int i = 0; i < filas; i++) {
        X[i] = malloc(sizeof(Complejo) * columnas);
        Y[i] = malloc(sizeof(Complejo) * columnas);
    }

    signalFFT = fft(signal, filas, columnas, X, Y, signalFFT, no);

    for(int i = 0; i < no * 2; i++)
        muestras[i] = signalFFT[i];

    dArchivoSalida = abrirArchivo(archivoSalida, 2);
    cabeceraStereo(c, dArchivoSalida);

    fwrite(muestras, c -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
    fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);

    cerrarArchivo(dArchivoSalida);

    free(c);
    free(signal);
    free(signalFFT);
    free(pie);
    free(X);
    free(Y);
    free(muestras);
}

double *fft(short *signal, int filas, int columnas, Complejo **X, Complejo **Y, double *signalFFT, int numeroMuesrtrasNuevas) {
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            Y[i][j].real = 0;
            Y[i][j].imaginario = 0;

            for(int k = 0; k < columnas; k++) {
                Y[i][j].real += signal[(filas * k) + i] * cos((2 * PI * j * k) / columnas);
                Y[i][j].imaginario += (signal[(filas * k) + i] * sin((2 * PI * j * k) / columnas)) * (-1);
            }

            Y[i][j].real = ((Y[i][j].real * cos((2 * PI * i * j) / numeroMuesrtrasNuevas)) + (Y[i][j].imaginario * sin((2 * PI * i * j) / numeroMuesrtrasNuevas)));
            Y[i][j].imaginario = ((Y[i][j].imaginario * cos((2 * PI * i * j) / numeroMuesrtrasNuevas)) - (Y[i][j].real * sin((2 * PI * i * j) / numeroMuesrtrasNuevas)));
        }
    }

    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            X[i][j].real = 0;
            X[i][j].imaginario = 0;
            for(int k = 0; k < filas; k++) {
                X[i][j].real += ((Y[k][j].real * cos((2 * PI * i * k) / filas)) + (Y[k][j].imaginario * sin((2 * PI * i * k) / filas)));
                X[i][j].imaginario += ((Y[k][j].imaginario * cos((2 * PI * i * k) / filas)) - (Y[k][j].real * sin((2 * PI * i * k) / filas)));
            }

            X[i][j].real /= numeroMuesrtrasNuevas;
            X[i][j].imaginario /= numeroMuesrtrasNuevas;
            signalFFT[(2 * (i * columnas + j))] = X[i][j].real;
            signalFFT[(2 * (i * columnas + j)) + 1] = X[i][j].imaginario;
        } 
    }

    return signalFFT;
}

int esPotenciaDos(int numeroMuestras) { //1 Si es potencia de dos, 0 si no lo es
    return ((numeroMuestras != 0) && ((numeroMuestras & (numeroMuestras - 1)) == 0)) ? 1 : 0;
}

Cabecera *modificarCabecera(Cabecera *c, int numeroMuestras, int numeroMuestrasNuevo) {
    int m = numeroMuestrasNuevo - numeroMuestras;
    
    c -> tamArchivo += (m * 2);
    c -> tamSubBloque2 += (m * 2);

    return c;
}

short *rellenar(short *signal, int numeroMuestras, int numeroMuestrasNuevo) {   
    for(int i = numeroMuestras; i < numeroMuestrasNuevo; i++)
        signal[i] = 0;

    return signal;
}
