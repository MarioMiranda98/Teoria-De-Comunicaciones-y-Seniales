#include "FDFT.h"

void preparacionTDF(char *archivo, char* archivoSalida) {
    FILE* dArchivo = NULL;
    FILE* dArchivoSalida = NULL;
    Cabecera c = {0};
    int numeroMuestras = 0;
    short* signal = NULL;
    char* pie = NULL;
    int bytesPie = 0;
    double* signalTDF = NULL;
    short* muestras = NULL;

    dArchivo = abrirArchivo(archivo, 1);
    dArchivoSalida = abrirArchivo(archivoSalida, 2);
    
    //---------Obtencion de la señal de entrada--------------//
    obtenerCabecera(&c, dArchivo);
    bytesPie = (c.tamArchivo) - (c.tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);

    numeroMuestras = numeroDeMuestras(c.tamSubBloque2, c.bpm);
    signal = malloc(sizeof(short) * numeroMuestras);
    signal = muestrearSenial(dArchivo, numeroMuestras, signal);
    
    pie = obtenerPie(dArchivo, pie, bytesPie);
    mostrarCabecera(&c, archivo);
    
    cerrarArchivo(dArchivo);
    //!-------------Fin obtencion de la señal de entrada------//
    
    //Escribir la cabecera en la señal nueva
    cabeceraStereo(&c, dArchivoSalida);
    printf("\n");
    mostrarCabecera(&c, archivoSalida);//Cabecera archivo stereo
    
    //Creacion del arreglo de la señal de salida
    signalTDF = malloc(sizeof(double) * numeroMuestras * 2);
    muestras = malloc(sizeof(short) * numeroMuestras * 2);

    signalTDF = tdf(numeroMuestras, signal, signalTDF);//Obtenemos la señal transformada

    for (int i = 0; i < numeroMuestras * 2; i++) { //Escritura en el archivo
        muestras[i] = signalTDF[i];
    }

    fwrite(muestras, sizeof(short) * numeroMuestras * 2, 1, (FILE *) dArchivoSalida);

    colocarPie(&c, dArchivoSalida, pie);

    free(muestras);
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
            signalTDF[2 * k] += signal[n] * cos(((2 * PI) / numeroMuestras) * k * n);
            signalTDF[(2 * k) + 1] += signal[n] * sin(((2 * PI) / numeroMuestras) * k * n);
        }
        
        signalTDF[k] /= numeroMuestras;   
        signalTDF[(2 * k) + 1] *= -1;
    }

    return signalTDF;
}