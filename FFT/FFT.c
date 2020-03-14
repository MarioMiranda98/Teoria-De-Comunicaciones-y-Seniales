/*
Escriba un programa que calcule la transformada rapida de Fourier del archivo wav de entrada
(monoaural); generando un archivo wav con la salida (estereo); los nombres se pasan por linea 
de comando:

fft entrada.wav salida.wav

Aplicar 1/N al calcular la FFT como unico factor de escala (no buscar un maximo y escalar con 
respecto a el); de esta manera la FFT de 1 (corriente directa) debe subir hasta 1.

Si el archivo de entrada no contiene un numero de datos potencia de dos se deben completar los 
datos a la siguiente potencia de dos rellenando con ceros.
*/

#include "FFFT.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Error en la entrada\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionFFT(argv[1], argv[2]);
}