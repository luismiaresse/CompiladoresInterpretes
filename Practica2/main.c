// Compilador de lenguaje D

#include <stdio.h>
#include "errores.h"
#include "tablasimbolos.h"
#include "sintactico.h"
#include "lexico.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error_argumentos();
        return 1;
    }
    leer_archivo(argv[1]);
    TablaSimbolos t = NULL;
    crearTS(&t, TAM_TS_INICIAL);
    establecerTS(&t);
    printf("------ TABLA DE SÍMBOLOS INICIAL ------\n");
    imprimirTS();
    sintactico();
    printf("------ TABLA DE SÍMBOLOS FINAL ------\n");
    imprimirTS();
    destruirTS();
    cerrar_archivo();
}
