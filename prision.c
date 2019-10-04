#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "prision.h"

// Descripción: Imprime por pantalla los comandos y argumentos que deben ser ejecutados según la acción que el usuario desea realizar.
void imprimir_ayudas(){
	system("clear");
	printf("¡Bienvenido al registro virtual de los prisioneros de Azkaban!\n\n");
	printf("En este menú de ayudas encontrarás todo lo necesario para poder acceder a información confidencial de la prisión de magos más segura del mundo.\n");
	printf("Para realizar las distintas operaciones disponibles, podrás ejecutar los siguientes comandos:\n\n");
	
	printf("\t1) " "\e[4m" "Mostrar a los presos perdonables\n" "\e[0m");
	printf("\t\tCOMANDO: ./azkaban perdonables <nombre_archivo>\n");
	printf("\t\tARGUMENTOS: El nombre del archivo es decidido por el usuario.\n\n");
	
	printf("\t2) " "\e[4m" "Liberar presos (solo se liberan aquellos que sean perdonables)\n" "\e[0m");
	printf("\t\tCOMANDO: ./azkaban liberar <archivo_perdonables>.dat año/mes conducta\n");
	printf("\t\tARGUMENTOS: - Nombre del archivo de presos perdonables (elegido al ejecutar el comando Perdonables).\n");
	printf("\t\t\t    - El año/mes debe ser tipeado con el formato aaaamm.\n");
	printf("\t\t\t    - Las conductas pueden ser (escritas en mayúscula):\n");
	printf("\t\t\t\t - B: Buena\n");
	printf("\t\t\t\t - R: Regular\n");
	printf("\t\t\t\t - M: Mala\n\n");
	
	printf("\t3) " "\e[4m" "Actualizar los archivos (luego de liberar presos perdonables)\n" "\e[0m");
	printf("\t\tCOMANDO: ./azkaban actualizar año/mes\n");
	printf("\t\tARGUMENTOS: El año/mes del archivo que se toma para actualizar debe ser tipeado con el formato aaaamm.\n\n");
	
	printf("\t4) " "\e[4m" "Mostrar los presos que fueron liberados\n" "\e[0m");
	printf("\t\tCOMANDO: ./azkaban mostrar_liberados año/mes\n");
	printf("\t\tARGUMENTOS: El año/mes del archivo que se quiere mostrar debe ser tipeado con el formato aaaamm.\n\n");

	printf("\t5) " "\e[4m" "Mostrar nuevamente el menú de ayudas\n" "\e[0m");
	printf("\t\tCOMANDO: ./azkaban ayuda\n\n");
}

// Descripción: Imprime por pantalla un mensaje que informa al usuario que no se pudo abrir un archivo (con su respectivo nombre).
void imprimir_error_archivo(char nombre_archivo[MAX_ARCHIVO]){
	printf("Error al intentar abrir el archivo '%s'\n", nombre_archivo);
}

// Pre C: Se deben recibir dos strings.
// Post C: Se crea un string que une los 2 recibidos al que se le añade la extensión ".txt".
void crear_nombre_txt(char nombre_liberados[MAX_ARCHIVO], char fecha_ingresada[MAX_FECHA]){
	strcpy(nombre_liberados, F_LIBERADOS);
	strcat(nombre_liberados, fecha_ingresada);
	strcat(nombre_liberados, EXTENSION_TXT);
}

// Pre C: Recibe la fecha ingresada por el usuario.
// Post C: Devuelve 'true' si hubo liberados en la fecha mencionada.
bool existe_archivo_liberados(char fecha_ingresada[MAX_FECHA]){
	char nombre_liberados[MAX_ARCHIVO];
	crear_nombre_txt(nombre_liberados, fecha_ingresada);

	FILE* archivo = fopen(nombre_liberados, LECTURA);
	if(archivo){
		fclose(archivo);
		return true;
	}

	return false;
}

// Pre C: Recibe el nombre de un archivo de presos liberados.
// Post C: Devuelve 'true' si dicho archivo no fue creado previamente (es decir, si no falló la apertura del mismo en modo "lectura").
bool existe_archivo(char nombre_archivo[MAX_ARCHIVO]){
	FILE* archivo = fopen(nombre_archivo, LECTURA);
	if(archivo){
		fclose(archivo);
		return true;
	}
	return false;
}

