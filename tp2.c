#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "src/tp1.h"
#include "src/hash.h"

typedef struct Menu {
	hash_t *hospitales;
	size_t cantidad_hospitales;
} menu_t;

menu_t *crear_menu()
{
	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;
	menu->hospitales = hash_crear(10);
	return menu;
}

void mostrar_ayuda()
{
	printf("Comandos disponibles:\n");
	printf("S - Sale del programa\n");
	printf("H - Muestra todos los comandos y sus funcionalidades\n");
	printf("C - Carga un hospital a partir de un archivo\n");
	printf("E - Muestra un listado de todos los hospitales cargados con su identificador y el hospital activo en caso de que haya uno\n");
	printf("A - Activa un hospital a partir de un identificador\n");
	printf("M - Muestra los nombres de todos los pokemones que estan en el hospital activo\n");
	printf("L - Muestra los datos de todos los pokemones que estan en el hospital activo\n");
	printf("D - Elimina el hospital activo del listado de hospitales\n");
}

void cargar_hospital(menu_t *menu)
{
	char archivo[100];
	printf("Ingresa el nombre del archivo: ");
	scanf("%s", archivo);

	hospital_t *hospital = hospital_crear_desde_archivo(archivo);

	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		continue;

	char clave[10];
	printf("Ingresa una clave para guardar con el hospital: ");
	fgets(clave, sizeof(clave), stdin);
	clave[strcspn(clave, "\n")] = '\0';

	hash_insertar(menu->hospitales, clave, hospital, NULL);
	printf("Cargado correctamente\n");
}

bool listar_hospitales(const char *clave, void *valor, void *aux)
{
	printf("Hospital - %s\n", clave);
	return true;
}

void mostrar_estado(menu_t *menu)
{
	printf("Hospitales cargados:\n");
	hash_con_cada_clave(menu->hospitales, listar_hospitales, NULL);
}

bool destruir_hospitales(const char *clave, void *elemento, void *aux)
{
	hospital_destruir((hospital_t *)elemento);
	return true;
}

void destruir_menu(menu_t *menu)
{
	hash_con_cada_clave(menu->hospitales, destruir_hospitales, NULL);
	hash_destruir(menu->hospitales);
	free(menu);
}

void convertir_a_minusculas(char *cadena)
{
	int i = 0;
	while (cadena[i] != '\0') {
		cadena[i] = (char)tolower(cadena[i]);
		i++;
	}
}

hospital_t *activar_hospital(menu_t *menu, char *clave_activa)
{
	hospital_t *hospital = NULL;
	char identificador[10];
	printf("Ingresa la clave del hospital que deseas activar: ");
	scanf("%s", identificador);

	hospital = (hospital_t *)hash_obtener(menu->hospitales, identificador);
	printf("Hospital %s activado, este hospital tiene %ld pokemones\n",
	       identificador, hospital_cantidad_pokemones(hospital));
	strcpy(clave_activa, identificador);
	return hospital;
}

bool imprimir_pokemones(pokemon_t *pokemon, void *aux)
{
	printf("%s\n", pokemon_nombre(pokemon));
	return true;
}

void mostrar_pokemones(hospital_t *activo)
{
	if (!activo) {
		printf("No hay ningun hospital activo\n");
		return;
	}
	hospital_a_cada_pokemon(activo, imprimir_pokemones, NULL);
}

bool imprimir_pokemones_detallado(pokemon_t *pokemon, void *aux)
{
	printf("Id del pokemon: %ld, Nombre: %s, Salud: %ld\n",
	       pokemon_id(pokemon), pokemon_nombre(pokemon),
	       pokemon_salud(pokemon));
	return true;
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
	hospital_destruir(hospital);
}

void inicio()
{
	printf("Bienvenido a hospitales pokemon!\n");
	printf("Puede comenzar cargando un hospital escribiendo 'c' o si no sabes como funciona escribe 'h' para recibir ayuda\n");
}

int main()
{
	menu_t *menu = crear_menu();
	char *comando = (char *)calloc(1, sizeof(char) * 30);
	if (!comando) {
		destruir_menu(menu);
		return -1;
	}
	inicio();
	printf(">");
	scanf("%s", comando);
	convertir_a_minusculas(comando);
	hospital_t *activo = NULL;
	char *clave_activa = calloc(1, sizeof(char) * 10);
	if (!clave_activa)
		return -1;
	while (true) {
		if (strcmp(comando, "h") == 0 ||
		    strcmp(comando, "ayuda") == 0 ||
		    strcmp(comando, "help") == 0) {
			mostrar_ayuda();

		} else if (strcmp(comando, "s") == 0 ||
			   strcmp(comando, "salir") == 0 ||
			   strcmp(comando, "exit") == 0) {
			destruir_menu(menu);
			free(comando);
			free(clave_activa);
			break;

		} else if (strcmp(comando, "c") == 0 ||
			   strcmp(comando, "cargar") == 0) {
			cargar_hospital(menu);

		} else if (strcmp(comando, "e") == 0 ||
			   strcmp(comando, "estado") == 0) {
			mostrar_estado(menu);

		} else if (strcmp(comando, "a") == 0 ||
			   strcmp(comando, "activar") == 0) {
			activo = activar_hospital(menu, clave_activa);

		} else if (strcmp(comando, "m") == 0 ||
			   strcmp(comando, "mostrar") == 0) {
			mostrar_pokemones(activo);

		} else if (strcmp(comando, "l") == 0 ||
			   strcmp(comando, "listar") == 0) {
			mostrar_pokemones_detallado(activo);

		} else if (strcmp(comando, "d") == 0 ||
			   strcmp(comando, "destruir") == 0) {
			destruir_hospital_activo(menu, activo, clave_activa);
		} else
			printf("No existe ese comando... Intenta con otro o escribe h para recibir ayuda");
		printf(">");
		scanf("%s", comando);
		convertir_a_minusculas(comando);
	}
	return 0;
}
