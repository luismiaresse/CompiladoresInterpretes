
#include "sistemaentrada.h"
#include "errores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición e instancia del doble centinela
typedef struct {
    char centinelaA[TAM_CENTINELA + 1];   // +1 para EOF
    char centinelaB[TAM_CENTINELA + 1];
    int inicio;    // Posición del apuntador inicio
    int fin;       // Posición del apuntador delantero
    int activo;     // Indica qué centinela se está usando (0 -> A, 1 -> B)
    int cargado;    // Indica el último centinela cargado (0 -> A, 1 -> B)
} buffer;

buffer buf;
int tamLexActual = 0;    // Tamaño del lexema actual

// Archivo de entrada
FILE *archivo;

void _cargarNuevoCentinela() {
    // Intercambia centinelas y vacía los caracteres antiguos
    buf.activo = !buf.activo;
    buf.cargado = buf.activo;
    memset(buf.activo == 0 ? buf.centinelaA : buf.centinelaB, 0, TAM_CENTINELA);
    char *lectura = malloc((TAM_CENTINELA + 1) * sizeof(char));
    int charsALeer = TAM_CENTINELA;
    while (charsALeer > 0) {
        fgets(lectura, charsALeer, archivo);
        if (lectura == NULL) {
            error_archivo();
            exit(1);
        }
        charsALeer -= strlen(lectura);
        strcat(buf.activo == 0 ? buf.centinelaA : buf.centinelaB, lectura);
        // Se debe leer un caracter más para completar el centinela y evitar eliminar el EOF artificial
        if (charsALeer == 1) {
            char c = fgetc(archivo);
            if (buf.activo == 0) buf.centinelaA[TAM_CENTINELA - 1] = c;
            else buf.centinelaB[TAM_CENTINELA - 1] = c;
            charsALeer--;
        }
        // Si se llegó al final del archivo, se coloca ese EOF en el centinela
        if (feof(archivo)) {
            if (buf.activo == 0) buf.centinelaA[TAM_CENTINELA-charsALeer] = EOF;
            else buf.centinelaB[TAM_CENTINELA-charsALeer] = EOF;
            break;
        }
    }
    free(lectura);
}

void _inicializarCentinela() {
    buf.centinelaA[TAM_CENTINELA] = EOF;
    buf.centinelaB[TAM_CENTINELA] = EOF;
    buf.inicio = 0;
    buf.fin = -1;   // Para que al leer el primer caracter se actualice a 0
    buf.activo = 1;
    _cargarNuevoCentinela();
}

void leer_archivo(char *path) {
    archivo = fopen(path, "r");
    if (archivo == NULL) {
        error_archivo();
        exit(1);
    }
    _inicializarCentinela();
}

void cerrar_archivo() {
    fclose(archivo);
}

char sig_caracter(int nuevoLexema) {
    buf.fin++;
    if (tamLexActual == TAM_CENTINELA && nuevoLexema != -1) {
        error_tam_lexema();
        tamLexActual = 0;
        // Espacio artificial para aceptar hasta ahora
        // El lexema se "parte" en dos (o más)
        return ' ';
    }
    if (nuevoLexema || tamLexActual == 0) {
        buf.inicio = buf.fin;
        tamLexActual = 0;
    }
    tamLexActual++;
    if (buf.fin == TAM_CENTINELA) {
        buf.fin = 0;
        if (buf.cargado == buf.activo) _cargarNuevoCentinela();
        else buf.activo = !buf.activo;
    }
    char c = buf.activo == 0 ? buf.centinelaA[buf.fin] : buf.centinelaB[buf.fin];
    return c;
}

void retroceder_caracter() {
    tamLexActual--;
    if (buf.fin == 0) {
        buf.fin = TAM_CENTINELA - 1;
        buf.activo = !buf.activo;
    } else buf.fin--;
}


char * recuperar_lexema() {
    int tam;
    int activoInicial = buf.activo;
    int marcador = 0;   // Para guardar las iteraciones hasta cambiar de centinela
    int pos;
    int inicio = buf.inicio;   // Copia de inicio para no perderla
    if (buf.fin >= buf.inicio) {
        tam = buf.fin - buf.inicio + 1;
    } else {
        tam = TAM_CENTINELA - buf.inicio + buf.fin + 1;
        activoInicial = !activoInicial;
    }
    char *lexema = malloc((tam+1) * sizeof(char));   // +1 para \0
    for (int i = 0; i < tam; i++) {
        pos = inicio + i - marcador;
        // Si se llega al EOF del primer centinela, cambio al otro
        if (pos == TAM_CENTINELA) {
            inicio = 0;
            marcador = i;
            activoInicial = !activoInicial;
            pos = 0;
        }
        lexema[i] = activoInicial == 0 ? buf.centinelaA[pos] : buf.centinelaB[pos];
    }
    lexema[tam] = '\0';
    return lexema;
}
