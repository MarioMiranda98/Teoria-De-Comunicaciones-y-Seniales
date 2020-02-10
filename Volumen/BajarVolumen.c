#include "BajarVolumen.h"

void bajarVolumen(char *archivo, char *archivoSalida) {
    FILE* dArchivo = NULL;
    FILE* dArchivoSalida = NULL;
    short int muestra;
    Cabecera* c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    dArchivoSalida = abrirArchivo(archivoSalida, 2);

    manejarCabecera(c, dArchivo, dArchivoSalida);
    mostrarCabecera(c, archivo);

    for (int i = 0; i < numeroDeMuestras(c -> tamSubBloque2, c -> bpm); i++) {
        fread(&muestra, sizeof(short int), 1, (FILE *) dArchivo);
        muestra /= 2;
        fwrite(&muestra, sizeof(short int), 1, (FILE *) dArchivoSalida);
    }

    manejarPie(c, dArchivo, dArchivoSalida);

    free(c);
    cerrarArchivo(dArchivo);
    cerrarArchivo(dArchivoSalida);
}