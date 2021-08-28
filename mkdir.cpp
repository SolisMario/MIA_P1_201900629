//
// Created by mario on 26/08/21.
//

#include <fstream>
#include "mkdir.h"

void MKDIR::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void MKDIR::set_p(bool p) {
    this->p = p;
}

void MKDIR::mkdir() {
    if (this->path.empty()) {
        cout << "El parametro path es obligatorio." << endl;
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
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
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

        int inodo_abuelo = 0;
        int inodo_padre = 0;
        int inodo_carpeta = 0;
        list<string> carpetas = separar_carpetas(this->path);
        list<string>::iterator it;

        //navegar entre las carpetas o crearlas si es permitido
        for (it = carpetas.begin(); it != carpetas.end(); it++) {
            inodo_carpeta = get_inodo(*it, carpeta_tmp, discos_montados[disk_pos].path, part_start, '0');
            if (inodo_carpeta == -1) { //si el inodo carpeta es -1 las carpetas no existen
                if (this->p) {//si p esta activa se crea la carpeta y se vuelve a llamar al siguiente inodo
                    inodo_carpeta = crear_inodo(*it, inodo_padre, inodo_abuelo, carpeta_tmp,
                                                discos_montados[disk_pos].path, part_start, '0');
                } else {
                    cout << "La carpeta " << *it << " no existe " << endl;
                    return;
                }
            }
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_carpeta, SEEK_SET);
            fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
            fclose(recuperar);
            inodo_abuelo = inodo_padre;
            inodo_padre = inodo_carpeta;
        }
    }
}

bool MKDIR::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

list<string> MKDIR::separar_carpetas(string path) {
    if (path[0] == '/') {
        path = path.substr(1, path.length());
    }
    list<string> lista_carpetas;
    char ruta[path.length() + 1];
    strcpy(ruta, path.c_str());
    string nombre_carpeta;
    for (int i = 0; i < path.length() + 1; i++) {
        if (ruta[i] == '/' || i == path.length()) {
            lista_carpetas.push_back(nombre_carpeta);
            nombre_carpeta.clear();
            continue;
        }
        nombre_carpeta += ruta[i];
    }
    return lista_carpetas;
}

int MKDIR::get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo) {
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
                cout << carpeta_tmp.b_content[j].b_name << " apunta a " << carpeta_tmp.b_content[j].b_inodo << endl;
                if (strcmp(nombre_buscado.c_str(), carpeta_tmp.b_content[j].b_name) == 0) {
                    tabla_inodos inodo_tmp;
                    file = fopen(path, "rb+");
                    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * carpeta_tmp.b_content[j].b_inodo,
                          SEEK_SET);
                    fread(&inodo_tmp, sizeof(tabla_inodos), 1, file);
                    fclose(file);
                    if (inodo_tmp.i_type == tipo) {
                        cout << "-----------------------------------------------------------------------" << endl;
                        return carpeta_tmp.b_content[j].b_inodo;
                    }
                }
            }
            cout << "-----------------------------------------------------------------------" << endl;
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

