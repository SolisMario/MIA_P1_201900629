//
// Created by mario on 26/08/21.
//

#ifndef MIA_P1_201900629_MKDIR_H
#define MIA_P1_201900629_MKDIR_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class MKDIR {
private:
    string path;
    bool p = false;
public:
    void set_path(bool comillas, string path);

    void set_p(bool p);

    void mkdir();

    bool verificar_disco(char const *path);

    EBR leer_ebr(char const *sc, int seek);

    list<string> separar_carpetas(string path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo);

    int crear_inodo(string nombre_carpeta, int inodo_padre, int inodo_abuelo, tabla_inodos carpeta_actual,
                    char const *path, int part_start, char tipo);

    int bitmap_libre(int start, int final, char const *path);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
                            int tipo);

    void crear_inodo_carpeta(int inode_start, int block_start, int bm_inode, int bm_block, int inodo_actual, int inodo_padre,
                        int bloque_libre, char const *path);

    int crear_inodo_indirecto(int nivel, int apuntador_ind, string nombre_carpeta, char const *path, int part_start,
                              int tipo, int inodo_padre, tabla_inodos carpeta_actual);

    string nombre_carpeta(const char *path);
};

#endif //MIA_P1_201900629_MKDIR_H
