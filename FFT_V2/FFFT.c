#include "FFFT.h"

void preparacionFFT(char *archivo, char *archivoSalida) {
    FILE *dArchivo = NULL;
    FILE *dArchivoSalida = NULL;
    short* signal = NULL;
    float* signalFFT = NULL;
    Cabecera *c = NULL;
    int numeroMuestras = 0;
    int numeroMuestrasNuevo = 0;
    char* pie = NULL;
    int bytesPie = 0;
    short* muestras = NULL;
    int no = 0;
    float *xr = NULL;
    float *xi = NULL;

    c = malloc(sizeof(Cabecera));

    dArchivo = abrirArchivo(archivo, 1);
    c = obtenerCabecera(c, dArchivo);
    mostrarCabecera(c, archivo);

    numeroMuestras = numeroDeMuestras(c -> tamSubBloque2, c -> bpm);
    bytesPie = (c -> tamArchivo) - (c -> tamSubBloque2) - 36;
    pie = malloc(sizeof(char) * bytesPie);

    if(!esPotenciaDos(numeroMuestras)) {
        int l = log2(numeroMuestras);
        l += 1;

        numeroMuestrasNuevo = pow(2, l);

        c = modificarCabecera(c, numeroMuestras, numeroMuestrasNuevo);

        signal = malloc(sizeof(short) * numeroMuestrasNuevo);
        signal = muestrearSenial(dArchivo, numeroMuestras, signal);

        signal = rellenar(signal, numeroMuestras, numeroMuestrasNuevo);

        signalFFT = malloc(sizeof(float) * (numeroMuestrasNuevo * 2));
        muestras = malloc(sizeof(short) * (numeroMuestrasNuevo * 2));
        no = numeroMuestrasNuevo;
    } else {
        signal = malloc(sizeof(short) * numeroMuestras);
        signal = muestrearSenial(dArchivo, numeroMuestras, signal);
        
        signalFFT = malloc(sizeof(float) * (numeroMuestras * 2));
        muestras = malloc(sizeof(short) * (numeroMuestras * 2));
        no = numeroMuestras;
    }

    pie = obtenerPie(dArchivo, pie, bytesPie);
    cerrarArchivo(dArchivo);

    xr = malloc(sizeof(float) * no);
    xi = malloc(sizeof(float) * no);

    for(int i = 0; i < no; i++) {
        xr[i] = (float) signal[i];
        xi[i] = 0.0;
    }

    signalFFT = fft(xr, xi, no, signalFFT);

    for(int i = 0; i < no * 2; i++)
        muestras[i] = signalFFT[i];

    dArchivoSalida = abrirArchivo(archivoSalida, 2);
    cabeceraStereo(c, dArchivoSalida);

    fwrite(muestras, c -> tamSubBloque2, 1, (FILE *) dArchivoSalida);
    fwrite(pie, bytesPie, 1, (FILE *) dArchivoSalida);

    cerrarArchivo(dArchivoSalida);

    free(c);
    free(signal);
    free(signalFFT);
    free(pie);
    free(muestras);
    free(xr);
    free(xi);
}

float *fft(float *xr, float *xi, int numeroMuestras, float *signalFFT) {
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
                arg = -j * w;
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
    arg = 1.0 / numeroMuestras;

    for(i = 0; i < numeroMuestras; i++) {
        xr[i] *= arg;
        xi[i] *= arg;

        signalFFT[2 * i] = xr[i];
        signalFFT[(2 * i) + 1] = xi[i];
    }

    return signalFFT;
}

void swap(float *a, float *b) {
    float aux;
    
    aux = *a;
    *a = *b;
    *b = aux; 
}

int esPotenciaDos(int numeroMuestras) { //1 Si es potencia de dos, 0 si no lo es
    return ((numeroMuestras != 0) && ((numeroMuestras & (numeroMuestras - 1)) == 0)) ? 1 : 0;
}

Cabecera *modificarCabecera(Cabecera *c, int numeroMuestras, int numeroMuestrasNuevo) {
    int m = numeroMuestras - numeroMuestrasNuevo;
    
    c -> tamArchivo += (m * 2);
    c -> tamSubBloque2 += (m * 2);

    return c;
}

short *rellenar(short *signal, int numeroMuestras, int numeroMuestrasNuevo) {   
    for(int i = numeroMuestras; i < numeroMuestrasNuevo; i++)
        signal[i] = 0;

    return signal;
}
