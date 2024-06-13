
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

void error_tam_lexema() {
    printf("Error: un lexema excede el tamaño máximo permitido.\n");
}

void error_lexico(int codigo, char *lexema, int linea) {
    printf("Error léxico en la línea %d: ", linea);
    switch (codigo) {
        case ERR_LEX_ENTERO_INVALIDO:
            printf("formato de entero inválido: %s\n", lexema);
            break;
        case ERR_LEX_COMENTARIO_NO_CERRADO:
            printf("comentario no cerrado: %s\n", lexema);
            break;
        case ERR_LEX_OPERADOR_INVALIDO:
            printf("operador inválido: %s\n", lexema);
            break;
        case ERR_LEX_BINARIO_INVALIDO:
            printf("formato de número binario inválido: %s\n", lexema);
            break;
        case ERR_LEX_STRING_NO_CERRADO:
            printf("string no cerrado: %s\n", lexema);
            break;
        case ERR_LEX_EXPONENCIAL_INVALIDO:
            printf("formato de número exponencial inválido: %s\n", lexema);
            break;
        default:
            printf("código de error inválido: %d\n", codigo);
    }
}
