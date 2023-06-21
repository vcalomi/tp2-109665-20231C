#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "src/tp1.h"
#include "src/menu.h"

void convertir_a_minusculas(char *cadena)
{
	int i = 0;
	while (cadena[i] != '\0') {
		cadena[i] = (char)tolower(cadena[i]);
		i++;
	}
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
	char *clave_activa = calloc(1, sizeof(char) * 100);
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
			printf("Saliendo...\n");
			destruir_menu(menu);
			free(comando);
			free(clave_activa);
			break;

		} else if (strcmp(comando, "c") == 0 ||
			   strcmp(comando, "cargar") == 0) {
			cargar_hospital(menu);

		} else if (strcmp(comando, "e") == 0 ||
			   strcmp(comando, "estado") == 0) {
			mostrar_estado(menu, activo, clave_activa);

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
			activo = NULL;
			clave_activa = NULL;
		} else
			printf("No existe ese comando... Intenta con otro o escribe h para recibir ayuda\n");
		printf(">");
		scanf("%s", comando);
		convertir_a_minusculas(comando);
	}
	return 0;
}
