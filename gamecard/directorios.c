#include "directorios.h"

void log_debug_metadata(char* pokemon) {
	char* archivo_pokemon = formar_archivo_pokemon(pokemon);
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(archivo_pokemon);
	char* vector_de_bloques = list_to_char_array(metadata_pokemon->blocks);


	log_info(logger, "[MODIFICADO ARCHIVO] %s -> Tamaño:%d , Bloques: %s",
			pokemon, metadata_pokemon->size, vector_de_bloques);

	free(archivo_pokemon);
	free(vector_de_bloques);
	destruir_metadata_pokemon(metadata_pokemon);

}

void crear_archivo_metadata_y_bitmap_fs() {
	metadata = malloc(sizeof(t_metadata));
	metadata->block_size = gamecard_config->block_size;
	metadata->blocks = gamecard_config->blocks;
	metadata->magic_number = gamecard_config->magic_number;

	//Creo archivo Metadata.bin

	char* ruta_absoluta = crear_ruta("Metadata/Metadata.bin");

	t_bloque* bloque_metadata_bin = crear_bloque(ruta_absoluta);
	config_set_value(bloque_metadata_bin, "BLOCK_SIZE",string_itoa(metadata->block_size));
	config_set_value(bloque_metadata_bin, "BLOCKS",string_itoa(metadata->blocks));
	config_set_value(bloque_metadata_bin, "MAGIC_NUMBER",metadata->magic_number);
	config_save(bloque_metadata_bin);
	config_destroy(bloque_metadata_bin);

	//Creo archivo Bitmap.bin
	t_bitarray*bitmap = crear_bitmap(metadata->blocks);
	actualizar_archivo_bitmap(bitmap);
}

//Inicializar directorios-> t config
void inicializar_directorios() {
	//Creo Punto de Montaje si no existe
	DIR *dp;

	if ((dp = opendir(gamecard_config->punto_montaje_tallgrass)) == NULL) {
		printf("No existe punto de montaje: %s", gamecard_config->punto_montaje_tallgrass);

		mkdir(gamecard_config->punto_montaje_tallgrass, 0777);

		if ((dp = opendir(gamecard_config->punto_montaje_tallgrass)) == NULL){
			printf("Imposible crear punto de montaje. Finalizando el programa\n");
			log_error(logger, "Imposible crear punto de montaje. Finalizando el programa");
			exit(-1);
		}
		else {
			printf("Se creó el punto de montaje: %s \n", gamecard_config->punto_montaje_tallgrass);
			log_info(logger, "Se creó el punto de montaje: %s \n", gamecard_config->punto_montaje_tallgrass);
			(void) closedir (dp);
		}
	}
	else {
		(void) closedir (dp);
	}





	//Creo Metadata

	path_directorio_metadata = string_new();
	string_append(&path_directorio_metadata,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_metadata, "/Metadata");
	mkdir(path_directorio_metadata, 0777);
	crear_metadata_para_directorios(path_directorio_metadata);

	//Creo Files
	path_directorio_files = string_new();
	string_append(&path_directorio_files,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_files, "/Files");
	mkdir(path_directorio_files, 0777);
	crear_metadata_para_directorios(path_directorio_files);

	//Creo Blocks
	//TODO: escribir archivos blocks
	path_directorio_blocks = string_new();
	string_append(&path_directorio_blocks,
			gamecard_config->punto_montaje_tallgrass);
	string_append(&path_directorio_blocks, "/Blocks");
	mkdir(path_directorio_blocks, 0777);
	crear_metadata_para_directorios(path_directorio_blocks);

	crear_archivo_metadata_y_bitmap_fs();

	//free(metadata);
	//free(metadata_aux);

	//liberar_paths();

}

