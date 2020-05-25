#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<netdb.h>
#include<string.h>
#include<sys/socket.h>
#include<pthread.h>
#include "serializacion.h"

pthread_t thread;

typedef struct {
	op_code codigo_operacion;
	int id_correlativo;
	int socket_cliente;
	int cola; //Solo para el caso q sea una suscripcion
	int tiempo;
	t_buffer* buffer;
} t_paquete_socket;

typedef struct {
	int cod_operacion;
	int cola_a_suscribir;
	int tiempo;

} t_suscripcion;


int iniciar_servidor(char* ip, char* puerto);
void esperar_cliente(int socket_cliente, void(*procesar_mensaje_recibido)(t_paquete_socket*));
t_paquete_socket* recibir_mensaje_servidor(int socket_cliente);

#endif
