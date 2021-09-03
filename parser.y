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
#include "touch.h"
#include "mkdir.h"
#include "exec.h"
#include "cat.h"
#include "ren.h"
#include "mv.h"
#include "rm.h"
#include "edit.h"
#include "cp.h"
#include "find.h"
#include "users.h"
#include "simulate.h"

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
TOUCH * touchV;
MKDIR * mkdirV;
EXEC * execV;
CAT * catV;
REN * renV;
MV * moveV;
RM * rmV;
EDIT * editV;
CP * cpV;
FIND * findV;
USERS * usersV;
SIMULATE * simulateV;
%}

%start INI

%token<NUM> numero
%token<STRING> cadena
%token<STRING> guion
%token<STRING> igual
%token<STRING> mkdisk
%token<STRING> rfind
%token<STRING> size
%token<STRING> fit
%token<STRING> unit
%token<STRING> rp
%token<STRING> rr
%token<STRING> path
%token<STRING> rmdisk
%token<STRING> fdisk
%token<STRING> rtype
%token<STRING> rdelete
%token<STRING> rname
%token<STRING> radd
%token<STRING> rmount
%token<STRING> rumount
%token<STRING> rmkfs
%token<STRING> id
%token<STRING> rfs
%token<STRING> ruta
%token<STRING> rep
%token<STRING> rid
%token<STRING> retruta
%token<STRING> root
%token<STRING> id_particion
%token<STRING> rmkdir
%token<STRING> touch
%token<STRING> rcont
%token<STRING> rstdin
%token<STRING> rexec
%token<STRING> rcat
%token<STRING> rren
%token<STRING> rmv
%token<STRING> rdest
%token<STRING> rrm
%token<STRING> redit
%token<STRING> rpause
%token<STRING> rcp
%token<STRING> rpwd
%token<STRING> ruser
%token<STRING> rlogin
%token<STRING> rlogout
%token<STRING> rmkgrp
%token<STRING> rrmgrp
%token<STRING> rmkusr
%token<STRING> rusr
%token<STRING> rgrp
%token<STRING> rrmusr
%token<STRING> rloss
%token<STRING> rrecovery
%token<STRING> rchgrp

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
%type<STRING> TOUCHPARAMS
%type<STRING> TOUCHPARAM
%type<STRING> MKDIRPARAMS
%type<STRING> MKDIRPARAM
%type<STRING> CATPARAMS
%type<STRING> CATPARAM
%type<STRING> RENPARAMS
%type<STRING> RENPARAM
%type<STRING> MVPARAMS
%type<STRING> MVPARAM
%type<STRING> RMPARAM
%type<STRING> EDITPARAMS
%type<STRING> EDITPARAM
%type<STRING> CPPARAMS
%type<STRING> CPPARAM
%type<STRING> FINDPARAMS
%type<STRING> FINDPARAM
%type<STRING> LOGINPARAM
%type<STRING> LOGINPARAMS
%type<STRING> MKGRPPARAMS
%type<STRING> MKGRPPARAM
%type<STRING> RMGRPPARAMS
%type<STRING> RMGRPPARAM
%type<STRING> MKUSERPARAMS
%type<STRING> MKUSERPARAM
%type<STRING> RMUSERPARAMS
%type<STRING> RMUSERPARAM
%type<STRING> SIMULATEPARAM
%type<STRING> CHGRPPARAMS
%type<STRING> CHGRPPARAM

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
	| touch {touchV = new TOUCH();} TOUCHPARAMS {touchV->touch();}
	| rmkdir {mkdirV = new MKDIR();} MKDIRPARAMS {mkdirV->mkdir();}
	| rexec {execV = new EXEC();} EXECPARAM {execV->exec();}
	| rcat {catV = new CAT();} CATPARAMS {catV->cat();}
	| rren {renV = new REN();} RENPARAMS {renV->ren();}
	| rmv {moveV = new MV();} MVPARAMS {moveV->mv();}
	| rrm {rmV = new RM();} RMPARAM {rmV->rm();}
	| redit {editV = new EDIT();} EDITPARAMS {editV->edit();}
	| rcp {cpV = new CP();} CPPARAMS {cpV->cp();}
	| rpause {system("read -p 'Presione Enter para continuar...' var");}
	| rfind {findV = new FIND();} FINDPARAMS {findV->find();}
	| rlogin {usersV = new USERS();} LOGINPARAMS {usersV->login();}
	| rlogout {usersV = new USERS(); usersV->logout();}
	| rmkgrp {usersV = new USERS();} MKGRPPARAMS {usersV->mkgrp();}
	| rrmgrp {usersV = new USERS();} RMGRPPARAMS {usersV->rmgrp();}
	| rrmusr {usersV = new USERS();} RMUSERPARAMS {usersV->rmusr();}
	| rmkusr {usersV = new USERS();} MKUSERPARAMS {usersV->mkusr();}
	| rchgrp {usersV = new USERS();} CHGRPPARAMS {usersV->chgrp();}
	| rloss {simulateV = new SIMULATE();} SIMULATEPARAM {simulateV->loss();}
	| rrecovery {simulateV = new SIMULATE();} SIMULATEPARAM {simulateV->recovery();}
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
	| guion rtype igual rp {fdiskV->setType($4);}
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
        | guion retruta igual ruta {repV->setRuta(false, $4);}
        | guion retruta igual cadena {repV->setRuta(true, $4);}
        | guion root igual numero {repV->setRoot(atoi($4));}
