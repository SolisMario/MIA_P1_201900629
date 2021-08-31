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
id ([a-zA-Z])([a-zA-Z]|[0-9]|"_"|".")*

%%
('\s'|'\r'|'\t')+ {};
"\n" {linea++;};
{comentario} {};
{id_particion} {yylval.STRING = strdup(yytext);return (id_particion);};
{numero} {yylval.STRING = strdup(yytext);return (numero);};
{cadena} {yylval.STRING = strdup(yytext); return (cadena);};
("–"|"-") {yylval.STRING = strdup(yytext);return (guion);};
[=] {yylval.STRING = strdup(yytext);return (igual);};
"mkdisk" {yylval.STRING = strdup(yytext);return (mkdisk);};
"size" {yylval.STRING = strdup(yytext);return (size);};
"f" {yylval.STRING = strdup(yytext);return (fit);};
"u" {yylval.STRING = strdup(yytext);return (unit);};
"p" {yylval.STRING = strdup(yytext);return (rp);};
"r" {yylval.STRING = strdup(yytext);return (rr);};
"path" {yylval.STRING = strdup(yytext);return (path);};
"rmdisk" {yylval.STRING = strdup(yytext);return (rmdisk);};
"fdisk" {yylval.STRING = strdup(yytext);return (fdisk);};
"type" {yylval.STRING = strdup(yytext);return (rtype);};
"delete" {yylval.STRING = strdup(yytext);return (rdelete);};
"name" {yylval.STRING = strdup(yytext);return (rname);};
"add" {yylval.STRING = strdup(yytext);return (radd);};
"mount" {yylval.STRING = strdup(yytext);return (rmount);};
"umount" {yylval.STRING = strdup(yytext);return (rumount);};
"ruta" {yylval.STRING = strdup(yytext);return (retruta);};
"root" {yylval.STRING = strdup(yytext);return (root);};
"rep" {yylval.STRING = strdup(yytext);return (rep);};
"id" {yylval.STRING = strdup(yytext);return (rid);};
"mkfs" {yylval.STRING = strdup(yytext);return (rmkfs);};
"fs" {yylval.STRING = strdup(yytext);return (rfs);};
"touch" {yylval.STRING = strdup(yytext);return (touch);};
"cont" {yylval.STRING = strdup(yytext);return (rcont);};
"stdin" {yylval.STRING = strdup(yytext);return (rstdin);};
"mkdir" {yylval.STRING = strdup(yytext);return (rmkdir);};
"exec" {yylval.STRING = strdup(yytext);return (rexec);};
"cat" {yylval.STRING = strdup(yytext);return (rcat);};
"ren" {yylval.STRING = strdup(yytext);return (rren);};
"mv" {yylval.STRING = strdup(yytext);return (rmv);};
"dest" {yylval.STRING = strdup(yytext);return (rdest);};
"rm" {yylval.STRING = strdup(yytext);return (rrm);};
"edit" {yylval.STRING = strdup(yytext);return (redit);};
"pause" {yylval.STRING = strdup(yytext);return (rpause);};
"cp" {yylval.STRING = strdup(yytext);return (rcp);};
{id} {yylval.STRING = strdup(yytext); return (id);};
{ruta} {yylval.STRING = strdup(yytext); return (ruta);};
. {}
%%