//
// Created by mario on 2/09/21.
//

#include "simulate.h"


void SIMULATE::set_id(string id) {
    this->id = id;
}

void SIMULATE::loss() {
    if (this->id.empty()) {
        cout << "El parametro id es obligatorio." << endl;
        return;
    }

    string logged_partition = this->id;

    //se recupera la posicion del dico y particion, se verifica si estan montados
    string disk_number = logged_partition.substr(2, logged_partition.length() - 3);
    int disk_pos = stoi(disk_number) - 1;
    if (disk_pos < 0 || disk_pos > 98 || discos_montados[disk_pos].estado == 0) {
        cout << "ERROR: El disco indicado no esta montado." << endl;
        return;
    }

    char part_letter = logged_partition.at(logged_partition.length() - 1);
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

    int part_start = -1;

    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start != -1) {
        //se encontro la particion
        super_bloque superBloque;
        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(recuperar, part_start, SEEK_SET);
        fread(&superBloque, sizeof(super_bloque), 1, recuperar);
        fclose(recuperar);

        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        for (int i = superBloque.s_bm_block_start; i < superBloque.s_bm_block_start + superBloque.s_blocks_count; ++i) {
            fseek(recuperar, i, SEEK_SET);
            fwrite("\0", 1, 1, recuperar);
        }
        fclose(recuperar);

        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        for (int i = superBloque.s_bm_inode_start; i < superBloque.s_bm_inode_start + superBloque.s_inodes_count; ++i) {
            fseek(recuperar, i, SEEK_SET);
            fwrite("\0", 1, 1, recuperar);
        }
        fclose(recuperar);
    }
}

void SIMULATE::recovery() {
    if (this->id.empty()) {
        cout << "El parametro id es obligatorio." << endl;
        return;
    }

    string logged_partition = usuario_loggeado.particion_loggeada;

    //se recupera la posicion del dico y particion, se verifica si estan montados
    string disk_number = logged_partition.substr(2, logged_partition.length() - 3);
    int disk_pos = stoi(disk_number) - 1;
    if (disk_pos < 0 || disk_pos > 98 || discos_montados[disk_pos].estado == 0) {
        cout << "ERROR: El disco indicado no esta montado." << endl;
        return;
    }

    char part_letter = logged_partition.at(logged_partition.length() - 1);
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

    int part_start = -1;

    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
        mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start != -1) {
        //se encontro la particion
        super_bloque superBloque;
        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(recuperar, part_start, SEEK_SET);
        fread(&superBloque, sizeof(super_bloque), 1, recuperar);
        fclose(recuperar);

        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        for (int i = superBloque.s_bm_block_start; i < superBloque.s_bm_block_start + superBloque.s_blocks_count; ++i) {
            fseek(recuperar, i, SEEK_SET);
            fwrite("0", 1, 1, recuperar);
        }
        fclose(recuperar);

        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        for (int i = superBloque.s_bm_inode_start; i < superBloque.s_bm_inode_start + superBloque.s_inodes_count; ++i) {
            fseek(recuperar, i, SEEK_SET);
            fwrite("0", 1, 1, recuperar);
        }
        fclose(recuperar);

        recorrer_inodo(0, disk_pos, superBloque.s_inode_start, superBloque.s_block_start, superBloque.s_bm_inode_start, superBloque.s_bm_block_start,part_start);
    }
}