void enviar_mensaje_appeared(t_paquete_socket* paquete_socket, t_mensaje_new* mensaje_new){
	int conexion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_error(logger, "No se pude conectar con el broker");
	}
	else {
		int bytes=0;
		void* a_enviar = serializar_appeared_pokemon(&bytes, mensaje_new->pokemon,mensaje_new->posx, mensaje_new->posy,0, paquete_socket->id_mensaje);
		log_info(logger_debug,"Enviando mensaje %s",op_code_to_string(APPEARED_POKEMON));
		enviar_mensaje(conexion, a_enviar, bytes);
		liberar_conexion(conexion);
	}
	free(mensaje_new->pokemon);
	free(mensaje_new);
}

void loggear_msg_new(t_paquete_socket* paquete_socket, t_mensaje_new* mensaje_new) {
	log_info(logger, "[MSG RECIBIDO] ID:%d NEW_POKEMON %s (%d,%d) %d",
			paquete_socket->id_correlativo,mensaje_new->pokemon,
			mensaje_new->posx, mensaje_new->posy,mensaje_new->cantidad);
}

void procesar_new_pokemon(t_paquete_socket* paquete_socket) {

	t_mensaje_new*mensaje_new;
	mensaje_new = deserializar_mensaje_new_pokemon(paquete_socket->buffer);

	loggear_msg_new(paquete_socket, mensaje_new);

	if(dictionary_has_key(archivos_existentes,mensaje_new->pokemon)){//si el archivo ya se encontraba
		checkear_archivo_abierto(mensaje_new->pokemon);
	}

	else{ //si el archivo no existia
		crear_archivo_pokemon(mensaje_new);}

		agregar_posicion(mensaje_new); //aqui tendrias las posiciones dentro del mensaje y la lista de bloques
		sleep(gamecard_config->tiempo_retardo_operacion);

	cerrar_archivo(mensaje_new->pokemon);

	enviar_mensaje_appeared(paquete_socket, mensaje_new);

}

void enviar_mensaje_localized(void* a_enviar,int bytes){
	int conexion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);
	if (conexion == -1) {
		log_error(logger, "No se pude conectar con el broker");
		free(a_enviar);
	}
	else {
		log_info(logger_debug,"Enviando mensaje %s",op_code_to_string(LOCALIZED_POKEMON));
		enviar_mensaje(conexion, a_enviar, bytes);
		liberar_conexion(conexion);
	}
}

void loggear_msg_get(t_paquete_socket* paquete_socket,
		t_mensaje_get* mensaje_get) {
	log_info(logger, "[MSG RECIBIDO] ID:%d GET_POKEMON %s",
			paquete_socket->id_correlativo, mensaje_get->pokemon);
}

void procesar_get_pokemon(t_paquete_socket* paquete_socket){
	t_mensaje_get* mensaje_get;
	mensaje_get=deserializar_mensaje_get_pokemon(paquete_socket->buffer);

	loggear_msg_get(paquete_socket, mensaje_get);

	if(dictionary_has_key(archivos_existentes,mensaje_get->pokemon)){

		checkear_archivo_abierto(mensaje_get->pokemon);
			t_list* posiciones_pokemon = obtener_posiciones_pokemon(mensaje_get->pokemon);
			sleep(gamecard_config->tiempo_retardo_operacion);
		cerrar_archivo(mensaje_get->pokemon);

		log_info(logger_debug,"[GAMECARD] Cerro archivo");
		int bytes=0;
		void*localized_package=serializar_localized_pokemon(&bytes, mensaje_get->pokemon,posiciones_pokemon,0, paquete_socket->id_mensaje);

		enviar_mensaje_localized(localized_package,bytes);
		log_info(logger_debug,"[GAMECARD] Se envio el mensaje localized");
	}
	else{
		log_error(logger_debug,"No se encontro el pokemon :%s",mensaje_get->pokemon);
		log_error(logger,"No se encontro el pokemon :%s",mensaje_get->pokemon);}
}

