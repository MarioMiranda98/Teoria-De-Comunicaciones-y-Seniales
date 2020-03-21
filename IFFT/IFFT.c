/*
Escriba un programa que calcule la transformada rapida de Fourier inversa del archivo wav de 
entrada (estereo); generando un archivo wav con la salida (estereo); los nombres de los 
archivos se pasan por la linea de comando:

ftti entrada.wav salida.wav

No aplicar ninguna escala al calcular la FFTI (no buscar un maximo y escalar respecto a el); 
de esta manera la FFTI regresaria una señal constante de amplitud 1 en la parte real cuando se
introduzca un impulso de amplitud 1 en la parte real de la entrada. Se asume que la entrada
debe tener un numero de datos que sea potencia de dos.
*/

#include "FIFFT.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Error en la entrada\n");
        printf("Ejemplo:\n");
        printf("./Nombre Programa archivo1.wav archivo2.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionIFFT(argv[1], argv[2]);
}