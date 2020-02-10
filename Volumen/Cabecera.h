#ifndef __CABECERA__
#define __CABECERA__

#include <stdio.h>

struct Cabecera {
    char riff[4];//Contiene las letras RIFF
    int tamArchivo;//Tamaño del wav
    char wave[4];//Contiene las letras WAVE
    
    //Comienza primer bloque
    char fmt[4];//Contiene las letras fmt'', indicando car. de grabacion
    int tamSubBloque;//Indice el resto de bytes del bloque, si es impar, poner byte de relleno
    short int tipoGrabacion;//1 es grabacion PCM, formato
    short int numeroCanales;//1 es grabacion mono, 2 es grabacion estereo
    int frecuenciaMuestreo;//La frecuencia de muestreo
    int bps;//Bytes por segundo, "buffer"
    short int alineamiento;//No. Bytes usados en el archivo por muestra
    short int bpm;//Bits por muestra

    //Comienza el segundo bloque
    char data[4];//Contiene las letras data
    int tamSubBloque2; //Numero de bytes en los datos
};

typedef struct Cabecera Cabecera;

void mostrarCabecera(Cabecera *, char *);

#endif //!__CABECERA__
