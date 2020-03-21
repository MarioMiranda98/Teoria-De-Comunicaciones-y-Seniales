#include "FIDFT.h"

void preparacionIDFT(char* archivo, char* archivoSalida) {
    FILE* dArchivo;
    FILE* dArchivoSalida;
    Cabecera *c = NULL;
    int numeroMuestras = 0;
    short* canal1 = NULL;
    short* canal2 = NULL;
    short* signal = NULL;
    char* pie = NULL;
    int bytesPie = 0;
    double* signalIDFT = NULL;
    short* muestras = NULL;

    dArchivo = abrirArchivo(archivo, 1);

    c = malloc(sizeof(Cabecera));
    c = obtenerCabecera(c, dArchivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);

    signal = malloc(sizeof(short) * (numeroMuestras));
    signal = muestrearSenial(dArchivo, (numeroMuestras), signal);

    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);
    pie = obtenerPie(dArchivo, pie, numeroMuestras);

    mostrarCabecera(c, archivo);

    cerrarArchivo(dArchivo);

    canal1 = malloc(sizeof(canal1) * (numeroMuestras / 2));
    canal2 = malloc(sizeof(canal2) * (numeroMuestras / 2));

    canal1 = obtenerCanal1(signal, canal1, numeroMuestras / 2);
    canal2 = obtenerCanal2(signal, canal2, numeroMuestras / 2);

    signalIDFT = malloc(sizeof(double) * (numeroMuestras));
    signalIDFT = idft(canal1, canal2, signalIDFT, numeroMuestras);

    muestras = malloc(sizeof(short) * numeroMuestras);

    for(int i = 0; i < numeroMuestras; i++) { 
        muestras[i] = signalIDFT[i];
    }

    dArchivoSalida = abrirArchivo(archivoSalida, 2);
    fwrite(c, 44, 1, (FILE *) dArchivoSalida);

    fwrite(muestras, c -> tamSubBloque2, 1, (FILE *) dArchivoSalida);

    fwrite(pie, sizeof(char) * bytesPie, 1, (FILE *) dArchivoSalida);

    cerrarArchivo(dArchivoSalida);

    free(c);
    free(signal);
    free(pie);
    free(canal1);
    free(canal2);
    free(signalIDFT);
    free(muestras);
}

double *idft(short *canal1, short *canal2, double *signalIDFT, int numeroMuestras) {
    double cons = ((2 * PI) / (numeroMuestras / 2));

    for(int n = 0; n < (numeroMuestras / 2); n++) {
        signalIDFT[2 * n] = 0;
        signalIDFT[(2 * n) + 1] = 0;

        for(int k = 0; k < (numeroMuestras / 2); k++) {
            signalIDFT[2 * n] += ((canal1[k] * cos(cons * k * n)) - (canal2[k] * sin(cons * k * n)));
        }

        if(signalIDFT[2 * n] > 32767)
            signalIDFT[2 * n] = 32767;
        else if(signalIDFT[2 * n] < -32768)
            signalIDFT[2 * n] = -32768;

        for(int k = 0; k < (numeroMuestras / 2); k++) {
            signalIDFT[(2 * n) + 1] += ((canal1[k] * sin(cons * k * n)) + (canal2[k] * cos(cons * k * n)));
        }

        if(signalIDFT[(2 * n) + 1] > 32767)
            signalIDFT[(2 * n) + 1] = 32767;
        else if(signalIDFT[(2 * n) + 1] < -32768)
            signalIDFT[(2 * n) + 1] = -32768;
    }

    return signalIDFT;
}
