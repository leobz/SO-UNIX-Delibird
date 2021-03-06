#ifndef CACHE_H_
#define CACHE_H_

#include <signal.h>
#include "broker-commons.h"
#include "../utils/listas.h"

typedef struct{
	int id_mensaje;
	int id_correlativo;
	op_code codigo_operacion;
	struct particion_bs* particion_bs;
	struct particion_dinamica *particion_dinamica;
	t_list* suscriptores_enviados;
	t_list* suscriptores_confirmados;
}
t_adm_mensaje;

typedef struct particion_bs {
	int id;
	int tamanio_particion;
	bool esta_libre;
	int offset;
	int size_mensaje;
	int contador_uso;
	int orden_creacion;
	t_adm_mensaje* adm_mensaje;
	struct particion_bs* padre;
	struct particion_bs* primer_hijo;
	struct particion_bs* segundo_hijo;
} t_particion_bs;

typedef struct particion_dinamica {
	int offset;
	int tamanio_particion;
	int contador_uso;
	int orden_creacion;
	bool esta_libre;
	t_adm_mensaje* adm_mensaje;
	struct particion_dinamica* siguiente_particion;
} t_particion_dinamica;

typedef enum {
	PRIMER_AJUSTE = 0, MEJOR_AJUSTE = 1
} t_ordenamiento;

pthread_mutex_t m_cache;

t_particion_bs* particion_bs;
void* memoria_cache;
t_list* particiones_dinamicas;
int ordenamiento;
int id_particion;

int contador_uso;
int orden_creacion;

// ********************************** INICIALIZACION MEMORIA CACHE ********************************** //
void inicializar_memoria_cache();
void inicializar_mutex_cache();
void inicializar_lista_particiones();
void inicializar_particion_bs();
void inicializar_particiones_dinamicas();
int obtener_tamanio_memoria_cache_bs();


// ********************************** FUNCIONES MEMORIA CACHE ********************************** //
int es_particion_dinamica();
int es_buddy_system();
int es_fifo();
int es_lru();
void guardar_en_cache(void* payload, int offset, int size);
void dump_cache(int senial);
char* obtener_fecha_actual();
char* obtener_hora_actual();

