//
// Created by mario on 22/08/21.
//

#ifndef MIA_P1_201900629_TOUCH_H
#define MIA_P1_201900629_TOUCH_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class TOUCH {
private:
    string path;
    bool r = false;
    int size = 0;
    string cont;
    bool stdin_;
public:
    void set_path(bool comillas, string path);

    void set_r(bool r);

    void set_size(int size);

    void set_cont(bool comillas, string cont);

    void set_stdin(bool stdin_);

    void touch();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(const char *path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo);

    int crear_inodo(string nombre_carpeta, int inodo_padre, int inodo_abuelo, tabla_inodos carpeta_actual,
                     char const *path, int part_start, char tipo);

    int bitmap_libre(int start, int final, char const *path);

    void create_file_inode(int inode_start, int bm_inode, int inodo_actual, char const *path);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
                            int tipo);

    void
    crear_inodo_carpeta(int inode_start, int block_start, int bm_inode, int bm_block, int inodo_actual, int inodo_padre,
                        int bloque_libre, char const *path);

    int crear_inodo_indirecto(int nivel, int apuntador_ind, string nombre_carpeta, char const *path, int part_start,
                              int tipo, int inodo_padre, tabla_inodos carpeta_actual);

    void escribir_archivo(const char *path, int part_start, int inodo_archivo);

    string escribir_indirecto(int nivel, string contenido, char const *path, int part_start);
};

#endif //MIA_P1_201900629_TOUCH_H
