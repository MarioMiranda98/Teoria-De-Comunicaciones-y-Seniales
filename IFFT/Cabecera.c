#include "Cabecera.h"

void mostrarCabecera(Cabecera *c, char *nombreArchivo) {
    printf("Archivo: %s\n", nombreArchivo);
    printf("Id Bloque: %s\n", c -> riff);
    printf("Tam del archivo: %d\n", c -> tamArchivo);
    printf("Formato: %s\n", c -> wave);
    printf("Id Sub Bloque 1: %s\n", c -> fmt);
    printf("Tam del sub bloque 1: %d\n", c -> tamSubBloque);
    (c -> tipoGrabacion == 1) ? printf("Formato tipo: PCM\n") : printf("Formato tipo: %d\n", c -> tipoGrabacion);
    printf("Numero de canales: %u\n", c -> numeroCanales);
    printf("Frecuencia de muestreo: %d Hz\n", c -> frecuenciaMuestreo);
    printf("Bytes por segundo: %d\n", c -> bps);
    printf("Alineamiento: %u\n", c -> alineamiento);
    printf("Bytes por muestra: %u\n", c -> bpm);
    printf("Id Sub Bloque 2: %s\n", c -> data);
    printf("Tam del sub bloque 2: %u\n", c -> tamSubBloque2);
}   
