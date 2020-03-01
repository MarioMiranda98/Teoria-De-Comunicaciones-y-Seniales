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

void manejarCabecera(Cabecera *c, FILE* dArchivo, FILE* dArchivoSalida) {
    fread(c, 44, 1, (FILE *) dArchivo);
    fwrite(c, 44, 1, (FILE *)dArchivoSalida);
}

void manejarPie(Cabecera *c, FILE* dArchivo, FILE* dArchivoSalida) {
    int pie = c -> tamArchivo - c -> tamSubBloque2 - 36;
    char* bytesPie = malloc(sizeof(char) * pie);
    fread(bytesPie, sizeof(char) * pie, 1, (FILE *) dArchivo);
    fwrite(bytesPie, sizeof(char) * pie, 1, (FILE *) dArchivoSalida);

    free(bytesPie);
}

int numeroDeMuestras(int tamSubBloque2, int bpm) {
    return (tamSubBloque2 / (bpm / 8));
}