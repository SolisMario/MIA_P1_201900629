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
    string fs = "2fs";
public:
    void set_type(string type_set);

    void set_id(string id_set);

    void set_fs(string fs_set);

    void mkfs();

    bool verificar_disco(char const *path);

    int get_n(int tam_particion);

    EBR leer_ebr(char const *sc, int seek);

    partition_ get_particion(char const *nombre);

};

#endif //MIA_P1_201900629_MKFS_H
