
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintactico.h"
#include "errores.h"
#include "lexico.h"

void _imprime_comp_lexico(int comp_lexico, char *lexema) {
    printf("<%d, \"%s\">\n", comp_lexico, lexema);
}

void sintactico() {
    char *lexema;
    int comp_lexico;
    do {
        comp_lexico = sig_comp_lexico();
        lexema = strdup(yytext);
        if (comp_lexico != EOF && comp_lexico != _ERR_LEXICO) {
            _imprime_comp_lexico(comp_lexico, lexema);
        } else if (comp_lexico == _ERR_LEXICO) {
            error_lexico(lexema, yylineno);
        }
        free(lexema);
    } while (comp_lexico != EOF);
}
