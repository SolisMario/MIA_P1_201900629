/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    numero = 258,
    cadena = 259,
    guion = 260,
    igual = 261,
    mkdisk = 262,
    rfind = 263,
    size = 264,
    fit = 265,
    unit = 266,
    rp = 267,
    rr = 268,
    path = 269,
    rmdisk = 270,
    fdisk = 271,
    rtype = 272,
    rdelete = 273,
    rname = 274,
    radd = 275,
    rmount = 276,
    rumount = 277,
    rmkfs = 278,
    id = 279,
    rfs = 280,
    ruta = 281,
    rep = 282,
    rid = 283,
    retruta = 284,
    root = 285,
    id_particion = 286,
    rmkdir = 287,
    touch = 288,
    rcont = 289,
    rstdin = 290,
    rexec = 291,
    rcat = 292,
    rren = 293,
    rmv = 294,
    rdest = 295,
    rrm = 296,
    redit = 297,
    rpause = 298,
    rcp = 299,
    rpwd = 300,
    ruser = 301,
    rlogin = 302,
    rlogout = 303,
    rmkgrp = 304,
    rrmgrp = 305,
    rmkusr = 306,
    rusr = 307,
    rgrp = 308,
    rrmusr = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 153 "parser.y"

    char*STRING;
    char*NUM;

#line 117 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
