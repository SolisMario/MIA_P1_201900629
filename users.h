//
// Created by mario on 1/09/21.
//

#ifndef MIA_P1_201900629_USERS_H
#define MIA_P1_201900629_USERS_H

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <cstring>
#include <list>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class USERS{
private:
    string user;
    string password;
    string id;
    string name;
    string grp;
public:
    void set_user(string user);

    void set_pwd(string parte1, string parte2);

    void set_id(string id);

    void set_name(bool comillas, string name);

    void login();

    void logout();

    void mkgrp();

    void rmgrp();

    void mkusr();

    void rmusr();

    vector<string> get_parts(string line);

    list<string> get_lines_txt(string txt);

    string get_txt(int part_start, char const *path, int inode_start, int block_start);

    list<string> get_grps_usrs(int cant_coma, list<string> lines);//cantidad de comas dira si es grupo o usuario

    int get_part_start(string logged_partition, int disk_pos, int part_pos);

    bool verificar_disco(char const *path);

    EBR leer_ebr(char const *sc, int seek);

    int get_disk_pos(string id);

    int get_part_pos(string id, int disk_pos);

    void recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start);

    void escribir_archivo(const char *path, int part_start, int inodo_archivo, string contenido);

    int bitmap_libre(int start, int final, char const *path);
};

#endif //MIA_P1_201900629_USERS_H
