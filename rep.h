//
// Created by mario on 11/08/21.
//

#ifndef MIA_P1_201900629_REP_H
#define MIA_P1_201900629_REP_H

#include <iostream>
#include "aux_structs.h"
#include "mbr.h"
#include "ebr.h"

using namespace std;

class REP {
private:
    string name;
    string path;
    string id;
    string ruta;
    int root;
public:
    void setName(string name);

    void setPath(bool comillas, string path);

    void setID(string id);

    void setRuta(string ruta);

    void setRoot(int indice);

    void rep();

    void graficar_MBR();

    bool verificar_disco(char const *path);

    void archivo_dot(string content, string name);

    string toLowerCase(string sl);

    EBR leer_ebr(char const *path, int seek);

    void graficar_disk();

    string nombre_disco(const char* path);

};

#endif //MIA_P1_201900629_REP_H