// Pre C: Los archivos que recibe la función deben haberse podido abrir con éxito en la funcion donde fueron abiertos.
// Post C: Se copian los presos que figuran en uno de los dos primeros archivos ("crucio.dat" o "imperius.dat") en un tercer archivo.
void imprimir_perdonables(FILE* crucio, FILE* imperius, FILE* perdonables){
	preso_t preso_crucio, preso_imperius;
	fread(&preso_crucio, sizeof(preso_t), 1, crucio);
	fread(&preso_imperius, sizeof(preso_t), 1, imperius);

	while((!feof(crucio)) && (!feof(imperius))){
		if(strcmp(preso_crucio.nombre, preso_imperius.nombre) < 0){
			fwrite(&preso_crucio, sizeof(preso_t), 1, perdonables);
			fread(&preso_crucio, sizeof(preso_t), 1, crucio);
		}
		else if(strcmp(preso_crucio.nombre, preso_imperius.nombre) > 0){
			fwrite(&preso_imperius, sizeof(preso_t), 1, perdonables);
			fread(&preso_imperius, sizeof(preso_t), 1, imperius);
		}
		else{
			fread(&preso_crucio, sizeof(preso_t), 1, crucio);
			fread(&preso_imperius, sizeof(preso_t), 1, imperius);			
		}
	}
	while(!feof(crucio)){
		fwrite(&preso_crucio, sizeof(preso_t), 1, perdonables);
		fread(&preso_crucio, sizeof(preso_t), 1, crucio);
	}
	while(!feof(imperius)){
		fwrite(&preso_imperius, sizeof(preso_t), 1, perdonables);
		fread(&preso_imperius, sizeof(preso_t), 1, imperius);
	}
}

// Pre C: Se debe recibir un nombre, junto con la extensión ".dat" (ingresada por el usuario).
// Post C: Se crea un archivo de presos perdonables con el nombre ingresado.
int crear_archivo_perdonables(char nombre_archivo[MAX_ARCHIVO]){
	FILE* perdonables = fopen(nombre_archivo, ESCRITURA);
	if(!perdonables){
		imprimir_error_archivo(nombre_archivo);
		return -1;
	}

	FILE* crucio = fopen(F_CRUCIO, LECTURA);
	if(!crucio){
		fclose(perdonables);
		imprimir_error_archivo(F_CRUCIO);
		return -1;
	}

	FILE* imperius = fopen(F_IMPERIUS, LECTURA);
	if(!imperius){
		fclose(perdonables);
		fclose(crucio);
		imprimir_error_archivo(F_IMPERIUS);
		return -1;
	}

	imprimir_perdonables(crucio, imperius, perdonables);

	fclose(perdonables);
	fclose(crucio);
	fclose(imperius);

	return 0;
}

// Pre C: Se debe recibir un string de la fecha que respete el formato "aaaamm".
// Post C: Devuelve 'true' si el mes de la fecha está en el rango cerrado [1;12].
bool es_fecha_valida(char fecha[MAX_FECHA]){
	int fecha_int = atoi(fecha);
	int mes_fecha = fecha_int % 100;
	return ((mes_fecha >= ENERO) && (mes_fecha <= DICIEMBRE));
}

// Pre C: Se debe recibir un caracter.
// Post C: Devuelve 'true' si el caracter recibido está en mayúscula y es "B", "R" o "M".
bool es_conducta_valida(char conducta){
	return((conducta == BUENA) || (conducta == REGULAR) || (conducta == MALA));
}

// Pre C: Se deben recibir el string fecha y el caracter ingresado como conducta.
// Post C: Devuelve 'true' si la fecha y el caracter conducta son válidos.
bool es_comando_valido(char fecha_ingresada[MAX_FECHA], char conducta_ingresada){
	return((es_fecha_valida(fecha_ingresada)) && (es_conducta_valida(conducta_ingresada)));
}

// Pre C: Recibe los argumentos (y la cantidad de estos) recibidos por el programa. A excepción de argv[0].
// Post C: Devuelve 'true' si los argumentos ingresados coinciden con los valores esperados para ejecutar el comando Perdonables.
bool comando_perdonables_valido(char identificador[MAX_PARAMETRO]){
	return (strcmp(identificador, PERDONABLES) == 0);
}

