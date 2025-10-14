#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Necesaria para system("clear") o system("cls")

// --- ESTRUCTURAS (sin cambios) ---
typedef struct {
    char estado[20];
} Estado;

typedef struct {
    char nombre[50];
    char apellido[50];
    int legajo;
} Responsable;

typedef struct {
    char codigo[5];
    char descripcion[256];
    Responsable responsable;
    Estado estado;
} Tarea;

// --- CONSTANTES Y GLOBALES ---
#define MAX_TAREAS 50
#define NOMBRE_ARCHIVO "tareas.dat"

// --- PROTOTIPOS DE FUNCIONES ---
void limpiarBuffer();
void limpiarPantalla();
void presioneParaContinuar();
void cargarUnaTarea(Tarea *tarea);
void mostrarTareas(Tarea lista[], int numTareas);
void buscarPorResponsable(Tarea lista[], int numTareas);
void cambiarEstadoTarea(Tarea lista[], int numTareas);
void guardarTareasEnArchivo(Tarea lista[], int numTareas);
int cargarTareasDesdeArchivo(Tarea lista[]);
int mostrarMenu();


int main() {
    Tarea listaDeTareas[MAX_TAREAS];
    int tareasCargadas = 0;
    int opcion = 0;

    // Al iniciar, intentamos cargar las tareas desde el archivo
    tareasCargadas = cargarTareasDesdeArchivo(listaDeTareas);

    do {
        opcion = mostrarMenu();

        switch (opcion) {
            case 1:
                if (tareasCargadas < MAX_TAREAS) {
                    printf("\n--- Cargando Nueva Tarea ---\n");
                    cargarUnaTarea(&listaDeTareas[tareasCargadas]);
                    tareasCargadas++;
                    printf("\n¡Tarea cargada con éxito!\n");
                } else {
                    printf("\nNo se pueden agregar más tareas. Límite alcanzado.\n");
                }
                presioneParaContinuar();
                break;
            case 2:
                mostrarTareas(listaDeTareas, tareasCargadas);
                presioneParaContinuar();
                break;
            case 3:
                buscarPorResponsable(listaDeTareas, tareasCargadas);
                presioneParaContinuar();
                break;
            case 4:
                cambiarEstadoTarea(listaDeTareas, tareasCargadas);
                presioneParaContinuar();
                break;
            case 5:
                guardarTareasEnArchivo(listaDeTareas, tareasCargadas);
                printf("\n¡Tareas guardadas correctamente! Saliendo del programa...\n\n");
                break;
            default:
                printf("\nOpción no válida. Intente de nuevo.\n");
                presioneParaContinuar();
        }
    } while (opcion != 5);

    return 0;
}

// --- IMPLEMENTACIÓN DE FUNCIONES ---

int mostrarMenu() {
    int opcion;
    limpiarPantalla();
    printf("=================================\n");
    printf("      GESTOR DE TAREAS v1.0\n");
    printf("=================================\n");
    printf("1. Cargar Nueva Tarea\n");
    printf("2. Mostrar Todas las Tareas\n");
    printf("3. Buscar Tareas por Responsable\n");
    printf("4. Cambiar Estado de una Tarea\n");
    printf("5. Guardar y Salir\n");
    printf("---------------------------------\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);
    limpiarBuffer();
    return opcion;
}

void buscarPorResponsable(Tarea lista[], int numTareas) {
    char nombreBusqueda[50];
    int encontradas = 0;
    
    printf("\n--- Buscar Tareas por Responsable ---\n");
    printf("Ingrese el nombre del responsable a buscar: ");

    fgets(nombreBusqueda, sizeof(nombreBusqueda), stdin);
    nombreBusqueda[strcspn(nombreBusqueda, "\n")] = 0; // Limpiar newline

    printf("\nResultados de la busqueda para '%s':\n", nombreBusqueda);
    printf("-------------------------------------------\n");

    for (int i = 0; i < numTareas; i++) {
        // Usamos strcasecmp para ignorar mayúsculas/minúsculas.
        // Si no funciona en tu compilador, usa strcmp.
        if (strcasecmp(lista[i].responsable.nombre, nombreBusqueda) == 0) {
            printf("  Codigo: %s\n", lista[i].codigo);
            printf("  Descripcion: %s\n", lista[i].descripcion);
            printf("  Estado: %s\n", lista[i].estado.estado);
            printf("-------------------------------------------\n");
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("No se encontraron tareas para ese responsable.\n");
    }
}

void cambiarEstadoTarea(Tarea lista[], int numTareas) {
    char codigoBusqueda[5];
    // en -1 porque no hay una tarea encontrada
    int indiceEncontrado = -1;

    printf("\n--- Cambiar Estado de una Tarea ---\n");
    // se comprueba si hay tareas existentes
    if (numTareas == 0) {
        printf("No hay tareas existentes aun.\n");
        return;
    }
    
    // Mostramos las tareas para que el usuario elija
    mostrarTareas(lista, numTareas);
    
    printf("\nIngrese el codigo de la tarea a modificar: ");
    fgets(codigoBusqueda, sizeof(codigoBusqueda), stdin);
    codigoBusqueda[strcspn(codigoBusqueda, "\n")] = 0;

    for (int i = 0; i < numTareas; i++) {
        if (strcmp(lista[i].codigo, codigoBusqueda) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado != -1) {
        // variables para el mensaje del cambio
        char estadoAnterior[20];
        char estadoNuevo[20];

        // guardamos el estado viejo en la variable
        strcpy(estadoAnterior, lista[indiceEncontrado].estado.estado);

        // ingresamos el nuevo estado
        printf("Tarea encontrada. Ingrese el nuevo estado: ");
        fgets(lista[indiceEncontrado].estado.estado, sizeof(lista[indiceEncontrado].estado.estado), stdin);
        lista[indiceEncontrado].estado.estado[strcspn(lista[indiceEncontrado].estado.estado, "\n")] = 0;
        
        strcpy(estadoNuevo, lista[indiceEncontrado].estado.estado);

        // mostramos el estado nuevo
        printf("\nSe cambio el estado de '%s'-> '%s'\n", estadoAnterior, estadoNuevo);
    } else {
        printf("\nNo se encontró ninguna tarea con el código '%s'.\n", codigoBusqueda);
    }
}

// --- FUNCIONES DE PERSISTENCIA EN ARCHIVO ---

void guardarTareasEnArchivo(Tarea lista[], int numTareas) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "wb"); // "wb" -> Write Binary
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar.\n");
        return;
    }

    // Escribimos todo el array de tareas en el archivo de una sola vez
    fwrite(lista, sizeof(Tarea), numTareas, archivo);
    
    fclose(archivo);
}

