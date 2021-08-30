//
// Created by mario on 29/08/21.
//

#ifndef MIA_P1_201900629_REN_H
#define MIA_P1_201900629_REN_H


#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class REN {
private:
    string path;
    string name;
public:
    void set_path(bool comillas, string path);

    void set_name(string name);

    void ren();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(char const *path);

    int get_inodo(string nombre_buscado, string new_name, tabla_inodos inodo_carpeta, char const *path, int part_start, int operacion);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, string new_name, char const *path, int part_start, int operacion);

    EBR leer_ebr(char const *sc, int seek);

};


#endif //MIA_P1_201900629_REN_H
