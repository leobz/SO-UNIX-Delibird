
#include "main.h"



int main() {

	inicializar_broker(&broker_config,&logger);
	char*ip=broker_config->ip_broker;
	char*puerto=broker_config->puerto_broker;
	int socket_servidor = iniciar_servidor(ip, puerto);


	for(int i = 0; i < 6; i++)
	     sem_init(&cola_vacia[i], 0, 0);


	for(int j = 0; j < 6; j++)
		     sem_init(&sem_proceso[j], 0, 0);

	pthread_create(&sem_mensajes[NEW_POKEMON],NULL,(void*)extraer_new_pokemon,NULL);
	pthread_create(&sem_mensajes[GET_POKEMON],NULL,(void*)extraer_get_pokemon,NULL);
	pthread_create(&sem_mensajes[CATCH_POKEMON],NULL,(void*)extraer_catch_pokemon,NULL);
	pthread_create(&sem_mensajes[APPEARED_POKEMON],NULL,(void*)extraer_appeared_pokemon,NULL);
	pthread_create(&sem_mensajes[LOCALIZED_POKEMON],NULL,(void*)extraer_localized_pokemon,NULL);
	pthread_create(&sem_mensajes[CAUGHT_POKEMON],NULL,(void*)extraer_caught_pokemon,NULL);

	while (1) {
		esperar_cliente(socket_servidor, &procesar_mensaje_recibido);
	}

	pthread_detach(sem_mensajes[NEW_POKEMON]);
	pthread_detach(sem_mensajes[GET_POKEMON]);
	pthread_detach(sem_mensajes[CATCH_POKEMON]);
	pthread_detach(sem_mensajes[APPEARED_POKEMON]);
	pthread_detach(sem_mensajes[LOCALIZED_POKEMON]);
	pthread_detach(sem_mensajes[CAUGHT_POKEMON]);

	finalizar_broker(broker_config,logger);
	return 0;
}

void extraer_new_pokemon(){
	while(1){
		sem_wait(&cola_vacia[NEW_POKEMON]);

		pthread_mutex_lock(&mutex[NEW_POKEMON]);
		mensaje[NEW_POKEMON] = extraer_mensaje(NEW_POKEMON);
		pthread_mutex_unlock(&mutex[NEW_POKEMON]);
		//t_cola_proceso *proceso;
		//int bytes=0;

		/*while(proceso_vacio[NEW_POKEMON]==false){
			sem_wait(&sem_proceso[NEW_POKEMON]);
				proceso=desencolar_proceso(NEW_POKEMON);
				void *sent_package=empaquetar_mensaje_broker(mensaje[NEW_POKEMON],&bytes);
				enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		}*/
		log_info(logger, "mensaje: %d",mensaje[NEW_POKEMON]->id_correlativo);
		free(mensaje[NEW_POKEMON]->payload);
		free(mensaje[NEW_POKEMON]);
	}
}



void extraer_get_pokemon(){
	while(1){
		sem_wait(&cola_vacia[GET_POKEMON]);
		pthread_mutex_lock(&mutex[GET_POKEMON]);
		mensaje[GET_POKEMON] = extraer_mensaje(GET_POKEMON);
		pthread_mutex_unlock(&mutex[GET_POKEMON]);
		//t_cola_proceso *proceso;
		//int bytes=0;

		/*while(proceso_vacio[GET_POKEMON]==false){
			sem_wait(&sem_proceso[GET_POKEMON]);
				proceso=desencolar_proceso(GET_POKEMON);
				void *sent_package=empaquetar_mensaje_broker(mensaje[GET_POKEMON],&bytes);
				enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		}*/
		log_info(logger, "id_correlativo: %d",mensaje[GET_POKEMON]->id_correlativo);
		free(mensaje[GET_POKEMON]->payload);
		free(mensaje[GET_POKEMON]);
	}
}


