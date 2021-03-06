/*
 * bitmap.h
 *
 *  Created on: Jul 22, 2020
 *      Author: diego
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#include "commons/string.h"
#include "commons/bitarray.h"
#include "../utils/config.h"
#include "../utils/listas.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include "../utils/diccionarios.h"
#include "../utils/cliente.h"
#define BIT_SIZE(x,y) ((x - 1) / y + 1);


char* path_directorio_metadata;
char* path_directorio_files;
char* path_directorio_blocks;
int id_proceso_gamecard;


pthread_mutex_t mutex_bitmap;

typedef struct{
	int block_size;
	int blocks;
	char* magic_number;
}t_metadata;

typedef struct {
	double tiempo_reintento_conexion;
	double tiempo_reintento_operacion;
	double tiempo_retardo_operacion;
	char* punto_montaje_tallgrass;
	char* ip_broker;
	char* puerto_broker;
	char* ip_gamecard;
	char* puerto_gamecard;
	int id_proceso;
	int block_size;
	int blocks;
	char* magic_number;
	char* path_logger;
} t_gamecard_config;

t_log* logger;
t_log* logger_debug;
t_gamecard_config* gamecard_config;

t_metadata* metadata;


/**
* @NAME: crear_ruta(char* ruta)
* @DESC: Convierte una ruta relativa a absoluta
* Ejemplo: crear_ruta("unDirectorio/unArchivo.bin") => "home/../punto-de-montaje/unDirectorio/unArchivo.bin"
*
*/
char* crear_ruta(char* ruta);
t_bitarray *checkear_bitmap();
char*ruta_bitmap();
void modificar_bit(t_bitarray*bitmap,bool valor,int bit);
void actualizar_archivo_bitmap(t_bitarray*bitmap);
t_bitarray *crear_bitmap(int cant_bloques);
t_bitarray *leer_bitmap();
void setear_bloque_ocupado(int numero_bloque);
void setear_bloque_libre(int numero_bloque);
int bloque_disponible_en_bitmap();

#endif /* BITMAP_H_ */
