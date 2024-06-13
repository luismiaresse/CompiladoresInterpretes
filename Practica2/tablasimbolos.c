
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tablasimbolos.h"
#include "definiciones.h"

typedef unsigned int uint;

int huecos = TAM_TS_INICIAL;     // Número de huecos libres en la tabla
int tamActual = TAM_TS_INICIAL;  // Tamaño actual de la tabla
elemento reservadas[] = {           // Palabras reservadas
        {"import",  _IMPORT},
        {"while",   _WHILE},
        {"int",     _INT},
        {"double",  _DOUBLE},
        {"void",    _VOID},
        {"return",  _RETURN},
        {"foreach", _FOREACH},
        {"cast",    _CAST}
};

TablaSimbolos *t;    // Referencia global a la tabla de símbolos

int _buscar(TablaSimbolos tabla, elemento *e);
void _insertar(TablaSimbolos *tabla, elemento e);

void establecerTS(TablaSimbolos *tabla) {
    t = tabla;
}

void _inicializarTS(TablaSimbolos tabla) {
    if (tabla == NULL) tabla = *t;
    for (int i = 0; i < tamActual; i++) {
        tabla[i].lexema = NULL;
        tabla[i].codigo = 0;
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
uint _posicion(TablaSimbolos tabla, char *lexema) {
    uint posicion;
    uint ini = FuncionHash(lexema);
    if (tabla == NULL) tabla = *t;
    for (int i = 0; i < tamActual; i++) {
        posicion = (ini + i) % tamActual;
        // Compruebo si está vacío o si es el elemento que busco
        if (tabla[posicion].lexema == NULL || !strcmp(tabla[posicion].lexema, lexema)) {
            return posicion;
        }
    }
    return ini; //devuelvo la posición dada por la función hash
}

void _redimensionarTS(int nuevoTam) {
    int tamAnterior = tamActual;
    huecos = nuevoTam;
    TablaSimbolos nueva = NULL;
    crearTS(&nueva, nuevoTam);

    // Copia los elementos de la tabla anterior a la nueva
    for (int i = 0; i < tamAnterior; i++) {
        if ((*t)[i].lexema != NULL) {
            _insertar(&nueva, (*t)[i]);
        }
    }
    free(*t);
    *t = nueva;
}

/* Funcion que busca un elemento en la tabla, 0 si no lo encuentra, 1 si lo encuentra */
int _buscar(TablaSimbolos tabla, elemento *e) {
    if (tabla == NULL) tabla = *t;
    char *lexema = e->lexema;
    uint posicion = _posicion(tabla, lexema);
    if (tabla[posicion].lexema == NULL)
        return 0;
    else {
        if (strcmp(tabla[posicion].lexema, lexema) == 0) {
            *e = tabla[posicion];
            return 1;
        } else return 0;
    }
}

/* Funcion que inserta un elemento en la tabla */
void _insertar(TablaSimbolos *tabla, elemento e) {
    if (tabla == NULL) tabla = t;
    uint posicion = _posicion(*tabla, e.lexema);
    if ((*tabla)[posicion].lexema == NULL) {
        (*tabla)[posicion] = e;
        huecos--;
    }
}

void crearTS(TablaSimbolos *tabla, int tam) {
    tamActual = tam;
    *tabla = malloc(sizeof(elemento) * tamActual);
    _inicializarTS(*tabla);
    for (int i = 0; i < sizeof(reservadas) / sizeof(elemento); i++)
        _insertar(tabla, reservadas[i]);
}

void imprimirTS() {
    for (int i = 0; i < tamActual; i++) {
        if ((*t)[i].lexema != NULL)
            printf("Elemento [%d] = {%s, %d}\n", i, (*t)[i].lexema, (*t)[i].codigo);
    }
}

int comprobarTS(char *lexema) {
    if (huecos < tamActual / 4)        // Mantener un factor de carga de al menos 0.75
        _redimensionarTS(tamActual * MULTIPLICADOR);
    elemento e = {lexema, 0};
    if (!_buscar(*t, &e)) {    // No está en la tabla, identificador nuevo
        e.codigo = _IDENTIFICADOR;
        _insertar(t, e);
    } else {
        free(lexema);
    }
    int codigo = e.codigo;
    return codigo;
}

void destruirTS() {
    // Las palabras reservadas no se destruyen (son estáticas)
    int esReservada = 0;
    for (int i = 0; i < tamActual; i++) {
        if ((*t)[i].lexema != NULL) {
            for (int j = 0; j < sizeof(reservadas) / sizeof(elemento); j++) {
                if (!strcmp((*t)[i].lexema, reservadas[j].lexema)) {
                    esReservada = 1;
                    break;
                }
            }
        }
        if (!esReservada) free((*t)[i].lexema);
        esReservada = 0;
    }
    free(*t);
}