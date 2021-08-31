//
// Created by mario on 30/08/21.
//

#ifndef MIA_P1_201900629_EDIT_H
#define MIA_P1_201900629_EDIT_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class EDIT {
private:
    string path;
    string cont;
    bool stdin_ = false;
public:
    void set_path(bool comillas, string path);

    void set_stdin(bool stdin_);

    void set_cont(bool comillas, string cont);

    void edit();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(string path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start, int tipo);

    EBR leer_ebr(char const *sc, int seek);

    int recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    int recorrer_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo, int bm_inode, int bm_block, int part_start);

    void escribir_archivo(const char *path, int part_start, int inodo_archivo);

    string escribir_indirecto(int nivel, string contenido, char const *path, int part_start);

    int bitmap_libre(int start, int final, char const *path);

};

#endif //MIA_P1_201900629_EDIT_H
