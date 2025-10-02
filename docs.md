# Gestor de Tareas en C - Documentación del Código
Este documento explica el funcionamiento del programa "Gestor de Tareas", una aplicación de consola desarrollada en C. El programa permite al usuario crear, listar, buscar y modificar tareas, guardando los datos en un archivo para que la información persista entre sesiones.

1. Estructura de Datos
La información se organiza a través de estructuras (`structs`) anidadas que definen el modelo de datos.

`struct Responsable`
Almacena los datos de la persona a cargo de una tarea.

``` C
typedef struct {
    char nombre[50];
    char apellido[50];
    int legajo;
} Responsable;
```

`struct Estado`
Contiene el estado actual de la tarea como una cadena de texto.

``` C
typedef struct {
    char estado[20];
} Estado;
```

`struct Tarea`
Es la estructura principal que agrupa toda la información de una tarea, incluyendo las dos estructuras anteriores.

``` C
typedef struct {
    char codigo[5];
    char descripcion[256];
    Responsable responsable; // Estructura anidada
    Estado estado;           // Estructura anidada
} Tarea;
```

-----

2. Flujo Principal (`main`)
La función `main` es el cerebro del programa y orquesta todas las operaciones.

- **Inicialización**: Declara el array `listaDeTareas` para almacenar las tareas en memoria y un contador `tareasCargadas`.
- **Carga Inicial**: Llama a `cargarTareasDesdeArchivo()` para leer y cargar en memoria las tareas guardadas en una sesión anterior.
- **Bucle Principal**: Un bucle `do-while` mantiene el programa en ejecución, mostrando el menú y esperando la acción del usuario hasta que se elija la opción de salir (5).
- **Despachador de Tareas**: Un `switch` dirige el flujo del programa a la función correspondiente según la opción seleccionada por el usuario.
- **Guardado Final**: Antes de terminar, llama a `guardarTareasEnArchivo()` para escribir todos los cambios en el disco.

-----

3. Funciones Principales (Lógica del Gestor)
`void cargarUnaTarea(Tarea *tarea)`
- **Propósito**: Rellena una única estructura `Tarea` con los datos que el usuario introduce por teclado.
- **Técnica Clave**: Recibe un puntero (`Tarea *`). Esto le permite modificar la estructura original que se encuentra en el array de `main`, en lugar de trabajar sobre una copia.

`void mostrarTareas(Tarea lista[], int numTareas)`
- **Propósito**: Muestra en pantalla una lista formateada de todas las tareas que han sido cargadas.
- **Funcionamiento**: Recorre el array `lista` desde el índice 0 hasta `numTareas - 1` e imprime los campos de cada `Tarea`.

`void buscarPorResponsable(Tarea lista[], int numTareas)`
- **Propósito**: Permite al usuario encontrar todas las tareas asignadas a un responsable específico.
- **Técnica Clave** : Utiliza la función `strcasecmp()` para comparar el nombre introducido con el de cada tarea. Esta función es insensible a mayúsculas/minúsculas (busca "juan" y "Juan" por igual), lo que mejora la experiencia de usuario.

`void cambiarEstadoTarea(Tarea lista[], int numTareas)`
- **Propósito**: Busca una tarea por su código único y permite actualizar su estado.
- **Funcionamiento**:
- 1. Muestra todas las tareas para que el usuario sepa qué código ingresar.
- 2. Pide el código y busca una coincidencia exacta usando `strcmp()` (esta vez, **sensible a mayúsculas/minúsculas**).
- 3. Si la encuentra, solicita el nuevo estado y actualiza el campo correspondiente.

----

4. Funciones de Persistencia (Manejo de Archivos)
Estas funciones permiten que los datos no se pierdan al cerrar la aplicación.

`void guardarTareasEnArchivo(Tarea lista[], int numTareas)`
- **Propósito**: Escribe el array completo de tareas desde la memoria a un archivo en el disco.
- **Modo de Archivo**: Usa `"wb"` (Write Binary). El modo binario es ideal para `structs`, ya que guarda una copia exacta de los bytes en memoria, siendo rápido y eficiente. La función `fwrite()` realiza la escritura.

`int cargarTareasDesdeArchivo(Tarea lista[])`
- **Propósito**: Lee las tareas desde el archivo en el disco y las carga en el array en memoria.
- **Modo de Archivo**: Usa `"rb"` (Read Binary). La función `fread()` lee los datos del archivo, estructura por estructura, y los coloca en el array hasta llegar al final del archivo.

-----

5. Funciones de Utilidad y Auxiliares
Estas funciones mejoran la interfaz de usuario y la modularidad del código.

`int mostrarMenu()`
- **Propósito**: Limpia la pantalla, muestra las opciones al usuario y lee su elección.

`void limpiarPantalla()`
- **Propósito**: Limpia la ventana de la terminal.
- **Técnica Clave**: Usa directivas de preprocesador (`#ifdef _WIN32`) para ser **multiplataforma**. Ejecuta `system("cls")` en Windows y `system("clear")` en Linux y macOS.

`void limpiarBuffer()`
- **Propósito**: Soluciona un problema común en C al mezclar `scanf()` con `fgets()`.
- **Explicación**: `scanf` lee un número pero deja el carácter de nueva línea (`\n`) en el búfer de entrada. Esta función consume todos los caracteres restantes en el búfer hasta encontrar ese `\n`, dejándolo limpio para la siguiente lectura.

`void presioneParaContinuar()`
- **Propósito**: Pausa la ejecución del programa y espera a que el usuario presione "Enter". Sirve para que el usuario pueda leer la información en pantalla antes de que esta se borre.