int MKDIR::crear_inodo(string nombre_carpeta, int inodo_padre, int inodo_abuelo, tabla_inodos carpeta_actual,
                       char const *path, int part_start, char tipo) {
    int inodo_creado = -1;
    FILE *file;
    //for que itera los 12 apuntadores directos del inodo
    for (int i = 0; i < 12; ++i) {
        if (carpeta_actual.i_block[i] != -1) {//se ve si hay un bloque de carpeta ya creado con espacio libre
            super_bloque superBloque;
            file = fopen(path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fread(&superBloque, sizeof(super_bloque), 1, file);
            fclose(file);

            bloque_carpeta carpeta_tmp;
            file = fopen(path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * carpeta_actual.i_block[i], SEEK_SET);
            fread(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
            fclose(file);
            for (int j = 0; j < 4; ++j) {
                if (carpeta_tmp.b_content[j].b_inodo == -1) {//si hay espacio libre se apunta hacia el 1er inodo libre
                    strcpy(carpeta_tmp.b_content[j].b_name, nombre_carpeta.c_str());
                    carpeta_tmp.b_content[j].b_inodo = superBloque.s_first_ino;
                    file = fopen(path, "rb+");
                    fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * carpeta_actual.i_block[i],
                          SEEK_SET);
                    fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
                    fclose(file);

                    crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start,
                                        superBloque.s_bm_inode_start, superBloque.s_bm_block_start,
                                        superBloque.s_first_ino, inodo_padre, superBloque.s_first_blo, path);
                    superBloque.s_free_blocks_count--;


                    inodo_creado = superBloque.s_first_ino;
                    //se busca el primer bloque e inodo libre
                    int first = bitmap_libre(superBloque.s_bm_block_start,
                                             superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
                    superBloque.s_first_blo = first;
                    first = bitmap_libre(superBloque.s_bm_inode_start,
                                         superBloque.s_bm_inode_start + superBloque.s_inodes_count, path);
                    superBloque.s_first_ino = first;

                    superBloque.s_free_inodes_count--;

                    file = fopen(path, "rb+");
                    fseek(file, part_start, SEEK_SET);
                    fwrite(&superBloque, sizeof(super_bloque), 1, file);
                    fclose(file);

                    return inodo_creado;
                }
            }
        } else {
            super_bloque superBloque;
            file = fopen(path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fread(&superBloque, sizeof(super_bloque), 1, file);
            fclose(file);

            //se busca un apuntador libre y se apunta hacia el primer bloque libre
            carpeta_actual.i_block[i] = superBloque.s_first_blo;
            file = fopen(path, "rb+");
            fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
            fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);
            fclose(file);

            bloque_carpeta bloqueCarpeta;//nuevo bloque de carpeta que contendra nuestra carpeta
            strcpy(bloqueCarpeta.b_content[0].b_name, nombre_carpeta.c_str());
            bloqueCarpeta.b_content[0].b_inodo = superBloque.s_first_ino;
            file = fopen(path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * carpeta_actual.i_block[i], SEEK_SET);
            fwrite(&bloqueCarpeta, sizeof(bloque_carpeta), 1, file);
            fclose(file);

            //marcamos como usado el bitmap del bloque y del inodo
            file = fopen(path, "rb+");
            fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
            fwrite("1", 1, 1, file);
            fclose(file);
            superBloque.s_free_blocks_count--;

            //se busca el siguiente bloque de carpeta libre
            int bloque_carpeta_libre = bitmap_libre(superBloque.s_bm_block_start,
                                                    superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                                    path);

            crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start, superBloque.s_bm_inode_start,
                                superBloque.s_bm_block_start, superBloque.s_first_ino, inodo_padre,
                                bloque_carpeta_libre, path);
            superBloque.s_free_blocks_count--;


            inodo_creado = superBloque.s_first_ino;//para retornar el indice del inodo creado
            //se busca el primer bloque e inodo libre
            int first = bitmap_libre(superBloque.s_bm_block_start,
                                     superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
            superBloque.s_first_blo = first;
            first = bitmap_libre(superBloque.s_bm_inode_start,
                                 superBloque.s_bm_inode_start + superBloque.s_inodes_count, path);
            superBloque.s_first_ino = first;

            superBloque.s_free_inodes_count--;

            file = fopen(path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(super_bloque), 1, file);
            fclose(file);
            return inodo_creado;
        }
    }

    super_bloque superBloque;
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    //si el contenido no es 0 se sigue con apuntadores indirectos

    if(carpeta_actual.i_block[12] == -1){//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        carpeta_actual.i_block[12] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
        fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(1, carpeta_actual.i_block[12], nombre_carpeta, path, part_start, tipo,
                                         inodo_padre, carpeta_actual);
    if (inodo_creado != -1) return inodo_creado;

    if(carpeta_actual.i_block[13] == -1){//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        carpeta_actual.i_block[13] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
        fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(2, carpeta_actual.i_block[13], nombre_carpeta, path, part_start, tipo,
                                         inodo_padre, carpeta_actual);
    if (inodo_creado != -1) return inodo_creado;

    if(carpeta_actual.i_block[14] == -1){//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        carpeta_actual.i_block[14] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
        fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(3, carpeta_actual.i_block[14], nombre_carpeta, path, part_start, tipo,
                                         inodo_padre, carpeta_actual);
    if (inodo_creado != -1) return inodo_creado;

    return -1;
}

int MKDIR::crear_inodo_indirecto(int nivel, int apuntador_ind, string nombre_carpeta, char const *path, int part_start,
                                 int tipo, int inodo_padre, tabla_inodos carpeta_actual) {
    int inodo_creado = -1;
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
                    if (carpeta_tmp.b_content[j].b_inodo == -1) {
                        //crear el inodo
                        strcpy(carpeta_tmp.b_content[j].b_name, nombre_carpeta.c_str());
                        carpeta_tmp.b_content[j].b_inodo = superBloque.s_first_ino;
                        file = fopen(path, "rb+");
                        fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * apuntadores.b_pointers[i],
                              SEEK_SET);
                        fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
                        fclose(file);

                        crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start,
                                            superBloque.s_bm_inode_start, superBloque.s_bm_block_start,
                                            superBloque.s_first_ino, inodo_padre, superBloque.s_first_blo, path);
                        superBloque.s_free_blocks_count--;


                        inodo_creado = superBloque.s_first_ino;
                        //se busca el primer bloque e inodo libre
                        int first = bitmap_libre(superBloque.s_bm_block_start,
                                                 superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
                        superBloque.s_first_blo = first;
                        first = bitmap_libre(superBloque.s_bm_inode_start,
                                             superBloque.s_bm_inode_start + superBloque.s_inodes_count, path);
                        superBloque.s_first_ino = first;

                        superBloque.s_free_inodes_count--;

                        file = fopen(path, "rb+");
                        fseek(file, part_start, SEEK_SET);
                        fwrite(&superBloque, sizeof(super_bloque), 1, file);
                        fclose(file);

                        return inodo_creado;
                    }
                }
            } else {
                inodo_creado = crear_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_carpeta, path,
                                                     part_start, tipo, inodo_padre, carpeta_actual);
                if (inodo_creado != -1) return inodo_creado;
            }
        } else {
            //aqui se crea el bloque de carpeta nuevo
            if (nivel == 1) {
                super_bloque superBloque;
                file = fopen(path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fread(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);

                //se busca un apuntador libre y se apunta hacia el primer bloque libre
                apuntadores.b_pointers[i] = superBloque.s_first_blo;
                file = fopen(path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * apuntador_ind, SEEK_SET);
                fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);
                fclose(file);

                bloque_carpeta bloqueCarpeta;//nuevo bloque de carpeta que contendra nuestra carpeta
                strcpy(bloqueCarpeta.b_content[0].b_name, nombre_carpeta.c_str());
                bloqueCarpeta.b_content[0].b_inodo = superBloque.s_first_ino;
                file = fopen(path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * apuntadores.b_pointers[i], SEEK_SET);
                fwrite(&bloqueCarpeta, sizeof(bloque_carpeta), 1, file);
                fclose(file);

                //marcamos como usado el bitmap del bloque bloque nuevo
                file = fopen(path, "rb+");
                fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
                fwrite("1", 1, 1, file);
                fclose(file);
                superBloque.s_free_blocks_count--;

                //se busca el siguiente bloque de carpeta libre
                int bloque_carpeta_libre = bitmap_libre(superBloque.s_bm_block_start,
                                                        superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                                        path);

                crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start,
                                    superBloque.s_bm_inode_start,
                                    superBloque.s_bm_block_start, superBloque.s_first_ino, inodo_padre,
                                    bloque_carpeta_libre, path);
                superBloque.s_free_blocks_count--;

                inodo_creado = superBloque.s_first_ino;//para retornar el indice del inodo creado
                //se busca el primer bloque e inodo libre
                int first = bitmap_libre(superBloque.s_bm_block_start,
                                         superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
                superBloque.s_first_blo = first;
                first = bitmap_libre(superBloque.s_bm_inode_start,
                                     superBloque.s_bm_inode_start + superBloque.s_inodes_count, path);
                superBloque.s_first_ino = first;

                superBloque.s_free_inodes_count--;

                file = fopen(path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);
                return inodo_creado;

            } else {

                apuntadores.b_pointers[i] = superBloque.s_first_blo;// actualizamos el inodo
                file = fopen(path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * apuntador_ind, SEEK_SET);
                fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);//escribimos el inodo actualizado

                bloque_apuntadores apuntadores2;//creamo el bloque de apuntadores
                fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
                fwrite(&apuntadores2 , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

                //marcamos como usado el bitmap del bloque y del inodo
                fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
                fwrite("1", 1, 1, file);
                fclose(file);

                //actualizamos el superbloque y lo escribimos
                superBloque.s_free_blocks_count--;
                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
                file = fopen(path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);

                inodo_creado = crear_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_carpeta, path,
                                                     part_start, tipo, inodo_padre, carpeta_actual);
                if (inodo_creado != -1) return inodo_creado;
            }
        }
    }
    return -1;
}

