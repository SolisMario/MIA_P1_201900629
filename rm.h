//
// Created by mario on 30/08/21.
//

#ifndef MIA_P1_201900629_RM_H
#define MIA_P1_201900629_RM_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class RM {
private:
    string path;
public:
    void set_path(bool comillas, string path);

    void rm();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(string path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start, int tipo);

    EBR leer_ebr(char const *sc, int seek);

    int iniciar_borrado(int bloque_contiene, int disk_pos, string nombre, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    int recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    int recorrer_carpeta(int indice_bloque, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    int recorrer_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo, int bm_inode, int bm_block, int part_start);

    int bitmap_libre(int start, int final, char const *path);

    void add_to_journal(char const *path, int partStart);

    int posicion_journal(char const *path, int partStart);
};

#endif //MIA_P1_201900629_RM_H
