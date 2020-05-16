#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// ESTRUCTURAS


typedef enum {
	OP_ERROR = -1,
	// Mensaje sin correlativo t_mensaje_sc
	NEW_POKEMON = 0, GET_POKEMON = 1, CATCH_POKEMON = 2,
	// Mensaje con correlativo t_mensaje_cc
	APPEARED_POKEMON = 3, LOCALIZED_POKEMON = 4, CAUGHT_POKEMON = 5,
	SUSCRIPCION = 6
	// TODO: Definir codigos de operacion restantes
} op_code;

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


typedef struct {
	int length_pokemon;
	char* pokemon;
	int posx;
	int posy;
} t_mensaje_appeared;



typedef struct {
	int length_pokemon;
	char* pokemon;
	int posx;
	int posy;
	int cantidad;
}t_mensaje_new;

// PAQUETES GENERAL
t_paquete* crear_paquete(int codigo_operacion, t_buffer* buffer);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, int bytes);



// BUFFERS GENERAL
void* deserializar_buffer_de_un_string(t_buffer* buffer);

// NEW_POKEMON
void* serializar_new_pokemon(int* bytes, char* nombre_pokemon, int pos_x,int pos_y,int cantidad);
t_buffer* buffer_new_pokemon(char* nombre_pokemon, int pos_x, int pos_y,int cantidad);

// GET_POKEMON
void* serializar_get_pokemon(int* bytes, char* nombre_pokemon);
t_buffer* buffer_get_pokemon(char* nombre_pokemon);


// APPEARED_POKEMON
void* serializar_appeared_pokemon(int* bytes, char* nombre_pokemon, int pos_x,int pos_y);

t_buffer* buffer_appeared_pokemon(char* nombre_pokemon, int pos_x, int pos_y);

t_mensaje_appeared* get_mensaje_appeared_by_buffer(t_buffer* buffer);
void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared);

#endif /* SERIALIZACION_H_ */
