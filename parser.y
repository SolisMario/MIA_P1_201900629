%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include "scanner.h"
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include "mount.h"
#include "rep.h"
#include "mkfs.h"

using namespace std;
extern int yylex(void);
extern char *yytext;
extern FILE *yyin;
extern void yyerror(const char *s);

MKDISK * mkdiskV;
RMDISK * rmdiskV;
FDISK * fdiskV;
MOUNT * mountV;
REP * repV;
MKFS * mkfsV;
%}

%start INI

%token<STRING> id_particion
%token<NUM> numero
%token<STRING> cadena
%token<STRING> guion
%token<STRING> igual
%token<STRING> mkdisk
%token<STRING> size
%token<STRING> fit
%token<STRING> unit
%token<STRING> path
%token<STRING> rid
%token<STRING> id
%token<STRING> ruta
%token<STRING> rmdisk
%token<STRING> fdisk
%token<STRING> rtype
%token<STRING> rdelete
%token<STRING> rname
%token<STRING> radd
%token<STRING> rmount
%token<STRING> rumount
%token<STRING> retruta
%token<STRING> root
%token<STRING> rep
%token<STRING> rmkfs
%token<STRING> rfs

%type<STRING> INI
%type<STRING> INSTRUCCION
%type<STRING> INSTRUCCIONES
%type<STRING> MKDISKPARAMS
%type<STRING> MKDISKPAR
%type<STRING> RMDISKPARAMS
%type<STRING> FDISKPARAMS
%type<STRING> FDISKPAR
%type<STRING> MOUNTPARAM
%type<STRING> MOUNTPARAMS
%type<STRING> UMOUNTPARAM
%type<STRING> REPPARAMS
%type<STRING> REPPARAM
%type<STRING> MKFSPARAMS
%type<STRING> MKFSPARAM

%define parse.error verbose
%locations

%union{
    char*STRING;
    char*NUM;
}

%%

INI:
    	INSTRUCCIONES
;

INSTRUCCIONES:
   	INSTRUCCIONES INSTRUCCION
    	| INSTRUCCION
;

INSTRUCCION:
    	mkdisk {mkdiskV = new MKDISK();} MKDISKPARAMS {mkdiskV->mkdisk();/*realiza la creación del disco*/}
    	| rmdisk {rmdiskV = new RMDISK();} RMDISKPARAMS {rmdiskV->rmdisk();}
	| fdisk {fdiskV = new FDISK();} FDISKPARAMS {fdiskV->fdisk();}
	| rmount {mountV = new MOUNT();} MOUNTPARAMS {mountV->mount();}
	| rumount {mountV = new MOUNT();} UMOUNTPARAM {mountV->umount();}
	| rep {repV = new REP();} REPPARAMS {repV->rep();}
	| rmkfs {mkfsV = new MKFS();} MKFSPARAMS {mkfsV->mkfs();}
;

MKDISKPARAMS:
    	MKDISKPARAMS MKDISKPAR
    	| MKDISKPAR
;

MKDISKPAR:
	guion size igual numero {mkdiskV->setSize(atoi($4));}
    	| guion fit igual id {mkdiskV->setFit($4);}
    	| guion unit igual id {mkdiskV->setUnit($4);}
    	| guion path igual ruta {mkdiskV->setPath(false, $4);}
    	| guion path igual cadena {mkdiskV->setPath(true, $4);}
;

RMDISKPARAMS:
	guion path igual ruta {rmdiskV->setPath(false, $4);}
        | guion path igual cadena {rmdiskV->setPath(true, $4);}
;

FDISKPARAMS:
	FDISKPARAMS FDISKPAR
	| FDISKPAR
;

FDISKPAR:
	guion size igual numero {fdiskV->setSize(atoi($4));}
	| guion unit igual id {fdiskV->setUnit($4);}
	| guion path igual ruta {fdiskV->setPath(false, $4);}
	| guion path igual cadena {fdiskV->setPath(true, $4);}
	| guion rtype igual id {fdiskV->setType($4);}
	| guion fit igual id {fdiskV->setFit($4);}
	| guion rdelete igual id {fdiskV->setDelete($4);}
	| guion rname igual id {fdiskV->setName(false, $4);}
	| guion rname igual cadena {fdiskV->setName(true, $4);}
	| guion radd igual numero {fdiskV->setAdd(atoi($4));}
;

MOUNTPARAMS:
	MOUNTPARAMS MOUNTPARAM
	| MOUNTPARAM
;

MOUNTPARAM:
	guion path igual ruta {mountV->setPath(false, $4);}
        | guion path igual cadena {mountV->setPath(true, $4);}
	| guion rname igual id {mountV->setName(false, $4);}
        | guion rname igual cadena {mountV->setName(true, $4);}
;

UMOUNTPARAM:
	guion rid igual id_particion {mountV->setID($4);}
;

REPPARAMS:
	REPPARAMS REPPARAM
	| REPPARAM
;

REPPARAM:
	guion rname igual id {repV->setName($4);}
	| guion path igual ruta {repV->setPath(false, $4);}
        | guion path igual cadena {repV->setPath(true, $4);}
        | guion rid igual id_particion {repV->setID($4);}
        | guion retruta igual id {repV->setRuta($4);}
        | guion root igual numero {repV->setRoot(atoi($4));}
;

MKFSPARAMS:
	MKFSPARAMS MKFSPARAM
	| MKFSPARAM
;

MKFSPARAM:
	guion rid igual id_particion {mkfsV->set_id($4);}
	| guion rtype igual id {mkfsV->set_type($4);}
	| guion rfs igual id {mkfsV->set_fs($4);}
;

%%
void yyerror(const char *s)
{
    printf("Error sintactico en la linea %i: %s\n", 0, s);
}