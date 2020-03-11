/*
Escriba un programa que multiplique dos archivos wav de entrada y genere un archivo nuevo de 
salida; los nombres de los archivos se pasan por linea de comando:

programa entrada1.wav entrada2.wav salida.wav

El programa debe ser capaz de aceptar archivos monoaurales o archivos estereo; en el primer 
caso se considera que el canal corresponde  a una señal real, mientras que en el caso de las 
señales estereo se asumira que se trata de señales complejas, cuyo primer canal corresponde a
la parte real y el segundo canal contiene la parte la imaginaria.
*/

#include "FMultiplicacion.h"

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Error en la entrada\n");
        printf("Ejemplo\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav Archivo3.wav\n");
        exit(EXIT_FAILURE);
    }

    multiplica(argv[1], argv[2], argv[3]);
}