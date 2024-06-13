/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SINTACTICO_H_INCLUDED
# define YY_YY_SINTACTICO_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    _NUMERO = 258,                 /* _NUMERO  */
    _IDENTIFICADOR = 259,          /* _IDENTIFICADOR  */
    _FUNCION = 260,                /* _FUNCION  */
    _CONSTANTE = 261,              /* _CONSTANTE  */
    _COMANDO = 262,                /* _COMANDO  */
    _STRING = 263,                 /* _STRING  */
    _OPERADOR = 264,               /* _OPERADOR  */
    _OP_COMPARACION = 265,         /* _OP_COMPARACION  */
    _OP_INCREMENTO = 266,          /* _OP_INCREMENTO  */
    _OP_DECREMENTO = 267,          /* _OP_DECREMENTO  */
    _OP_SUMA_ASIGNACION = 268,     /* _OP_SUMA_ASIGNACION  */
    _OP_RESTA_ASIGNACION = 269,    /* _OP_RESTA_ASIGNACION  */
    _OP_MULTIPLICACION_ASIGNACION = 270, /* _OP_MULTIPLICACION_ASIGNACION  */
    _OP_DIVISION_ASIGNACION = 271, /* _OP_DIVISION_ASIGNACION  */
    _OP_DISTINTO = 272,            /* _OP_DISTINTO  */
    _OP_MAYOR_IGUAL = 273,         /* _OP_MAYOR_IGUAL  */
    _OP_MENOR_IGUAL = 274,         /* _OP_MENOR_IGUAL  */
    _OP_MAYOR = 275,               /* _OP_MAYOR  */
    _OP_MENOR = 276,               /* _OP_MENOR  */
    _EOF = 277                     /* _EOF  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "bison.y"

    char *str;
    double num;

#line 91 "sintactico.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SINTACTICO_H_INCLUDED  */