void extraer_catch_pokemon(){
	while(1){

		log_info(logger, "Hola");

		if(cola_procesos[CATCH_POKEMON][FRENTE]==NULL)//Me permite que verificar que la cola no este vacia si se desuscribe el gameboy
			sem_wait(&sem_proceso[CATCH_POKEMON]); //existen procesos en la cola



		sem_wait(&cola_vacia[CATCH_POKEMON]); //existen mensajes en la cola

		pthread_mutex_lock(&mutex[CATCH_POKEMON]);
		mensaje[CATCH_POKEMON] = extraer_mensaje(CATCH_POKEMON);


		//t_cola_proceso *proceso;
		int bytes=0;

		//while(proceso_vacio[CATCH_POKEMON][VACIA]==false){
		log_info(logger, "id_correlativo: %d",mensaje[CATCH_POKEMON]->id_correlativo);



			//proceso=desencolar_proceso(CATCH_POKEMON);

			//log_info(logger, "socket: %d",proceso->socket_cliente);

				void *sent_package=empaquetar_mensaje_broker(mensaje[CATCH_POKEMON],&bytes);

				t_mensaje_catch *mensaje_catch= deserializar_paquete_catch_pokemon(sent_package);

				log_info(logger,
									"Mensaje recibido de [Broker]: CATCH_POKEMON %s %d %d",
									mensaje_catch->pokemon, mensaje_catch->pos_x,
									mensaje_catch->pos_y);
							free(mensaje_catch->pokemon);
							free(mensaje_catch);

				//enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		//}
		//proceso_vacio[CATCH_POKEMON][VACIA]=false;
		//proceso_vacio[CATCH_POKEMON][ULTIMO]=false;

		//free(sent_package);
		free(mensaje[CATCH_POKEMON]->payload);
		free(mensaje[CATCH_POKEMON]);
		pthread_mutex_unlock(&mutex[CATCH_POKEMON]);

	}
}

void extraer_appeared_pokemon(){
	while(1){
		sem_wait(&cola_vacia[APPEARED_POKEMON]);
		pthread_mutex_lock(&mutex[APPEARED_POKEMON]);
		mensaje[APPEARED_POKEMON] = extraer_mensaje(APPEARED_POKEMON);


		pthread_mutex_unlock(&mutex[APPEARED_POKEMON]);
		//t_cola_proceso *proceso;
		//int bytes=0;

		/*while(proceso_vacio[APPEARED_POKEMON]==false){
			sem_wait(&sem_proceso[APPEARED_POKEMON]);
				proceso=desencolar_proceso(APPEARED_POKEMON);
				void *sent_package=empaquetar_mensaje_broker(mensaje[APPEARED_POKEMON],&bytes);
				enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		}*/
		log_info(logger, "id_correlativo: %d",mensaje[APPEARED_POKEMON]->id_correlativo);
		free(mensaje[APPEARED_POKEMON]->payload);
		free(mensaje[APPEARED_POKEMON]);
	}
}

void extraer_localized_pokemon(){
	while(1){
		sem_wait(&cola_vacia[LOCALIZED_POKEMON]);
		pthread_mutex_lock(&mutex[LOCALIZED_POKEMON]);
		mensaje[LOCALIZED_POKEMON] = extraer_mensaje(LOCALIZED_POKEMON);
		pthread_mutex_unlock(&mutex[LOCALIZED_POKEMON]);
		//t_cola_proceso *proceso;
		//int bytes=0;

		/*while(proceso_vacio[LOCALIZED_POKEMON]==false){
			sem_wait(&sem_proceso[LOCALIZED_POKEMON]);
				proceso=desencolar_proceso(LOCALIZED_POKEMON);
				void *sent_package=empaquetar_mensaje_broker(mensaje[LOCALIZED_POKEMON],&bytes);
				enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		}*/
		log_info(logger, "id_correlativo: %d",mensaje[LOCALIZED_POKEMON]->id_correlativo);
		free(mensaje[LOCALIZED_POKEMON]->payload);
		free(mensaje[LOCALIZED_POKEMON]);
	}
}

void extraer_caught_pokemon(){
	while(1){
		sem_wait(&cola_vacia[CAUGHT_POKEMON]);
		pthread_mutex_lock(&mutex[CAUGHT_POKEMON]);
		mensaje[CAUGHT_POKEMON] = extraer_mensaje(CAUGHT_POKEMON);
		pthread_mutex_unlock(&mutex[CAUGHT_POKEMON]);
		//t_cola_proceso *proceso;
		//int bytes=0;

		/*while(proceso_vacio[CAUGHT_POKEMON]==false){
			sem_wait(&sem_proceso[CAUGHT_POKEMON]);
				proceso=desencolar_proceso(CAUGHT_POKEMON);
				void *sent_package=empaquetar_mensaje_broker(mensaje[CAUGHT_POKEMON],&bytes);
				enviar_mensaje(proceso->socket_cliente,sent_package,bytes);
				//proceso->mensaje_recibido=malloc(sizeof(t_cola_mensaje_recibido));
		}*/
		log_info(logger, "id_correlativo: %d",mensaje[CAUGHT_POKEMON]->id_correlativo);
		free(mensaje[CAUGHT_POKEMON]->payload);
		free(mensaje[CAUGHT_POKEMON]);
	}
}


