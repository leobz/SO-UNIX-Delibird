/*
 * broker.c
 *
 *  Created on: May 7, 2020
 *      Author: diego
 */
#include "broker.h"

void* empaquetar_mensaje_broker(t_mensaje* mensaje,int *bytes){


	*bytes=mensaje->payload_size + sizeof(int)*3;
	void* paquete_broker=malloc(*bytes);
	int offset=0;

	memcpy(paquete_broker + offset, &mensaje->codigo_operacion, sizeof(int));
			offset += sizeof(int);

	if ((mensaje->codigo_operacion >= 0) && (mensaje->codigo_operacion <= 2)){ //si es un mensaje de accion le envio el id_mensaje

		memcpy(paquete_broker + offset, &mensaje->id_mensaje, sizeof(int));

	}

	if ((mensaje->codigo_operacion >= 3) && (mensaje->codigo_operacion <= 5)){ //si es un mensaje de respuesta le envio el id_correlativo

		memcpy(paquete_broker + offset, &mensaje->id_correlativo, sizeof(int));


	}
	offset += sizeof(int);

	memcpy(paquete_broker + offset, &mensaje->payload_size, sizeof(int));
		offset += sizeof(int);
	memcpy(paquete_broker + offset, mensaje->payload, mensaje->payload_size);
		offset += mensaje->payload_size;


	return paquete_broker;
}

void parsear_broker_config(t_broker_config *broker_config, t_config *config) {
	broker_config->ip_broker = strdup(
			config_get_string_value(config, "IP_BROKER"));

	broker_config->puerto_broker = strdup(
			config_get_string_value(config, "PUERTO_BROKER"));

}


void destruir_broker_config(t_broker_config *broker_config) {
	free(broker_config->ip_broker);
	free(broker_config->puerto_broker);
	free(broker_config);
}


t_broker_config *cargar_broker_config(char *path_archivo) {
	t_config *config;
	t_broker_config *broker_config;

	config = leer_config(path_archivo);
	broker_config = malloc(sizeof(t_broker_config));

	parsear_broker_config(broker_config, config);
	destruir_config(config);
	return broker_config;
}


void inicializar_broker(t_broker_config **broker_config, t_log **logger) {
	*broker_config = cargar_broker_config("broker.config");
	*logger = iniciar_logger("broker.log", "broker", LOG_LEVEL_INFO);

}

void finalizar_broker(t_broker_config* broker_config, t_log* logger) {
	destruir_broker_config(broker_config);
	destruir_logger(logger);
}




