/*
Escriba un programa que calcule la transformada discreta de Fourier inversa del archivo wav
de entrada (estereo); generando un archivo wav con la salida (estereo); los nombres de los
archivos se pasan por linea de comando:

tdf entrada.wav salida.wav 

El resultado no se escalara solo se truncaran los valores que excedan al maximo y al minimo  
para evitar que aparezcan valores con otro signo (se esta asumiendo que el factor de escala
de 1/N se aplico en el programa de la TDF).
*/

#include "FIDFT.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Entrada Invalida\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav");
        exit(EXIT_FAILURE);
    }

    preparacionIDFT(argv[1], argv[2]);
}