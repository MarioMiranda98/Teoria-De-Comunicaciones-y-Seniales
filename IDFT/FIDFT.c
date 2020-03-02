#include "FIDFT.h"

void preparacionIDFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    Cabecera *c = malloc(sizeof(Cabecera));
    short *signal = NULL;
    short *canal1 = NULL;
    short *canal2 = NULL;
    double *signalIDFT = NULL;
    char *pie = NULL;
    int numeroMuestras = 0;
    int bytesPie = 0;


    //------------------Obtencion señal de entrada------------------//
    dArchivo = abrirArchivo(archivo, 1);

    c = obtenerCabecera(c, dArchivo);   
    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);
    pie = obtenerPie(dArchivo, pie, bytesPie);

    cerrarArchivo(dArchivo);
    //-----------------Fin obtencion señal de entrada-------------//

    //-----------------Obtener canales 1 y 2---------------------//
    canal1 = malloc(sizeof(short) * (numeroMuestras / 2));
    canal2 = malloc(sizeof(short) * (numeroMuestras / 2));

    canal1 = obtenerCanal1(signal, canal1, numeroMuestras);
    canal2 = obtenerCanal2(signal, canal2, numeroMuestras);
    //----------------Fin Obtener canales 1 y 2-----------------//


    free(signal);
    free(canal1);
    free(canal2);
    free(c);
    free(pie);
    free(signalIDFT);
}

double *idft(short *canal1, short *canal2, double *siganlIDFT, int numeroMuestras) {

    return siganlIDFT;
}