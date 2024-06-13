
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tablasimbolos.h"
#include "definiciones.h"

typedef unsigned int uint;

int huecos = TAM_TS_INICIAL;     // Número de huecos libres en la tabla
int tamActual = TAM_TS_INICIAL;  // Tamaño actual de la tabla
elemento reservadas[] = {           // Palabras reservadas
        {"import", IMPORT},
        {"while", WHILE},
        {"int", INT},
        {"double", DOUBLE},
        {"void", VOID},
        {"return", RETURN},
        {"foreach", FOREACH},
        {"cast", CAST}
};

int _buscar(TablaSimbolos t, elemento *e);
void _insertar(TablaSimbolos *t, elemento e);


void _inicializarTS(TablaSimbolos t) {
    for (int i = 0; i < tamActual; i++) {
        t[i].lexema = NULL;
        t[i].codigo = 0;
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
uint _posicion(TablaSimbolos t, char *lexema) {
    uint posicion;
    uint ini = FuncionHash(lexema);

    for (int i = 0; i < tamActual; i++) {
        posicion = (ini + i) % tamActual;
        // Compruebo si está vacío o si es el elemento que busco
        if (t[posicion].lexema == NULL || !strcmp(t[posicion].lexema, lexema)) {
            return posicion;
        }
    }
    return ini; //devuelvo la posición dada por la función hash
}

void _redimensionarTS(TablaSimbolos *t, int nuevoTam) {
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
int _buscar(TablaSimbolos t, elemento *e) {
    char *lexema = e->lexema;
    uint posicion = _posicion(t, lexema);
    if (t[posicion].lexema == NULL)
        return 0;
    else {
        if (strcmp(t[posicion].lexema, lexema) == 0) {
            *e = t[posicion];
            return 1;
        } else return 0;
    }
}

/* Funcion que inserta un elemento en la tabla */
void _insertar(TablaSimbolos *t, elemento e) {
    uint posicion = _posicion(*t, e.lexema);
    if ((*t)[posicion].lexema == NULL) {
        (*t)[posicion] = e;
        huecos--;
    }
}

void crearTS(TablaSimbolos *t, int tam) {
    tamActual = tam;
    *t = malloc(sizeof(elemento) * tamActual);
    _inicializarTS(*t);
    for (int i = 0; i < sizeof(reservadas) / sizeof(elemento); i++)
        _insertar(t, reservadas[i]);
}

void imprimirTS(TablaSimbolos t) {
    for (int i = 0; i < tamActual; i++) {
        if (t[i].lexema != NULL)
            printf("Elemento [%d] = {%s, %d}\n", i, t[i].lexema, t[i].codigo);
    }
}

int comprobarTS(TablaSimbolos *t, char *lexema) {
    if (huecos < tamActual / 4)        // Mantener un factor de carga de al menos 0.75
        _redimensionarTS(t, tamActual * MULTIPLICADOR);
    elemento e = {lexema, 0};
    if (!_buscar(*t, &e)) {    // No está en la tabla, identificador nuevo
        e.codigo = IDENTIFICADOR;
        _insertar(t, e);
    } else {
        free(lexema);
    }
    int codigo = e.codigo;
    return codigo;
}

void destruirTS(TablaSimbolos t) {
    // Las palabras reservadas no se destruyen (son estáticas)
    int esReservada = 0;
    for (int i = 0; i < tamActual; i++) {
        if (t[i].lexema != NULL) {
            for (int j = 0; j < sizeof(reservadas) / sizeof(elemento); j++) {
                if (!strcmp(t[i].lexema, reservadas[j].lexema)) {
                    esReservada = 1;
                    break;
                }
            }
        }
        if (!esReservada) free(t[i].lexema);
        esReservada = 0;
    }
    free(t);
}