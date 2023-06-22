#include <string.h>

#include "pa2m.h"
#include "src/menu.h"
#include "src/tp1.h"

void pruebas_creacion()
{
	menu_t *menu = crear_menu();
	pa2m_afirmar(menu != NULL, "Puedo crear un menu");
	pa2m_afirmar(cantidad_hospitales(menu) == 0,
		     "Al crear un menu, no tiene ningun hospital cargado");
	destruir_menu(menu);
	pa2m_mostrar_reporte();
}

void pruebas_cargar_hospitales()
{
	menu_t *menu = crear_menu();
	pa2m_afirmar(cargar_hospital_desde_archivo(menu, NULL, "g", NULL) ==
			     NULL,
		     "No puedo cargar el menu sin un archivo");
	pa2m_afirmar(cargar_hospital_desde_archivo(
			     menu, "ejemplos/invalido.txt", "g", NULL) == NULL,
		     "No puedo cargar el menu con un archivo invalido");
	pa2m_afirmar(
		cargar_hospital_desde_archivo(menu, "//////", "g", NULL) ==
			NULL,
		"No puedo cargar el menu con un archivo valido, pero vacio");
	pa2m_afirmar(
		cargar_hospital_desde_archivo(menu, "ejemplos/grande.txt", NULL,
					      NULL) == NULL,
		"No puedo cargar el menu con un archivo valido, pero clave NULL");
	pa2m_afirmar(
		cargar_hospital_desde_archivo(menu, "ejemplos/grande.txt", "",
					      NULL) == NULL,
		"No puedo cargar el menu con un archivo valido, pero clave vacia");
	pa2m_afirmar(cargar_hospital_desde_archivo(menu, "ejemplos/grande.txt",
						   "g", NULL) == menu,
		     "Puedo cargar el menu con un archivo valido");
	pa2m_afirmar(
		cantidad_hospitales(menu) == 1,
		"Luego de cargar un hospital, la cantidad de hospitales cargados aumento");
	pa2m_afirmar(cargar_hospital_desde_archivo(
			     menu, "ejemplos/hospital1.txt", "1", NULL) == menu,
		     "Puedo cargar otro hospital mas");
	pa2m_afirmar(
		cantidad_hospitales(menu) == 2,
		"Luego de cargar otro hospital, la cantidad de hospitales cargados aumento y es correcta");
	void *anterior;
	pa2m_afirmar(cargar_hospital_desde_archivo(menu,
						   "ejemplos/hospital2.txt",
						   "g", &anterior) == menu,
		     "Cargo un hospital con un nombre repetido");
	pa2m_afirmar(cantidad_hospitales(menu) == 2,
		     "La cantidad no aumenta si sobreescribo un hospital");
	pa2m_afirmar(anterior != NULL,
		     "En la variable anterior se guarda el hospital viejo");
	pa2m_afirmar(cargar_hospital(menu, "p", anterior) == menu,
		     "Puedo reinsertar el hospital que reemplace");
	pa2m_afirmar(
		cantidad_hospitales(menu) == 3,
		"La cantidad aumenta cuando guardo el hospital que reemplace");
	destruir_menu(menu);
	pa2m_mostrar_reporte();
}

void pruebas_activar_hospital()
{
	menu_t *menu = crear_menu();

	cargar_hospital_desde_archivo(menu, "ejemplos/grande.txt", "grande",
				      NULL);
	cargar_hospital_desde_archivo(menu, "ejemplos/hospital1.txt", "chico",
				      NULL);

	char clave_activa[100];
	hospital_t *activo = activar_hospital(menu, "grande", clave_activa);
	pa2m_afirmar(activar_hospital(NULL, "grande", clave_activa) == NULL,
		     "No puedo activar un hospital con un menu NULL");
	pa2m_afirmar(activar_hospital(menu, NULL, clave_activa) == NULL,
		     "No puedo activar un hospital con una clave NULL");
	pa2m_afirmar(activar_hospital(menu, "", clave_activa) == NULL,
		     "No puedo activar un hospital con clave vacia");
	pa2m_afirmar(
		activar_hospital(menu, "grande", NULL) == NULL,
		"No puedo activar un hospital con clave valida si clave activa es NULL");
	pa2m_afirmar(
		activo != NULL,
		"Puedo activar un hospital que esta cargado con la clave correcta");
	activo = activar_hospital(menu, "chico", clave_activa);
	pa2m_afirmar(activo != NULL,
		     "Puedo activar otro hospital que esta cargado en el menu");
	destruir_hospital_activo(menu, activo, clave_activa);

	destruir_menu(menu);
	pa2m_mostrar_reporte();
}