int SIMULATE::recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start) {
    FILE *file;
    tabla_inodos inodo;// se recupera el inodo
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    int resultado = -1;

    if (inodo.i_type == '1') {
        file = fopen(discos_montados[disk_pos].path, "rb+");
        for (int i = 0; i < 12; ++i) {
            if (inodo.i_block[i] != -1) {
                fseek(file, bm_block + inodo.i_block[i], SEEK_SET);
                fwrite("1", 1, 1, file);
            }
        }
        fclose(file);

        if (inodo.i_block[12] != -1) {
            recorrer_bloque_apuntadores(inodo.i_block[12], disk_pos, inode_start, block_start, 1, '1', bm_inode, bm_block, part_start);
        }

        if (inodo.i_block[13] != -1) {
            recorrer_bloque_apuntadores(inodo.i_block[13], disk_pos, inode_start, block_start, 2, '1', bm_inode, bm_block, part_start);
        }

        if (inodo.i_block[14] != -1) {
            recorrer_bloque_apuntadores(inodo.i_block[14], disk_pos, inode_start, block_start, 3, '1', bm_inode, bm_block, part_start);
        }
    } else {
        for (int i = 0; i < 12; ++i) {
            if (inodo.i_block[i] != -1) {
                resultado = recorrer_bloque_carpeta(inodo.i_block[i], disk_pos, inode_start, block_start, bm_inode, bm_block, part_start);
                if(resultado != 0) {
                    return  -1;
                }
            }
        }

        if (inodo.i_block[12] != -1) {
            resultado = recorrer_bloque_apuntadores(inodo.i_block[12], disk_pos, inode_start, block_start, 1, '0', bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return  -1;
            }
        }

        if (inodo.i_block[13] != -1) {
            resultado = recorrer_bloque_apuntadores(inodo.i_block[13], disk_pos, inode_start, block_start, 2, '0', bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return  -1;
            }
        }

        if (inodo.i_block[14] != -1) {
            resultado  = recorrer_bloque_apuntadores(inodo.i_block[14], disk_pos, inode_start, block_start, 3, '0', bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return  -1;
            }
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, bm_inode + indice_inodo, SEEK_SET);
    fwrite("1", 1, 1, file);
    fclose(file);

    return 0;
}

int SIMULATE::recorrer_bloque_carpeta(int indice_bloque, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start) {
    bloque_carpeta bloque;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, block_start + sizeof(bloque_carpeta) * indice_bloque, SEEK_SET);
    fread(&bloque, sizeof(bloque_archivos), 1, file);
    fclose(file);
    int resultado = -1;

    for (int i = 0; i < 4; ++i) {
        if(bloque.b_content[i].b_inodo != -1 && strcmp(bloque.b_content[i].b_name, ".") != 0 && strcmp(bloque.b_content[i].b_name, "..") != 0){
            resultado = recorrer_inodo(bloque.b_content[i].b_inodo, disk_pos, inode_start, block_start, bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return -1;
            }
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, bm_block + indice_bloque, SEEK_SET);
    fwrite("1", 1, 1, file);
    fclose(file);

    return 0;
}

int SIMULATE::recorrer_bloque_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo, int bm_inode, int bm_block, int part_start) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    int resultado = -1;

    if(tipo == '1') {
        for (int i = 0; i < 16; ++i) {
            if(nivel == 1) {
                if (apuntadores.b_pointers[i] != -1) {
                    file = fopen(discos_montados[disk_pos].path, "rb+");
                    fseek(file, bm_block + apuntadores.b_pointers[i], SEEK_SET);
                    fwrite("1", 1, 1, file);
                    fclose(file);
                }
            } else {
                recorrer_bloque_apuntadores(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, nivel - 1,tipo, bm_inode, bm_block, part_start);
            }
        }
    } else {
        for (int i = 0; i < 16; ++i) {
            if(nivel == 1) {
                if (apuntadores.b_pointers[i] != -1) {
                    recorrer_bloque_carpeta(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, bm_inode, bm_block, part_start);
                }
            } else {
                resultado = recorrer_bloque_apuntadores(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, nivel - 1, tipo, bm_inode, bm_block, part_start);
                if(resultado != 0) {
                    return -1;
                }
            }
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, bm_block + indice_bloque, SEEK_SET);
    fwrite("1", 1, 1, file);
    fclose(file);

    return 0;
}

int SIMULATE::bitmap_libre(int start, int final, char const *path) {
    FILE *file = fopen(path, "rb+");
    int libre = -1;
    char bitmap_leido;
    for (int i = start; i < final; i++) {
        fseek(file, i, SEEK_SET);
        fread(&bitmap_leido, sizeof(char), 1, file);
        if (bitmap_leido == '0') {
            libre = i - start;
            fclose(file);
            return libre;
        }
    }
    fclose(file);
    return libre;
}

list<string> SIMULATE::separar_carpetas(string path) {
    if (path[0] == '/') {
        path = path.substr(1, path.length());
    }
    list<string> lista_carpetas;
    char ruta[path.length() + 1];
    strcpy(ruta, path.c_str());
    string nombre_carpeta;
    for (int i = 0; i < path.length() + 1; i++) {
        if (ruta[i] == '/') {
            lista_carpetas.push_back(nombre_carpeta);
            nombre_carpeta.clear();
            continue;
        }
        nombre_carpeta += ruta[i];
    }
    return lista_carpetas;
}

bool SIMULATE::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR SIMULATE::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

string SIMULATE::nombre_archivo(const char *path) {
    string nombre_char = path;
    string nombre_disco;
    for (int i = nombre_char.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}