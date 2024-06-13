#ifndef PRACTICA2_LEX_YY_H
#define PRACTICA2_LEX_YY_H

#include <bits/types/FILE.h>

int sig_comp_lexico();
void leer_archivo(char *path);
void cerrar_archivo();
extern char *yytext;
extern int yylineno;

#endif //PRACTICA2_LEX_YY_H