void enviar_mensaje_caught(t_paquete_socket* paquete_socket, int estado) {
	int conexion = crear_conexion(gamecard_config->ip_broker,gamecard_config->puerto_broker);

	if (conexion == -1) {
		log_error(logger, "No se pude conectar con el broker");
	}
	else {
		int bytes;

		void* a_enviar = serializar_caught_pokemon(&bytes, estado,0, paquete_socket->id_mensaje);
		log_info(logger_debug,"Enviando mensaje %s",op_code_to_string(CAUGHT_POKEMON));
		enviar_mensaje(conexion, a_enviar, bytes);
		liberar_conexion(conexion);
	}
}

void loggear_msg_catch(t_paquete_socket* paquete_socket, t_mensaje_catch* mensaje_catch) {
	log_info(logger, "[MSG RECIBIDO] ID:%d CATCH_POKEMON %s (%d,%d)",
			paquete_socket->id_correlativo, mensaje_catch->pokemon, mensaje_catch->posx, mensaje_catch->posy);
}

void procesar_catch_pokemon(t_paquete_socket* paquete_socket) {
	t_mensaje_catch* mensaje_catch;

	mensaje_catch = deserializar_mensaje_catch_pokemon(paquete_socket->buffer);

	loggear_msg_catch(paquete_socket, mensaje_catch);

	if(dictionary_has_key(archivos_existentes,mensaje_catch->pokemon)){//si el archivo ya se encontraba
		checkear_archivo_abierto(mensaje_catch->pokemon);
			int resultado = decrementar_cantidad(mensaje_catch);
			sleep(gamecard_config->tiempo_retardo_operacion);
		cerrar_archivo(mensaje_catch->pokemon);
		enviar_mensaje_caught(paquete_socket, resultado);
	}
	else{
		log_error(logger_debug,"No se encontro el pokemon :%s",mensaje_catch->pokemon);
		log_error(logger,"No se encontro el pokemon :%s",mensaje_catch->pokemon);
		enviar_mensaje_caught(paquete_socket, FAIL);
	}
	eliminar_mensaje_catch(mensaje_catch);
}

