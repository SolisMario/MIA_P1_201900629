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
    comentarioSS = 259,
    cadena = 260,
    guion = 261,
    igual = 262,
    mkdisk = 263,
    rfind = 264,
    size = 265,
    fit = 266,
    unit = 267,
    rp = 268,
    rr = 269,
    path = 270,
    rmdisk = 271,
    fdisk = 272,
    rtype = 273,
    rdelete = 274,
    rname = 275,
    radd = 276,
    rmount = 277,
    rumount = 278,
    rmkfs = 279,
    id = 280,
    rfs = 281,
    ruta = 282,
    rep = 283,
    rid = 284,
    retruta = 285,
    root = 286,
    id_particion = 287,
    rmkdir = 288,
    touch = 289,
    rcont = 290,
    rstdin = 291,
    rexec = 292,
    rcat = 293,
    rren = 294,
    rmv = 295,
    rdest = 296,
    rrm = 297,
    redit = 298,
    rpause = 299,
    rcp = 300,
    rpwd = 301,
    ruser = 302,
    rlogin = 303,
    rlogout = 304,
    rmkgrp = 305,
    rrmgrp = 306,
    rmkusr = 307,
    rusr = 308,
    rgrp = 309,
    rrmusr = 310,
    rloss = 311,
    rrecovery = 312,
    rchgrp = 313
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 162 "parser.y"

    char*STRING;
    char*NUM;

#line 121 "parser.h"

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
