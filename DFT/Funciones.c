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

/*void manejarCabecera(Cabecera *c, FILE* dArchivo, FILE* dArchivoSalida) {
    fread(c, 44, 1, (FILE *) dArchivo);
    fwrite(c, 44, 1, (FILE *) dArchivoSalida);
}*/

/*void manejarPie(Cabecera *c, FILE* dArchivo, FILE* dArchivoSalida) {
    int pie = c -> tamArchivo - c -> tamSubBloque2 - 36;
    char* bytesPie = malloc(sizeof(char) * pie);
    fread(bytesPie, sizeof(char) * pie, 1, (FILE *) dArchivo);

    fwrite(bytesPie, sizeof(char) * pie, 1, (FILE *) dArchivoSalida);
    free(bytesPie);
}*/

//--------------Funciones de obtencion señal entrada-------------//
Cabecera *obtenerCabecera(Cabecera* c, FILE* dArchivo) {
    fread(c, 44, 1, (FILE *) dArchivo);
    return c;
}

short *muestrearSenial(FILE* dArchivo, int numeroMuestras, short* signal) { 
    fread(signal, sizeof(short) * numeroMuestras, 1, (FILE *) dArchivo);
    return signal;
}

char *obtenerPie(FILE* dArchivo, char* pie, int bytesPie) {
    fread(pie, bytesPie, 1, (FILE *) dArchivo);
    return pie;
}
//!------Fin funciones de obtencion Señal de entrada-----------//

//----------Funciones de conversion a stereo-------------------//
void cabeceraStereo(Cabecera* c, FILE* dArchivoSalida) {
    //Conversion cabecera (Mono a Stereo)
    c -> tamArchivo = (c -> tamArchivo + c -> tamSubBloque2);
    c -> numeroCanales = 2;
    c -> bps *= 2;
    c -> alineamiento *= 2;
    c -> tamSubBloque2 = (c -> tamSubBloque2 * 2);

    fwrite(c, 44, 1, (FILE *) dArchivoSalida);
}

void colocarPie(FILE* dArchivoSalida, char *pie, int bytesPie) {
    fwrite(pie, sizeof(short) * bytesPie, 1, (FILE *) dArchivoSalida);
}
//!---------Fin Funciones de conversion a stereo---------------//

int numeroDeMuestras(int tamSubBloque2, int bpm) {
    return (tamSubBloque2 / (bpm / 8));
}