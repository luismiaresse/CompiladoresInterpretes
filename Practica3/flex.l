/* Archivo de definición Flex para lenguaje D */

%{
#include "tablasimbolos.h"
#include "errores.h"
#include "sintactico.h"
void cerrar_archivo();
void yyerror(char *msg);
%}

%option noyywrap
%option yylineno


/* Tokens */
SEPARADOR                           [ |\t|\r]
LINEA                               \n
DIGITO                              [0-9]
ENTERO                              {DIGITO}+(\_)*{DIGITO}*
STRING                              \"(?:\\\"|[^"])*\"
SUF_EXPONENCIAL                     [eE][+-]?{DIGITO}+
IDENTIFICADOR                       [A-Za-z\_][A-Za-z0-9\_]*
PUNTUACION                          [\.\;\,\(\)\{\}\[\]]
OPERADOR                            [\+\-\*\/\=\<\>\%\^]
OPERADOR_SUMA_ASIGNACION            \+\=
OPERADOR_RESTA_ASIGNACION           \-\=
OPERADOR_MULTIPLICACION_ASIGNACION  \*\=
OPERADOR_DIVISION_ASIGNACION        \/\=
OPERADOR_COMPARACION                \=\=
OPERADOR_INCREMENTO                 \+\+
OPERADOR_DECREMENTO                 \-\-
OPERADOR_MAYOR_IGUAL                \>\=
OPERADOR_MENOR_IGUAL                \<\=
OPERADOR_DISTINTO                   \!\=

%%

{STRING}                                                    { yylval.str = strdup(yytext); return _STRING; }
{OPERADOR_SUMA_ASIGNACION}                                  { return _OP_SUMA_ASIGNACION; }
{OPERADOR_RESTA_ASIGNACION}                                 { return _OP_RESTA_ASIGNACION; }
{OPERADOR_MULTIPLICACION_ASIGNACION}                        { return _OP_MULTIPLICACION_ASIGNACION; }
{OPERADOR_DIVISION_ASIGNACION}                              { return _OP_DIVISION_ASIGNACION; }
{OPERADOR_COMPARACION}                                      { return _OP_COMPARACION; }
{OPERADOR_INCREMENTO}                                       { return _OP_INCREMENTO; }
{OPERADOR_DECREMENTO}                                       { return _OP_DECREMENTO; }
{OPERADOR_MAYOR_IGUAL}                                      { return _OP_MAYOR_IGUAL; }
{OPERADOR_MENOR_IGUAL}                                      { return _OP_MENOR_IGUAL; }
{OPERADOR_DISTINTO}                                         { return _OP_DISTINTO; }
{OPERADOR}                                                  { return (int)yytext[0]; }
{PUNTUACION}                                                { return (int)yytext[0]; }
{IDENTIFICADOR}                                             { yylval.str = strdup(yytext);
                                                              elemento e = comprobarTS(yylval.str);
                                                              if(e.codigo != 0) return e.codigo;
                                                              else return _IDENTIFICADOR; }
{ENTERO}{SUF_EXPONENCIAL}?                                  { yylval.num = atof(yytext); return _NUMERO; }
{DIGITO}*\.{DIGITO}*[\_]*{DIGITO}*{SUF_EXPONENCIAL}?        { yylval.num = atof(yytext); return _NUMERO; }
{LINEA}                                                    { return (int)yytext[0]; }
{SEPARADOR}+                                                /* Ignorar espacios */
<<EOF>>                                                     { if (yyin != stdin) {
                                                                cerrar_archivo();
                                                              } return _EOF; }
.                                                           { yyerror("Caracter desconocido"); }

%%

void leer_archivo(char *path) {
    FILE *archivo = fopen(path, "r");
    if (archivo == NULL) {
        error_lectura_archivo();
        return;
    }
    yyin = archivo;
    yylineno = 0;
}

void cerrar_archivo() {
    if (yyin != stdin) {
        fclose(yyin);
        yyin = stdin;
        yyrestart(yyin);
    }
}

void yyerror(char *msg) {
    error_interprete(msg, yytext, yylineno);
}
