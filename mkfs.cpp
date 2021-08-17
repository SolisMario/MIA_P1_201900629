//
// Created by mario on 13/08/21.
//

#include <cmath>
#include "mkfs.h"
#include <cstring>

void MKFS::set_type(string type_set) {
    this->type = type_set;
}

void MKFS::set_id(string id_set) {
    this->id = id_set;
}

void MKFS::set_fs(string fs_set) {
    this->fs = fs_set;
}

void MKFS::mkfs() {
    if (this->id.empty()) {
        cout << "Error: el parametro id es obligatorio." << endl;
        return;
    }

    //se recupera la posicion del dico y particion, se verifica si estan montados
    string disk_number = this->id.substr(2, this->id.length() - 3);
    int disk_pos = stoi(disk_number) - 1;
    if (disk_pos < 0 || disk_pos > 98 || discos_montados[disk_pos].estado == 0) {
        cout << "ERROR: El disco indicado no esta montado." << endl;
        return;
    }
    char part_letter = this->id.at(this->id.length() - 1);
    int part_pos = int(part_letter - 65);
    if (part_pos < 0 || part_pos > 25 || discos_montados[disk_pos].particiones[part_pos].estado == 0) {
        cout << "ERROR: La particion indicada no esta montada." << endl;
        return;
    }

    if (discos_montados[disk_pos].estado == 0 || discos_montados[disk_pos].particiones[part_pos].estado == 0) {
        cout << "ERROR: el disco indicado no esta montado." << endl;
        return;
    }

    //se verifica que el disco exista
    if (!verificar_disco(discos_montados[disk_pos].path)) return;

    FILE *recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    MBR mbr_leido;
    fread(&mbr_leido, sizeof(MBR), 1, recuperar);
    fclose(recuperar);
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    int n = 0;

    for (auto &particione : mbr_leido.particiones) {
        if (strcmp(nombre_particion, particione.part_name) == 0 && particione.part_status == 'a') {
            //se obtiene el numero de inodos
            n = get_n(particione.part_size);

            //se llena el superbloque
            super_bloque SB;
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, particione.part_start, SEEK_SET);
            fread(&SB, sizeof(EBR), 1, recuperar);

            if (this->fs == "2fs") {
                SB.s_filesystem_type = 2;
            } else {
                SB.s_filesystem_type = 3;
            }
            SB.s_inodes_count = n;
            SB.s_blocks_count = 3 * n;
            SB.s_free_blocks_count = 3 * n - 2;
            SB.s_free_inodes_count = n - 2;
            SB.s_inode_size = sizeof(tabla_inodos);
            SB.s_block_size = sizeof(bloque_archivos);
            SB.s_firts_ino = 2;
            SB.s_first_blo = 2;
            if (this->fs == "2fs") {
                SB.s_bm_inode_start = particione.part_start + sizeof(super_bloque);
                SB.s_bm_block_start = particione.part_start + sizeof(super_bloque) + n;
                SB.s_inode_start = particione.part_start + sizeof(super_bloque) + n + n * 3;
                SB.s_block_start = particione.part_start + sizeof(super_bloque) + n + n * 3 + n * sizeof(tabla_inodos);
            } else {
                SB.s_bm_inode_start = particione.part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos);
                SB.s_bm_block_start = particione.part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos) + n;
                SB.s_inode_start =
                        particione.part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos) + n + n * 3;
                SB.s_block_start =
                        particione.part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos) + n + n * 3 +
                        n * sizeof(tabla_inodos);
            }
            cout << " espacio libre: " << (particione.part_start + particione.part_size) -
                                          (SB.s_block_start + n * 3 * sizeof(bloque_archivos)) << endl;
            cout << "tam " << n* sizeof(tabla_inodos);
            cout << "tam " << n* sizeof(bloque_archivos) * 3;


        } else if (particione.part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, particione.part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    n = get_n(ebr_actual.part_size);
                }
                if (ebr_actual.part_next == -1) break;
                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
            }
        }
    }

    if (n == 0) {
        cout << "No se puede crear el sistema de archivos. La particion no se encuentra activa" << endl;
        return;
    }

    cout << "El numero de estructuras es: " << n << endl;
}

bool MKFS::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == NULL) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

int MKFS::get_n(int tam_particion) {
    float numerador;
    float denominador;
    float n = 0;
    if (this->fs == "2fs") {
        numerador = tam_particion - sizeof(super_bloque);
        denominador = 4 + sizeof(tabla_inodos) + (3 * sizeof(bloque_archivos));
    } else {
        numerador = tam_particion - sizeof(super_bloque) - 100 * sizeof(bloque_archivos);
        denominador = 4 + sizeof(tabla_inodos) + 3 * sizeof(bloque_archivos);
    }

    n = numerador / denominador;

    return floor(n);
}

EBR MKFS::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}