//
// Created by mario on 11/08/21.
//

#ifndef MIA_P1_201900629_REP_H
#define MIA_P1_201900629_REP_H

#include <iostream>
#include <cstring>
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

    void graficar_tree();

    string graficar_inodo(int indice_inodo, int inode_start, int block_start, char const *path);

    string graficar_bloque_carpeta(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path);

    string graficar_bloque_archivo(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path);

    string
    graficar_bloque_apuntadores(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path,
                                char tipo, int nivel);

    string recorrer_inodo(int indice_inodo, int inode_start, int block_start, char const *path, char const *name);

    string recorrer_bloque_carpeta(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path);

    void graficar_tabla_inodos();

    void graficar_super();

    string recorrer_apuntadores_inodos(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path, int nivel);

    string graficar_inodo_bloque(int indice_inodo, int inode_start, int block_start, char const *path);

    string graficar_bloque_carpeta_bloque(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path);

    string graficar_bloque_archivo_bloque(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path);

    string graficar_bloque_apuntadores_bloque(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path, char tipo, int nivel);

    void graficar_bloque();

};

#endif //MIA_P1_201900629_REP_H
