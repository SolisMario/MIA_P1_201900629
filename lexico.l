%option noyywrap
%option case-insensitive
%option yylineno
%option nounput

%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "parser.h"
int linea = 1;
%}

comentario [#]([^\n])*
numero [-]?[0-9]+
id_particion "29"[0-9]+[A-Z]
cadena \"(\\\"|\\n|\\t|\\r|\\\\|[^\"])*\"
ruta (([/]([a-zA-Z]|[0-9]|"_"|".")+)+)
id ([a-zA-Z])([a-zA-Z]|[0-9]|"_")*

%%
('\s'|'\r'|'\t')+ {};
"\n" {linea++;};
{comentario} {};
{id_particion} {yylval.STRING = strdup(yytext);return (id_particion);};
{numero} {yylval.STRING = strdup(yytext);return (numero);};
{cadena} {yylval.STRING = strdup(yytext); return (cadena);};
("–"|"-") {yylval.STRING = strdup(yytext);return (guion);};
"=" {yylval.STRING = strdup(yytext);return (igual);};
"mkdisk" {yylval.STRING = strdup(yytext);return (mkdisk);};
"size" {yylval.STRING = strdup(yytext);return (size);};
"f" {yylval.STRING = strdup(yytext);return (fit);};
"u" {yylval.STRING = strdup(yytext);return (unit);};
"path" {yylval.STRING = strdup(yytext);return (path);};
"rmdisk" {yylval.STRING = strdup(yytext);return (rmdisk);};
"fdisk" {yylval.STRING = strdup(yytext);return (fdisk);};
"type" {yylval.STRING = strdup(yytext);return (rtype);};
"delete" {yylval.STRING = strdup(yytext);return (rdelete);};
"name" {yylval.STRING = strdup(yytext);return (rname);};
"add" {yylval.STRING = strdup(yytext);return (radd);};
"mount" {yylval.STRING = strdup(yytext);return (rmount);};
"umount" {yylval.STRING = strdup(yytext);return (rumount);};
"id" {yylval.STRING = strdup(yytext);return (rid);};
{id} {yylval.STRING = strdup(yytext); return (id);};
{ruta} {yylval.STRING = strdup(yytext); return (ruta);};
. {}
%%