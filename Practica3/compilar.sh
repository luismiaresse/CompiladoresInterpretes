#!/usr/bin/env sh
gcc -Wall comandos.c main.c tablasimbolos.c sintactico.c lexico.c errores.c -o calculadora -lm
