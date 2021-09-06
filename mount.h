//
// Created by mario on 10/08/21.
//

#ifndef MIA_P1_201900629_MOUNT_H
#define MIA_P1_201900629_MOUNT_H

using namespace std;
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

class MOUNT {
private:
    string path;
    string name;
    string id;
public:
    void setPath(bool comillas, const string &path_set);
    void setName(bool comillas, const string &name_set);
    void setID(bool comillas, string ID);
    void mount();
    void umount();
    bool verificar_disco(char const *path);
    EBR leer_ebr(char const *path, int seek);
    void mostrar_montadas();
};

#endif //MIA_P1_201900629_MOUNT_H
