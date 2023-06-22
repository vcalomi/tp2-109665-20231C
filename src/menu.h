#ifndef __MENU_H__
#define __MENU_H__

#include <stddef.h>
#include "tp1.h"

typedef struct menu menu_t;

/*
 * Crea el menu reservando la memoria necesaria para el.
 * Devuelve un puntero al menu o NULL en caso de error.
 */
menu_t *crear_menu();

/*
 * Devuelve la cantidad de hospitales en el menu o 0 en caso de error.
 */
size_t cantidad_hospitales(menu_t *menu);

/*
 * Recibe el nombre de un archivo y un nombre para cargar el hospital en el programa.
 * En caso que haya un hospital con ese nombre, se reemplaza el hospital anterior por el nuevo y se guarda el viejo en la variable anterior.
 * Devuelve el menu si cargo el hospital o NULL en caso de error.
 */
menu_t *cargar_hospital_desde_archivo(menu_t *menu, char *nombre_archivo,
				      char *clave, void **anterior);

/*
 * Carga un hospital en el menu.
 * Devuelve el menu si cargo el hospital o NULL en caso de error.
 */
menu_t *cargar_hospital(menu_t *menu, char *clave, void *elemento);

/*
 * Guarda en la variable estado, un listado con los hospitales cargados.
 * En caso de error, no se almacena nada en estado.
 */
void mostrar_estado(menu_t *menu, char *estado);

/*
 * Destruye el menu liberando la memoria reservada.
 */
void destruir_menu(menu_t *menu);

/*
 * Recibe el nombre de un hospital y activa el hospital con ese nombre.
 * Devuelve un puntero al hospital activado o NULL en caso de error o si no hay un hospital con ese nombre.
 */
hospital_t *activar_hospital(menu_t *menu, char *identificador,
			     char *clave_activa);

/*
 * Guarda en la variable pokemones los nombres de los pokemones en el hospital activo.
 * En caso de error, no se almacena nada en pokemones.
 */
void mostrar_pokemones(hospital_t *activo, char *pokemones);

/*
 * Guarda en la variable detalles los datos de los pokemones en el hospital activo.
 * En caso de error, no se almacena nada en detalles.
 */
void mostrar_pokemones_detallado(hospital_t *activo, char *detalles);

/*
 * Destruye el hospital activo, liberando toda la memoria reservada.
 * Devuelve 0 si se libero con exito o -1 en caso de error.
 */
int destruir_hospital_activo(menu_t *menu, hospital_t *hospital,
			     char *clave_activa);

#endif /* __MENU_H__ */