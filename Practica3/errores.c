
#include <stdio.h>
#include "errores.h"
#include "lexico.h"
#include "comandos.h"

void uso() {
    printf("Uso: ./calculadora [archivo opcional]\n");
}

void error_argumentos(int imprime_ayuda) {
    printf("Error: argumentos inválidos.\n");
    if (imprime_ayuda) uso();
}

void error_lectura_archivo() {
    printf("Error: no se pudo acceder al archivo fuente.\n");
}

void error_interprete_archivo(char *msg, char *lexema, int linea) {
    printf("Error en la línea %d: %s, lexema '%s'\n", linea, msg, lexema);
}

void error_interprete_consola(char *msg, char *lexema) {
    printf("Error: %s, lexema '%s'\n", msg, lexema);
    prompt();
}

void error_interprete(char *msg, char *lexema, int linea) {
    if (yyin == stdin) {
        error_interprete_consola(msg, lexema);
    } else {
        error_interprete_archivo(msg, lexema, linea);
    }
}

// Errores: variable sin inicializar, división por cero y asignación a constante
void error_variable_sin_inicializar(char *nombre) {
    printf("Error: la variable '%s' no ha sido inicializada.\n", nombre);
    prompt();
}

void error_division_por_cero() {
    printf("Error: división por cero.\n");
    prompt();
}

void error_asignacion_constante(char *nombre) {
    printf("Error: no se puede asignar un valor a la constante '%s'.\n", nombre);
    prompt();
}

void error_funcion_ya_definida(char *nombre) {
    printf("Error: la función '%s' ya ha sido definida, o existe un comando o variable con el mismo nombre.\n", nombre);
    prompt();
}

void error_funcion_no_encontrada(char *nombre) {
    printf("Error: la función '%s' no existe.\n", nombre);
    prompt();
}