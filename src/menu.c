#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "tp1.h"
#include "menu.h"
#include "hash.h"

struct menu {
	hash_t *hospitales;
	size_t cantidad_hospitales;
};

menu_t *crear_menu()
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;
	menu->hospitales = hash_crear(10);
	menu->cantidad_hospitales = 0;
	return menu;
}

void to_lower(char *cadena)
{
	int i = 0;
	while (cadena[i] != '\0') {
		cadena[i] = (char)tolower(cadena[i]);
		i++;
	}
}

bool listar_hospitales(const char *clave, void *valor, void *aux)
{
	if (strcmp(clave, (char *)aux) == 0)
		return true;
	printf("- Hospital %s\n", clave);
	return true;
}

bool destruir_hospitales(const char *clave, void *elemento, void *aux)
{
	hospital_destruir((hospital_t *)elemento);
	return true;
}

bool imprimir_pokemones(pokemon_t *pokemon, void *aux)
{
	printf("%s\n", pokemon_nombre(pokemon));
	return true;
}

bool imprimir_pokemones_detallado(pokemon_t *pokemon, void *aux)
{
	printf("Id del pokemon: %ld, Nombre: %s, Salud: %ld, Entrenador/es: %s\n",
	       pokemon_id(pokemon), pokemon_nombre(pokemon),
	       pokemon_salud(pokemon), pokemon_entrenador(pokemon));
	return true;
}

menu_t *cargar_hospital_desde_archivo(menu_t *menu, char *nombre_archivo,
				      char *clave, void **anterior)
{
	hospital_t *hospital = hospital_crear_desde_archivo(nombre_archivo);
	if (hospital == NULL) {
		return NULL;
	}

	hash_insertar(menu->hospitales, clave, hospital, anterior);
	menu->cantidad_hospitales++;
	return menu;
}

menu_t *cargar_hospital(menu_t *menu, char *clave, void *elemento)
{
	if (!menu)
		return NULL;
	hash_insertar(menu->hospitales, clave, (hospital_t *)elemento, NULL);
	return menu;
}

void mostrar_estado(menu_t *menu, hospital_t *activo, char *clave_activa)
{
	if (menu->cantidad_hospitales == 0) {
		printf("No hay ningun hospital cargado\n");
		return;
	}
	if (menu->cantidad_hospitales == 1)
		printf("Hay %ld Hospital cargado:\n",
		       menu->cantidad_hospitales);
	else
		printf("Hay %ld Hospitales cargados:\n",
		       menu->cantidad_hospitales);
	hash_con_cada_clave(menu->hospitales, listar_hospitales,
			    (void *)clave_activa);
	if (activo != NULL) {
		printf("Hospital activo - %s\n", clave_activa);
	} else
		printf("No hay hospital activo\n");
}

hospital_t *activar_hospital(menu_t *menu, char *clave_activa)
{
	char identificador[100];
	printf("Ingresa la clave del hospital que deseas activar: ");
	scanf("%s", identificador);

	void *hospital_void_ptr = hash_obtener(menu->hospitales, identificador);
	if (!hospital_void_ptr) {
		printf("No hay un hospital con ese nombre, intente nuevamente\n");
		return NULL;
	}
	hospital_t *hospital = (hospital_t *)hospital_void_ptr;
	printf("Hospital %s activado, este hospital tiene %ld pokemones\n",
	       identificador, hospital_cantidad_pokemones(hospital));
	strcpy(clave_activa, identificador);
	return hospital;
}

void mostrar_pokemones(hospital_t *activo)
{
	if (!activo) {
		printf("No hay ningun hospital activo\n");
		return;
	}
	hospital_a_cada_pokemon(activo, imprimir_pokemones, NULL);
}

void mostrar_pokemones_detallado(hospital_t *activo)
{
	if (!activo) {
		printf("No hay ningun hospital activo\n");
		return;
	}
	hospital_a_cada_pokemon(activo, imprimir_pokemones_detallado, NULL);
}

void destruir_hospital_activo(menu_t *menu, hospital_t *hospital,
			      char *clave_activa)
{
	if (!menu || !hospital) {
		printf("No hay hospital activo");
		return;
	}

	hash_quitar(menu->hospitales, clave_activa);
	free(clave_activa);
	hospital_destruir(hospital);
	hospital = NULL;
	clave_activa = NULL;
	menu->cantidad_hospitales -= 1;
	if (!hospital) {
		printf("Hospital destruido con exito\n");
	}
}

void destruir_menu(menu_t *menu)
{
	if (menu->hospitales != NULL)
		hash_con_cada_clave(menu->hospitales, destruir_hospitales,
				    NULL);
	hash_destruir(menu->hospitales);
	free(menu);
}