

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lexico.h"
#include "tablasimbolos.h"
#include "comandos.h"
#include "errores.h"
int operacion_asignacion(char *lex, double valor, int codigo_op);
%}

// Campos de variables YYSTYPE
%union {
    char *str;
    double num;
}

%start input

%token <num> _NUMERO
%token <str> _IDENTIFICADOR
%token <str> _FUNCION
%token <str> _CONSTANTE
%token <str> _COMANDO
%token <str> _STRING
%token _OPERADOR
%token _OP_COMPARACION
%token _OP_INCREMENTO
%token _OP_DECREMENTO
%token _OP_SUMA_ASIGNACION
%token _OP_RESTA_ASIGNACION
%token _OP_MULTIPLICACION_ASIGNACION
%token _OP_DIVISION_ASIGNACION
%token _OP_DISTINTO
%token _OP_MAYOR_IGUAL
%token _OP_MENOR_IGUAL
%token _OP_MAYOR
%token _OP_MENOR
%token _EOF

// Tipos de dato para las reglas
%type <num> operacion expr funcion comparacion

// Establecer la precedencia de los operadores
%right '=' _OP_DISTINTO _OP_SUMA_ASIGNACION _OP_RESTA_ASIGNACION _OP_MULTIPLICACION_ASIGNACION _OP_DIVISION_ASIGNACION
%right '^'
%left '+' '-' _OP_INCREMENTO _OP_DECREMENTO
%left '*' '/' '%'

%%

/*
$$ = Retorno de la regla
$1, $2, $3, ... = Campos de la regla
*/

input:  {  }
        | input line
;

line: '\n' { if(yyin == stdin) prompt(); }
        | sentencia '\n'  { if(yyin == stdin) prompt(); }
        | sentencia _EOF  { if(yyin == stdin) prompt(); }
        | error '\n'
        | error _EOF
        | _EOF  { if(yyin == stdin) prompt(); }
;

sentencia:    expr  { printf("%lf\n", $1); }
              | asignacion
              | comparacion  { if ($1) printf("true\n"); else printf("false\n"); }
              | comando
;

expr:  _NUMERO       { $$ = yylval.num; if(echo_flag) printf("NUMERO: %f\n", $$); }
       | _IDENTIFICADOR { elemento e = comprobarTS($1); if (e.codigo != 0) { $$ = e.dato.valor; if(echo_flag) printf("IDENTIFICADOR: %lf\n", $$); free($1); } else { error_variable_sin_inicializar($1); free($1); YYABORT; } }
       | _CONSTANTE    { $$ = comprobarTS($1).dato.valor; if(echo_flag) printf("CONSTANTE: %lf\n", $$); free($1); }
       | '(' expr ')'        { $$ = $2; }
       | operacion
       | funcion    /* Todas las funciones matemáticas deben devolver un double */
;



operacion:    expr '+' expr   { $$ = $1 + $3; if(echo_flag) printf("SUMA: %lf\n", $$); }
                     | expr '-' expr   { $$ = $1 - $3; if(echo_flag) printf("RESTA: %lf\n", $$); }
                     | expr '*' expr   { $$ = $1 * $3; if(echo_flag) printf("MULTIPLICACIÓN: %lf\n", $$); }
                     | expr '/' expr   { $$ = $1 / $3; if(echo_flag) printf("DIVISIÓN: %lf\n", $$); if (isnan($$) || isinf($$)) { error_division_por_cero(); YYABORT; } }
                     | expr '%' expr   { $$ = (int)$1 % (int)$3; if(echo_flag) printf("MÓDULO: %lf\n", $$); }
                     | expr '^' expr   { $$ = pow($1, $3); if(echo_flag) printf("POTENCIA: %lf\n", $$); }
                     | '-' expr             { $$ = -$2; if(echo_flag) printf("NEGATIVO: %lf\n", $$); }
                     | '+' expr             { $$ = $2; if(echo_flag) printf("POSITIVO: %lf\n", $$); }

;

