/*
 * gamecard.h
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#ifndef GAMECARD_H_
#define GAMECARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "bloques.h"
#include "../utils/log.h"
#include "../utils/servidor.h"
#include <sys/stat.h>
#include <dirent.h>

// Estructuras
pthread_mutex_t mutex_abrir_archivos;


// Variables
t_dictionary* archivos_existentes;
t_dictionary* cantidad_posiciones_pokemon;
pthread_t suscripcion_broker;





// Funciones
t_gamecard_config* cargar_gamecard_config(char* path_archivo);
void suscribirme_al_broker();
void destruir_gamecard_config(t_gamecard_config* gamecard_config);
void inicializar_gamecard();
void inicializar_diccionarios();
void cargar_pokemones_existentes();
void cargar_posiciones_existentes();
void finalizar_gamecard();
void inicializar_semaforos();

#endif /* GAMECARD_H_ */
