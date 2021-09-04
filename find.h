//
// Created by mario on 31/08/21.
//

#ifndef MIA_P1_201900629_FIND_H
#define MIA_P1_201900629_FIND_H

#include <iostream>
#include <cstring>
#include <list>
#include <regex>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class FIND{
private:
    string path;
    string name;
public:
    void set_path(bool comillas, string path);

    void set_name(string name);

    void find();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(string path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start, int tipo);

    EBR leer_ebr(char const *sc, int seek);

    string recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int part_start, bool coincidencia_precedente, string ruta_precede, string regex, string name);

    string recorrer_carpeta(int indice_bloque, int disk_pos, int inode_start, int block_start, int part_start, string ruta_precede, string regex);

    string recorrer_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo, int part_start, string ruta_precede, string regex);

    int bitmap_libre(int start, int final, char const *path);

    string formar_regex(string name);

    bool verificar_match(string nombre, string regex);
};

#endif //MIA_P1_201900629_FIND_H