int cargarTareasDesdeArchivo(Tarea lista[]) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb"); // "rb" -> Read Binary
    int numTareas = 0;
    if (archivo == NULL) {
        // Si el archivo no existe, es la primera vez que se ejecuta. No es un error.
        printf("Archivo de tareas no encontrado. Se creará uno nuevo al guardar.\n");
        presioneParaContinuar();
        return 0;
    }

    // Leemos del archivo y guardamos en el array hasta que no haya más que leer
    while (fread(&lista[numTareas], sizeof(Tarea), 1, archivo) == 1 && numTareas < MAX_TAREAS) {
        numTareas++;
    }

    fclose(archivo);
    return numTareas;
}


// --- FUNCIONES AUXILIARES ---

void limpiarBuffer() {
    int basura; // Declara una variable para guardar un carácter
    
    // Inicia un bucle que se repite MIENTRAS se cumplan dos condiciones
    while ((basura = getchar()) != '\n' && basura != EOF); 
    // se obtiene con getchar si quedo algo "en el aire"
    // en caso de no ser \n y NO ser EOF ("fin del archivo")
}

// funcion para limpiar pantalla de la consola, dependiendo del SO que se tenga va a usar cls (windows) o clear(linux)
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void presioneParaContinuar() {
    printf("\nPresione Enter para continuar...");
    getchar();
}

void cargarUnaTarea(Tarea *tarea) {

    printf("Ingrese el codigo de la tarea (ej: T01): ");
    // se utiliza fgets porque obtiene todo lo que el usuario ingresa hasta apretar ENTER (contrario a scanf que lee hasta donde hay un espacio " ")
    fgets(tarea->codigo, sizeof(tarea->codigo), stdin);
    //asignamos con strcspn a donde apuntaria (en este caso el codigo de la tarea) y eliminar el salto de línea (\n) que fgets agrega al final del texto.
    tarea->codigo[strcspn(tarea->codigo, "\n")] = 0; 

    printf("Ingrese la descripcion: ");
    fgets(tarea->descripcion, sizeof(tarea->descripcion), stdin);
    tarea->descripcion[strcspn(tarea->descripcion, "\n")] = 0;

    printf("Ingrese el nombre del responsable: ");
    fgets(tarea->responsable.nombre, sizeof(tarea->responsable.nombre), stdin);
    tarea->responsable.nombre[strcspn(tarea->responsable.nombre, "\n")] = 0;

    printf("Ingrese el apellido del responsable: ");
    fgets(tarea->responsable.apellido, sizeof(tarea->responsable.apellido), stdin);
    tarea->responsable.apellido[strcspn(tarea->responsable.apellido, "\n")] = 0;

    printf("Ingrese el legajo del responsable: ");
    scanf("%d", &tarea->responsable.legajo);
    // igual que arriba con limpiar buffer eliminamos el \n que agrega scanf
    limpiarBuffer();

    printf("Ingrese el estado de la tarea (ej: Pendiente): ");
    fgets(tarea->estado.estado, sizeof(tarea->estado.estado), stdin);
    tarea->estado.estado[strcspn(tarea->estado.estado, "\n")] = 0;
}

void mostrarTareas(Tarea lista[], int numTareas) {

    limpiarPantalla();
    printf("\n--- LISTADO GENERAL DE TAREAS ---\n");
    if (numTareas == 0) {
        printf("No hay tareas para mostrar.\n");
        return;
    }
    for (int i = 0; i < numTareas; i++) {
        printf("----------------------------------\n");
        printf("  Codigo: %s\n", lista[i].codigo);
        printf("  Descripcion: %s\n", lista[i].descripcion);
        printf("  Responsable: %s %s (Legajo: %d)\n", lista[i].responsable.nombre, lista[i].responsable.apellido, lista[i].responsable.legajo);
        printf("  Estado: %s\n", lista[i].estado.estado);
    }
    printf("----------------------------------\n");
}