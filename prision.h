#ifndef __PRISION_H__
#define __PRISION_H__

// Constantes
	#define LECTURA   	 "r"
	#define ESCRITURA 	 "w"

	#define BUENA 	  'B'
	#define REGULAR   'R'
	#define MALA 	  'M'
	
	#define F_CRUCIO 		  "crucio.dat"
	#define F_IMPERIUS 		  "imperius.dat"
	#define F_CRUCIO_AUX	  "crucio_aux.dat"
	#define F_IMPERIUS_AUX 	  "imperius_aux.dat"
	#define F_LIBERADOS 	  "liberados_"
	#define AYUDA 			  "ayuda"
	#define PERDONABLES 	  "perdonables"
	#define LIBERAR 		  "liberar"
	#define ACTUALIZAR 		  "actualizar"
	#define MOSTRAR_LIBERADOS "mostrar_liberados"
	#define EXTENSION_TXT     ".txt"
	
	#define FORMATO_LIBERADOS_PRINTF "%s\n"
	#define FORMATO_LIBERADOS_SCANF "%[^\n]\n"
	
	#define MAX_COND_ACTUALIZAR		  3
	#define MAX_COND_LIBERAR		  4

	#define ENERO 		   			  1
	#define DICIEMBRE 	  			 12
	
	#define MAX_FECHA 	   			  7
	#define MAX_PARAMETRO 			 18
	#define MAX_ARCHIVO   			 20
	#define MAX_NOMBRE   			200
	

typedef struct preso{
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta; // B: Buena, R: Regular, M: Mala
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA]; // Formato "aaaamm"
	int maldicion_realizada; // 1: Crucio, 2: Imperius
} preso_t;


void imprimir_ayudas();

bool comando_perdonables_valido(char identificador[MAX_PARAMETRO]);

bool comando_liberar_valido(char identificador[MAX_PARAMETRO], char nombre_perdonables[MAX_ARCHIVO], char fecha_ingresada[MAX_FECHA], char conducta_ingresada);

bool comando_actualizar_valido(char identificador[MAX_PARAMETRO], char fecha_ingresada[MAX_FECHA]);

bool comando_mostrar_liberados_valido(char identificador[MAX_PARAMETRO], char fecha_ingresada[MAX_FECHA]);

int crear_archivo_perdonables(char nombre_archivo[MAX_ARCHIVO]);

int liberar_presos(char nombre_perdonables[MAX_ARCHIVO], char fecha_ingresada[MAX_FECHA], char conducta_ingresada);

int actualizar_presos(char fecha_ingresada[MAX_FECHA]);

int mostrar_liberados(char fecha_ingresada[MAX_FECHA]);

#endif /* __PRISION_H__ */