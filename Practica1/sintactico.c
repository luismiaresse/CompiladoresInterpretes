
#include <stdio.h>
#include <stdlib.h>
#include "sintactico.h"
#include "lexico.h"
#include "sistemaentrada.h"
#include "errores.h"
#include "tablasimbolos.h"

void _imprime_comp_lexico(int comp_lexico, char *lexema) {
    printf("<%d, \"%s\">\n", comp_lexico, lexema);
}

void sintactico(TablaSimbolos *t) {
    char *lexema;
    int comp_lexico;
    do {
        comp_lexico = sig_comp_lexico(t);
        if (comp_lexico != EOF && comp_lexico != ERR_LEXICO) {
            lexema = recuperar_lexema();
            _imprime_comp_lexico(comp_lexico, lexema);
            free(lexema);
        }
    } while (comp_lexico != EOF);
}
