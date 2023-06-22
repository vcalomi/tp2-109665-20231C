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
 * Muestra un listado de los hospitales cargados y el hospital activo en caso de haber uno.
 */
void mostrar_estado(menu_t *menu, hospital_t *activo, char *clave_activa);

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
void mostrar_pokemones(hospital_t *activo);

/*
 * Muestra un listado detallando los datos de cada pokemon en el hospital activo.
 */
void mostrar_pokemones_detallado(hospital_t *activo);

/*
 * Destruye el hospital activo, liberando toda la memoria reservada.
 */
void destruir_hospital_activo(menu_t *menu, hospital_t *hospital,
			      char *clave_activa);

#endif /* __MENU_H__ */