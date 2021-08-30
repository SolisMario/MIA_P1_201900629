//
// Created by mario on 29/08/21.
//

#ifndef MIA_P1_201900629_MV_H
#define MIA_P1_201900629_MV_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class MV{
private:
    string path;
    string dest;
public:
    void set_path(bool comillas, string path);

    void set_dest(bool comillas, string dest);

    void mv();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(char const *path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int operacion);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string new_name, char const *path, int part_start, int operacion);

    EBR leer_ebr(char const *sc, int seek);

    int bitmap_libre(int start, int final, char const *path);

    void cambiar_referencia(int inodo_destino, int bloque_origen, string nombre_origen, int disk_pos, int part_start);

    int crear_inodo_indirecto(int nivel, int apuntador_ind, string nombre_origen, int disk_pos, int part_start, tabla_inodos carpeta_actual, int b_inodo);

};

#endif //MIA_P1_201900629_MV_H
