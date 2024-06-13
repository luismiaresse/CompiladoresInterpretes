
#include <stdio.h>
#include "errores.h"

void uso() {
    printf("Uso: ./Compilador [código.d]\n");
}

void error_argumentos() {
    printf("Error: número inválido de argumentos.\n");
    uso();
}

void error_archivo() {
    printf("Error: no se pudo acceder al archivo fuente.\n");
}

void error_lexico(char *lexema, int linea) {
    printf("Error léxico en la línea %d, lexema: '%s'\n", linea, lexema);
}
