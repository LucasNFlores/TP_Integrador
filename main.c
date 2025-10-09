#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Necesaria para system("clear") o system("cls")

// --- CONSTANTES Y DEFINICIONES GLOBALES ---
#define MAX_TAREAS 50
#define NOMBRE_ARCHIVO "tareas_final.dat" // Archivo para la versión final

// --- DEFINICIÓN DEL ENUM Y SU "TRADUCTOR" A TEXTO ---
typedef enum {
    PENDIENTE,      // Valor 0
    EN_PROGRESO,    // Valor 1
    FINALIZADO      // Valor 2
} EstadoTarea;

// Array de strings que corresponde a los valores del enum (en el mismo orden)
const char* estadoStrings[] = {
    "Pendiente",
    "En progreso",
    "Finalizado"
};

// --- ESTRUCTURAS DE DATOS ---
typedef struct {
    char nombre[50];
    char apellido[50];
    int legajo;
} Responsable;

typedef struct {
    char codigo[5];
    char descripcion[256];
    Responsable responsable;
    EstadoTarea estado; // El estado ahora es un enum, no un char[]
} Tarea;

// --- PROTOTIPOS DE FUNCIONES (Índice del programa) ---
void limpiarBuffer();
void limpiarPantalla();
void presioneParaContinuar();
const char* obtenerStringDeEstado(EstadoTarea estado);
void cargarUnaTarea(Tarea *tarea);
void mostrarTareas(Tarea lista[], int numTareas);
void buscarPorResponsable(Tarea lista[], int numTareas);
void cambiarEstadoTarea(Tarea lista[], int numTareas);
void guardarTareasEnArchivo(Tarea lista[], int numTareas);
int cargarTareasDesdeArchivo(Tarea lista[]);
int mostrarMenu();

// --- FUNCIÓN PRINCIPAL ---
int main() {
    Tarea listaDeTareas[MAX_TAREAS];
    int tareasCargadas = 0;
    int opcion = 0;

    // Al iniciar, cargamos las tareas guardadas en el archivo
    tareasCargadas = cargarTareasDesdeArchivo(listaDeTareas);

    do {
        opcion = mostrarMenu();

        switch (opcion) {
            case 1: // Cargar Tarea
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
            case 2: // Mostrar Tareas
                mostrarTareas(listaDeTareas, tareasCargadas);
                presioneParaContinuar();
                break;
            case 3: // Buscar por Responsable
                buscarPorResponsable(listaDeTareas, tareasCargadas);
                presioneParaContinuar();
                break;
            case 4: // Cambiar Estado
                cambiarEstadoTarea(listaDeTareas, tareasCargadas);
                presioneParaContinuar();
                break;
            case 5: // Guardar y Salir
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

// --- IMPLEMENTACIÓN DE LAS FUNCIONES ---

int mostrarMenu() {
    int opcion;
    limpiarPantalla();
    printf("=================================\n");
    printf("      GESTOR DE TAREAS v2.0\n");
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

const char* obtenerStringDeEstado(EstadoTarea estado) {
    // Devuelve el texto correspondiente del array global
    return estadoStrings[estado];
}

void cargarUnaTarea(Tarea *tarea) {
    int opcionEstado = 0;

    printf("Ingrese el codigo de la tarea (ej: T01): ");
    fgets(tarea->codigo, sizeof(tarea->codigo), stdin);
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
    limpiarBuffer();

    // Bucle para forzar al usuario a elegir una opción de estado válida
    do {
        printf("Seleccione el estado de la tarea:\n");
        printf("  1. %s\n", estadoStrings[PENDIENTE]);
        printf("  2. %s\n", estadoStrings[EN_PROGRESO]);
        printf("  3. %s\n", estadoStrings[FINALIZADO]);
        printf("Opcion: ");
        scanf("%d", &opcionEstado);
        limpiarBuffer();

        if (opcionEstado < 1 || opcionEstado > 3) {
            printf("Opción inválida. Por favor, elija 1, 2 o 3.\n");
        }
    } while (opcionEstado < 1 || opcionEstado > 3);

    // Convertimos la opción del usuario (1, 2, 3) al valor del enum (0, 1, 2)
    tarea->estado = (EstadoTarea)(opcionEstado - 1);
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
        printf("  Estado: %s\n", obtenerStringDeEstado(lista[i].estado)); // Usamos la función "traductora"
    }
    printf("----------------------------------\n");
}

void buscarPorResponsable(Tarea lista[], int numTareas) {
    char nombreBusqueda[50];
    int encontradas = 0;
    
    printf("\n--- Buscar Tareas por Responsable ---\n");
    printf("Ingrese el nombre del responsable a buscar: ");
    fgets(nombreBusqueda, sizeof(nombreBusqueda), stdin);
    nombreBusqueda[strcspn(nombreBusqueda, "\n")] = 0;

    printf("\nResultados de la busqueda para '%s':\n", nombreBusqueda);
    printf("-------------------------------------------\n");

    for (int i = 0; i < numTareas; i++) {
        // Usamos strcasecmp para ignorar mayúsculas/minúsculas.
        // Si no funciona en tu compilador (ej: en Windows), puedes usar strcmp.
        if (strcasecmp(lista[i].responsable.nombre, nombreBusqueda) == 0) {
            printf("  Codigo: %s\n", lista[i].codigo);
            printf("  Descripcion: %s\n", lista[i].descripcion);
            printf("  Estado: %s\n", obtenerStringDeEstado(lista[i].estado));
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
    int indiceEncontrado = -1;
    int opcionEstado = 0;

    printf("\n--- Cambiar Estado de una Tarea ---\n");
    if (numTareas == 0) {
        printf("No hay tareas para modificar.\n");
        return;
    }
    
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
        do {
            printf("Seleccione el nuevo estado para la tarea '%s':\n", lista[indiceEncontrado].codigo);
            printf("  1. %s\n", estadoStrings[PENDIENTE]);
            printf("  2. %s\n", estadoStrings[EN_PROGRESO]);
            printf("  3. %s\n", estadoStrings[FINALIZADO]);
            printf("Opcion: ");
            scanf("%d", &opcionEstado);
            limpiarBuffer();
            if (opcionEstado < 1 || opcionEstado > 3) {
                printf("Opción inválida.\n");
            }
        } while (opcionEstado < 1 || opcionEstado > 3);
        
        lista[indiceEncontrado].estado = (EstadoTarea)(opcionEstado - 1);
        printf("\n¡Estado actualizado con éxito!\n");
    } else {
        printf("\nNo se encontró ninguna tarea con el código '%s'.\n", codigoBusqueda);
    }
}

// --- FUNCIONES DE ARCHIVO (PERSISTENCIA) ---

void guardarTareasEnArchivo(Tarea lista[], int numTareas) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "wb"); // "wb" = Write Binary
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo para guardar.\n");
        return;
    }
    fwrite(lista, sizeof(Tarea), numTareas, archivo);
    fclose(archivo);
}

int cargarTareasDesdeArchivo(Tarea lista[]) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb"); // "rb" = Read Binary
    int numTareas = 0;
    if (archivo == NULL) {
        printf("Archivo de tareas no encontrado. Se creará uno nuevo al guardar.\n");
        presioneParaContinuar();
        return 0;
    }
    while (numTareas < MAX_TAREAS && fread(&lista[numTareas], sizeof(Tarea), 1, archivo) == 1) {
        numTareas++;
    }
    fclose(archivo);
    return numTareas;
}

// --- FUNCIONES AUXILIARES (UTILIDADES) ---

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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