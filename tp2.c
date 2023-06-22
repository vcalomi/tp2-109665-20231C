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

menu_t *cargar(menu_t *menu)
{
	char archivo[100];
	printf("Ingresa el nombre del archivo: ");
	scanf("%s", archivo);
	if (strlen(archivo) == 0) {
		printf("Nombre de archivo vacio, intente nuevamente\n");
		return NULL;
	}

	char clave[100];
	printf("Ingresa un nombre para el hospital: ");
	scanf("%s", clave);
	if (strlen(clave) == 0) {
		printf("El nombre no puede estar vacio, ingrese un nombre valido: ");
		return NULL;
	}
	void *anterior;
	menu_t *resultado =
		cargar_hospital_desde_archivo(menu, archivo, clave, &anterior);
	if (resultado == NULL) {
		anterior = NULL;
		printf("Error al cargar un archivo, chequee que el nombre o formato del archivo sea correcto\n");
	} else
		printf("Cargado correctamente!\n");

	if (anterior != NULL) {
		printf("Le pusiste al hospital un nombre repetido...\n");
		printf("Quiere guardar el hospital viejo con otro nombre o destruirlo? Presione 'g' o 'd' segun corresponda\n");
		printf(">");
		char decision[5];
		scanf("%s", decision);
		if (strcmp(decision, "g") == 0) {
			char clave[100];
			printf("Ingresa un nombre para el hospital: ");
			scanf("%s", clave);
			if (strlen(clave) == 0) {
				printf("El nombre no puede estar vacio, ingrese un nombre valido: ");
				return NULL;
			}
			cargar_hospital(menu, clave, anterior);
			printf("Hospital anterior cargado con exito!\n");
		} else if (strcmp(decision, "d") == 0) {
			hospital_destruir((hospital_t *)anterior);
			printf("Hospital viejo destruido con exito!\n");
		}
	}
	return menu;
}

void inicio()
{
	printf("Bienvenido a hospitales pokemon!\n");
	printf("Puede comenzar cargando un hospital escribiendo 'c' o si no sabes como funciona escribe 'h' para recibir ayuda\n");
}

hospital_t *activar(menu_t *menu, char *clave_activa)
{
	char identificador[100];
	printf("Ingresa el nombre del hospital que deseas activar: ");
	scanf("%s", identificador);
	hospital_t *activo =
		activar_hospital(menu, identificador, clave_activa);
	if (!activo) {
		printf("No existe un hospital con ese nombre\n");
		return NULL;
	}
	printf("Hospital %s activado, este hospital tiene %ld pokemones\n",
	       clave_activa, hospital_cantidad_pokemones(activo));
	return activo;
}

void destruir_activo(menu_t *menu, hospital_t *hospital_activo,
		     char *clave_activa)
{
	int resultado =
		destruir_hospital_activo(menu, hospital_activo, clave_activa);
	if (resultado == -1) {
		printf("No hay hospital activo\n");
		return;
	}
	printf("Hospital eliminado con exito\n");
	clave_activa = NULL;
}

void imprimir_estado(menu_t *menu, hospital_t *activo, char *clave_activa)
{
	if (cantidad_hospitales(menu) == 0) {
		printf("No hay ningun hospital cargado\n");
		return;
	}
	if (cantidad_hospitales(menu) == 1)
		printf("Hay %ld Hospital cargado:\n",
		       cantidad_hospitales(menu));
	else
		printf("Hay %ld Hospitales cargados:\n",
		       cantidad_hospitales(menu));
	char estado[1000];
	estado[0] = '\0';
	mostrar_estado(menu, activo, estado);
	printf("%s", estado);
	if (!activo)
		printf("No hay hospital activo\n");
	else
		printf("Hospital activo - %s\n", clave_activa);
}

void imprimir_pokemones(hospital_t *activo)
{
	if (!activo) {
		printf("No hay ningun hospital activo\n");
		return;
	}
	char pokemones[1000];
	pokemones[0] = '\0';
	mostrar_pokemones(activo, pokemones);
	printf("%s", pokemones);
}

void imprimir_pokemones_detallado(hospital_t *activo)
{
	if (!activo) {
		printf("No hay ningun hospital activo\n");
		return;
	}
	char detalles[1000];
	detalles[0] = '\0';
	mostrar_pokemones_detallado(activo, detalles);
	printf("%s", detalles);
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
	char clave_activa[100];
	clave_activa[0] = '\0';
	// = calloc(1, sizeof(char) * 100);
	// if (!clave_activa)
	// 	return -1;
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
			// if (activo != NULL)
			// 	free(clave_activa);
			break;

		} else if (strcmp(comando, "c") == 0 ||
			   strcmp(comando, "cargar") == 0) {
			cargar(menu);

		} else if (strcmp(comando, "e") == 0 ||
			   strcmp(comando, "estado") == 0) {
			imprimir_estado(menu, activo, clave_activa);

		} else if (strcmp(comando, "a") == 0 ||
			   strcmp(comando, "activar") == 0) {
			activo = activar(menu, clave_activa);

		} else if (strcmp(comando, "m") == 0 ||
			   strcmp(comando, "mostrar") == 0) {
			imprimir_pokemones(activo);

		} else if (strcmp(comando, "l") == 0 ||
			   strcmp(comando, "listar") == 0) {
			imprimir_pokemones_detallado(activo);

		} else if (strcmp(comando, "d") == 0 ||
			   strcmp(comando, "destruir") == 0) {
			destruir_activo(menu, activo, clave_activa);
			activo = NULL;
		} else
			printf("No existe ese comando... Intenta con otro o escribe h para recibir ayuda\n");
		printf(">");
		scanf("%s", comando);
		convertir_a_minusculas(comando);
	}
	return 0;
}
