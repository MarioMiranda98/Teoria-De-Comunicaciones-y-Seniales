/*
    Escriba un programa que simule un circuito RC en configuracion de pasabajas con frecuencia
    de corte de 2000Hz (2KHz) y una frecuencia de muestreo de 44100 muestras por segundo,
    mediante la convolucion de la respuesta a dicho impulso de dicho circuito con el archivo
    wav de entrada; generando un archivo wav con la salida; los nombres los archivos se pasan
    por la linea de comando:

        Convolucion entrada.wav salida.wav

    El programa tomara 100 muestras de la respuesta impulso para realizar la convolucion;
    el factor de escala debe ser tal que al aplicar corriente directa se obtenga 
    (despues de 100 muestras) el mismo nivel en la salida que el que se aplica en la entrada
*/

#include "FConvolucion.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Entrada incorrecta\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav\n");
        exit(EXIT_FAILURE);
    }

    convolucion(argv[1], argv[2]);
}