;

MKFSPARAMS:
	MKFSPARAMS MKFSPARAM
	| MKFSPARAM
;

MKFSPARAM:
	guion rid igual id_particion {mkfsV->set_id($4);}
	| guion rtype igual id {mkfsV->set_type($4);}
	| guion rfs igual numero rfs {mkfsV->set_fs(atoi($4));}
;

TOUCHPARAMS:
	TOUCHPARAMS TOUCHPARAM
	| TOUCHPARAM
;

TOUCHPARAM:
	guion path igual ruta {touchV->set_path(false, $4);}
	| guion path igual cadena {touchV->set_path(true, $4);}
	| guion rr {touchV->set_r(true);}
	| guion size igual numero {touchV->set_size(atoi($4));}
	| guion rcont igual ruta {touchV->set_cont(false, $4);}
       	| guion rcont igual cadena {touchV->set_cont(true, $4);}
	| guion rstdin {touchV->set_stdin(true);}
;

MKDIRPARAMS:
	MKDIRPARAMS MKDIRPARAM
	| MKDIRPARAM
;

MKDIRPARAM:
	guion path igual ruta {mkdirV->set_path(false, $4);}
        | guion path igual cadena {mkdirV->set_path(true, $4);}
        | guion rp {mkdirV->set_p(true);}
;

EXECPARAM:
	guion path igual ruta {execV->set_path(false, $4);}
        | guion path igual cadena {execV->set_path(true, $4);}
;

CATPARAMS:
	CATPARAMS CATPARAM
	| CATPARAM
;

CATPARAM:
	guion id igual ruta {catV->add_file(false, $4);}
	| guion id igual cadena {catV->add_file(true, $4);}
;

RENPARAMS:
	RENPARAMS RENPARAM
	| RENPARAM
;
RENPARAM:
	guion path igual ruta {renV->set_path(false, $4);}
        | guion path igual cadena {renV->set_path(true, $4);}
        | guion rname igual id {renV->set_name($4);}
;

MVPARAMS:
	MVPARAMS MVPARAM
	| MVPARAM
;
MVPARAM:
	guion path igual ruta {moveV->set_path(false, $4);}
        | guion path igual cadena {moveV->set_path(true, $4);}
        | guion rdest igual ruta {moveV->set_dest(false, $4);}
        | guion rdest igual cadena {moveV->set_dest(true, $4);}
;

