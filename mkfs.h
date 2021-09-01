//
// Created by mario on 13/08/21.
//

#ifndef MIA_P1_201900629_MKFS_H
#define MIA_P1_201900629_MKFS_H

#include <iostream>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"
using namespace std;

class MKFS {
private:
    string type;
    string id;
    int fs = 2;
public:
    void set_type(string type_set);

    void set_id(string id_set);

    void set_fs(int fs_set);

    void mkfs();

    bool verificar_disco(char const *path);

    int get_n(int tam_particion);

    EBR leer_ebr(char const *sc, int seek);

    super_bloque editar_super(int n, super_bloque &SB, int part_start);

    void llenar_journal(int part_start, FILE* file);

};

#endif //MIA_P1_201900629_MKFS_H
