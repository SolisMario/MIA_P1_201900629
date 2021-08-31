//
// Created by mario on 30/08/21.
//
#include <fstream>
#include "rm.h"

void RM::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void RM::rm() {
    if (this->path.empty()) {
        cout << "No se ingreso ningun archivo para mostrar." << endl;
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
            int posicion_eliminada = 0;
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
                posicion_eliminada++;
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

        tabla_inodos carpeta_tmp;
        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(recuperar, superBloque.s_inode_start, SEEK_SET);
        fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
        fclose(recuperar);

        int inodo_carpeta = -1;
        list<string> carpetas = separar_carpetas(this->path);
        list<string>::iterator it;

        //navegar entre las carpetas
        for (it = carpetas.begin(); it != carpetas.end(); it++) {
            inodo_carpeta = get_inodo(*it, carpeta_tmp, discos_montados[disk_pos].path, part_start, '0');
            if (inodo_carpeta == -1) { //si el inodo carpeta es -1 las carpetas no existen
                cout << "La carpeta " << *it << " no existe " << endl;
                return;
            }
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_carpeta, SEEK_SET);
            fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
            fclose(recuperar);
        }

        string file_name = nombre_archivo(this->path);
        int bloque_carpeta = -1;
        bloque_carpeta = get_inodo(file_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, '1');
        if (bloque_carpeta == -1) {
            cout << "El archivo/carpeta " << file_name << " no existe." << endl;
        } else {
            //iniciar_borrado
            int borrado = iniciar_borrado(bloque_carpeta, disk_pos, file_name, superBloque.s_inode_start, superBloque.s_block_start, superBloque.s_bm_inode_start, superBloque.s_bm_block_start, part_start);
            if(borrado != 0) {
                cout << "ERROR no se tiene permiso de borrado en uno de los archivos/carpetas." << endl;
            }
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, part_start, SEEK_SET);
            fread(&superBloque, sizeof(super_bloque), 1, recuperar);
            fclose(recuperar);

            superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, discos_montados[disk_pos].path);
            superBloque.s_first_ino = bitmap_libre(superBloque.s_bm_inode_start, superBloque.s_bm_inode_start + superBloque.s_inodes_count, discos_montados[disk_pos].path);

            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(super_bloque), 1, recuperar);
            fclose(recuperar);
        }
    }
}

int RM::get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo) {
    //for que itera los apuntadores directos del inodo
    FILE *file;
    for (int i = 0; i < 12; i++) {
        if (inodo_carpeta.i_block[i] != -1) {
            super_bloque superBloque;
            file = fopen(path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fread(&superBloque, sizeof(super_bloque), 1, file);
            fclose(file);
            file = fopen(path, "rb+");
            bloque_carpeta carpeta_tmp;
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo_carpeta.i_block[i], SEEK_SET);
            fread(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
            fclose(file);
            //for que itera los contents del blque carpeta
            for (int j = 0; j < 4; ++j) {
                if (strcmp(nombre_buscado.c_str(), carpeta_tmp.b_content[j].b_name) == 0) {
                    if (tipo == '0') {
                        return carpeta_tmp.b_content[j].b_inodo;
                    } else {
                        return inodo_carpeta.i_block[i];
                    }
                }
            }
        }
    }

    if (inodo_carpeta.i_block[12] != -1) {
        int inodo_indirecto = get_inodo_indirecto(1, inodo_carpeta.i_block[12], nombre_buscado, path, part_start, tipo);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    if (inodo_carpeta.i_block[13] != -1) {
        int inodo_indirecto = get_inodo_indirecto(2, inodo_carpeta.i_block[13], nombre_buscado, path, part_start, tipo);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    if (inodo_carpeta.i_block[13] != -1) {
        int inodo_indirecto = get_inodo_indirecto(3, inodo_carpeta.i_block[14], nombre_buscado, path, part_start, tipo);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    return -1;
}

//metodo para traer el inodo en el que se encuentra el archivo o carpeta
int RM::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
                            int tipo) {
    FILE *file;
    //se trae el superbloque y el inodo indirecto
    super_bloque superBloque;
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    bloque_apuntadores apuntadores;
    file = fopen(path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * apuntador_ind, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                bloque_carpeta carpeta_tmp;
                file = fopen(path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * apuntadores.b_pointers[i],
                      SEEK_SET);
                fread(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
                fclose(file);

                //for que itera los contents del blque carpeta
                for (int j = 0; j < 4; ++j) {
                    if (strcmp(nombre_buscado.c_str(), carpeta_tmp.b_content[j].b_name) == 0) {
                        tabla_inodos inodo_tmp;
                        file = fopen(path, "rb+");
                        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * carpeta_tmp.b_content[j].b_inodo,
                              SEEK_SET);
                        fread(&inodo_tmp, sizeof(tabla_inodos), 1, file);
                        fclose(file);
                        if (inodo_tmp.i_type) {
                            return carpeta_tmp.b_content[j].b_inodo;
                        }
                    }
                }
            } else {
                int inodo = get_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_buscado, path, part_start,
                                                tipo);
                if (inodo != -1) return inodo;
            }
        }
    }
    return -1;
}

