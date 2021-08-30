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

    string contenido_archivo(int indice_inodo, int disk_pos, int inode_start, int block_start);

    string contenido_bloque(int indice_bloque, int disk_pos, int inode_start, int block_start);

    string contenido_indirectos(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel);
};

#endif //MIA_P1_201900629_RM_H
