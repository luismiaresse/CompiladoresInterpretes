
#ifndef COMPILADOR_SISTEMAENTRADA_H
#define COMPILADOR_SISTEMAENTRADA_H

// Tamaño de cada centinela (sin EOF) y límite de tamaño de lexema
#define TAM_CENTINELA 64

// Abre el archivo de entrada
void leer_archivo(char *path);

// Cierra el archivo de entrada
void cerrar_archivo();

// Devuelve el siguiente caracter del archivo de entrada al analizador léxico
// nuevoLexema indica si se actualiza la posición inicio, -1 para ignorar el tamaño del lexema
char sig_caracter(int nuevoLexema);

// Retrocede al caracter anterior
void retroceder_caracter();

// Recuperar lexema desde inicio hasta fin/delantero
char * recuperar_lexema();

#endif //COMPILADOR_SISTEMAENTRADA_H
