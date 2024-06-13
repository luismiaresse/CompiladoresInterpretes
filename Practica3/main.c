
#include <stdio.h>
#include <stdlib.h>
#include "errores.h"
#include "lexico.h"
#include "sintactico.h"
#include "tablasimbolos.h"
#include "comandos.h"

int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 2) {
        error_argumentos(1);
        return 1;
    } else if (argc == 2) {
        leer_archivo(argv[1]);
    }
    crearTS(TAM_TS_INICIAL);

    // Bucle infinito para no terminar cuando haya un error
    printf("Bienvenido a la calculadora. Para obtener ayuda, escribe 'help()'.\n");
    prompt();
    while (1) {
        int out = yyparse();
        if (out == 0) break;
    }
}

