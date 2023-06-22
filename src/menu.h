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
 * Pide al usuario un archivo y un nombre para cargar el hospital en el programa.
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
 * Devuelve los hospitales cargados y el hospital activo o NULL en caso de error.
 */
char *mostrar_estado(menu_t *menu, hospital_t *activo, char *estado);

/*
 * Destruye el menu liberando la memoria reservada.
 */
void destruir_menu(menu_t *menu);

/*
 * Pide una nombre al usuario y activa el hospital con ese nombre.
 * Devuelve un puntero al hospital activado o NULL en caso de error o si no hay un hospital con ese nombre.
 */
hospital_t *activar_hospital(menu_t *menu, char *identificador,
			     char *clave_activa);

/*
 * Muestra un listado con los nombres de los pokemones en el hospital activo.
 */
char *mostrar_pokemones(hospital_t *activo, char *pokemones);

/*
 * Muestra un listado detallando los datos de cada pokemon en el hospital activo.
 */
void mostrar_pokemones_detallado(hospital_t *activo);

/*
 * Destruye el hospital activo, liberando toda la memoria reservada.
 * Devuelve 0 si se libero con exito o -1 en caso de error.
 */
int destruir_hospital_activo(menu_t *menu, hospital_t *hospital,
			     char *clave_activa);

#endif /* __MENU_H__ */