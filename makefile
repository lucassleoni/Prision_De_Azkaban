CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wconversion

compile_convertir: 1convertir.c
	$(CC) 1convertir.c -o 1convertir
	clear

compile_pruebas_p: 2pruebas_perdonables.c
	$(CC) 2pruebas_perdonables.c -o 2pruebas_perdonables

compile_pruebas_l: 3pruebas_liberados.c
	$(CC) 3pruebas_liberados.c -o 3pruebas_liberados

compile_azkaban: azkaban.c prision.c
	$(CC) $(CFLAGS) *.c -o azkaban
	clear



convertir: compile_convertir
	./1convertir
	clear

pruebas_p: compile_pruebas_p
	clear
	./2pruebas_perdonables

pruebas_l: compile_pruebas_l
	clear
	./3pruebas_liberados

azkaban: compile_azkaban
	./azkaban

perdonables: compile_azkaban
	./azkaban perdonables PerdonablesNuevo.dat
	clear

liberar: compile_azkaban
	./azkaban liberar PerdonablesNuevo.dat 201901 M

actualizar: compile_azkaban
	./azkaban actualizar 201901

mostrar: compile_azkaban
	./azkaban mostrar_liberados 201901

.PHONY: clean

clean:
	rm azkaban
