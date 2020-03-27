/*
Escriba un programa que reciba el nombre de un archivo wav conteniendo el sonido generado al 
presionar una tecla de un telefono (codigo DTMF) y cuya unica salida sea la impresion de la 
tecla que lo genero. El archivo sera monoaural con frecuencia de muestreo de 44100 muestras
por segundo pero la duracion es variable.

dtmf entrada.wav
*/

#include "FDTMF.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Error en la entrada\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma archivo.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionDTMF(argv[1]);
}