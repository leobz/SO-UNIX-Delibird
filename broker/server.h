/*
 * server.h
 *
 *  Created on: May 2, 2020
 *      Author: diego
 */

#ifndef SERVER_H_
#define SERVER_H_


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>



#include "../utils/cliente.h"
#include "broker.h"


t_log* logger;
t_broker_config* broker_config;

int id_mensaje; //este es vector de contadores para cada cola cuando les llega un nuevo mensaje


pthread_mutex_t mutex[6];
pthread_mutex_t global;
sem_t cola_vacia[6];
sem_t sem_proceso[6];
sem_t sem;



void procesar_mensaje_recibido(t_paquete_socket* paquete);
t_mensaje* preparar_mensaje(t_paquete_socket* paquete);
void enviar_confirmacion(int id,op_code confirmacion,int socket);



#endif /* SERVER_H_ */
