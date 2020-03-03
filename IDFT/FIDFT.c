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

    signal = malloc(sizeof(short) * numeroMuestras);
    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);
    pie = obtenerPie(dArchivo, pie, numeroMuestras);

    mostrarCabecera(c, archivo);

    cerrarArchivo(dArchivo);

    canal1 = malloc(sizeof(canal1) * (numeroMuestras / 2));
    canal2 = malloc(sizeof(canal2) * (numeroMuestras / 2));

    canal1 = obtenerCanal1(signal, canal1, numeroMuestras);
    canal2 = obtenerCanal2(signal, canal2, numeroMuestras);

    signalIDFT = malloc(sizeof(double) * numeroMuestras);
    signalIDFT = idft(canal1, canal2, signalIDFT, numeroMuestras);

    muestras = malloc(sizeof(short) * numeroMuestras);

    for(int i = 0; i < numeroMuestras; i++) {
        /*signalIDFT[i] /= 32767;

        if(signalIDFT[i] > 1)
            muestras[i] = 32767;
        else if(signalIDFT[i] < -1)
            muestras[i] = -32767;
        else 
            muestras[i] = signalIDFT[i] * 32767;*/

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

double *idft(short *canal1, short *canal2, double *siganlIDFT, int numeroMuestras) {
    //Deduccion IDFT
    //X[n] = Sum (X[k])* (cos() + jsin())
    //X[n] = Sum (r + jw) *(cos() + jsin()
    //X[n] = Sum r * cos() + r * sin() + jw * cos() - w sin()
    //r * cos() - w sin()
    //r * sin() + w cos()
    //r -> canal1
    //w -> canal2 

    for(int k = 0; k < (numeroMuestras / 2); k++) {
        siganlIDFT[2 * k] = 0;
        siganlIDFT[(2 * k) + 1] = 0;
        for(int n = 0; n < (numeroMuestras / 2); n++) {
            siganlIDFT[2 * k] += (canal1[n] * cos((2 * PI * k * n) / numeroMuestras))
                                -(canal2[n] * sin((2 * PI * k * n) / numeroMuestras));

            siganlIDFT[(2 * k) + 1] += (canal1[n] * sin((2 * PI * k * n) / numeroMuestras))
                                        +(canal2[n] * cos((2 * PI * k * n) / numeroMuestras));
        }
    }

    return siganlIDFT;
}