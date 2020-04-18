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
    short int tipoGrabacion;
    short int numeroCanales;
    int frecuenciaMuestreo;
    int bps;
    short int alineamiento;
    short int bpm;

    char data[4];
    int tamSubBloque2; 
};

typedef struct Cabecera Cabecera;

void preparacionDTMF(char *);
double *tdf(int numeroMuestras, short* signal, double *);
void dtmf(short *, double);
short absoluto(short);
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

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Error en la entrada\n");
        printf("Ejemplo:\n");
        printf("./NombrePrograma archivo.wav\n");
        exit(EXIT_FAILURE);
    }

    preparacionDTMF(argv[1]);
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

void preparacionDTMF(char *archivo) {
    FILE *dArchivo = NULL;
    short* signal = NULL;
    int numeroMuestras;
    int fMuestreo;
    double *signalDFT = NULL;
    Cabecera *c = NULL;
    short *muestras = NULL;

    dArchivo = abrirArchivo(archivo, 1);

    c = malloc(sizeof(Cabecera));
    c = obtenerCabecera(c, dArchivo);
    numeroMuestras = numeroDeMuestras(c->tamSubBloque2, c->bpm);
    fMuestreo = c->frecuenciaMuestreo;

    signal = malloc(sizeof(short) * numeroMuestras);
    signalDFT = malloc(sizeof(double) * numeroMuestras);
    muestras = malloc(sizeof(short) * numeroMuestras);

    signal = muestrearSenial(dArchivo, numeroMuestras, signal);

    cerrarArchivo(dArchivo);

    double duracion = (double) (((double) numeroMuestras) / (double) fMuestreo);
    signalDFT = tdf(numeroMuestras, signal, signalDFT);

    for(int i = 0; i < numeroMuestras; i++) {
        muestras[i] = signalDFT[i];
    }
  
    dtmf(muestras, duracion);

    free(c);
    free(signal);
    free(signalDFT);
}

void dtmf(short *muestras, double duracion) {
    int frecuenciasDTMF[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    int posicion[8];
    
    char teclado[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    short f;

    for(int i = 0; i < 8; i++) {
        int pos = (int)(frecuenciasDTMF[i] * duracion);
        f = absoluto(muestras[pos]);

        if(f > 500) 
            posicion[i] = 1;
        else 
            posicion[i] = 0;
    }

    for(int i = 0; i < 4; i++) {
        if(posicion[i]) {
            if(posicion[4]) {
                printf("%c", teclado[i][0]);
            }
            
            if(posicion[5]) {
                printf("%c", teclado[i][1]);
            }

            if(posicion[6]) {
                printf("%c", teclado[i][2]);
            }

            if(posicion[7]) {
                printf("%c", teclado[i][3]);
            }
        }
    }

    printf("\n");
}

double *tdf(int numeroMuestras, short* signal, double* signalTDF) {
    double *xr = malloc(sizeof(double) * numeroMuestras);
    double *xi = malloc(sizeof(double) * numeroMuestras);
    for (int k = 0; k < numeroMuestras; k++) {
        xr[k] = 0;
        xi[k] = 0;
        for (int n = 0; n < numeroMuestras; n++) {
            xr[k] += signal[n] * cos((2 * PI * k * n) / numeroMuestras);
            xi[k] += (signal[n] * sin((2 * PI* k * n) / numeroMuestras)) * (-1);
        }
        
        xr[k] /= numeroMuestras;
        xi[k] /= numeroMuestras;
        signalTDF[k] = sqrt((pow(xr[k], 2) + pow(xi[k], 2)));
    }

    return signalTDF;
}

short absoluto(short n) {
    if(n < 0)
        n *= (-1);
    
    return n;
}