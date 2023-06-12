#include "tp1.h"

#include "pokemon.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LONGITUD_LINEA 30

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

void ordenar_pokemones_por_salud(struct _hospital_pkm_t *hospital)
{
	if (!hospital || !hospital->pokemones)
		return;
	size_t i, j;
	pokemon_t *temp;
	for (i = 0; i < hospital->cantidad_pokemon - 1; i++) {
		for (j = 0; j < hospital->cantidad_pokemon - i - 1; j++) {
			if (pokemon_salud(hospital->pokemones[j]) >
			    pokemon_salud(hospital->pokemones[j + 1])) {
				temp = hospital->pokemones[j];
				hospital->pokemones[j] =
					hospital->pokemones[j + 1];
				hospital->pokemones[j + 1] = temp;
			}
		}
	}
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if (!nombre_archivo)
		return NULL;

	hospital_t *hospital = malloc(sizeof(hospital_t));
	if (!hospital)
		return NULL;
	hospital->pokemones = NULL;
	hospital->cantidad_entrenadores = 0;
	hospital->cantidad_pokemon = 0;

	char linea[LONGITUD_LINEA];
	FILE *f = fopen(nombre_archivo, "r");
	if (!f) {
		free(hospital);
		return NULL;
	}

	if (fgets(linea, LONGITUD_LINEA, f) == NULL) {
		free(hospital);
		fclose(f);
		return NULL;
	}

	pokemon_t *nuevo_pokemon = pokemon_crear_desde_string(linea);
	if (!nuevo_pokemon) {
		fclose(f);
		hospital_destruir(hospital);
		return NULL;
	}

	pokemon_t **aux =
		realloc(hospital->pokemones,
			sizeof(pokemon_t *) * (hospital->cantidad_pokemon + 1));
	if (!aux) {
		fclose(f);
		hospital_destruir(hospital);
		return NULL;
	}
	hospital->pokemones = aux;
	hospital->pokemones[hospital->cantidad_pokemon] = nuevo_pokemon;
	hospital->cantidad_pokemon++;
	hospital->cantidad_entrenadores++;

	while (fgets(linea, LONGITUD_LINEA, f)) {
		nuevo_pokemon = pokemon_crear_desde_string(linea);
		if (!nuevo_pokemon) {
			fclose(f);
			hospital_destruir(hospital);
			return NULL;
		}

		pokemon_t **aux = realloc(
			hospital->pokemones,
			sizeof(pokemon_t *) * (hospital->cantidad_pokemon + 1));
		if (!aux) {
			fclose(f);
			hospital_destruir(hospital);
			return NULL;
		}
		hospital->pokemones = aux;
		hospital->pokemones[hospital->cantidad_pokemon] = nuevo_pokemon;
		hospital->cantidad_pokemon++;
		hospital->cantidad_entrenadores++;
	}

	fclose(f);
	ordenar_pokemones_por_salud(hospital);
	return hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (!hospital)
		return 0;
	return hospital->cantidad_pokemon;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if (!hospital || !funcion) {
		return 0;
	}
	size_t cant_veces_invocada = 0;
	size_t i = 0;
	while (i < hospital->cantidad_pokemon) {
		if (funcion(hospital->pokemones[cant_veces_invocada], aux) ==
		    false) {
			cant_veces_invocada++;
			return cant_veces_invocada;
		}
		cant_veces_invocada++;
		i++;
	}
	return cant_veces_invocada;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if (!hospital || !pokemones_ambulancia)
		return ERROR;
	if (cant_pokes_ambulancia == 0) {
		return EXITO;
	}
	pokemon_t **aux =
		realloc(hospital->pokemones,
			sizeof(pokemon_t *) * (cant_pokes_ambulancia +
					       hospital->cantidad_pokemon));

	if (!aux) {
		hospital_destruir(hospital);
		return ERROR;
	}
	hospital->pokemones = aux;
	size_t j = 0;
	for (size_t i = hospital->cantidad_pokemon;
	     i < hospital->cantidad_pokemon + cant_pokes_ambulancia; i++) {
		hospital->pokemones[i] = pokemones_ambulancia[j];
		j++;
		hospital->cantidad_entrenadores++;
	}
	hospital->cantidad_pokemon += cant_pokes_ambulancia;
	ordenar_pokemones_por_salud(hospital);
	return EXITO;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (!hospital || !hospital->pokemones[prioridad])
		return NULL;
	return hospital->pokemones[prioridad];
}

void hospital_destruir(hospital_t *hospital)
{
	if (!hospital)
		return;
	for (size_t i = 0; i < hospital->cantidad_pokemon; i++) {
		free(hospital->pokemones[i]);
	}
	free(hospital->pokemones);
	free(hospital);
}