#include "serializacion.h"

t_paquete* crear_paquete(uint32_t codigo_operacion, t_buffer* buffer,uint32_t id_mensaje,uint32_t id_correlativo) {
	t_paquete *paquete = (t_paquete*) malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;
	paquete->id_mensaje=id_mensaje;
	paquete->id_correlativo = id_correlativo;
	paquete->buffer = buffer;

	return paquete;
}


// ELIMINAR
void eliminar_paquete(t_paquete* paquete) {
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_paquete_socket(t_paquete_socket* paquete) {
	if(paquete->buffer != NULL){
		free(paquete->buffer->stream);
		free(paquete->buffer);
	}
	free(paquete);
}

void eliminar_mensaje_appeared(t_mensaje_appeared* mensaje_appeared) {
	free(mensaje_appeared->pokemon);
	free(mensaje_appeared);
}

void eliminar_mensaje_catch(t_mensaje_catch* mensaje_catch) {
	free(mensaje_catch->pokemon);
	free(mensaje_catch);
}

void eliminar_mensaje_new(t_mensaje_new* mensaje_new){
	free(mensaje_new->pokemon);
	free(mensaje_new);
}

void eliminar_mensaje_get(t_mensaje_get* mensaje_get){
	free(mensaje_get->pokemon);
	free(mensaje_get);
}

void eliminar_mensaje_localized(t_mensaje_localized* mensaje_localized) {
	free(mensaje_localized->pokemon);
	free(mensaje_localized);
}


// UTILIDADES
void* serializar_paquete(t_paquete* paquete, uint32_t bytes) {
	void* a_enviar = malloc(bytes);
	uint32_t offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(paquete->id_mensaje), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(paquete->id_correlativo), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

void* deserializar_buffer_de_un_string(t_buffer* buffer) {
	// SOLO SIRVE PARA DESERIALIZAR PAQUETES QUE CONTENGAN SOLO UN STRING
	// NO SIRVE PARA DESERIALIZAR OTRO TIPO DE BUFFERS
	char* msj;
	uint32_t length = 0;

	void* stream = buffer->stream;

	memcpy(&length, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msj = malloc(length);
	memcpy(msj, stream, length);

	return msj;
}

void* serializar_segun_codigo_sin_barra(void*payload,op_code codigo,uint32_t* size){

	*size-=1;

	switch(codigo){

		case NEW_POKEMON:{

			t_mensaje_new* men=deserializar_payload_new_pokemon(payload);

			void*payd=payload_new_sin_barra(men->pokemon,men->posx,men->posy,men->cantidad);
			free(men->pokemon);
			free(men);
			return payd;
			break;
		}
		case GET_POKEMON:{

			t_mensaje_get* men=deserializar_payload_get_pokemon(payload);
			void*payd=payload_get_sin_barra(men->pokemon);
			free(men->pokemon);
			free(men);
			return payd;

			break;
		}
		case CATCH_POKEMON:{

			t_mensaje_catch* men=deserializar_payload_catch_pokemon(payload);
			void*payd=payload_catch_sin_barra(men->pokemon,men->posx,men->posy);
			free(men->pokemon);
			free(men);
			return payd;
			break;
		}
		case APPEARED_POKEMON:{

			t_mensaje_appeared* men=deserializar_payload_appeared_pokemon(payload);

			void*payd=payload_appeared_sin_barra(men->pokemon,men->posx,men->posy);
			free(men->pokemon);
			free(men);
			return payd;

			break;
		}
		case LOCALIZED_POKEMON:{

			t_mensaje_localized* men=deserializar_payload_localized_pokemon(payload);

			void*payd=payload_localized_sin_barra(men->pokemon,men->cantidad_posiciones,men->pos);
			free(men->pokemon);
			free(men);
			return payd;

			break;
		}

		case CAUGHT_POKEMON:{

			*size+=1;
			return payload;

			break;
		}
		default:
			return NULL;
		}
	return NULL;

}

void* serializar_segun_codigo_con_barra(void* payload,op_code codigo,uint32_t* size){

	*size+=1;

	switch(codigo){

		case NEW_POKEMON:{

			t_mensaje_new* men=deserializar_payload_new_pokemon_con_barra(payload);
			char*pokemonn=men->pokemon;
			pokemonn[men->length_pokemon]='\0';

			void*payd=payload_new_con_barra(pokemonn,men->posx,men->posy,men->cantidad);
			free(men->pokemon);
			free(men);
			return payd;


			break;
		}
		case GET_POKEMON:{

			t_mensaje_get* men=deserializar_payload_get_pokemon_con_barra(payload);
			char*pokemonn=men->pokemon;
			pokemonn[men->length_pokemon]='\0';

			void*payd=payload_get_con_barra(men->pokemon);
			free(men->pokemon);
			free(men);
			return payd;



			break;
		}
		case CATCH_POKEMON:{

			t_mensaje_catch* men=deserializar_payload_catch_pokemon_con_barra(payload);
			char*pokemonn=men->pokemon;
			pokemonn[men->length_pokemon]='\0';

			void*payd=payload_catch_con_barra(pokemonn,men->posx,men->posy);
			free(men->pokemon);
			free(men);
			return payd;

			break;
		}
		case APPEARED_POKEMON:{

			t_mensaje_appeared* men=deserializar_payload_appeared_pokemon_con_barra(payload);
			char*pokemonn=men->pokemon;
			pokemonn[men->length_pokemon]='\0';

			void*payd=payload_appeared_con_barra(pokemonn,men->posx,men->posy);
			free(men->pokemon);
			free(men);
			return payd;

			break;
		}
		case LOCALIZED_POKEMON:{

			t_mensaje_localized* men=deserializar_payload_localized_pokemon_con_barra(payload);
			char*pokemonn=men->pokemon;
			pokemonn[men->length_pokemon]='\0';

			void*payd=payload_localized_con_barra(men->pokemon,men->cantidad_posiciones,men->pos);
			free(men->pokemon);
			free(men);
			return payd;

			break;
		}

		case CAUGHT_POKEMON:{

			*size-=1;
			return payload;

			break;
		}
		default:
			return NULL;
		}
	return NULL;

}

// NEW_POKEMON
t_buffer* buffer_new_pokemon(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y,uint32_t cantidad) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length = strlen(pokemon) + 1;

	buffer->size = sizeof(uint32_t) * 4 + pokemon_length;
	buffer->stream = malloc(buffer->size);

	uint32_t offset = 0;
	memcpy(buffer->stream + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(buffer->stream + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, &pos_y, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, &cantidad, sizeof(uint32_t));

	free(pokemon);

	return buffer;
}

void* payload_new_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y,uint32_t cantidad) {

	uint32_t pokemon_length=strlen(nombre_pokemon);
	char *pokemon = strndup(nombre_pokemon,pokemon_length);
	void* payload =malloc(sizeof(uint32_t)*4+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(payload + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, &pos_y, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, &cantidad, sizeof(uint32_t));

	free(pokemon);

	return payload;
}

void* payload_new_con_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y,uint32_t cantidad) {

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length=strlen(pokemon)+1;
	void* payload =malloc(sizeof(uint32_t)*4+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(payload + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, &pos_y, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, &cantidad, sizeof(uint32_t));

	free(pokemon);

	return payload;
}


void* serializar_new_pokemon(uint32_t* bytes, char* nombre_pokemon, uint32_t pos_x,
		uint32_t pos_y, uint32_t cantidad, uint32_t id_mensaje, uint32_t id_correlativo) {

	t_buffer* buffer = buffer_new_pokemon(nombre_pokemon, pos_x, pos_y,cantidad);
	t_paquete *paquete = crear_paquete(NEW_POKEMON, buffer,id_mensaje, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(uint32_t) * 4;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_new* deserializar_mensaje_new_pokemon(t_buffer* buffer) {
	t_mensaje_new* mensaje_new = (t_mensaje_new*) malloc(sizeof(t_mensaje_new));

	uint32_t offset = 0;
	memcpy(&(mensaje_new->length_pokemon), buffer->stream + offset,
			sizeof(uint32_t));
	mensaje_new->pokemon = malloc(mensaje_new->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_new->pokemon, buffer->stream + offset,
			mensaje_new->length_pokemon);
	offset += mensaje_new->length_pokemon;
	memcpy(&(mensaje_new->posx), buffer->stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->posy), buffer->stream + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->cantidad), buffer->stream + offset, sizeof(uint32_t));

	return mensaje_new;
}

t_mensaje_new* deserializar_payload_new_pokemon(void*payload) {
	t_mensaje_new* mensaje_new = (t_mensaje_new*) malloc(sizeof(t_mensaje_new));

	uint32_t offset = 0;
	memcpy(&(mensaje_new->length_pokemon), payload + offset,sizeof(uint32_t));
	mensaje_new->pokemon = malloc(mensaje_new->length_pokemon);
		offset += sizeof(uint32_t);
	memcpy(mensaje_new->pokemon, payload + offset,mensaje_new->length_pokemon);
		offset += mensaje_new->length_pokemon;
	memcpy(&(mensaje_new->posx), payload + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->posy), payload + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->cantidad), payload + offset, sizeof(uint32_t));

	return mensaje_new;
}

t_mensaje_new* deserializar_payload_new_pokemon_con_barra(void*payload) {
	t_mensaje_new* mensaje_new = (t_mensaje_new*) malloc(sizeof(t_mensaje_new));

	uint32_t offset = 0;
	memcpy(&(mensaje_new->length_pokemon), payload + offset,sizeof(uint32_t));
	mensaje_new->pokemon = malloc((mensaje_new->length_pokemon)+1);
		offset += sizeof(uint32_t);
	memcpy(mensaje_new->pokemon, payload + offset,mensaje_new->length_pokemon);
		offset += mensaje_new->length_pokemon;
	memcpy(&(mensaje_new->posx), payload + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->posy), payload + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->cantidad), payload + offset, sizeof(uint32_t));

	free(payload);

	return mensaje_new;
}


