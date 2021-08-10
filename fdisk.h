//
// Created by mario on 2/08/21.
//

#ifndef MIA_P1_201900629_FDISK_H
#define MIA_P1_201900629_FDISK_H

#include <iostream>
#include "mbr.h"
#include "ebr.h"

using namespace std;

class FDISK {
private:
    int size = 0;
    string unit = "k";
    string path;
    string type = "p";
    string fit = "wf";
    string delete_;
    string name;
    int add;
public:
    void setSize(int size_set);

    void setUnit(string unit_set);

    void setPath(bool comillas, const string &path_set);

    void setType(const string &type);

    void setFit(string fit_set);

    void setDelete(string delete_type);

    void setName(bool comillas, const string &name_set);

    void setAdd(int add);

    void fdisk();

    MBR leer_MBR(char const *path);

    int set_start(MBR &mbr);

    void mostrar_particiones(MBR &mbr);

    void mover_vacias(MBR &mbr);

    void ordenar_particiones(MBR &mbr);

    void delete_partition(string name, string path, string delete_type);

    void create_logic_partition(MBR &mbr_leido, partition_ &part_extendida);

    EBR leer_ebr(char const *path, int seek);

    void add_size(int size_change);

    void escribir_MBR(MBR &mbr);

    void escribir_EBR(EBR &ebr);

    string toLowerCase(string str);
};

#endif //MIA_P1_201900629_FDISK_H
