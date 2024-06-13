// Compilador de lenguaje D

#include <stdio.h>
#include "errores.h"
#include "tablasimbolos.h"
#include "sistemaentrada.h"
#include "sintactico.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error_argumentos();
        return 1;
    }
    leer_archivo(argv[1]);
    TablaSimbolos t = NULL;
    crearTS(&t, TAM_TS_INICIAL);
    printf("------ TABLA DE SÍMBOLOS INICIAL ------\n");
    imprimirTS(t);
    sintactico(&t);
    printf("------ TABLA DE SÍMBOLOS FINAL ------\n");
    imprimirTS(t);
    destruirTS(t);
    cerrar_archivo();
}
