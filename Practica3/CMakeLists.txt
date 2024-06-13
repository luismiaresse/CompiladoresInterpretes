cmake_minimum_required(VERSION 3.24)
project(Practica3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Practica3
        errores.c
        errores.h
        main.c
        tablasimbolos.c
        tablasimbolos.h
        lexico.c
        lexico.h
        sintactico.c
        sintactico.h
        comandos.c comandos.h libreria.c)

target_link_libraries(Practica3 m)