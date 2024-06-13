/* Archivo de definición Flex para lenguaje D */

%{
#include "definiciones.h"
#include "tablasimbolos.h"
#include "errores.h"

// Variable para controlar el nivel de anidamiento de comentarios /+ +/
int nivelComAnidado = 0;
int enComentario = 0;
%}


/* Contar líneas */
%option yylineno

%option noyywrap

SEPARADOR                           [ |\t|\r]
LINEA                               \n
DIGITO                              [0-9]
DIGITO_BINARIO                      [0-1]
ENTERO                              {DIGITO}+(\_)*{DIGITO}*
SUF_EXPONENCIAL                     [eE][+-]?{DIGITO}+
IDENTIFICADOR                       [A-Za-z\_][A-Za-z0-9\_]*
PUNTUACION                          [\.\;\,\(\)\{\}\[\]]
OPERADOR                            [\+\-\*\/\=\<]
OPERADOR_SUMA_ASIGNACION            \+\=
OPERADOR_COMPARACION                \=\=
OPERADOR_INCREMENTO                 \+\+
STRING                              \"(?:\\\"|[^"])*\"
COMENTARIO_LINEA                    \/\/
AP_COMENTARIO_BLOQUE                \/\*
CIERRE_COMENTARIO_BLOQUE            \*\/
AP_COMENTARIO_ANIDADO               \/\+
CIERRE_COMENTARIO_ANIDADO           \+\/

%%

{STRING}                                                    { if (!enComentario) return _TIPO_STRING;
                                                             else {     // Devolver el string excepto las primeras "
                                                                 char *lexema = strdup(yytext);
                                                                 for (int i = strlen(lexema) - 1; i > 0; i--) {
                                                                    unput(lexema[i]);
                                                                 }
                                                                 free(lexema);
                                                              }
                                                            }
{OPERADOR_COMPARACION}                                      { if (!enComentario) return _OP_COMPARACION; }
{OPERADOR_SUMA_ASIGNACION}                                  { if (!enComentario) return _OP_SUMA_ASIGNACION; }
{OPERADOR_INCREMENTO}                                       { if (!enComentario) return _OP_INCREMENTO; }
{OPERADOR}                                                  { if (!enComentario) return (int)yytext[0]; }
{PUNTUACION}                                                { if (!enComentario) return (int)yytext[0]; }
{IDENTIFICADOR}                                             { if (!enComentario) return _IDENTIFICADOR; }
{ENTERO}                                                    { if (!enComentario) return _TIPO_ENTERO; }
{ENTERO}{SUF_EXPONENCIAL}                                   { if (!enComentario) return _TIPO_DOUBLE; }
{DIGITO}*\.{DIGITO}*[\_]*{DIGITO}*{SUF_EXPONENCIAL}*        { if (!enComentario) return _TIPO_DOUBLE; }
0[Bb]{DIGITO_BINARIO}*                                      { if (!enComentario) return _TIPO_BINARIO; }
{LINEA}+                                                    { if (enComentario == 1) enComentario = 0; }
{SEPARADOR}+                                                /* Ignorar espacios */
{COMENTARIO_LINEA}                                          { if (!enComentario) enComentario = 1; }
{AP_COMENTARIO_BLOQUE}                                      { if (!enComentario) enComentario = 2; }
{CIERRE_COMENTARIO_BLOQUE}                                  { if (enComentario == 2) enComentario = 0; }
{AP_COMENTARIO_ANIDADO}                                     { nivelComAnidado++; enComentario = 3; }
{CIERRE_COMENTARIO_ANIDADO}                                 { if (nivelComAnidado > 0) nivelComAnidado--;
                                                              else return _ERR_LEXICO;
                                                              if (!nivelComAnidado) enComentario = 0; }
.                                                           { if (!enComentario) return _ERR_LEXICO; }
<<EOF>>                                                     { return (-1); }

%%

int sig_comp_lexico() {
    int tipo = yylex();
    if (tipo == _IDENTIFICADOR) {
        char *lexema = strdup(yytext);
        tipo = comprobarTS(lexema);
    }
    return tipo;
}

void leer_archivo(char *path) {
    FILE *archivo = fopen(path, "r");
    if (archivo == NULL) {
        error_archivo();
        exit(1);
    }
    yyin = archivo;
}

void cerrar_archivo() {
    fclose(yyin);
    yylex_destroy();
}

