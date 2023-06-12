#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if (!string)
		return NULL;

	pokemon_t *pokemon = malloc(sizeof(pokemon_t));
	if (!pokemon)
		return NULL;

	char *string_modificable = malloc(sizeof(char *) * strlen(string) + 1);
	if (!string_modificable) {
		free(pokemon);
		return NULL;
	}
	strcpy(string_modificable, string);

	char *campo = strtok(string_modificable, ",");
	int contador_campo = 0;

	while (campo) {
		contador_campo++;
		if (contador_campo > 4) {
			free(string_modificable);
			free(pokemon);
			return NULL;
		}

		switch (contador_campo) {
		case 1:
			pokemon->id = (size_t)atoi(campo);
			break;
		case 2:
			strcpy(pokemon->nombre, campo);
			break;
		case 3:
			pokemon->salud = (size_t)atoi(campo);
			break;
		case 4:
			if (campo[strlen(campo) - 1] == '\n')
				campo[strlen(campo) - 1] = '\0';
			strcpy(pokemon->nombre_entrenador, campo);
			break;
		default:
			break;
		}
		campo = strtok(NULL, ",");
	}

	free(string_modificable);
	if (contador_campo != 4) {
		free(pokemon);
		return NULL;
	}

	return pokemon;
}

pokemon_t *pokemon_copiar(pokemon_t *poke)
{
	if (poke == NULL)
		return NULL;

	pokemon_t *copia_poke = malloc(sizeof(pokemon_t));
	if (copia_poke == NULL)
		return NULL;

	memcpy(copia_poke, poke, sizeof(pokemon_t));

	return copia_poke;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (pokemon1 == NULL || pokemon2 == NULL)
		return NULL;
	if (strcmp(pokemon1->nombre, pokemon2->nombre) == 0 &&
	    strcmp(pokemon1->nombre_entrenador, pokemon2->nombre_entrenador) ==
		    0 &&
	    pokemon1->id == pokemon2->id && pokemon1->salud == pokemon2->salud)
		return true;
	return false;
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;
	return pokemon->nombre;
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre_entrenador;
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;
	return pokemon->salud;
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;
	return pokemon->id;
}

void pokemon_destruir(pokemon_t *pkm)
{
	free(pkm);
}