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
} particion_montada;

typedef struct {
    char path[100];
    int estado;
    int numero;
    particion_montada particiones[99];
} disco_montado;

typedef struct {
    int s_filesystem_type = -1;
    int s_inodes_count = -1;
    int s_blocks_count = -1;
    int s_free_blocks_count = -1;
    int s_free_inodes_count = -1;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count = 0;
    int s_magic = 0xEF53;
    int s_inode_size = -1;
    int s_block_size = -1;
    int s_first_ino = -1;
    int s_first_blo = -1;
    int s_bm_inode_start = -1;
    int s_bm_block_start = -1;
    int s_inode_start = -1;
    int s_block_start = -1;
} super_bloque;

typedef struct {
    int i_uid = 1;
    int i_gid = 1;
    int i_size = 1;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    char i_type = '0';
    int i_perm = 664;
} tabla_inodos;

typedef struct {
    char b_name[12];
    int b_inodo = -1;
} content;

typedef struct {
    content b_content[4];
} bloque_carpeta;

typedef struct {
    char b_content[64];
} bloque_archivos;

typedef struct {
    int b_pointers[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
} bloque_apuntadores;

typedef struct
{
    char tipo_operacion[10] = "";
    char tipo = '2';
    char path[40] = "";
    char contenido[100] = ""; // Validar cuando espacio necesitan.
    time_t log_fecha;
    int size = -1;
}journal;

extern disco_montado discos_montados[26];
#endif //MIA_P1_201900629_AUX_STRUCTS_H
