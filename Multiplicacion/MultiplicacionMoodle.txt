#include <stdio.h>
#include <stdlib.h>

struct Cabecera {
    char riff[4];
    int tamArchivo;
    char wave[4]; 
    
    char fmt[4];
    int tamSubBloque;
    short tipoGrabacion;
    short numeroCanales;
    int frecuenciaMuestreo;
    int bps;
    short alineamiento;
    short bpm;

    char data[4];
    int tamSubBloque2; 
};

typedef struct Cabecera Cabecera;

FILE* abrirArchivo(char *, int);
void cerrarArchivo(FILE *);
Cabecera *obtenerCabecera(Cabecera *, FILE *);
short *muestrearSenial(FILE *, int, short *);
char *obtenerPie(FILE *, char*, int);
void colocarPie(FILE *, char *, int);
short *obtenerCanal1(short *, short *, int);
short *obtenerCanal2(short *, short *, int);
int numeroDeMuestras(int, int);
void multiplica(char *, char *, char *);
double *multiplicar(short *, short *, int, int, double *);
short *multiplicarMonoXMono(short *, short *, int, int, double *, short *);
short *multiplicarStereoXStereo(short *, short *, short *, short *, int, int, short *);

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Error en la entrada\n");
        printf("Ejemplo\n");
        printf("./NombrePrograma Archivo1.wav Archivo2.wav Archivo3.wav\n");
        exit(EXIT_FAILURE);
    }

    multiplica(argv[1], argv[2], argv[3]);
}

FILE* abrirArchivo(char *archivo, int modo) {
    FILE* descriptor = NULL;

    if (modo == 1) 
        descriptor = fopen(archivo, "rb"); 
    else 
        descriptor = fopen(archivo, "wb");
    
    if(descriptor == NULL) { 
        printf("No se pudo abrir el archivo\n");
        exit(EXIT_FAILURE);
    }

    return descriptor;
}

void cerrarArchivo(FILE *dArchivo) {
    fclose(dArchivo);
}

Cabecera *obtenerCabecera(Cabecera* c, FILE* dArchivo) {
    fread(c, 44, 1, (FILE *) dArchivo);
    return c;
}

short *muestrearSenial(FILE* dArchivo, int numeroMuestras, short* signal) { 
    fread(signal, sizeof(short) * numeroMuestras, 1, (FILE *) dArchivo);
    return signal;
}

char *obtenerPie(FILE* dArchivo, char* pie, int bytesPie) {
    fread(pie, bytesPie, 1, (FILE *) dArchivo);
    return pie;
}

void colocarPie(FILE* dArchivoSalida, char *pie, int bytesPie) {
    fwrite(pie, sizeof(char) * bytesPie, 1, (FILE *) dArchivoSalida);
}

short *obtenerCanal1(short *signal, short *canal1, int numeroMuestras) {
    for (int i = 0; i < (numeroMuestras); i++)
        canal1[i] = signal[2 * i];

    return canal1;
}

short *obtenerCanal2(short *signal, short *canal2, int numeroMuestras) {
    for (int i = 0; i < (numeroMuestras); i++)
        canal2[i] = signal[(2 * i) + 1];

    return canal2;
} 

