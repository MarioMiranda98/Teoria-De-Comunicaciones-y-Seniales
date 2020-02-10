/*
    Escriba un programa que lea un archivo wav y genere otro archivo wav cuyo volumen sea 
    la mitad del primero. El programa recibe los nombres de los archivos de entrada por 
    linea de comando; por ejemplo:

    volumen entrada.wav salida.wav 
*/

#include "BajarVolumen.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Entrada incorrecta\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav\n");
        exit(EXIT_FAILURE);
    }

    bajarVolumen(argv[1], argv[2]);
}