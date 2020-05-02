#include "Funciones.h"

FILE* abrirArchivo(char *archivo, int modo) {
    FILE* descriptor = NULL;

    if (modo == 1) 
        descriptor = fopen(archivo, "rb"); 
    else 
        descriptor = fopen(archivo, "wb");
    
    if(descriptor == NULL) { 
        printf("No se pudo abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    return descriptor;
}

void cerrarArchivo(FILE *dArchivo) {
    fclose(dArchivo);
}

Cabecera *obtenerCabecera(Cabecera* c, FILE* dArchivo) {
    fread(c, 44, 1, (FILE *) dArchivo);
    return c;
}

short *muestrearSenial(FILE* dArchivo, int numeroMuestras, short* signal) { 
    fread(signal, sizeof(short) * numeroMuestras, 1, (FILE *) dArchivo);
    return signal;
}

int numeroDeMuestras(int tamSubBloque2, int bpm) {
    return (tamSubBloque2 / (bpm / 8));
}