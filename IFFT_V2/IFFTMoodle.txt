#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926535

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
void cabeceraStereo(Cabecera *, FILE *);
void colocarPie(FILE *, char *, int);
short *obtenerCanal1(short *, short *, int);
short *obtenerCanal2(short *, short *, int);
int numeroDeMuestras(int, int);
void preparacionIFFT(char *, char *);
float *ifft(float *xr, float *xi, int numeroMuestras, float *signalIFFT);
void swap(float *a, float *b);

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Error en la entrada\n");
        printf("Ejemplo:\n");
        printf("./Nombre Programa archivo1.wav archivo2.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionIFFT(argv[1], argv[2]);
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
    fread(signal, numeroMuestras * sizeof(short), 1, (FILE *) dArchivo);
    return signal;
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

char *obtenerPie(FILE* dArchivo, char* pie, int bytesPie) {
    fread(pie, bytesPie, 1, (FILE *) dArchivo);
    return pie;
}

void colocarPie(FILE* dArchivoSalida, char *pie, int bytesPie) {
    fwrite(pie, sizeof(char) * bytesPie, 1, (FILE *) dArchivoSalida);
}

int numeroDeMuestras(int tamSubBloque2, int bpm) {
    return (tamSubBloque2 / (bpm / 8));
}

void preparacionIFFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    short* signal = NULL;
    short* canal1 = NULL;
    short* canal2 = NULL;
    Cabecera *c = NULL;
    char *pie = NULL;
    int numeroMuestras = 0;
    int numeroMuestrasCanales = 0;
    int bytesPie = 0;
    short *muestras = NULL;
    float *signalIFFT = NULL;
    float *xr = NULL;
    float *xi = NULL;

    c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    c = obtenerCabecera(c, dArchivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    numeroMuestrasCanales = (numeroMuestras / 2);
    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;

    signal = malloc(sizeof(short) * numeroMuestras);
    canal1 = malloc(sizeof(short) * numeroMuestrasCanales);
    canal2 = malloc(sizeof(short) * numeroMuestrasCanales);
    xr = malloc(sizeof(float) * numeroMuestrasCanales);
    xi = malloc(sizeof(float) * numeroMuestrasCanales);
    pie = malloc(sizeof(char) * bytesPie);

    signal = muestrearSenial(dArchivo, numeroMuestras, signal);
    canal1 = obtenerCanal1(signal, canal1, numeroMuestrasCanales);
    canal2 = obtenerCanal2(signal, canal2, numeroMuestrasCanales);

    pie = obtenerPie(dArchivo, pie, bytesPie);

    cerrarArchivo(dArchivo);
  
    for(int i = 0; i < numeroMuestrasCanales; i++) {
        xr[i] = canal1[i];
        xi[i] = canal2[i];
    }

    signalIFFT = malloc(sizeof(float) * numeroMuestras);
    muestras = malloc(sizeof(short) * numeroMuestras);

    signalIFFT = ifft(xr, xi, numeroMuestrasCanales, signalIFFT);

    for(int i = 0; i < numeroMuestras; i++) {
        muestras[i] = signalIFFT[i];
    }

    dArchivoSalida = abrirArchivo(archivoSalida, 2);

    fwrite(c, 44, 1, (FILE *) dArchivoSalida);
    fwrite(muestras, c -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
    colocarPie(dArchivoSalida, pie, bytesPie);

    cerrarArchivo(dArchivoSalida);

    free(c);
    free(signal);
    free(canal1);
    free(canal2);
    free(pie);
    free(signalIFFT);
    free(muestras);
    free(xr);
    free(xi);
}

float *ifft(float *xr, float *xi, int numeroMuestras, float *signalIFFT) {
    int m, n, i, j, k, j1;
    float arg, s, c, tempr, tempi, w;
    
    m = log((float) numeroMuestras) / log(2.0);

    for(i = 0; i < numeroMuestras; ++i) {
        j = 0;
        for(k = 0; k < m; ++k) 
            j = (j << 1) | (1 & (i >> k));

        if(j < i) {
            swap(&xr[i], &xr[j]);
            swap(&xi[i], &xi[j]);
        }
    }

    for(i = 0; i < m; i++) {
        n = w = pow(2.0, (float) i);
        w = PI / n;

        k = 0;

        while(k < (numeroMuestras - 1)) {
            for(j = 0; j < n; j++) {
                arg = j * w;
                c = cos(arg);
                s = sin(arg);
                j1 = k + j;
                tempr = xr[j1 + n] * c - xi[j1 + n] * s;
                tempi = xi[j1 + n] * c + xr[j1 + n] * s;
                xr[j1 + n] = xr[j1] - tempr;
                xi[j1 + n] = xi[j1] - tempi;
                xr[j1] = xr[j1] + tempr;
                xi[j1] = xi[j1] + tempi; 
            }
            k += 2 * n;
        }
    }


    for(i = 0; i < numeroMuestras; i++) {
        signalIFFT[2 * i] = xr[i];
        signalIFFT[(2 * i) + 1] = xi[i];
    }

    return signalIFFT;
}

void swap(float *a, float *b) {
    float aux;
    
    aux = *a;
    *a = *b;
    *b = aux; 
}