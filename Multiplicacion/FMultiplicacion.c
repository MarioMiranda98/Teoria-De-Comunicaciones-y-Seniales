#include "FMultiplicacion.h"

void multiplica(char* archivo1, char* archivo2, char* archivoSalida) {
    FILE* dArchivo1 = NULL;
    FILE* dArchivo2 = NULL;
    FILE* dArchivoSalida = NULL;
    Cabecera *c1 = NULL;
    Cabecera *c2 = NULL;
    int numMuestras1 = 0;
    int numMuestras2 = 0;
    int tamArchivo1 = 0;
    int tamArchivo2 = 0;
    int numCanales1 = 0;
    int numCanales2 = 0;
    int bytesPie = 0;
    char* pie = NULL;
    short* signal1 = NULL;
    short* signal2 = NULL;
    double* signalMultiplicada = NULL;
    short* muestras = NULL;
    int tipo = 0;

    dArchivo1 = abrirArchivo(archivo1, 1);
    dArchivo2 = abrirArchivo(archivo2, 1);
    dArchivoSalida = abrirArchivo(archivoSalida, 2);

    c1 = malloc(sizeof(Cabecera));
    c2 = malloc(sizeof(Cabecera));

    fread(c1, 44, 1, (FILE *) dArchivo1);
    fread(c2, 44, 1, (FILE *) dArchivo2);

    tamArchivo1 = c1 -> tamArchivo;
    tamArchivo2 = c2 -> tamArchivo;
    numMuestras1 = numeroDeMuestras(c1 -> tamSubBloque2, c1 -> bpm);
    numMuestras2 = numeroDeMuestras(c2 -> tamSubBloque2, c2 -> bpm);
    numCanales1 = c1 -> numeroCanales;
    numCanales2 = c2 -> numeroCanales;
    bytesPie = (c1 -> tamArchivo) - (c1 -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);

    signal1 = malloc(sizeof(short) * numMuestras1);
    signal2 = malloc(sizeof(short) * numMuestras2);

    //#TODO: Muestrear Seañles estereo 
    if(numCanales1 == 1) 
        signal1 = muestrearSenial(dArchivo1, numMuestras1, signal1);
    
    if(numCanales2 == 1)
        signal2 = muestrearSenial(dArchivo2, numMuestras2, signal2);

    pie = obtenerPie(dArchivo1, pie, bytesPie);

    if((numCanales1 == 1) && (numCanales2 ==1))
        tipo = 1;
    else if((numCanales1 == 2) && (numCanales2 == 2))
        tipo = 2;
    else if(((numCanales1 == 1) && (numCanales2 == 2)) || ((numCanales1 == 2) && (numCanales2 == 1)))
        tipo = 3;

    //#TODO: Añadir tipo estereoXestereo y monoXestero
    if(tipo == 1) {
        if(tamArchivo1 >= tamArchivo2) {
            //Archivo 1 mas grande
            fwrite(c1, 44, 1, (FILE *) dArchivoSalida);
            muestras = malloc(sizeof(short) * numMuestras1);
            signalMultiplicada = malloc(sizeof(double) * numMuestras1);

            muestras = multiplicarMonoXMono(signal1, signal2, numMuestras1, numMuestras2, signalMultiplicada, muestras);
            fwrite(muestras, c1 -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
            fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);
        } else {
            //Archivo 2 mas grande
            fwrite(c2, 44, 1, (FILE *) dArchivoSalida);
            muestras = malloc(sizeof(short) * numMuestras2);
            signalMultiplicada = malloc(sizeof(double) * numMuestras2);

            muestras = multiplicarMonoXMono(signal2, signal1, numMuestras2, numMuestras1, signalMultiplicada, muestras);
            fwrite(muestras, c2 -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
            fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);
        }
    }

    cerrarArchivo(dArchivo1);
    cerrarArchivo(dArchivo2);
    cerrarArchivo(dArchivoSalida);
    free(c1);
    free(c2);
    free(signal1);
    free(signal2);
    free(signalMultiplicada);
    free(muestras);
    free(pie);
}

//#TODO: implementar funciones de estereoXestereo y monoXmono
short* multiplicarMonoXMono(short* signalMayor, short* signalMenor, int muestrasMayor, int muestrasMenor, double* signalMultiplicada, short* muestras) {
    for(int i = 0; i < muestrasMenor; i++) {
        signalMultiplicada[i] = (signalMayor[i] * signalMenor[i]);
        signalMultiplicada[i] /= 32767;
        if(signalMultiplicada[i] > 32767)
            signalMultiplicada[i] = 32767;
        
        muestras[i] = signalMultiplicada[i];
    }

    for(int i = muestrasMenor; i < muestrasMayor; i++)
        muestras[i] = 0;
    
    return muestras;
}