RMPARAM:
	guion path igual ruta {rmV->set_path(false, $4);}
        | guion path igual cadena {rmV->set_path(true, $4);}
;

EDITPARAMS:
	EDITPARAMS EDITPARAM
	| EDITPARAM
;

EDITPARAM:
	guion path igual ruta {editV->set_path(false, $4);}
	| guion path igual cadena {editV->set_path(true, $4);}
	| guion rcont igual ruta {editV->set_cont(false, $4);}
       	| guion rcont igual cadena {editV->set_cont(true, $4);}
	| guion rstdin {editV->set_stdin(true);}
;

CPPARAMS:
	CPPARAMS CPPARAM
	| CPPARAM
;
CPPARAM:
	guion path igual ruta {cpV->set_path(false, $4);}
        | guion path igual cadena {cpV->set_path(true, $4);}
        | guion rdest igual ruta {cpV->set_dest(false, $4);}
        | guion rdest igual cadena {cpV->set_dest(true, $4);}
;

FINDPARAMS:
	FINDPARAMS FINDPARAM
	| FINDPARAM
;

FINDPARAM:
	guion path igual ruta {findV->set_path(false, $4);}
	| guion path igual cadena {findV->set_path(true, $4);}
	| guion rname igual cadena {findV->set_name($4);}
;

LOGINPARAMS:
	LOGINPARAMS LOGINPARAM
	| LOGINPARAM
;

LOGINPARAM:
	guion ruser igual id {usersV->set_user($4);}
	| guion ruser igual root {usersV->set_user($4);}
	| guion rpwd igual id {usersV->set_pwd($4, "");}
	| guion rpwd igual numero id {usersV->set_pwd($4, $5);}
	| guion rpwd igual numero {usersV->set_pwd($4, "");}
	| guion rid igual id_particion {usersV->set_id($4);}
;

MKGRPPARAMS:
	MKGRPPARAMS MKGRPPARAM
	| MKGRPPARAM
;

MKGRPPARAM:
	guion rname igual id {usersV->set_name(false, $4);}
        | guion rname igual cadena {usersV->set_name(true, $4);}
;

RMGRPPARAMS:
	RMGRPPARAMS RMGRPPARAM
	| RMGRPPARAM
;

RMGRPPARAM:
	guion rname igual id {usersV->set_name(false, $4);}
        | guion rname igual cadena {usersV->set_name(true, $4);}
;

MKUSERPARAMS:
	MKUSERPARAMS MKUSERPARAM
	| MKUSERPARAM
;

MKUSERPARAM:
	guion rgrp igual id {usersV->set_name(false, $4);}
        | guion rgrp igual cadena {usersV->set_name(true, $4);}
        | guion rusr igual id {usersV->set_user($4);}
        | guion rusr igual root {usersV->set_user($4);}
        | guion rpwd igual id {usersV->set_pwd($4, "");}
        | guion rpwd igual numero id {usersV->set_pwd($4, $5);}
        | guion rpwd igual numero {usersV->set_pwd($4, "");}
;

RMUSERPARAMS:
	RMUSERPARAMS RMUSERPARAM
	| RMUSERPARAM
;

RMUSERPARAM:
	guion rusr igual id {usersV->set_name(false, $4);}
        | guion rusr igual cadena {usersV->set_name(true, $4);}
;

SIMULATEPARAM:
	guion rid igual id_particion {simulateV->set_id($4);}
;

CHGRPPARAMS:
	CHGRPPARAMS CHGRPPARAM
	| CHGRPPARAM
;

CHGRPPARAM:
	guion rgrp igual id {usersV->set_name(false, $4);}
        | guion rgrp igual cadena {usersV->set_name(true, $4);}
        | guion rusr igual id {usersV->set_user($4);}
        | guion rusr igual root {usersV->set_user($4);}
;
%%
void yyerror(const char *s)
{
    printf("Error sintactico en la linea %i: %s\n", 0, s);
}