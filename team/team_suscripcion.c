#include "team_suscripcion.h"


t_datos_suscripcion* crear_datos_suscripcion(int id_proceso, int cola) {
	t_datos_suscripcion* datos_suscripcion = malloc(sizeof(t_datos_suscripcion));
	datos_suscripcion->conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);
	datos_suscripcion->cola = cola;
	//TIENE QUE SER EL MISMO ID_PROCESO PARA TODAS LAS COLAS
	datos_suscripcion->id_proceso = id_proceso * 10 + datos_suscripcion->cola;
	return datos_suscripcion;
}

void solicitar_suscripcion(int id_proceso, int cola, int conexion) {
	t_suscripcion* suscripcion = crear_t_suscripcion(id_proceso, cola);
	void* a_enviar = empaquetar_suscripcion(suscripcion);
	log_info(logger, "Conexion %s establecida con [Broker]", op_code_to_string(cola));
	enviar_mensaje(conexion, a_enviar, sizeof(int) * 3);
	free(suscripcion);

}


void recibir_mensajes_team(t_datos_suscripcion* datos_suscripcion) {
	iniciar_suscripcion(datos_suscripcion);

		while (true){
			t_paquete_socket* paquete =  recibir_mensajes(datos_suscripcion->conexion);

			if (paquete->codigo_operacion != OP_ERROR) {
				procesar_mensaje_recibido_broker(paquete);
			}
			else {
				close(datos_suscripcion->conexion);
				datos_suscripcion->conexion = -1;
				iniciar_suscripcion(datos_suscripcion);
				free(paquete);
			}
		}
}


void iniciar_suscripcion(t_datos_suscripcion* datos_suscripcion) {
	while (datos_suscripcion->conexion == -1) {
		liberar_conexion(datos_suscripcion->conexion);
		//printf("ERROR: Conexion %s con [Broker] no establecida\n", op_code_to_string(datos_suscripcion->cola));
		sleep(1);
		datos_suscripcion->conexion = crear_conexion(team_config->ip_broker, team_config->puerto_broker);
	}

	solicitar_suscripcion(datos_suscripcion->id_proceso, datos_suscripcion->cola, datos_suscripcion->conexion);
}


void iniciar_hilo_suscripcion(t_datos_suscripcion* datos_suscripcion) {
	pthread_t hilo_suscripcion;
	pthread_create(&hilo_suscripcion, NULL, (void*) recibir_mensajes_team, datos_suscripcion);
	pthread_detach(hilo_suscripcion);
}
