
#include "lexico.h"
#include "errores.h"
#include "sistemaentrada.h"
#include "definiciones.h"
#include "tablasimbolos.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Identificadores de automatas
#define AF_INICIAL 0
#define AF_ALFANUMERICO 1
#define AF_NUMERICO 2
#define AF_OPERADOR 3
#define AF_DECIMAL 5
#define AF_STRING 6
#define AF_PUNTO 7
#define AF_COMENTARIO 8
#define AF_EXPONENCIAL 9

// Variables globales
int linea = 1;              // Línea actual
int error = 0;              // Indica si se detectó un error léxico (AF que no acepta)
int aceptar = 0;            // Indica si se debe aceptar el lexema actual
int estado = 0;             // Estado actual (indica el AF a usar en la siguiente iteración)
int componente = 0;         // Componente léxico actual

void _aceptar_lexema(TablaSimbolos *t) {
    estado = AF_INICIAL;
    // Sólo se inserta en la TS si es un identificador
    if (!componente) {
        char *lexema = recuperar_lexema();
        componente = comprobarTS(t, lexema);
    }
}

void _AFstring() {    // Comienza con el caracter "
    char c = sig_caracter(0);
    char sig = sig_caracter(0);
    if (c == '\\' && sig == '"') return;
    else if (c == '\n') {
        linea++;
    } else if (c == '"') {
        aceptar = 1;
        componente = TIPO_STRING;
    } else if (c == EOF) {
        retroceder_caracter();
        error = ERR_LEX_STRING_NO_CERRADO;
    }
    retroceder_caracter();
}

void _AFexponencial() {
    char c = sig_caracter(0);
    if (c == '+' || c == '-') {
        c = sig_caracter(0);
        while (isdigit(c)) {
            c = sig_caracter(0);
        }
        retroceder_caracter();
        componente = TIPO_DOUBLE;
        aceptar = 1;
    } else {
        error = ERR_LEX_EXPONENCIAL_INVALIDO;
    }
}

void _AFnumerico() {
    char c = sig_caracter(0);
    if (isdigit(c) || c == '_');    // _ mejora la legibilidad pero no varía nada
    else if (c == '.') {
        estado = AF_DECIMAL;
    } else if (c == 'b' || c == 'B') {    // Número binario
        while (1) {
            c = sig_caracter(0);
            if (c == '0' || c == '1' || c == '_');
            else if (isalnum(c)) {
                error = ERR_LEX_BINARIO_INVALIDO;
                while (isalnum(c)) {
                    c = sig_caracter(0);
                }
                retroceder_caracter();
                return;
            } else {
                retroceder_caracter();
                componente = TIPO_BINARIO;
                aceptar = 1;
                return;
            }
        }
    } else if (c == 'e' || c == 'E') {   // Número exponencial
        estado = AF_EXPONENCIAL;
    } else {
        retroceder_caracter();
        componente = TIPO_ENTERO;
        aceptar = 1;
    }
}

void _AFdecimal() {
    char c = sig_caracter(0);
    if (isdigit(c));
    else if (c == 'e' || c == 'E') {   // Número exponencial
        estado = AF_EXPONENCIAL;
    }
    else {
        retroceder_caracter();
        componente = TIPO_DOUBLE;
        aceptar = 1;
    }
}

void _AFpunto() {
    char c = sig_caracter(0);
    if (isdigit(c)) {
        estado = AF_DECIMAL;
    } else {
        retroceder_caracter();
        componente = (int)'.';
        aceptar = 1;
    }
}

