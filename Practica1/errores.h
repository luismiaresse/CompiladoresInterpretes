
#ifndef COMPILADOR_ERRORES_H
#define COMPILADOR_ERRORES_H

void error_argumentos();
void error_archivo();
void error_tam_lexema();
void error_lexico(int codigo, char *lexema, int linea);

#define ERR_LEXICO (-10)
// Códigos de error léxico
#define ERR_LEX_ENTERO_INVALIDO 1
#define ERR_LEX_COMENTARIO_NO_CERRADO 2
#define ERR_LEX_OPERADOR_INVALIDO 3
#define ERR_LEX_BINARIO_INVALIDO 4
#define ERR_LEX_EXPONENCIAL_INVALIDO 5
#define ERR_LEX_STRING_NO_CERRADO 6

#endif //COMPILADOR_ERRORES_H
