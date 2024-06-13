
#ifndef ERRORES_H
#define ERRORES_H

void error_argumentos(int imprime_ayuda);
void error_lectura_archivo();
void error_interprete(char *msg, char *lexema, int linea);
void error_variable_sin_inicializar(char *lexema);
void error_division_por_cero();
void error_asignacion_constante(char *lexema);
void error_funcion_ya_definida(char *lexema);
void error_funcion_no_encontrada(char *lexema);

#endif //ERRORES_H