// Pre C: Recibe los argumentos (y la cantidad de estos) recibidos por el programa. A excepción de argv[0].
// Post C: Devuelve 'true' si los argumentos ingresados coinciden con los valores esperados para ejecutar el comando Liberar.
bool comando_liberar_valido(char identificador[MAX_PARAMETRO], char nombre_perdonables[MAX_ARCHIVO], char fecha_ingresada[MAX_FECHA], char conducta_ingresada){
	int condiciones_cumplidas = 0;
	char nombre_liberados[MAX_ARCHIVO];
	crear_nombre_txt(nombre_liberados, fecha_ingresada);

	if(strcmp(identificador, LIBERAR) == 0){
		condiciones_cumplidas++;
	}
	if(existe_archivo(nombre_perdonables)){
		condiciones_cumplidas++;
	}
	if(!existe_archivo(nombre_liberados)){
		condiciones_cumplidas++;
	}
	if(es_comando_valido(fecha_ingresada, conducta_ingresada)){
		condiciones_cumplidas++;
	}

	return (condiciones_cumplidas == MAX_COND_LIBERAR);
}

// Pre C: Recibe los argumentos (y la cantidad de estos) recibidos por el programa. A excepción de argv[0].
// Post C: Devuelve 'true' si los argumentos ingresados coinciden con los valores esperados para ejecutar el comando Actualizar.
bool comando_actualizar_valido(char identificador[MAX_PARAMETRO], char fecha_ingresada[MAX_FECHA]){
	int condiciones_cumplidas = 0;
	
	if(strcmp(identificador, ACTUALIZAR) == 0){
		condiciones_cumplidas++;
	}
	if(es_fecha_valida(fecha_ingresada)){
		condiciones_cumplidas++;
	}
	if(existe_archivo_liberados(fecha_ingresada)){
		condiciones_cumplidas++;
	}

	return (condiciones_cumplidas == MAX_COND_ACTUALIZAR);
}

// Pre C: Recibe los argumentos (y la cantidad de estos) recibidos por el programa. A excepción de argv[0].
// Post C: Devuelve 'true' si los argumentos ingresados coinciden con los valores esperados para ejecutar el comando Mostrar Liberados.
bool comando_mostrar_liberados_valido(char identificador[MAX_PARAMETRO], char fecha_ingresada[MAX_FECHA]){
	return ((strcmp(identificador, MOSTRAR_LIBERADOS) == 0) && (es_fecha_valida(fecha_ingresada)));
}

// Pre C: Se reciben la fecha ingresada por el usuario y la fecha de liberación del preso.
// Post C: Devuelve 'true' si la fecha ingresada es igual o posterior a la fecha de liberación previamente recibida.
bool fecha_liberacion_valida(char fecha_ingresada[MAX_FECHA], char fecha_liberacion_preso[MAX_FECHA]){
	int fecha_liberacion_preso_int = atoi(fecha_liberacion_preso);
	int fecha_ingresada_int = atoi(fecha_ingresada);
	return (fecha_liberacion_preso_int <= fecha_ingresada_int);
}

// Pre C: Se reciben el caracter conducta ingresado por el usuario y la conducta del preso en el mes solicitado por el usuario.
// Post C: Devuelve 'true' si la conducta del preso en el mes solicitado es igual o mejor que la ingresada por el usuario.
bool conducta_adecuada(char conducta_ingresada, char conducta_preso){
	bool es_adecuada = false;
	if((conducta_ingresada == BUENA) && (conducta_preso == BUENA)){
		es_adecuada = true;
	}
	else if((conducta_ingresada == REGULAR) && ((conducta_preso == BUENA) || (conducta_preso == REGULAR))){
		es_adecuada = true;
	}
	else if(conducta_ingresada == MALA){
		es_adecuada = true;
	}
	return es_adecuada;
}

// Pre C: Recibe dos fechas (la ingresada y la de liberación del preso) y dos conductas (la ingresada y la del preso en el mes solicitado).
// Post C: Devuelve 'true' si la fecha de liberación y conducta del preso son válidas con respecto a las mismas ingresadas por el usuario.
bool puede_ser_liberado(char fecha_ingresada[MAX_FECHA], char fecha_liberacion_preso[MAX_FECHA], char conducta_ingresada, char conducta_preso){
	return ((fecha_liberacion_valida(fecha_ingresada, fecha_liberacion_preso)) && (conducta_adecuada(conducta_ingresada, conducta_preso)));
}