t_mensaje_new* deserializar_paquete_new_pokemon(void* package) {

	uint32_t offset = 0;
	uint32_t op = 0;
	uint32_t id=0;
	uint32_t cod = 0;
	uint32_t size = 0;
	memcpy(&(op), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(id), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(cod), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(size), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	t_mensaje_new* mensaje_new = (t_mensaje_new*) malloc(sizeof(t_mensaje_new));

	memcpy(&(mensaje_new->length_pokemon), package + offset, sizeof(uint32_t));

	mensaje_new->pokemon = malloc(mensaje_new->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_new->pokemon, package + offset,
			mensaje_new->length_pokemon);

	offset += mensaje_new->length_pokemon;
	memcpy(&(mensaje_new->posx), package + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->posy), package + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&(mensaje_new->cantidad), package + offset, sizeof(uint32_t));

	free(package);

	return mensaje_new;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//GET_POKEMON


t_buffer* buffer_get_pokemon(char* nombre_pokemon) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length = strlen(pokemon) + 1;

	buffer->size = sizeof(uint32_t) + pokemon_length;
	buffer->stream = malloc(buffer->size);

	uint32_t offset = 0;
	memcpy(buffer->stream + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, pokemon, pokemon_length);

	free(pokemon);

	return buffer;
}


t_buffer* buffer_get_sin_barra(char* nombre_pokemon) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	uint32_t pokemon_length=strlen(nombre_pokemon);
	char *pokemon = strndup(nombre_pokemon,pokemon_length);


	buffer->size = sizeof(uint32_t) + pokemon_length;
	buffer->stream = malloc(buffer->size);

	uint32_t offset = 0;
	memcpy(buffer->stream + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, pokemon, pokemon_length);

	free(pokemon);

	return buffer;
}

