#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "prision.h"

// Pre  Condiciones: - Los archivos crucio.dat e immperiurs.dat tienen los datos cargados correctamente (fechas, extensión de nombres y conductas válidas).
//					 - Las fechas ingresadas deben ser válidas y respetando el formato "aaaamm", y las conductas ingresadas pueden ser "B", "R" o "M" (en mayúscula).

// Post Condiciones: El programa realizará las tareas que el usuario ejecute en la línea de comandos,
//					 siempre y cuando estas coincidan con las indicadas en el menú deyudas desplegado.

int main(int argc, char *argv[]){
	if((argc == 3) && (comando_perdonables_valido(argv[1]))){
		crear_archivo_perdonables(argv[2]);
	}
	else if((argc == 5) && (comando_liberar_valido(argv[1], argv[2], argv[3], *argv[4]))){
		liberar_presos(argv[2], argv[3], *argv[4]);
	}
	else if((argc == 3) && (comando_actualizar_valido(argv[1], argv[2]))){
		actualizar_presos(argv[2]);
	}
	else if((argc == 3) && (comando_mostrar_liberados_valido(argv[1], argv[2]))){
		mostrar_liberados(argv[2]);
	}
	else{
		imprimir_ayudas();
	}

	return 0;
}