asignacion:    _IDENTIFICADOR '=' expr    { elemento e = {$1, _IDENTIFICADOR, .dato.valor = $3};
                                            insertarTS(e); if(echo_flag) printf("ASIGNACIÓN: %lf\n", e.dato.valor); }
               | _IDENTIFICADOR _OP_SUMA_ASIGNACION expr    { if(!operacion_asignacion($1, $3, _OP_SUMA_ASIGNACION)) { YYABORT; } }
               | _IDENTIFICADOR _OP_RESTA_ASIGNACION expr    { if(!operacion_asignacion($1, $3, _OP_RESTA_ASIGNACION)) { YYABORT; } }
               | _IDENTIFICADOR _OP_MULTIPLICACION_ASIGNACION expr    { if(!operacion_asignacion($1, $3, _OP_MULTIPLICACION_ASIGNACION)) { YYABORT; } }
               | _IDENTIFICADOR _OP_DIVISION_ASIGNACION expr    { if(!operacion_asignacion($1, $3, _OP_DIVISION_ASIGNACION)) { YYABORT; } }
               | _IDENTIFICADOR _OP_INCREMENTO  { if (!operacion_asignacion($1, 1, _OP_SUMA_ASIGNACION)) { YYABORT; } }
               | _IDENTIFICADOR _OP_DECREMENTO  { if (!operacion_asignacion($1, 1, _OP_RESTA_ASIGNACION)) { YYABORT; } }
               | _CONSTANTE '=' expr        { error_asignacion_constante($1); free($1); YYABORT; }
               | _CONSTANTE _OP_SUMA_ASIGNACION expr        { error_asignacion_constante($1); free($1); YYABORT; }
               | _CONSTANTE _OP_RESTA_ASIGNACION expr        { error_asignacion_constante($1); free($1); YYABORT; }
               | _CONSTANTE _OP_MULTIPLICACION_ASIGNACION expr        { error_asignacion_constante($1); free($1); YYABORT; }
               | _CONSTANTE _OP_DIVISION_ASIGNACION expr        { error_asignacion_constante($1); free($1); YYABORT; }
;

funcion:              /* Función de un parámetro */
                      _FUNCION '(' expr ')'     { elemento e = comprobarTS($1);
                                                    free($1);
                                                    double res = e.dato.funcion($3);
                                                    if(echo_flag) printf("FUNCIÓN: %lf\n", res);
                                                    $$ = res; }

                      /* Función de dos parámetros */
                      | _FUNCION '(' expr ',' expr ')'    { elemento e = comprobarTS($1);
                                                            free($1);
                                                            double res = e.dato.funcion($3, $5);
                                                            if(echo_flag) printf("FUNCIÓN: %lf\n", res);
                                                            $$ = res; }
;

comparacion:    expr _OP_COMPARACION expr    { $$ = $1 == $3; if(echo_flag) printf("IGUAL: ");  }
                      | expr _OP_DISTINTO expr    { $$ = $1 != $3; if(echo_flag) printf("DISTINTO: ");  }
                      | expr _OP_MAYOR_IGUAL expr   { $$ = $1 >= $3; if(echo_flag) printf("MAYOR IGUAL: ");  }
                      | expr _OP_MENOR_IGUAL expr   { $$ = $1 <= $3; if(echo_flag) printf("MENOR IGUAL: "); }
                      | expr '>' expr    { $$ = $1 > $3; if(echo_flag) printf("MAYOR: ");  }
                      | expr '<' expr    { $$ = $1 < $3; if(echo_flag) printf("MENOR: ");  }
;

comando:      _COMANDO '(' ')'  { if(echo_flag) printf("COMANDO: %s\n", $1); int tam = strlen($1); char *lex = malloc(tam+1); strncpy(lex, $1, tam); lex[tam] = '\0'; elemento e = comprobarTS($1); e.dato.comando(); free($1); if (strncmp(lex, "exit", tam+1)) free(lex); else { free(lex); YYACCEPT; } }
              | _COMANDO '(' _STRING ')'  { if(echo_flag) printf("COMANDO: %s\n", $1); elemento e = comprobarTS($1); free($1); e.dato.comando($3); free($3); }
              /* Comando import */
              | _COMANDO '(' _STRING ',' _STRING ')'  { if(echo_flag) printf("COMANDO: %s\n", $1); elemento e = comprobarTS($1); free($1); e.dato.comando($3, $5); free($3); }
;


%%

int operacion_asignacion(char *lex, double valor, int codigo_op) {
    elemento e = comprobarTS(lex);
    e.lex = lex;
    if (e.codigo == _IDENTIFICADOR) {
        switch (codigo_op) {
            case _OP_SUMA_ASIGNACION:
                e.dato.valor += valor;
                break;
            case _OP_RESTA_ASIGNACION:
                e.dato.valor -= valor;
                break;
            case _OP_MULTIPLICACION_ASIGNACION:
                e.dato.valor *= valor;
                break;
            case _OP_DIVISION_ASIGNACION:
                e.dato.valor /= valor;
                break;
        }
        insertarTS(e);
        if(echo_flag) printf("ASIGNACIÓN: %lf\n", e.dato.valor);
        return 1;
    } else {
        error_variable_sin_inicializar(lex);
        free(lex);
        return 0;
    }
}

