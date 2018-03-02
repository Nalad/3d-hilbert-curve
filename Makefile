CFLAGS = -Wall -std=c99 -lm

Hilbert3D: Hilbert3D.o Vector3D.o List.o Matrix.o
	gcc $(CFLAGS) -o Hilbert3D Hilbert3D.o Matrix.o Vector3D.o List.o -lm

Hilbert3D.o: Hilbert3D.c Vector3D.h List.h Matrix.h
	gcc $(CFLAGS) -c Hilbert3D.c

Vector3D.o: Vector3D.c Vector3D.h Matrix.h
	gcc $(CFLAGS) -c Vector3D.c 

List.o: List.c List.h
	gcc $(CFLAGS) -c List.c

Matrix.o: Matrix.c Matrix.h
	gcc $(CFLAGS) -c Matrix.c
