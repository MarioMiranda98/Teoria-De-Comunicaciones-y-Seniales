#include "FDFT.h"

void preparacionTDF(char *archivo, char* archivoSalida) {
    FILE* dArchivo = NULL;
    FILE* dArchivoSalida = NULL;
    Cabecera *c = malloc(sizeof(Cabecera));
    int numeroMuestras = 0;
    short* signal = NULL;
    int bytesPie = 0;
    double* signalTDF = NULL;
    short* muestras = NULL;
    char* pie = NULL;

    dArchivo = abrirArchivo(archivo, 1);
    dArchivoSalida = abrirArchivo(archivoSalida, 2);
    
    c = obtenerCabecera(c, dArchivo);
 
    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    signal = malloc(sizeof(short) * numeroMuestras);
    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);
    pie = obtenerPie(dArchivo, pie, bytesPie);

    mostrarCabecera(c, archivo);

    //Escribir la cabecera en la señal nueva
    cabeceraStereo(c, dArchivoSalida);
    printf("\n");
    mostrarCabecera(c, archivoSalida);//Cabecera archivo stereo
    
    //Creacion del arreglo de la señal de salida
    signalTDF = malloc(sizeof(double) * numeroMuestras * 2);
    muestras = malloc(sizeof(short) * numeroMuestras * 2);

    signalTDF = tdf(numeroMuestras, signal, signalTDF);//Obtenemos la señal transformada

    for (int i = 0; i < numeroMuestras * 2; i++) { 
        muestras[i] = signalTDF[i];
    }

    fwrite(muestras, c -> tamSubBloque2, 1, (FILE *) dArchivoSalida);//Escritura en el archivo

    colocarPie(dArchivoSalida, pie, bytesPie);

    free(muestras);
    free(c);
    free(signal);
    free(signalTDF);
    free(pie);
    cerrarArchivo(dArchivoSalida);
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