// Pre C: El archivo que recibe debe haber sido abierto con éxito.
// Post C: Imprime el nombre de cada preso que fue liberado por el usuario.
void imprimir_liberados(FILE* liberados, preso_t preso){
	fprintf(liberados, FORMATO_LIBERADOS_PRINTF, preso.nombre);
}

// Pre C: Recibe el nombre del archivo que el usuario busca crear para liberar presos según la fecha y conducta ingresadas.
// Post C: Crea un archivo de texto con los nombres de los presos liberados en un mes detereminado por el usuario.
int liberar_presos(char nombre_perdonables[MAX_ARCHIVO], char fecha_ingresada[MAX_FECHA], char conducta_ingresada){
	char nombre_liberados[MAX_ARCHIVO];
	crear_nombre_txt(nombre_liberados, fecha_ingresada);

	FILE* perdonables = fopen(nombre_perdonables, LECTURA);
	if(!perdonables){
		imprimir_error_archivo(nombre_perdonables);
	}

	FILE* liberados = fopen(nombre_liberados, ESCRITURA);
	if(!liberados){
		fclose(perdonables);
		imprimir_error_archivo(nombre_liberados);
		return -1;
	}

	preso_t preso;
	fread(&preso, sizeof(preso_t), 1, perdonables);

	while(!feof(perdonables)){
		if(puede_ser_liberado(fecha_ingresada, preso.fecha, conducta_ingresada, preso.conducta)){
			imprimir_liberados(liberados, preso);
		}
		fread(&preso, sizeof(preso_t), 1, perdonables);
	}
	fclose(perdonables);
	fclose(liberados);

	return 0;
}

// Pre C: Recibe el nombre del archivo a actualizar y del archivo auxiliar.
// Post C: Reemplaza el archivo viejo por el auxiliar (actualizado).
void reemplazar_archivos(char nombre_archivo[MAX_ARCHIVO], char nombre_archivo_aux[MAX_ARCHIVO]){
	remove(nombre_archivo);
	rename(nombre_archivo_aux, nombre_archivo);
}

// Pre C: Los archivos recibidos deben haberse podido abrir con éxito. Recibe el archivo de los liberados, de crucio y de imperius (junto con sus auxiliares).
// Post C: Copia los presos no liberados los archivos auxiliares "crucio_aux.dat" e "imperius_aux.dat".
void eliminar_liberados(FILE* liberados, FILE* crucio, FILE* imperius, FILE* crucio_aux, FILE* imperius_aux){
	preso_t preso_liberado, preso_crucio, preso_imperius;
	int leidos = fscanf(liberados, FORMATO_LIBERADOS_SCANF, preso_liberado.nombre);
	fread(&preso_crucio, sizeof(preso_t), 1, crucio);
	fread(&preso_imperius, sizeof(preso_t), 1, imperius);

	while((leidos == 1) && ((!feof(crucio)) || (!feof(imperius)))){
		if((strcmp(preso_liberado.nombre, preso_crucio.nombre) == 0) && (!feof(crucio))){
			leidos = fscanf(liberados, FORMATO_LIBERADOS_SCANF, preso_liberado.nombre);
			fread(&preso_crucio, sizeof(preso_t), 1, crucio);
		}
		else if((strcmp(preso_liberado.nombre, preso_crucio.nombre) > 0) && (!feof(crucio))){
			fwrite(&preso_crucio, sizeof(preso_t), 1, crucio_aux);
			fread(&preso_crucio, sizeof(preso_t), 1, crucio);
		}
		
		if((strcmp(preso_liberado.nombre, preso_imperius.nombre) == 0) && (!feof(imperius))){
			leidos = fscanf(liberados, FORMATO_LIBERADOS_SCANF, preso_liberado.nombre);
			fread(&preso_imperius, sizeof(preso_t), 1, imperius);
		}
		else if((strcmp(preso_liberado.nombre, preso_imperius.nombre) > 0) && (!feof(imperius))){
			fwrite(&preso_imperius, sizeof(preso_t), 1, imperius_aux);
			fread(&preso_imperius, sizeof(preso_t), 1, imperius);
		}
		else if((strcmp(preso_liberado.nombre, preso_crucio.nombre) < 0) && (strcmp(preso_liberado.nombre, preso_imperius.nombre) < 0)){
			leidos = fscanf(liberados, FORMATO_LIBERADOS_SCANF, preso_liberado.nombre);
		}
	}

	while(!feof(crucio)){
		fwrite(&preso_crucio, sizeof(preso_t), 1, crucio_aux);
		fread(&preso_crucio, sizeof(preso_t), 1, crucio);
	}

	while(!feof(imperius)){
		fwrite(&preso_imperius, sizeof(preso_t), 1, imperius_aux);
		fread(&preso_imperius, sizeof(preso_t), 1, imperius);
	}
}

