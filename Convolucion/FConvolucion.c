#include "FConvolucion.h"

float* crearFiltro(float *filtro, int cantidadMuestras) {
    int fMuestreo = 44100;
    int fCorte = 2000;

    double c = ((2 * PI * fCorte) / (fMuestreo));

    for (int t = 0; t < cantidadMuestras; t += 1) {
        filtro[t] = exp(-t * c);
    }

    return filtro;
}

float absoluto(float valor) {
    if (valor < 0) {
        valor *= (-1);
    }

    return valor;
}

void convolucion(char *archivo, char *archivoSalida) {
    FILE* dArchivo = NULL;
    FILE* dArchivoSalida = NULL;
    int cantidadMuestras = 100;
    Cabecera *c = malloc(sizeof(Cabecera));
    
    short muestra;
    float* filtro = NULL;
    float* conv = NULL;
    float* entrada = NULL;
    float* respuesta = NULL;
    float salidaMaxima = 0;
    float entradaMaxima = 0;
    int numeroMuestras = 0;

    filtro = malloc(sizeof(float) * cantidadMuestras);

    dArchivo = abrirArchivo(archivo, 1);
    dArchivoSalida = abrirArchivo(archivoSalida, 2);

    manejarCabecera(c, dArchivo, dArchivoSalida);
    mostrarCabecera(c, archivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    entrada = malloc(sizeof(float) * numeroMuestras);
    respuesta = malloc(sizeof(float) * numeroMuestras);

    filtro = crearFiltro(filtro, cantidadMuestras);
    conv = malloc(sizeof(float) * cantidadMuestras);
    memset(conv, 0, sizeof(conv));
    memset(respuesta, 0, sizeof(respuesta));

    for (int i = 0; i < numeroMuestras; i += 1) {
        fread(&muestra, sizeof(short), 1, (FILE *) dArchivo);
        entrada[i] = muestra;
    }

    entradaMaxima = absoluto(entrada[0]);
    for(int i = 1; i < numeroMuestras; i += 1) {
        if(entradaMaxima < absoluto(entrada[i])) 
            entradaMaxima = absoluto(entrada[i]);
    }

    if(entradaMaxima > 32767)
        entradaMaxima = 32767;
    
    //Convolucion
    for (int i = 0; i < numeroMuestras; i += 1) {
        for (int k = cantidadMuestras - 1; k >= 1; k--) conv[k] = conv[k - 1];
        conv[0] = entrada[i];
        for (int k = 0; k < cantidadMuestras; k++) respuesta[i] += filtro[k] * conv[k];
    }


    salidaMaxima = absoluto(respuesta[0]);
    for(int i = 1; i < numeroMuestras; i += 1) {
        if(salidaMaxima < absoluto(respuesta[i])) 
            salidaMaxima = absoluto(respuesta[i]);
    }

    float escala = (entradaMaxima / salidaMaxima);

    for(int i = 0; i < numeroMuestras; i += 1) {
        muestra = respuesta[i] * escala;
        fwrite(&muestra, sizeof(short), 1, (FILE *) dArchivoSalida);
    }

    manejarPie(c, dArchivo, dArchivoSalida);

    free(c);
    free(filtro);
    free(entrada);
    free(conv);
    cerrarArchivo(dArchivo);
    cerrarArchivo(dArchivoSalida);
}