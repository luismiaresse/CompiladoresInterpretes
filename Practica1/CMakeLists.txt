cmake_minimum_required(VERSION 3.24)
project(Compilador C)

set(CMAKE_C_STANDARD 11)

add_executable(Compilador main.c definiciones.h lexico.c lexico.h sintactico.c sintactico.h tablasimbolos.c tablasimbolos.h sistemaentrada.c sistemaentrada.h errores.c errores.h)