int numeroDeMuestras(int tamSubBloque2, int bpm) {
    return (tamSubBloque2 / (bpm / 8));
}

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
    short* canal1 = NULL;
    short* canal2 = NULL;
    short* canal3 = NULL;
    short* canal4 = NULL;
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

    if(numCanales1 == 1) 
        signal1 = muestrearSenial(dArchivo1, numMuestras1, signal1);
    else if(numCanales1 == 2) {
        signal1 = muestrearSenial(dArchivo1, numMuestras1, signal1);

        canal1 = malloc(sizeof(short) * (numMuestras1 / 2));
        canal1 = obtenerCanal1(signal1, canal1, (numMuestras1 / 2));

        canal2 = malloc(sizeof(short) * (numMuestras1 / 2));
        canal2 = obtenerCanal2(signal1, canal2, (numMuestras1 / 2));
    }
    
    if(numCanales2 == 1)
        signal2 = muestrearSenial(dArchivo2, numMuestras2, signal2);
    else if(numCanales2 == 2) {
        signal2 = muestrearSenial(dArchivo2, numMuestras2, signal2);

        canal3 = malloc(sizeof(short) * (numMuestras2 / 2));
        canal3 = obtenerCanal1(signal2, canal3, (numMuestras2 / 2));

        canal4 = malloc(sizeof(short) * (numMuestras2 / 2));
        canal4 = obtenerCanal2(signal2, canal4, (numMuestras2 / 2));
    }

    pie = obtenerPie(dArchivo1, pie, bytesPie);

    if((numCanales1 == 1) && (numCanales2 ==1))
        tipo = 1;
    else if((numCanales1 == 2) && (numCanales2 == 2))
        tipo = 2;

    if(tipo == 1) {
        if(tamArchivo1 >= tamArchivo2) {
            fwrite(c1, 44, 1, (FILE *) dArchivoSalida);
            muestras = malloc(sizeof(short) * numMuestras1);
            signalMultiplicada = malloc(sizeof(double) * numMuestras1);

            muestras = multiplicarMonoXMono(signal1, signal2, numMuestras1, numMuestras2, signalMultiplicada, muestras);
            fwrite(muestras, c1 -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
            fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);
        } else {
            fwrite(c2, 44, 1, (FILE *) dArchivoSalida);
            muestras = malloc(sizeof(short) * numMuestras2);
            signalMultiplicada = malloc(sizeof(double) * numMuestras2);

            muestras = multiplicarMonoXMono(signal2, signal1, numMuestras2, numMuestras1, signalMultiplicada, muestras);
            fwrite(muestras, c2 -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
            fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);
        }
    } else if(tipo == 2) {
       if(tamArchivo1 >= tamArchivo2) {
           fwrite(c1, 44, 1, (FILE *) dArchivoSalida);
           muestras = malloc(sizeof(short) * numMuestras1);

           muestras = multiplicarStereoXStereo(canal1, canal2, canal3, canal4, (numMuestras1 / 2), (numMuestras2 / 2), muestras);
           fwrite(muestras, c1 -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
           fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);
       } else {
           fwrite(c2, 44, 1, (FILE *) dArchivoSalida);
           muestras = malloc(sizeof(short) * numMuestras2);

           muestras = multiplicarStereoXStereo(canal3, canal4, canal1, canal2, (numMuestras2 / 2), (numMuestras1 / 2), muestras);
           fwrite(muestras, c2 -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
           fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);
       }
        free(canal1);
        free(canal2);
        free(canal3);
        free(canal4);
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

short *multiplicarStereoXStereo(short *canalMR, short *canalMI, short *canalMeR, short *canalMeI, int muestrasMayor, int muestrasMenor, short *muestras) {
    double *res1 = malloc(sizeof(double) * muestrasMayor);
    double *res2 = malloc(sizeof(double) * muestrasMayor);
    double *res3 = malloc(sizeof(double) * muestrasMayor);
    double *res4 = malloc(sizeof(double) * muestrasMayor);
    double *c1 = malloc(sizeof(double) * muestrasMayor);
    double *c2 = malloc(sizeof(double) * muestrasMayor);

    res1 = multiplicar(canalMR, canalMeR, muestrasMayor, muestrasMenor, res1);
    res2 = multiplicar(canalMI, canalMeI, muestrasMayor, muestrasMenor, res2);
    res3 = multiplicar(canalMR, canalMeI, muestrasMayor, muestrasMenor, res3);
    res4 = multiplicar(canalMI, canalMeR, muestrasMayor, muestrasMenor, res4);

    for(int i = 0; i < muestrasMayor; i++) {
        c1[i] = (res1[i] - res2[i]) / 2;
        c2[i] = (res3[i] + res4[i]) / 2;
    }

    for(int i = 0; i < muestrasMayor; i++) {
        muestras[2 * i] = c1[i];
        muestras[(2 * i) + 1] = c2[i];
    }

    free(res1);
    free(res2);
    free(res3);
    free(res4);
    free(c1);
    free(c2);

    return muestras;
}

double *multiplicar(short *signalMayor, short *signalMenor, int muestrasMayor, int muestrasMenor, double *res) {
    for(int i = 0; i < muestrasMenor; i++) {
        res[i] = (signalMayor[i] * signalMenor[i]);
        res[i] /= 32767;
        
        if(res[i] > 32767)
            res[i] = 32767;
    }

    for(int i = muestrasMenor; i < muestrasMayor; i++)
        res[i] = 0;

    return res;
}