void crear_metadata_para_directorios(char*ruta_directorio){
	string_append(&ruta_directorio, "/Metadata.bin");
	t_bloque* pokemon_config=crear_bloque(ruta_directorio);
	config_set_value(pokemon_config, "DIRECTORY","Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
}

void crear_archivo_pokemon(t_mensaje_new* mensaje_new) {
	pthread_mutex_lock(&mutex_bitmap);
	int bloque_disponible = bloque_disponible_en_bitmap();
	if(bloque_disponible!=-1){
		setear_bloque_ocupado(bloque_disponible);
		pthread_mutex_unlock(&mutex_bitmap);
		char*path_archivo_pokemon = crear_pokemon_metadata(mensaje_new->pokemon);
		crear_archivo_metadata(path_archivo_pokemon,bloque_disponible);
		dictionary_put(cantidad_posiciones_pokemon,mensaje_new->pokemon,0);
		dictionary_put(archivos_existentes,mensaje_new->pokemon,true);//indica que esta abierto

		log_info(logger, "[NUEVO ARCHIVO] %s", mensaje_new->pokemon);
	}
	else{
		pthread_mutex_unlock(&mutex_bitmap);
		log_error(logger, "No existen bloques disponibles para crear el archivo %s", mensaje_new->pokemon);
		exit(1);
	}
}


void crear_archivo_metadata(char *path_pokemonn, int contador_bloques) {

	t_bloque* pokemon_config=crear_bloque(path_pokemonn);

	//crear_bloque_vacio(contador_bloques);
	config_set_value(pokemon_config, "DIRECTORY","N");
	config_set_value(pokemon_config, "SIZE","0");

	char* block = string_new();
	string_append(&block, "[");
	string_append_with_format(&block, "%s", string_itoa(contador_bloques));
	string_append(&block, "]");

	config_set_value(pokemon_config, "BLOCKS", block);
	config_set_value(pokemon_config, "OPEN", "N");

	config_save(pokemon_config);

	config_destroy(pokemon_config);
}


char* crear_pokemon_metadata(char*pokemonn){
	char* path_archivo_pokemon = string_new();
	string_append(&path_archivo_pokemon, "Files/");
	string_append_with_format(&path_archivo_pokemon, "%s",pokemonn);
	char*path_completo=crear_ruta(path_archivo_pokemon);
	mkdir(path_completo, 0777);
	string_append(&path_completo, "/Metadata.bin");
	free(path_archivo_pokemon);

	return path_completo;
}

void setear_abierto_si_corresponde(char* pokemon) {
	pthread_mutex_lock(&mutex_abrir_archivos);
	if (!archivo_esta_abierto(pokemon)) {
		setear_archivo_abierto(pokemon);
		pthread_mutex_unlock(&mutex_abrir_archivos);
	} else {
		pthread_mutex_unlock(&mutex_abrir_archivos);
		sleep(gamecard_config->tiempo_reintento_operacion);
		checkear_archivo_abierto(pokemon);
	}
}

void checkear_archivo_abierto(char*pokemonn){
	if (archivo_esta_abierto(pokemonn)) {
		while(archivo_esta_abierto(pokemonn)){
			sleep(gamecard_config->tiempo_reintento_operacion);
		}
		setear_abierto_si_corresponde(pokemonn);
	}
	else{
		setear_abierto_si_corresponde(pokemonn);
	}
}

bool archivo_esta_abierto(char *pokemonn){
	return dictionary_get(archivos_existentes,pokemonn);
}

void setear_archivo_abierto(char*pokemonn){
	dictionary_put(archivos_existentes,pokemonn,true);
	char*path_pokemon=formar_archivo_pokemon(pokemonn);
	char*path_absoluta=crear_ruta(path_pokemon);
	t_config*pokemon_config=config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN","Y");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	free(path_pokemon);
	free(path_absoluta);
}

void cerrar_archivo(char* pokemonn){
	log_info(logger_debug,"[GAMECARD] Entro a funcion cerrar_archivo()");
	log_info(logger_debug,"[GAMECARD] Pokemon parametro: %s", pokemonn);
	char*path_pokemon = formar_archivo_pokemon(pokemonn);
	char*path_absoluta = crear_ruta(path_pokemon);
	t_config* pokemon_config = config_create(path_absoluta);
	config_set_value(pokemon_config, "OPEN","N");
	config_save(pokemon_config);
	config_destroy(pokemon_config);
	free(path_pokemon);
	free(path_absoluta);
	dictionary_put(archivos_existentes,pokemonn,false);
	log_info(logger_debug,"[GAMECARD] Tamaño diccionario archivos_existentes: %d", dictionary_size(archivos_existentes));
}

void agregar_posicion(t_mensaje_new*mensaje_new){
	t_config* archivo_pokemon_config = leer_config_pokemon(mensaje_new->pokemon);

	char* posx = string_itoa(mensaje_new->posx);
	char* posy = string_itoa(mensaje_new->posy);

	char* posicion_pokemonn = string_new();
	string_append_with_format(&posicion_pokemonn, "%s",posx);
	string_append(&posicion_pokemonn,"-");
	string_append_with_format(&posicion_pokemonn, "%s",posy);

	char* cantidad;

	if(config_has_property(archivo_pokemon_config, posicion_pokemonn)){//si esa posicion ya estaba en el archivo

		int cantidad_pokemon=config_get_int_value(archivo_pokemon_config,posicion_pokemonn);
		cantidad_pokemon+=mensaje_new->cantidad;
		cantidad = string_itoa(cantidad_pokemon);
		config_set_value(archivo_pokemon_config,posicion_pokemonn,cantidad);
		guardar_config_en_archivo_pokemon(archivo_pokemon_config,mensaje_new->pokemon);
	}
	else{ //si es una nueva posicion
		int cant_posiciones = dictionary_get(cantidad_posiciones_pokemon,mensaje_new->pokemon);
		cant_posiciones += 1;
		dictionary_put(cantidad_posiciones_pokemon, mensaje_new->pokemon, cant_posiciones);
		int posicioness=dictionary_get(cantidad_posiciones_pokemon,mensaje_new->pokemon);
		log_info(logger_debug,"Cantidad de Posiciones de %s: %d",mensaje_new->pokemon ,posicioness);
		cantidad = string_itoa(mensaje_new->cantidad);
		config_set_value(archivo_pokemon_config, posicion_pokemonn, cantidad);
		guardar_config_en_archivo_pokemon(archivo_pokemon_config, mensaje_new->pokemon);


	}

	log_debug_metadata(mensaje_new->pokemon);

	config_destroy(archivo_pokemon_config);
	free(posicion_pokemonn);
	free(cantidad);
	free(posx);
	free(posy);
}

t_list* obtener_posiciones_pokemon(char*pokemonn){

	int cant_posiciones=dictionary_get(cantidad_posiciones_pokemon,pokemonn);
	t_metadata_pokemon* metadata_pokemon = leer_metadata_pokemon(formar_archivo_pokemon(pokemonn));
	char* buffer_pokemon = buffer_del_archivo_completo(metadata_pokemon);

	char**string_vector=string_n_split(buffer_pokemon, cant_posiciones, "\n");

//	t_posiciones posiciones_pokemon;
	t_list* posiciones = list_create();
	int iterador=0;

	void separar_y_obtener_posiciones(char*linea){
		char**vector_separado=string_split(linea,"=");
		char*posicion=vector_separado[0];
		char**posiciones_separadas=string_split(posicion,"-");
		t_posiciones* estructura_posicion = malloc(sizeof(t_posiciones));

		estructura_posicion->posx=atoi(posiciones_separadas[0]);
		estructura_posicion->posy=atoi(posiciones_separadas[1]);

		list_add(posiciones, estructura_posicion);
//		posiciones_pokemon[iterador] = estructura_posiciones;

		log_info(logger_debug,"[GAMECARD] Tamaño de lista posiciones: %d", list_size(posiciones));

		iterador++;
	}
	string_iterate_lines(string_vector, separar_y_obtener_posiciones);

	free(metadata_pokemon);
	free(buffer_pokemon);


	return posiciones;
}


int decrementar_cantidad(t_mensaje_catch* mensaje_catch) {
	int resultado = FAIL;
	t_config* archivo_pokemon_config = leer_config_pokemon(mensaje_catch->pokemon);

	char* posx = string_itoa(mensaje_catch->posx);
	char* posy = string_itoa(mensaje_catch->posy);
	char* posicion_pokemon = string_new();
	string_append_with_format(&posicion_pokemon, "%s", posx);
	string_append_with_format(&posicion_pokemon, "-%s", posy);

	if (config_has_property(archivo_pokemon_config, posicion_pokemon)) {

		int cantidad_pokemon = config_get_int_value(archivo_pokemon_config, posicion_pokemon);
		if (cantidad_pokemon == 1) {
			int cant_posiciones=dictionary_get(cantidad_posiciones_pokemon,mensaje_catch->pokemon);
			dictionary_put(cantidad_posiciones_pokemon,mensaje_catch->pokemon,cant_posiciones-1);
			config_remove_key(archivo_pokemon_config, posicion_pokemon);
		}
		else {
			cantidad_pokemon-=1;

			config_set_value(archivo_pokemon_config, posicion_pokemon, string_itoa(cantidad_pokemon));
		}

		guardar_config_en_archivo_pokemon(archivo_pokemon_config, mensaje_catch->pokemon);
		resultado = OK;
	}
	config_destroy(archivo_pokemon_config);
	log_debug_metadata(mensaje_catch->pokemon);
	return resultado;
}

