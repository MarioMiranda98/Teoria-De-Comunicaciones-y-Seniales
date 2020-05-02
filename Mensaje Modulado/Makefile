# -*- Makefile -*-

Mensaje: Mensaje.o FMensaje.o Funciones.o Cabecera.o
	gcc Mensaje.o FMensaje.o Funciones.o Cabecera.o -o Mensaje -l m

Mensaje.o: Mensaje.c 
	gcc -c Mensaje.c

FMensaje.o: FMensaje.c FMensaje.h
	gcc -c FMensaje.c

Funciones.o: Funciones.c Funciones.h
	gcc -c Funciones.c

Cabecera.o: Cabecera.c Cabecera.h
	gcc -c Cabecera.c

clean:
	rm -f *.o  *.h.gch Mensaje