void* payload_get_sin_barra(char* nombre_pokemon) {

	uint32_t pokemon_length=strlen(nombre_pokemon);
	char *pokemon = strndup(nombre_pokemon,pokemon_length);

	void* payload =malloc(sizeof(uint32_t)+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);

	free(pokemon);

	return payload;
}


void* payload_get_con_barra(char* nombre_pokemon) {


	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length=strlen(pokemon)+1;

	void* payload =malloc(sizeof(uint32_t)+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);

	free(pokemon);

	return payload;
}

void* serializar_get_pokemon(uint32_t* bytes, char* nombre_pokemon,uint32_t id_mensaje,
		uint32_t id_correlativo) {

	t_buffer* buffer = buffer_get_pokemon(nombre_pokemon);
	t_paquete *paquete = crear_paquete(GET_POKEMON, buffer, id_mensaje, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(uint32_t) * 4;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_get* deserializar_mensaje_get_pokemon(t_buffer* buffer) {
	t_mensaje_get* mensaje_get = (t_mensaje_get*) malloc(sizeof(t_mensaje_get));

	uint32_t offset = 0;
	memcpy(&(mensaje_get->length_pokemon), buffer->stream + offset,sizeof(uint32_t));
	mensaje_get->pokemon = malloc(mensaje_get->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_get->pokemon, buffer->stream + offset,mensaje_get->length_pokemon);

	return mensaje_get;
}

t_mensaje_get* deserializar_paquete_get_pokemon(void* package) {

	uint32_t offset = 0;
	uint32_t op = 0;
	uint32_t id=0;
	uint32_t cod = 0;
	uint32_t size = 0;
	memcpy(&(op), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(id), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(cod), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(size), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	t_mensaje_get* mensaje_get = (t_mensaje_get*) malloc(sizeof(t_mensaje_get));

	memcpy(&(mensaje_get->length_pokemon), package + offset, sizeof(uint32_t));

	mensaje_get->pokemon = malloc(mensaje_get->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_get->pokemon, package + offset,mensaje_get->length_pokemon);

	free(package);

	return mensaje_get;
}

t_mensaje_get* deserializar_payload_get_pokemon(void*payload){

	t_mensaje_get* mensaje_get = (t_mensaje_get*) malloc(sizeof(t_mensaje_get));

	uint32_t offset = 0;
	memcpy(&(mensaje_get->length_pokemon), payload + offset,sizeof(uint32_t));
	mensaje_get->pokemon = malloc(mensaje_get->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_get->pokemon, payload + offset,mensaje_get->length_pokemon);


	return mensaje_get;
}

t_mensaje_get* deserializar_payload_get_pokemon_con_barra(void*payload){

	t_mensaje_get* mensaje_get = (t_mensaje_get*) malloc(sizeof(t_mensaje_get));

	uint32_t offset = 0;
	memcpy(&(mensaje_get->length_pokemon), payload + offset,sizeof(uint32_t));
	mensaje_get->pokemon = malloc((mensaje_get->length_pokemon)+1);
	offset += sizeof(uint32_t);
	memcpy(mensaje_get->pokemon, payload + offset,mensaje_get->length_pokemon);

	free(payload);

	return mensaje_get;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// CATCH_POKEMON
t_buffer* buffer_catch_pokemon(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_lenght = strlen(pokemon) + 1;

	buffer->size = sizeof(uint32_t) * 3 + pokemon_lenght;
	buffer->stream = malloc(buffer->size);

	uint32_t offset = 0;
	memcpy(buffer->stream + offset, &pokemon_lenght, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, pokemon, pokemon_lenght);
	offset += pokemon_lenght;
	memcpy(buffer->stream + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, &pos_y, sizeof(uint32_t));

	free(pokemon);

	return buffer;
}

void* payload_catch_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y) {
	uint32_t pokemon_length=strlen(nombre_pokemon);
	char *pokemon = strndup(nombre_pokemon,pokemon_length);
	void* payload =malloc(sizeof(uint32_t)*3+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(payload + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, &pos_y, sizeof(uint32_t));

	free(pokemon);

	return payload;
}



void* payload_catch_con_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y) {
	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length=strlen(pokemon)+1;
	void* payload =malloc(sizeof(uint32_t)*3+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
		offset += pokemon_length;
	memcpy(payload + offset, &pos_x, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(payload + offset, &pos_y, sizeof(uint32_t));

	free(pokemon);

	return payload;
}


void* serializar_catch_pokemon(uint32_t* bytes, char* nombre_pokemon, uint32_t pos_x,
		uint32_t pos_y,uint32_t id_mensaje, uint32_t id_correlativo) {
	t_buffer* buffer = buffer_catch_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(CATCH_POKEMON, buffer, id_mensaje,id_correlativo);

	*bytes = paquete->buffer->size + sizeof(uint32_t) * 4;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_catch* deserializar_mensaje_catch_pokemon(t_buffer* buffer) {
	t_mensaje_catch* mensaje_catch = (t_mensaje_catch*) malloc(
			sizeof(t_mensaje_catch));

	uint32_t offset = 0;
	memcpy(&(mensaje_catch->length_pokemon), buffer->stream + offset,sizeof(uint32_t));

	mensaje_catch->pokemon = malloc(mensaje_catch->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_catch->pokemon, buffer->stream + offset,mensaje_catch->length_pokemon);

	offset += mensaje_catch->length_pokemon;
	memcpy(&(mensaje_catch->posx), buffer->stream + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&(mensaje_catch->posy), buffer->stream + offset, sizeof(uint32_t));

	return mensaje_catch;
}

t_mensaje_catch* deserializar_payload_catch_pokemon(void* payload) {
	t_mensaje_catch* mensaje_catch = (t_mensaje_catch*) malloc(sizeof(t_mensaje_catch));

	uint32_t offset = 0;
	memcpy(&(mensaje_catch->length_pokemon), payload + offset,sizeof(uint32_t));
	mensaje_catch->pokemon = malloc(mensaje_catch->length_pokemon);
		offset += sizeof(uint32_t);
	memcpy(mensaje_catch->pokemon, payload + offset,mensaje_catch->length_pokemon);
		offset += mensaje_catch->length_pokemon;
	memcpy(&(mensaje_catch->posx), payload + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(mensaje_catch->posy),payload + offset, sizeof(uint32_t));


	return mensaje_catch;
}

t_mensaje_catch* deserializar_payload_catch_pokemon_con_barra(void* payload) {
	t_mensaje_catch* mensaje_catch = (t_mensaje_catch*) malloc(sizeof(t_mensaje_catch));

	uint32_t offset = 0;
	memcpy(&(mensaje_catch->length_pokemon), payload + offset,sizeof(uint32_t));
	mensaje_catch->pokemon = malloc((mensaje_catch->length_pokemon)+1);
		offset += sizeof(uint32_t);
	memcpy(mensaje_catch->pokemon, payload + offset,mensaje_catch->length_pokemon);
		offset += mensaje_catch->length_pokemon;
	memcpy(&(mensaje_catch->posx), payload + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(mensaje_catch->posy),payload + offset, sizeof(uint32_t));

	free(payload);

	return mensaje_catch;
}

t_mensaje_catch* deserializar_paquete_catch_pokemon(void* package) {

	uint32_t offset = 0;
	uint32_t op = 0;
	uint32_t id=0;
	uint32_t cod = 0;
	uint32_t size = 0;
	memcpy(&(op), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(id), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(cod), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(size), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	t_mensaje_catch* mensaje_catch = (t_mensaje_catch*) malloc(sizeof(t_mensaje_catch));

	memcpy(&(mensaje_catch->length_pokemon), package + offset, sizeof(uint32_t));

	mensaje_catch->pokemon = malloc(mensaje_catch->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_catch->pokemon, package + offset,
			mensaje_catch->length_pokemon);

	offset += mensaje_catch->length_pokemon;
	memcpy(&(mensaje_catch->posx), package + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&(mensaje_catch->posy), package + offset, sizeof(uint32_t));

	free(package);

	return mensaje_catch;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// APPEARED_POKEMON
t_buffer* buffer_appeared_pokemon(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_lenght = strlen(pokemon) + 1;

	buffer->size = sizeof(uint32_t) * 3 + pokemon_lenght;
	buffer->stream = malloc(buffer->size);

	uint32_t offset = 0;
	memcpy(buffer->stream + offset, &pokemon_lenght, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, pokemon, pokemon_lenght);
	offset += pokemon_lenght;
	memcpy(buffer->stream + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, &pos_y, sizeof(uint32_t));

	free(pokemon);

	return buffer;
}
void* payload_appeared_sin_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y) {

	uint32_t pokemon_length=strlen(nombre_pokemon);
	char *pokemon = strndup(nombre_pokemon,pokemon_length);
	void* payload =malloc(sizeof(uint32_t)*3+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(payload + offset, &pos_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload+ offset, &pos_y, sizeof(uint32_t));

	free(pokemon);

	return payload;
}

void* payload_appeared_con_barra(char* nombre_pokemon, uint32_t pos_x, uint32_t pos_y) {

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length=strlen(pokemon)+1;

	void* payload =malloc(sizeof(uint32_t)*3+pokemon_length);

	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
		offset += pokemon_length;
	memcpy(payload + offset, &pos_x, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(payload+ offset, &pos_y, sizeof(uint32_t));

	free(pokemon);

	return payload;
}

void* serializar_appeared_pokemon(uint32_t* bytes, char* nombre_pokemon, uint32_t pos_x,uint32_t pos_y,uint32_t id_mensaje, uint32_t id_correlativo) {

	t_buffer* buffer = buffer_appeared_pokemon(nombre_pokemon, pos_x, pos_y);
	t_paquete *paquete = crear_paquete(APPEARED_POKEMON, buffer, id_mensaje,id_correlativo);

	*bytes = paquete->buffer->size + sizeof(uint32_t) * 4;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_appeared* deserializar_mensaje_appeared_pokemon(t_buffer* buffer) {
	t_mensaje_appeared* mensaje_appeared = (t_mensaje_appeared*) malloc(sizeof(t_mensaje_appeared));

	void* stream = buffer->stream;

	memcpy(&(mensaje_appeared->length_pokemon), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	mensaje_appeared->pokemon = malloc(mensaje_appeared->length_pokemon);
	memcpy(mensaje_appeared->pokemon, stream, mensaje_appeared->length_pokemon);
	stream += mensaje_appeared->length_pokemon;

	memcpy(&(mensaje_appeared->posx), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&(mensaje_appeared->posy), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return mensaje_appeared;
}

t_mensaje_appeared* deserializar_payload_appeared_pokemon(void* payload) {
	t_mensaje_appeared* mensaje_appeared = (t_mensaje_appeared*) malloc(sizeof(t_mensaje_appeared));

	void* stream = payload;

	memcpy(&(mensaje_appeared->length_pokemon), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	mensaje_appeared->pokemon = malloc(mensaje_appeared->length_pokemon);
	memcpy(mensaje_appeared->pokemon, stream, mensaje_appeared->length_pokemon);
	stream += mensaje_appeared->length_pokemon;

	memcpy(&(mensaje_appeared->posx), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&(mensaje_appeared->posy), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);


	return mensaje_appeared;
}

t_mensaje_appeared* deserializar_payload_appeared_pokemon_con_barra(void* payload) {
	t_mensaje_appeared* mensaje_appeared = (t_mensaje_appeared*) malloc(sizeof(t_mensaje_appeared));

	void* stream = payload;

	memcpy(&(mensaje_appeared->length_pokemon), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	mensaje_appeared->pokemon = malloc((mensaje_appeared->length_pokemon)+1);
	memcpy(mensaje_appeared->pokemon, stream, mensaje_appeared->length_pokemon);
	stream += mensaje_appeared->length_pokemon;

	memcpy(&(mensaje_appeared->posx), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&(mensaje_appeared->posy), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(payload);

	return mensaje_appeared;
}

t_mensaje_appeared* deserializar_paquete_appeared_pokemon(void* package) {

	uint32_t offset = 0;
	uint32_t op = 0;
	uint32_t id=0;
	uint32_t cod = 0;
	uint32_t size = 0;
	memcpy(&(op), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(id), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(cod), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(size), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	t_mensaje_appeared* mensaje_appeared = (t_mensaje_appeared*) malloc(sizeof(t_mensaje_appeared));

	memcpy(&(mensaje_appeared->length_pokemon), package + offset, sizeof(uint32_t));

	mensaje_appeared->pokemon = malloc(mensaje_appeared->length_pokemon);
	offset += sizeof(uint32_t);
	memcpy(mensaje_appeared->pokemon, package + offset,
			mensaje_appeared->length_pokemon);

	offset += mensaje_appeared->length_pokemon;
	memcpy(&(mensaje_appeared->posx), package + offset, sizeof(uint32_t));

	offset += sizeof(uint32_t);
	memcpy(&(mensaje_appeared->posy), package + offset, sizeof(uint32_t));

	free(package);

	return mensaje_appeared;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCALIZED_POKEMON



t_buffer* buffer_localized_pokemon(char* nombre_pokemon, t_list* posiciones){
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));
	uint32_t cantidad_posiciones = list_size(posiciones);

	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_lenght = strlen(pokemon) + 1;

	buffer->size = sizeof(t_posiciones)*(cantidad_posiciones)+ sizeof(uint32_t)*3 + pokemon_lenght;
	buffer->stream = malloc(buffer->size);

	uint32_t offset = 0;
	memcpy(buffer->stream + offset, &pokemon_lenght, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer->stream + offset, pokemon, pokemon_lenght);
	offset += pokemon_lenght;
	memcpy(buffer->stream + offset, &cantidad_posiciones, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	void copiar_posiciones(t_posiciones* posicion){
		memcpy(buffer->stream + offset, &(posicion->posx), sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(buffer->stream + offset, &(posicion->posy), sizeof(uint32_t));
		offset += sizeof(uint32_t);
	}

	list_iterate(posiciones, (void*)copiar_posiciones);

	free(pokemon);

	return buffer;
}

void* serializar_localized_pokemon(uint32_t* bytes, char* nombre_pokemon, t_list* posiciones, uint32_t id_mensaje, uint32_t id_correlativo) {

	t_buffer* buffer = buffer_localized_pokemon(nombre_pokemon, posiciones);
	t_paquete *paquete = crear_paquete(LOCALIZED_POKEMON, buffer, id_mensaje,id_correlativo);

	*bytes = paquete->buffer->size + sizeof(uint32_t) * 4;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);
	void destruir_posiciones(t_posiciones* posicion){
		free(posicion);
	}
	list_destroy_and_destroy_elements(posiciones, (void*)destruir_posiciones);

	return a_enviar;
}

t_mensaje_localized* deserializar_mensaje_localized_pokemon(t_buffer* buffer) {
	t_mensaje_localized* mensaje_localized = (t_mensaje_localized*) malloc((buffer->size));

	void* stream = buffer->stream;

	memcpy(&(mensaje_localized->length_pokemon), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	mensaje_localized->pokemon = malloc(mensaje_localized->length_pokemon);

	memcpy(mensaje_localized->pokemon, stream, mensaje_localized->length_pokemon);
	stream += mensaje_localized->length_pokemon;

	memcpy(&(mensaje_localized->cantidad_posiciones), stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	for(uint32_t i=0;i<mensaje_localized->cantidad_posiciones;i++){

	memcpy(&(mensaje_localized->pos[i].posx), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

	memcpy(&(mensaje_localized->pos[i].posy), stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
	}


	return mensaje_localized;
}


t_mensaje_localized* deserializar_paquete_localized_pokemon(void* package) {

	uint32_t offset = 0;
	uint32_t op = 0;
	uint32_t id=0;
	uint32_t cod = 0;
	uint32_t size= 0;
	memcpy(&(op), package + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(id), package + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(cod), package + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	memcpy(&(size), package + offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	t_mensaje_localized* mensaje_localized = (t_mensaje_localized*)malloc((size));


	memcpy(&(mensaje_localized->length_pokemon), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	mensaje_localized->pokemon = malloc(mensaje_localized->length_pokemon);

	memcpy(mensaje_localized->pokemon, package+offset,mensaje_localized->length_pokemon);
		offset += mensaje_localized->length_pokemon;

	memcpy(&(mensaje_localized->cantidad_posiciones), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	for(uint32_t i=0;i<mensaje_localized->cantidad_posiciones;i++){

		memcpy(&(mensaje_localized->pos[i].posx), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

		memcpy(&(mensaje_localized->pos[i].posy), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	}
	free(package);

	return mensaje_localized;
}


void* payload_localized_sin_barra(char* nombre_pokemon, uint32_t cantidad_posiciones,t_posiciones pos[]){


	uint32_t pokemon_length=strlen(nombre_pokemon);
	char *pokemon = strndup(nombre_pokemon,pokemon_length);
	uint32_t cant_coordenadas=cantidad_posiciones*2;
	uint32_t size = sizeof(t_posiciones)*(cantidad_posiciones)+ sizeof(uint32_t)*3 + pokemon_length;
	void* payload = malloc(size);


	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(payload + offset, &cantidad_posiciones, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	for(uint32_t i=0;i<cantidad_posiciones;i++){

	memcpy(payload + offset, &pos[i].posx, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
	memcpy(payload+ offset, &pos[i].posy, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
	}

	free(pokemon);

	return payload;
}

void* payload_localized_con_barra(char* nombre_pokemon, uint32_t cantidad_posiciones,t_posiciones pos[]){


	char *pokemon = strdup(nombre_pokemon);
	uint32_t pokemon_length=strlen(pokemon)+1;
	uint32_t cant_coordenadas=cantidad_posiciones*2;
	uint32_t size = sizeof(t_posiciones)*(cantidad_posiciones)+ sizeof(uint32_t)*3 + pokemon_length;
	void* payload = malloc(size);


	uint32_t offset = 0;
	memcpy(payload + offset, &pokemon_length, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(payload + offset, pokemon, pokemon_length);
	offset += pokemon_length;
	memcpy(payload + offset, &cantidad_posiciones, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	for(uint32_t i=0;i<cantidad_posiciones;i++){

	memcpy(payload + offset, &pos[i].posx, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
	memcpy(payload+ offset, &pos[i].posy, sizeof(uint32_t));
		offset+=sizeof(uint32_t);
	}

	free(pokemon);

	return payload;
}


t_mensaje_localized* deserializar_payload_localized_pokemon(void* package) {

	uint32_t length=0;
	char*pokemonn;
	uint32_t cant_posiciones;

	uint32_t offset=0;

	memcpy(&(length), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	pokemonn = malloc(length);

	memcpy(pokemonn, package+offset, length);
		offset += length;

	memcpy(&(cant_posiciones), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	uint32_t size=sizeof(t_posiciones)*(cant_posiciones)+ sizeof(uint32_t)*3 + length;

	t_mensaje_localized* mensaje_localized = malloc(size);

	mensaje_localized->length_pokemon=length;
	mensaje_localized->pokemon=pokemonn;
	mensaje_localized->cantidad_posiciones=cant_posiciones;

	for(uint32_t i=0;i<mensaje_localized->cantidad_posiciones;i++){

	memcpy(&(mensaje_localized->pos[i].posx), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	memcpy(&(mensaje_localized->pos[i].posy), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	}


	return mensaje_localized;
}



t_mensaje_localized* deserializar_payload_localized_pokemon_con_barra(void* package) {


	uint32_t length=0;
	char*pokemonn;
	uint32_t cant_posiciones;

	uint32_t offset=0;

	memcpy(&(length), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	pokemonn = malloc((length)+1);

	memcpy(pokemonn, package+offset, length);
		offset += length;

	memcpy(&(cant_posiciones), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	uint32_t size=sizeof(t_posiciones)*(cant_posiciones)+ sizeof(uint32_t)*3 + length;

	t_mensaje_localized* mensaje_localized = malloc(size);

	mensaje_localized->length_pokemon=length;
	mensaje_localized->pokemon=pokemonn;
	mensaje_localized->cantidad_posiciones=cant_posiciones;


	for(uint32_t i=0;i<mensaje_localized->cantidad_posiciones;i++){

	memcpy(&(mensaje_localized->pos[i].posx), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);

	memcpy(&(mensaje_localized->pos[i].posy), package+offset, sizeof(uint32_t));
		offset += sizeof(uint32_t);
	}

	free(package);

	return mensaje_localized;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAUGHT_POKEMON
t_buffer* buffer_caught_pokemon(uint32_t estado) {
	t_buffer* buffer = (t_buffer*) malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t);
	buffer->stream = malloc(sizeof(uint32_t));

	memcpy(buffer->stream, &estado, sizeof(uint32_t));

	return buffer;
}



void* serializar_caught_pokemon(uint32_t* bytes, uint32_t estado,uint32_t id_mensaje, uint32_t id_correlativo) {
	t_buffer* buffer = buffer_caught_pokemon(estado);
	t_paquete *paquete = crear_paquete(CAUGHT_POKEMON, buffer, id_mensaje, id_correlativo);

	*bytes = paquete->buffer->size + sizeof(uint32_t) * 4;
	void* a_enviar = serializar_paquete(paquete, *bytes);
	eliminar_paquete(paquete);

	return a_enviar;
}

t_mensaje_caught* deserializar_mensaje_caught_pokemon(t_buffer* buffer) {
	t_mensaje_caught* mensaje_caught = (t_mensaje_caught*) malloc(sizeof(t_mensaje_caught));

	memcpy(&(mensaje_caught->resultado), buffer->stream ,sizeof(uint32_t));
	return mensaje_caught;
}


t_mensaje_caught* deserializar_paquete_caught_pokemon(void* package) {

	uint32_t offset = 0;
	uint32_t op = 0;
	uint32_t id=0;
	uint32_t size = 0;
	memcpy(&(op), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(id), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	t_mensaje_caught* mensaje_caught = (t_mensaje_caught*) malloc(sizeof(t_mensaje_caught));

	memcpy(&(mensaje_caught->id_correlativo), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&(size), package + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(&(mensaje_caught->resultado), package + offset, sizeof(uint32_t));

	return mensaje_caught;
}

/////////////////////////////////////////////////////////////////////////////////////
void* empaquetar_suscripcion(t_suscripcion* suscripcion){
	void*a_enviar=malloc(sizeof(uint32_t)*3);
	uint32_t offset=0;

	memcpy(a_enviar + offset, &suscripcion->cod_operacion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &suscripcion->cola_a_suscribir, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, &suscripcion->id_proceso, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return a_enviar;
};

t_confirmacion* deserializar_confirmacion(void*mensaje){
	t_confirmacion* confirmacion=malloc(sizeof(t_confirmacion));

	uint32_t offset=0;

	memcpy(&(confirmacion->confirmacion),mensaje,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(&(confirmacion->id_mensaje),mensaje+offset,sizeof(uint32_t));

	return confirmacion;
}
/*-------------------------------------------------------------------------------------*/
char* op_code_to_string(uint32_t enum_value) {
	switch (enum_value) {
	case OP_ERROR:
		return "OP_ERROR";
	case NEW_POKEMON:
		return "NEW_POKEMON";
	case GET_POKEMON:
		return "GET_POKEMON";
	case CATCH_POKEMON:
		return "CATCH_POKEMON";
	case APPEARED_POKEMON:
		return "APPEARED_POKEMON";
	case LOCALIZED_POKEMON:
		return "LOCALIZED_POKEMON";
	case CAUGHT_POKEMON:
		return "CAUGHT_POKEMON";
	case SUSCRIPCION:
		return "SUSCRIPCION";
	default:
		return "NULL";
	}
}

uint32_t string_to_op_code(char* enum_cola) {

	if (strcmp(enum_cola, "NEW_POKEMON") == 0)
		return NEW_POKEMON;

	else if (strcmp(enum_cola, "GET_POKEMON") == 0)
		return GET_POKEMON;

	else if (strcmp(enum_cola, "CATCH_POKEMON") == 0)
		return CATCH_POKEMON;

	else if (strcmp(enum_cola, "APPEARED_POKEMON") == 0)
		return APPEARED_POKEMON;

	else if (strcmp(enum_cola, "LOCALIZED_POKEMON") == 0)
		return LOCALIZED_POKEMON;

	else if (strcmp(enum_cola, "CAUGHT_POKEMON") == 0)
		return CAUGHT_POKEMON;

	else if (strcmp(enum_cola, "SUSCRIPCION") == 0)
		return SUSCRIPCION;

	else {
		return OP_ERROR;
	}
}

char* value_to_state(uint32_t value){

	if(value==1)
		return "OK";
	else
		return "FAIL";
}

char* pasar_a_char(uint32_t numero){

	uint32_t length = snprintf( NULL, 0, "%d", numero);
	char* numero_char = malloc( length + 1 );
	snprintf(numero_char, length + 1, "%d", numero);

	return numero_char;

}

t_suscripcion* crear_t_suscripcion(uint32_t id_proceso, uint32_t cola) {
	t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion));
	suscripcion->cod_operacion = SUSCRIPCION;
	suscripcion->cola_a_suscribir = cola;
	suscripcion->id_proceso = id_proceso;
	return suscripcion;
}