// ********************************** FUNCIONES BUDDY SYSTEM *********************************** //
t_particion_bs* agregar_mensaje_memoria_cache_bs(t_mensaje* mensaje, t_adm_mensaje* adm_mensaje);
t_particion_bs* agregar_mensaje_memoria_cache_bs_barra_cero(t_mensaje* mensaje, t_adm_mensaje* adm_mensaje);
int obtener_tamanio_particion_necesaria (int tamanio_mensaje);
void obtener_hojas_libres_con_espacio_suficiente(t_list* hojas_libres, t_particion_bs* particion, int tamanio_particion_necesaria);
bool es_menor_offset(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
bool es_menor_tamanio(t_particion_bs* hoja_libre, t_particion_bs* siguiente_hoja_libre);
void ordenar_hojas_libres_segun_algoritmo_particion_libre(t_list* hojas_libres);
t_particion_bs* dividir_particion_elegida (t_particion_bs* hoja_libre, int tamanio_particion_necesaria);
void cargar_particion_elegida (t_particion_bs* particion_elegida, int payload_size, t_adm_mensaje* adm_mensaje);
void* leer_particion_bs(t_particion_bs* particion);
void liberar_hoja_segun_algoritmo_reemplazo();
void obtener_hojas_posibles_victimas(t_list* hojas_posibles_victimas, t_particion_bs* particion);
bool es_menor_orden_creacion(t_particion_bs* hoja_posible_victima, t_particion_bs* siguiente_hoja_posible_victima);
bool es_menor_contador_uso(t_particion_bs* hoja_posible_victima, t_particion_bs* siguiente_hoja_posible_victima);
void ordenar_hojas_posibles_victimas_segun_algoritmo_reemplazo(t_list* hojas_posibles_victimas);
void liberar_particion_victima(t_particion_bs* particion_victima);
void consolidar_particiones_companieros(t_particion_bs* particion_victima);
void generar_archivo_dump_bs();
void obtener_hojas_bs(t_list* hojas_particion_bs, t_particion_bs* particion);
void agregar_contenido_bs_archivo_dump(FILE* archivo_dump, t_list* hojas_particion_bs);

// ********************************** FUNCIONES PARTICIONES DINAMICAS ********************************** //
t_particion_dinamica* agregar_mensaje_memoria_cache_particion_dinamica(t_mensaje* mensaje,t_adm_mensaje*);
t_particion_dinamica* agregar_mensaje_memoria_cache_particion_dinamica_barra_cero(t_mensaje* mensaje, t_adm_mensaje* admin);
int obtener_particion_minima_suficiente(int, op_code);
t_particion_dinamica* buscar_particion_dinamica_libre(int);
t_list* obtener_particiones_dinamicas_libres();
t_particion_dinamica* crear_particion_dinamica(int, int);
t_particion_dinamica* crear_particion_dinamica_libre(int , int);
void ordenar_segun_algoritmo_de_particiones_libres(t_list*);
bool pd_es_menor_offset(t_particion_dinamica*, t_particion_dinamica*);
bool pd_es_mayor_offset(t_particion_dinamica*, t_particion_dinamica*);
bool pd_es_menor_tamanio(t_particion_dinamica*, t_particion_dinamica*);
t_list* filtrar_particiones_por_tamanio(t_list*, int);
void eliminar_una_particion_dinamica_segun_algoritmo_de_eleccion_de_victima();
void compactar_particiones_dinamicas();
t_list* filtar_particiones_libres_y_suficientes(int);
t_list* obtener_particiones_posibles(int);
t_particion_dinamica* guardar_payload_con_adm_mensaje(void *payload, int tamanio, t_adm_mensaje *admin);
t_particion_dinamica* guardar_payload_en_particion_dinamica(void*, int);
t_particion_dinamica* guardar_payload_en_particion_dinamica_con_adm(void*, int,t_adm_mensaje*);
void* leer_particion_dinamica(t_particion_dinamica*);
int supero_limite_de_eliminaciones(int particiones_eliminadas);
void reubicar_particion(t_particion_dinamica* particion_din,int hueco_particiones);
void eliminar_particiones_libres();
void crear_particion_intermedia(t_particion_dinamica* );
bool pd_es_menor_contador_uso(t_particion_dinamica* particion, t_particion_dinamica* siguiente_particion);
t_particion_dinamica* guardar_payload_con_adm_mensaje(void *payload, int tamanio, t_adm_mensaje *admin);
void unir_particiones_dinamicas_libres();
t_list* obtener_particiones_dinamicas_ocupadas();
void generar_archivo_dump_particion_dinamica();
void agregar_contenido_particion_dinamica_archivo_dump(FILE* archivo_dump);
bool menor_orden_creacion(t_particion_dinamica* particiones, t_particion_dinamica* siguiente_particion) ;
void liberar_particion_dinamica(t_particion_dinamica* particion_victima);
bool pd_es_menor_contador_uso(t_particion_dinamica* particion, t_particion_dinamica* siguiente_particion);
void consolidar_particiones_dinamicas(t_particion_dinamica*particion_victima);
void anterior_particion_libre(t_particion_dinamica* anterior);
void anterior_particion_ocupada(t_particion_dinamica* anterior,t_particion_dinamica* particion_victima);
void eliminar_particiones_libres();
void siguiente_particion_libre(t_particion_dinamica*particion_victima);
void siguiente_particion_ocupada(t_particion_dinamica*particion_victima);

// ********************************** FINALIZACION MEMORIA CACHE ********************************** //
void finalizar_mutex_cache();
void finalizar_lista_particiones();
void elimimar_particiones_bs(t_particion_bs* particion);
void finalizar_particiones_dinamicas();
void finalizar_memoria_cache();


#endif /* CACHE_H_ */