void _AFcomentario() {
    char c = sig_caracter(0);
    char sig = sig_caracter(0);
    if (c == '/') {     // Comentario //
        while (sig != '\n') {
            sig = sig_caracter(-1);
            if (sig == EOF) {
                retroceder_caracter();
                error = ERR_LEX_COMENTARIO_NO_CERRADO;
                return;
            }
        }
        linea++;
        estado = AF_INICIAL;
        return;
    }
    else if (c == '+') {     // Comentario /+ +/
        int nivel = 1;
        while (1) {
            if (sig == EOF) {
                retroceder_caracter();
                error = ERR_LEX_COMENTARIO_NO_CERRADO;
                return;
            } else if (sig == '\n') linea++;
            else if (sig == '/') {
                sig = sig_caracter(-1);
                if (sig == '+') nivel++;
                else retroceder_caracter();
            }
            else if (sig == '+') {
                sig = sig_caracter(-1);
                if (sig == '/') {
                    nivel--;
                    if (nivel == 0) {
                        estado = AF_INICIAL;
                        return;
                    }
                } else retroceder_caracter();
            }
            sig = sig_caracter(-1);
        }
    }
    else if (c == '*') {        // Comentario /* */
        while (1) {
            if (sig == EOF) {
                retroceder_caracter();
                error = ERR_LEX_COMENTARIO_NO_CERRADO;
                return;
            } else if (sig == '\n') linea++;
            c = sig;
            sig = sig_caracter(-1);
            if (c == '*' && sig == '/') {
                estado = AF_INICIAL;
                return;
            }
        }
    }
}

void _AFoperador() {
    retroceder_caracter();
    char ant = sig_caracter(0);  // Operador leído en el AF inicial
    char c = sig_caracter(0);
    if (ant == '=' && c == '=') {
        componente = OP_COMPARACION;
        aceptar = 1;
    } else if (ant == '+' && c == '+') {
        componente = OP_INCREMENTO;
        aceptar = 1;
    } else if (ant == '+' && c == '=') {
        componente = OP_SUMA_ASIGNACION;
        aceptar = 1;
    } else if (isalnum(c) || c == '_' || c == ' ' || c == '\t' || c == '(' || c == '[') {   // Si es un identificador, aceptar
        retroceder_caracter();
        componente = (int)ant;  // =, +, -, *, /, etc.
        aceptar = 1;
    } else if (ant == '/' && (c == '/' || c == '*' || c == '+')) {   // Comentario
        retroceder_caracter();
        estado = AF_COMENTARIO;
    } else {
        error = ERR_LEX_OPERADOR_INVALIDO;
    }

}


// Devuelve el código del siguiente lexema
int sig_comp_lexico(TablaSimbolos *t) {
    char c;
    estado = AF_INICIAL;
    componente = 0;
    error = 0;
    aceptar = 0;
    while (!error) {
        switch (estado) {
            case AF_INICIAL:
                c = sig_caracter(1);
                if (c == ' ' || c == '\t');
                else if (isalpha(c) || c == '_') {
                    // Autómata alfanumérico
                    estado = AF_ALFANUMERICO;
                } else if (isdigit(c)) {
                    // Autómata numérico
                    estado = AF_NUMERICO;
                } else if (c == '"') {
                    // Autómata de string
                    estado = AF_STRING;
                } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!') {
                    // Autómata de operadores
                    estado = AF_OPERADOR;
                } else if (c == '.') {
                    // Autómata de punto (detecta si es numérico o un acceso a un componente de una estructura/librería)
                    estado = AF_PUNTO;
                } else if (c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',') {
                    aceptar = 1;
                    componente = (int)c;
                } else if (c == '\n') {
                    linea++;
                } else if (c == EOF) {
                    return EOF;
                }
                break;
            case AF_ALFANUMERICO:
                c = sig_caracter(0);
                if (isalnum(c) || c == '_');
                else {
                    retroceder_caracter();
                    aceptar = 1;
                }
                break;
            case AF_NUMERICO:
                _AFnumerico();
                break;
            case AF_DECIMAL:
                _AFdecimal();
                break;
            case AF_STRING:
                _AFstring();
                break;
            case AF_PUNTO:
                _AFpunto();
                break;
            case AF_OPERADOR:
                _AFoperador();
                break;
            case AF_COMENTARIO:
                _AFcomentario();
                break;
            case AF_EXPONENCIAL:
                _AFexponencial();
                break;
        }
        if (aceptar) _aceptar_lexema(t);
        if (componente) return componente;
    }
    char *lexema = recuperar_lexema();
    error_lexico(error, lexema, linea);
    free(lexema);
    return ERR_LEXICO;
}




