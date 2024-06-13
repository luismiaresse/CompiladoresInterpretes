
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tablasimbolos.h"
#include "sintactico.h"
#include "comandos.h"
#include <math.h>
#include <dlfcn.h>

typedef unsigned int uint;

int huecos = TAM_TS_INICIAL;     // Número de huecos libres en la tabla
int tamActual = TAM_TS_INICIAL;  // Tamaño actual de la tabla
elemento reservadas[] = {           // Palabras (Funciones) reservadas
    {"exit", _COMANDO, .dato.comando = __exit},
    {"help", _COMANDO, .dato.comando = help},
    {"clear", _COMANDO, .dato.comando = clear},
    {"workspace", _COMANDO, .dato.comando = workspace},
    {"echo", _COMANDO, .dato.comando = toggle_echo},
    {"load", _COMANDO, .dato.comando = load},
    {"import", _COMANDO, .dato.comando = import},
    {"sin", _FUNCION, .dato.funcion = sin},
    {"cos", _FUNCION, .dato.funcion = cos},
    {"tan", _FUNCION, .dato.funcion = tan},
    {"asin", _FUNCION, .dato.funcion = asin},
    {"acos", _FUNCION, .dato.funcion = acos},
    {"atan", _FUNCION, .dato.funcion = atan},
    {"sqrt", _FUNCION, .dato.funcion = sqrt},
    {"log", _FUNCION, .dato.funcion = log},
    {"exp", _FUNCION, .dato.funcion = exp},
    {"abs", _FUNCION, .dato.funcion = fabs},
    {"pow", _FUNCION, .dato.funcion = pow},
    {"log10", _FUNCION, .dato.funcion = log10},
    {"floor", _FUNCION, .dato.funcion = floor},
    {"ceil", _FUNCION, .dato.funcion = ceil},
    {"pi", _CONSTANTE, .dato.valor = M_PI},
    {"e", _CONSTANTE, .dato.valor = M_E},
};

TablaSimbolos *t = NULL;    // Referencia global a la tabla de símbolos

int _buscar(elemento *e);
void _insertar(elemento e);

void _inicializarTS() {
    for (int i = 0; i < tamActual; i++) {
        (*t)[i].lex = NULL;
        (*t)[i].codigo = 0;
    }
}

uint FuncionHash(char *cad) {
    int suma = 0;
    int K = 500;
    for (int i = strlen(cad) - 1; i >= 0; i--) {
        suma = (suma * K + cad[i]) % tamActual;
    }
    return suma;
}

/* Funcion que localiza la posicion del elemento */
uint _posicion(char *lexema) {
    uint posicion;
    uint ini = FuncionHash(lexema);
    for (int i = 0; i < tamActual; i++) {
        posicion = (ini + i) % tamActual;
        // Compruebo si está vacío o si es el elemento que busco
        if ((*t)[posicion].lex == NULL || !strcmp((*t)[posicion].lex, lexema)) {
            return posicion;
        }
    }
    return ini; //devuelvo la posición dada por la función hash
}

void _redimensionarTS() {
    int tamAnterior = tamActual;
    int nuevoTam = tamActual * MULTIPLICADOR;
    *t = realloc(*t, nuevoTam * sizeof(elemento));
    tamActual = nuevoTam;
    huecos += nuevoTam - tamAnterior;
    // Recolocar los elementos según su nueva posición
    int ocupada = 0;
    for (int i = 0; i < tamAnterior; i++) {
        if ((*t)[i].lex != NULL) {
            uint posicion = _posicion((*t)[i].lex);
            if (posicion != i) {
                if ((*t)[posicion].lex != NULL) {
                    // Trasladar el elemento que ocupa la posición
                    (*t)[i] = (*t)[posicion];
                    ocupada = 1;
                }
                (*t)[posicion] = (*t)[i];
                if (ocupada) i--;
            }
        }
    }
}

/* Funcion que busca un elemento en la tabla, 0 si no lo encuentra, 1 si lo encuentra */
int _buscar(elemento *e) {
    char *lexema = e->lex;
    uint posicion = _posicion(lexema);
    if ((*t)[posicion].lex == NULL)
        return 0;
    else if (!strcmp((*t)[posicion].lex, lexema)) {
        *e = (*t)[posicion];
        return 1;
    }
    return 0;
}

/* Funcion que inserta un elemento en la tabla */
void _insertar(elemento e) {
    uint posicion = _posicion(e.lex);
    if ((*t)[posicion].lex == NULL) {
        (*t)[posicion] = e;
        huecos--;
    } else if (!strcmp((*t)[posicion].lex, e.lex)) {
        free((*t)[posicion].lex);
        (*t)[posicion] = e;
    }

}

void crearTS(int tam) {
    if (t == NULL) {
        t = malloc(sizeof(TablaSimbolos));
    }
    huecos = tam;
    tamActual = tam;
    *t = malloc(sizeof(elemento) * tamActual);
    _inicializarTS();
    for (int i = 0; i < sizeof(reservadas) / sizeof(elemento); i++) {
        _insertar(reservadas[i]);
    }
}

void imprimirTS() {
    for (int i = 0; i < tamActual; i++) {
        if ((*t)[i].lex != NULL && (*t)[i].codigo == _IDENTIFICADOR)
            printf("%s = %lf\n", (*t)[i].lex, (*t)[i].dato.valor);
    }
}

elemento comprobarTS(char *lexema) {
    if (huecos < tamActual / 4)        // Mantener un factor de carga de al menos 0.75
        _redimensionarTS();
    elemento e = {lexema, 0};
    _buscar(&e);
    return e;
}

void insertarTS(elemento e) {
    if (huecos < tamActual / 4)        // Mantener un factor de carga de al menos 0.75
        _redimensionarTS();
    _insertar(e);
}

void destruirTS() {
    // Las palabras reservadas no se destruyen (son estáticas)
    int esReservada = 0;
    for (int i = 0; i < tamActual; i++) {
        if ((*t)[i].lex != NULL) {
            for (int j = 0; j < sizeof(reservadas) / sizeof(elemento); j++) {
                if (!strcmp((*t)[i].lex, reservadas[j].lex)) {
                    esReservada = 1;
                    break;
                }
            }
        }
        if (!esReservada && (*t)[i].codigo != _FUNCION) free((*t)[i].lex);
        else if (!esReservada && (*t)[i].codigo == _FUNCION && (*t)[i].handle != NULL) dlclose((*t)[i].handle);
        esReservada = 0;
    }
    free(*t);
    *t = NULL;
    free(t);
    t = NULL;
}
