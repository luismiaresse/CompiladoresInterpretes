cmake_minimum_required(VERSION 3.24)
project(Practica2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Practica2
        definiciones.h
        errores.c
        errores.h
        main.c
        sintactico.c
        sintactico.h
        tablasimbolos.c
        tablasimbolos.h
        lexico.c
        )
