//
// Created by mario on 31/08/21.
//

#ifndef MIA_P1_201900629_CP_H
#define MIA_P1_201900629_CP_H

#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class CP{
private:
    string path;
    string dest;
public:
    void set_path(bool comillas, string path);

    void set_dest(bool comillas, string dest);

    void cp();

    bool verificar_disco(char const *path);

    list<string> separar_carpetas(string path);

    string nombre_archivo(char const *path);

    int get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int operacion);

    int get_inodo_indirecto(int nivel, int apuntador_ind, string new_name, char const *path, int part_start, int operacion);

    EBR leer_ebr(char const *sc, int seek);

    int bitmap_libre(int start, int final, char const *path);

    int conseguir_bloque_carpeta(int inodo_destino, int bloque_origen, string nombre_origen, int disk_pos, int part_start);

    int conseguir_bloque_carpeta_ind(int nivel, int apuntador_ind, string nombre_origen, int disk_pos, int part_start, tabla_inodos carpeta_actual, int b_inodo);

    int recorrer_inodo(int disk_pos, int part_start, int indice_inodo, string nombre_origen, int indo_padre);

    int recorrer_bloque_carpeta(int disk_pos, int part_start, int indice_inodo, string nombre_origen, int inodo_padre, int inodo_abuelo);

    int recorrer_bloque_archivo(int disk_pos, int part_start, int indice_inodo, string nombre_origen);

    void iniciar_copiado(int bloque_origen, int disk_pos, int part_start, int inodo_origen, string nombre_origen, int inodo_destino);

    int recorrer_bloque_apuntadores(int disk_pos, int part_start, int indice_bloque, string nombre_origen, int inodo_padre, int inodo_abuelo, char tipo, int nivel);

    void add_to_journal(char const *path, int partStart);

    int posicion_journal(char const *path, int partStart);
};

#endif //MIA_P1_201900629_CP_H
