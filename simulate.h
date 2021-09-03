//
// Created by mario on 2/09/21.
//

#ifndef MIA_P1_201900629_SIMULATE_H
#define MIA_P1_201900629_SIMULATE_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class SIMULATE{
private:
    string id;
public:
    void set_id(string id);

    void loss();

    void recovery();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(char const *path);

    EBR leer_ebr(char const *sc, int seek);

    int bitmap_libre(int start, int final, char const *path);

    int recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    int recorrer_bloque_carpeta(int indice_bloque, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    void iniciar_copiado(int bloque_origen, int disk_pos, int part_start, int inodo_origen, string nombre_origen, int inodo_destino);

    int recorrer_bloque_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo, int bm_inode, int bm_block, int part_start);
};

#endif //MIA_P1_201900629_SIMULATE_H
