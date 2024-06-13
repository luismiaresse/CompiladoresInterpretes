// Tabla de Símbolos: tabla hash con recolocación lineal

#ifndef COMPILADOR_TABLASIMBOLOS_H
#define COMPILADOR_TABLASIMBOLOS_H

#define TAM_TS_INICIAL 53     // Para ser eficiente debe ser primo y tener un factor de carga entre 0.5 y 0.75
#define MULTIPLICADOR 2       // Factor de crecimiento de la tabla al redimensionar (en su lugar, se podría hacer con una lista de números primos)

typedef struct {
    char *lexema;  // clave
    int codigo;    // valor
}elemento;

// DEFINICIÓN DEL TAD
typedef elemento *TablaSimbolos;

void crearTS(TablaSimbolos *t, int tam);
void imprimirTS(TablaSimbolos t);
int comprobarTS(TablaSimbolos *t, char *lexema);
void destruirTS(TablaSimbolos t);

#endif //COMPILADOR_TABLASIMBOLOS_H