//metodo para crear el inodo carpteta junto con su bloque carpeta
void MKDIR::crear_inodo_carpeta(int inode_start, int block_start, int bm_inode, int bm_block, int inodo_actual,
                                int inodo_padre, int bloque_libre, char const *path) {
    FILE *file;
    //se debe crear el inodo carpeta
    tabla_inodos nueva_carpeta;
    nueva_carpeta.i_atime = time(0);
    nueva_carpeta.i_ctime = time(0);
    nueva_carpeta.i_mtime = time(0);
    nueva_carpeta.i_block[0] = bloque_libre;//apuntador hacia bloque de carpeta
    file = fopen(path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * inodo_actual, SEEK_SET);
    fwrite(&nueva_carpeta, sizeof(tabla_inodos), 1, file);
    fclose(file);

    //bloque de carpeta nueva, este si lleva padre y actual
    bloque_carpeta bloqueCarpeta1;
    strcpy(bloqueCarpeta1.b_content[0].b_name, ".");
    bloqueCarpeta1.b_content[0].b_inodo = inodo_actual;
    strcpy(bloqueCarpeta1.b_content[1].b_name, "..");
    bloqueCarpeta1.b_content[1].b_inodo = inodo_padre;
    file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_carpeta) * bloque_libre, SEEK_SET);
    fwrite(&bloqueCarpeta1, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    file = fopen(path, "rb+");
    fseek(file, bm_inode + inodo_actual, SEEK_SET);
    fwrite("1", 1, 1, file);
    fseek(file, bm_block + bloque_libre, SEEK_SET);
    fwrite("1", 1, 1, file);
    fclose(file);
}

int MKDIR::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
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
                        if (inodo_tmp.i_type == tipo) {
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

//metodo que busca el primer bitmap libre
int MKDIR::bitmap_libre(int start, int final, char const *path) {
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

EBR MKDIR::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}