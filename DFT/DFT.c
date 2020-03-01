/*
    Escriba un programa que calcule la transformada discreta de fourier del archivo wav de 
    de entrada (monoaural); generando un archivo wav con la salida (estereo); los nombres
    de los archivos se pasan por la linea de comando:

    tdf entrada.wav salida.wav

    Aplicar 1/N al calcular la TDF como unico factor de escala (no buscar un maximo y escalar
    con respecto a el); de esta manera la tdf de 1 (corriente directa) debe subir hasta 1. 
*/

#include "FDFT.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Entrada incorrecta\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionTDF(argv[1], argv[2]);
}