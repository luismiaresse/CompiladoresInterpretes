
#ifndef COMANDOS_H
#define COMANDOS_H

extern int echo_flag;

// Limpia las variables (identificadores) definidas por el usuario
void clear();

// Carga un archivo fuente
void load(char *archivo);

// Muestra la ayuda
void help();

// El comando es exit pero tiene conflicto con la función exit() de stdlib.h
void __exit();

// Muestra las variables definidas
void workspace();

// Alterna entre mostrar o no los resultados de las operaciones
void toggle_echo();

// Importa una función de una librería dinámica
void import(char *libreria, char *funcion);

void prompt();

#endif //COMANDOS_H
