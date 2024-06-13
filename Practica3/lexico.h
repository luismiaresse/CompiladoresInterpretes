#ifndef LEXICO_H
#define LEXICO_H

void leer_archivo(char *path);
void cerrar_archivo();
extern char *yytext;
extern int yylineno;
void yyerror(const char *s);
extern int yylex();
extern FILE *yyin;
extern void yylex_destroy();

#endif //LEXICO_H
