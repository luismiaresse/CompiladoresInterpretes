
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "comandos.h"
#include "lexico.h"
#include "tablasimbolos.h"
#include "errores.h"
#include "sintactico.h"
#include <dlfcn.h>

int echo_flag = 0;

void _finalizar_calculadora() {
    destruirTS();
    cerrar_archivo();
}

void clear() {
    destruirTS();
    crearTS(TAM_TS_INICIAL);
}

void load(char *archivo) {
    if (archivo == NULL || !strcmp(archivo, "load") || !strcmp(archivo, "\"\"")) {
        error_argumentos(0);
        return;
    }
    printf("Cargando archivo %s...\n", archivo);
    // Eliminar comillas
    archivo = strtok(archivo, "\"");
    leer_archivo(archivo);
}

void help() {
    printf("Comandos disponibles:\n"
    "\tload(\"/ruta/a/archivo\"): carga el archivo fuente indicado.\n"
    "\thelp(): muestra esta ayuda.\n"
    "\texit(): cierra la calculadora.\n"
    "\techo(\"on\" | \"off\"): muestra o no por pantalla información de análisis sintáctico.\n"
    "\tworkspace(): muestra las variables definidas por el usuario.\n"
    "\tclear(): borra las variables guardadas por el usuario.\n"
    "\timport(\"/ruta/a/libreria\", \"funcion\"): importa la función de la librería (ruta) indicada.\n"
    "Funciones disponibles:\n"
    "\tsin(x): seno de x.\n"
    "\tcos(x): coseno de x.\n"
    "\ttan(x): tangente de x.\n"
    "\tasin(x): arcoseno de x.\n"
    "\tacos(x): arcocoseno de x.\n"
    "\tatan(x): arcotangente de x.\n"
    "\texp(x): exponencial de x.\n"
    "\tlog(x): logaritmo natural de x.\n"
    "\tlog10(x): logaritmo base 10 de x.\n"
    "\tsqrt(x): raíz cuadrada de x.\n"
    "\tpow(x, y): x elevado a la potencia y.\n"
    "\tabs(x): valor absoluto de x.\n"
    "\tceil(x): redondea x hacia arriba.\n"
    "\tfloor(x): redondea x hacia abajo.\n"
    "Constantes disponibles:\n"
    "\tpi: número pi.\n"
    "\te: número de Euler.\n");

}

// El comando es exit() pero tiene conflicto con la función exit() de stdlib.h
void __exit() {
    printf("Cerrando calculadora...\n");
    _finalizar_calculadora();
    yylex_destroy();
}

void workspace() {
    imprimirTS();
}

void toggle_echo(char *toggle) {
    toggle = strtok(toggle, "\"");
    if (toggle == NULL || !strcmp(toggle, "echo")) {
        echo_flag = !echo_flag;
    } else if (!strcmp(toggle, "on")) {
        echo_flag = 1;
    } else if (!strcmp(toggle, "off")) {
        echo_flag = 0;
    } else {
        error_argumentos(0);
        return;
    }
    if (echo_flag) {
        printf("Echo activado.\n");
    } else {
        printf("Echo desactivado.\n");
    }
}

void prompt() {
    printf(">> ");
}

void import(char *libreria, char *nombre_funcion) {
    printf("Importando función dinámica %s de la librería %s...\n", nombre_funcion, libreria);
    if (libreria == NULL || nombre_funcion == NULL || !strcmp(libreria, "import") ||
        !strcmp(nombre_funcion, "import") || !strcmp(libreria, "\"\"") || !strcmp(nombre_funcion, "\"\"")) {
        error_argumentos(0);
        free(nombre_funcion);
        return;
    }
    // Eliminar comillas
    libreria = strtok(libreria, "\"");
    nombre_funcion = strtok(nombre_funcion, "\"");
    void *handle = dlopen(libreria, RTLD_LAZY);
    if (!handle) {
        error_lectura_archivo();
        free(nombre_funcion);
        return;
    }
    double (*funcion)(double);
    funcion = dlsym(handle, nombre_funcion);
    if (!funcion) {
        error_funcion_no_encontrada(nombre_funcion);
        dlclose(handle);
        free(nombre_funcion);
        return;
    }
    elemento e = {nombre_funcion, _FUNCION, .handle=handle, .dato.funcion=funcion};
    elemento comprob = comprobarTS(nombre_funcion);
    if (comprob.codigo != 0) {
        error_funcion_ya_definida(nombre_funcion);
        free(nombre_funcion);
        return;
    }
    insertarTS(e);
    printf("Función %s importada correctamente.\n", nombre_funcion);
}