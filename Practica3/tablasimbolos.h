// Tabla de Símbolos: tabla hash con recolocación lineal

#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#define TAM_TS_INICIAL 53     // Para ser eficiente debe ser primo y tener un factor de carga entre 0.5 y 0.75
#define MULTIPLICADOR 2       // Factor de crecimiento de la tabla al redimensionar (en su lugar, se podría hacer con una lista de números primos)


typedef union {
    double valor;
    double (*funcion)();
    void (*comando)();
}valor;

typedef struct {
    char *lex;  // clave
    int codigo;   // tipo para saber si es una función, identificador o constante
    void *handle; // handle para la librería dinámica
    valor dato;
}elemento;

// DEFINICIÓN DEL TAD
typedef elemento *TablaSimbolos;

void crearTS(int tam);
void imprimirTS();
elemento comprobarTS(char *lexema);
void insertarTS(elemento e);
void destruirTS();
TablaSimbolos * obtenerTS();

#endif //TABLASIMBOLOS_H
