
#ifndef COMPILADOR_ERRORES_H
#define COMPILADOR_ERRORES_H

void error_argumentos();
void error_archivo();
void error_lexico(char *lexema, int linea);

#define _ERR_LEXICO (-10)

#endif //COMPILADOR_ERRORES_H
