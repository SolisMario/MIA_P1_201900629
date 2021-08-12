//
// Created by mario on 10/08/21.
//

#ifndef MIA_P1_201900629_AUX_STRUCTS_H
#define MIA_P1_201900629_AUX_STRUCTS_H

#include <iostream>
using namespace std;

typedef struct {
    char letra;
    int estado;
    char nombre[16];
}particion_montada;

typedef struct {
    char path[100];
    int estado;
    int numero;
    particion_montada particiones[99];
}disco_montado;

typedef struct {
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic = 0xEF53;
    int s_inode_size;
    int s_block_size;
    int s_firts_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}super_bloque;

typedef struct {
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
}tabla_inodos;

typedef struct  {
    char b_name[12];
    int b_inodo;
}content;

typedef struct {
    content b_content[4];
}bloque_carpeta;

typedef struct {
    char b_content[64];
}bloque_archivos;

typedef struct {
    int b_pointers[16];
}bloque_apuntadores;

extern disco_montado discos_montados[26];
#endif //MIA_P1_201900629_AUX_STRUCTS_H
