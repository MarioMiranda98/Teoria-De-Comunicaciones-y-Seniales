#ifndef __CABECERA__
#define __CABECERA__

#include <stdio.h>

struct Cabecera {
    char riff[4];//Contiene las letras RIFF (chunkID)
    int tamArchivo;//Tamaño del wav (ChunkSize)
    char wave[4];//Contiene las letras WAVE 
    
    //Comienza primer bloque
    char fmt[4];//Contiene las letras fmt'', indicando car. de grabacion (SubChunk1ID)
    int tamSubBloque;//Indice el resto de bytes del bloque, si es impar, poner byte de relleno (SubChunk1Size)
    short tipoGrabacion;//1 es grabacion PCM, formato (AudioFormat)
    short numeroCanales;//1 es grabacion mono, 2 es grabacion estereo (NumChannels)
    int frecuenciaMuestreo;//La frecuencia de muestreo (SampleRate)
    int bps;//Bytes por segundo, "buffer" (Byte Rate)
    short alineamiento;//No. Bytes usados en el archivo por muestra (Block Align)
    short bpm;//Bits por muestra (Bits Per Sample)

    //Comienza el segundo bloque
    char data[4];//Contiene las letras data (SubChunk2ID)
    int tamSubBloque2; //Numero de bytes en los datos (SubChunk2Size)
};

typedef struct Cabecera Cabecera;

void mostrarCabecera(Cabecera *, char *);

#endif //!__CABECERA__
