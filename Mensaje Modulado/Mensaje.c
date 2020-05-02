/*
    Se tiene un archivo .wav que contiene un mensaje codificado; cada .016 segundos se tiene
    una letra cuyo codigo ascii se codifico modulando cada bit de los ocho del codigo con una
    frecuencia multiplo impar de 62.5 Hz, es decir al bit mas significativo (128) le corresponde
    una frecuencia de 62.5Hz, al siguiente (64) le corresponde una frecuencia de 187.5Hz 
    (tres veces 62.5) etc.

    Hay que decodificar el mensaje. 
*/

#include "FMensaje.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Error en la entrada\n");
        printf("Ejemplo\n");
        printf("./Programa archivo.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionMensaje(argv[1]);
}