int RM::iniciar_borrado(int bloque_contiene, int disk_pos, string nombre, int inode_start, int block_start, int bm_inode, int bm_block, int part_start) {
    bloque_carpeta bloque;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, block_start + sizeof(bloque_carpeta) * bloque_contiene, SEEK_SET);
    fread(&bloque, sizeof(bloque_archivos), 1, file);
    fclose(file);

    for (int j = 0; j < 4; ++j) {
        if (strcmp(nombre.c_str(), bloque.b_content[j].b_name) == 0) {
            recorrer_inodo(bloque.b_content[j].b_inodo, disk_pos, inode_start, block_start, bm_inode, bm_block, part_start);
            memset(bloque.b_content[j].b_name, '\0', 12);
            bloque.b_content[j].b_inodo = -1;
            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, block_start + sizeof(bloque_carpeta) * bloque_contiene, SEEK_SET);
            fwrite(&bloque, sizeof(bloque_archivos), 1, file);
            fclose(file);
        }
    }
    return 0;
}

int RM::recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start) {
    FILE *file;
    tabla_inodos inodo;// se recupera el inodo
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    int resultado = -1;

    //******************************************AQUI VA EL IF DE PERIMISOS********************************************//

    if (inodo.i_type == '1') {
        file = fopen(discos_montados[disk_pos].path, "rb+");
        for (int i = 0; i < 12; ++i) {
            if (inodo.i_block[i] != -1) {
                fseek(file, bm_block + inodo.i_block[i], SEEK_SET);
                fwrite("0", 1, 1, file);
            }
        }
        fclose(file);

        if (inodo.i_block[12] != -1) {
            recorrer_apuntadores(inodo.i_block[12], disk_pos, inode_start, block_start, 1, '1', bm_inode, bm_block, part_start);
        }

        if (inodo.i_block[13] != -1) {
            recorrer_apuntadores(inodo.i_block[13], disk_pos, inode_start, block_start, 2, '1', bm_inode, bm_block, part_start);
        }

        if (inodo.i_block[14] != -1) {
            recorrer_apuntadores(inodo.i_block[14], disk_pos, inode_start, block_start, 3, '1', bm_inode, bm_block, part_start);
        }
    } else {
        for (int i = 0; i < 12; ++i) {
            if (inodo.i_block[i] != -1) {
                resultado = recorrer_carpeta(inodo.i_block[i], disk_pos, inode_start, block_start, bm_inode, bm_block, part_start);
                if(resultado != 0) {
                    return  -1;
                }
            }
        }

        if (inodo.i_block[12] != -1) {
            resultado = recorrer_apuntadores(inodo.i_block[12], disk_pos, inode_start, block_start, 1, '0', bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return  -1;
            }
        }

        if (inodo.i_block[13] != -1) {
            resultado = recorrer_apuntadores(inodo.i_block[13], disk_pos, inode_start, block_start, 2, '0', bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return  -1;
            }
        }

        if (inodo.i_block[14] != -1) {
            resultado  = recorrer_apuntadores(inodo.i_block[14], disk_pos, inode_start, block_start, 3, '0', bm_inode, bm_block, part_start);
            if(resultado != 0) {
                return  -1;
            }
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    super_bloque super;
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    super.s_free_inodes_count++;
    fseek(file, part_start, SEEK_SET);
    fwrite(&super, sizeof(super_bloque), 1, file);

    fseek(file, bm_inode + indice_inodo, SEEK_SET);
    fwrite("0", 1, 1, file);
    fclose(file);

    return 0;
}

int RM::recorrer_carpeta(int indice_bloque, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start) {
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
    super_bloque super;
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    super.s_free_inodes_count++;
    fseek(file, part_start, SEEK_SET);
    fwrite(&super, sizeof(super_bloque), 1, file);

    fseek(file, bm_block + indice_bloque, SEEK_SET);
    fwrite("0", 1, 1, file);
    fclose(file);

    return 0;
}

int RM::recorrer_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo, int bm_inode, int bm_block, int part_start) {
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
                    fwrite("0", 1, 1, file);
                    fclose(file);
                }
            } else {
                recorrer_apuntadores(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, nivel - 1,tipo, bm_inode, bm_block, part_start);
            }
        }
    } else {
        for (int i = 0; i < 16; ++i) {
            if(nivel == 1) {
                if (apuntadores.b_pointers[i] != -1) {
                    recorrer_carpeta(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, bm_inode, bm_block, part_start);
                }
            } else {
                resultado = recorrer_apuntadores(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, nivel - 1, tipo, bm_inode, bm_block, part_start);
                if(resultado != 0) {
                    return -1;
                }
            }
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    super_bloque super;
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    super.s_free_inodes_count++;
    fseek(file, part_start, SEEK_SET);
    fwrite(&super, sizeof(super_bloque), 1, file);

    fseek(file, bm_block + indice_bloque, SEEK_SET);
    fwrite("0", 1, 1, file);
    fclose(file);

    return 0;
}

list<string> RM::separar_carpetas(string path) {
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

string RM::nombre_archivo(string path) {
    string nombre_disco;
    for (int i = path.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}

bool RM::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR RM::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

int RM::bitmap_libre(int start, int final, char const *path) {
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