// Pre C: Los archivos recibidos ("crucio_aux.dat" e "imperius_aux.dat") deben haberse podido abrir con éxito.
// Post C: Llama a la función que copia los registros de los presos no liberados en los archivos recibidos.
int actualizar_archivos(char nombre_liberados[MAX_ARCHIVO], FILE* crucio_aux, FILE* imperius_aux, char nombre_crucio[MAX_ARCHIVO], char nombre_imperius[MAX_ARCHIVO]){
	FILE* liberados = fopen(nombre_liberados, LECTURA);
	if(!liberados){
		imprimir_error_archivo(nombre_liberados);
		return -1;
	}

	FILE* crucio = fopen(nombre_crucio, LECTURA);
	if(!crucio){
		fclose(liberados);
		imprimir_error_archivo(nombre_crucio);
		return -1;
	}

	FILE* imperius = fopen(nombre_imperius, LECTURA);
	if(!imperius){
		fclose(liberados);
		fclose(crucio);
		imprimir_error_archivo(nombre_imperius);
		return -1;
	}

	eliminar_liberados(liberados, crucio, imperius, crucio_aux, imperius_aux);
	fclose(liberados);
	fclose(crucio);
	fclose(imperius);

	return 0;
}

// Pre C: Recibe la fecha ingresada por el usuario.
// Post C: Una vez copiados los registros a los archivos auxiliares, actualiza los antiguos con las datos de los auxiliares.
int actualizar_presos(char fecha_ingresada[MAX_FECHA]){
	char nombre_liberados[MAX_ARCHIVO];
	crear_nombre_txt(nombre_liberados, fecha_ingresada);

	FILE* crucio_aux = fopen(F_CRUCIO_AUX, ESCRITURA);
	if(!crucio_aux){
		imprimir_error_archivo(F_CRUCIO_AUX);
		return -1;
	}

	FILE* imperius_aux = fopen(F_IMPERIUS_AUX, ESCRITURA);
	if(!imperius_aux){
		fclose(crucio_aux);
		imprimir_error_archivo(F_IMPERIUS_AUX);
		return -1;
	}

	actualizar_archivos(nombre_liberados, crucio_aux, imperius_aux, F_CRUCIO, F_IMPERIUS);
	
	fclose(crucio_aux);
	fclose(imperius_aux);
	
	if(actualizar_archivos(nombre_liberados, crucio_aux, imperius_aux, F_CRUCIO, F_IMPERIUS) == 0){
		reemplazar_archivos(F_CRUCIO, F_CRUCIO_AUX);
		reemplazar_archivos(F_IMPERIUS, F_IMPERIUS_AUX);
	}

	return 0;
}

// Pre C: Recibe la fecha ingresada por el usuario.
// Post C: Imprime por pantalla los presos liberados en la fecha ingresada.
int mostrar_liberados(char fecha_ingresada[MAX_FECHA]){
	char nombre_liberados[MAX_ARCHIVO];
	crear_nombre_txt(nombre_liberados, fecha_ingresada);
	
	FILE* liberados = fopen(nombre_liberados, LECTURA);
	if(!liberados){
		imprimir_error_archivo(nombre_liberados);
		return -1;
	}

	preso_t preso_liberado;
	int contador_liberados = 0;
	int leidos = fscanf(liberados, "%s;", preso_liberado.nombre);
	printf("\e[4m" "\nLos presos liberados (según la conducta mínima exigida al perdonarlos), son los siguientes:\n\n" "\e[0m");

	if(leidos == 0){
		printf("No hay presos perdonables que cumplan con las condiciones solicitadas para ser liberados en la fecha %s\n", fecha_ingresada);
	}
	else{
		while(leidos == 1){
			contador_liberados++;
			printf("Liberado N° %i: %s\n\n", contador_liberados, preso_liberado.nombre);
			leidos = fscanf(liberados, "%s;", preso_liberado.nombre);
		}
	}

	fclose(liberados);

	return 0;
}