void pruebas_destruir_activo()
{
	menu_t *menu = crear_menu();

	cargar_hospital_desde_archivo(menu, "ejemplos/grande.txt", "grande",
				      NULL);
	cargar_hospital_desde_archivo(menu, "ejemplos/hospital1.txt", "chico",
				      NULL);
	char clave_activa[100];
	hospital_t *activo = activar_hospital(menu, "grande", clave_activa);
	pa2m_afirmar(destruir_hospital_activo(NULL, activo, clave_activa) == -1,
		     "No puedo destruir el hospital activo con menu NULL");
	pa2m_afirmar(
		destruir_hospital_activo(menu, NULL, clave_activa) == -1,
		"No puedo destruir el hospital activo si hospital activo es NULL");
	pa2m_afirmar(
		destruir_hospital_activo(menu, activo, NULL) == -1,
		"No puedo destruir el hospital activo si la clave activa es NULL");
	pa2m_afirmar(
		destruir_hospital_activo(menu, activo, clave_activa) == 0,
		"Si destruyo el hospital activo con los datos correctos, se destruye correctamente");
	destruir_menu(menu);
	pa2m_mostrar_reporte();
}

void pruebas_mostrar_estados()
{
	menu_t *menu = crear_menu();

	cargar_hospital_desde_archivo(menu, "ejemplos/grande.txt", "grande",
				      NULL);
	cargar_hospital_desde_archivo(menu, "ejemplos/hospital1.txt", "chico",
				      NULL);
	char estado[1000];
	estado[0] = '\0';
	char estado_final[] = "- Hospital grande\n- Hospital chico\n";
	mostrar_estado(NULL, estado);
	pa2m_afirmar(strlen(estado) == 0,
		     "Mostrar estado con menu NULL no es posible");
	mostrar_estado(menu, estado);
	pa2m_afirmar(strcmp(estado, estado_final) == 0,
		     "Mostrar estado imprime correctamente");
	char clave_activa[100];
	clave_activa[0] = '\0';
	hospital_t *activo = activar_hospital(menu, "grande", clave_activa);
	char pokemones[1000];
	pokemones[0] = '\0';
	char estado_pokemones[] =
		"Jynx\nCharmander\nPikachu\nChikorita\nVoltorb\nPichu\nLapras\nSnorlax\nMagcargo\nScyter\nMew\nEntei\n";
	mostrar_pokemones(activo, pokemones);
	pa2m_afirmar(
		strcmp(pokemones, estado_pokemones) == 0,
		"Mostrar los pokemones devuelve el listado correcto de pokemones");
	char pokemones_detallado[1000];
	pokemones_detallado[0] = '\0';
	char estado_pokemones_detallado[] =
		"Id del pokemon: 3, Nombre: Jynx, Salud: 2, Entrenador/es: Nico\n"
		"Id del pokemon: 4, Nombre: Charmander, Salud: 19, Entrenador/es: Lucas\n"
		"Id del pokemon: 1, Nombre: Pikachu, Salud: 20, Entrenador/es: Lucas\n"
		"Id del pokemon: 10, Nombre: Chikorita, Salud: 20, Entrenador/es: Lucas\n"
		"Id del pokemon: 9, Nombre: Voltorb, Salud: 29, Entrenador/es: Nico\n"
		"Id del pokemon: 7, Nombre: Pichu, Salud: 32, Entrenador/es: Lucas\n"
		"Id del pokemon: 2, Nombre: Lapras, Salud: 35, Entrenador/es: Abril\n"
		"Id del pokemon: 6, Nombre: Snorlax, Salud: 65, Entrenador/es: Nico\n"
		"Id del pokemon: 12, Nombre: Magcargo, Salud: 76, Entrenador/es: Nico\n"
		"Id del pokemon: 8, Nombre: Scyter, Salud: 88, Entrenador/es: Abril\n"
		"Id del pokemon: 5, Nombre: Mew, Salud: 98, Entrenador/es: Abril\n"
		"Id del pokemon: 11, Nombre: Entei, Salud: 99, Entrenador/es: Abril\n";
	mostrar_pokemones_detallado(activo, pokemones_detallado);
	pa2m_afirmar(
		strcmp(pokemones_detallado, estado_pokemones_detallado) == 0,
		"Mostrar pokemones detallado, imprime correctamente los datos");
	destruir_menu(menu);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de crear menu\n");
	pruebas_creacion();

	pa2m_nuevo_grupo("Pruebas de cargar hospitales\n");
	pruebas_cargar_hospitales();

	pa2m_nuevo_grupo("Pruebas de activar hospital\n");
	pruebas_activar_hospital();

	pa2m_nuevo_grupo("Pruebas de destruir activo\n");
	pruebas_destruir_activo();

	pa2m_nuevo_grupo(
		"Pruebas de mostrar estado, pokemones y pokemones detallado\n");
	pruebas_mostrar_estados();

	return pa2m_mostrar_reporte();
}