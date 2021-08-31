//
// Created by mario on 31/08/21.
//

#include "cp.h"

void CP::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void CP::set_dest(bool comillas, string dest) {
    if (comillas) {
        this->dest = dest.substr(1, dest.length() - 2);
    } else {
        this->dest = dest;
    }
}

void CP::cp() {
    if (this->path.empty()) {
        cout << "El parametro path es obligatorio." << endl;
        return;
    } else if (this->dest.empty()) {
        cout << "El parametro dest es obligatorio." << endl;
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

        int inodo_origen;
        int inodo_destino;

        for (int i = 0; i < 2; ++i) {

            tabla_inodos carpeta_tmp;
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, superBloque.s_inode_start, SEEK_SET);
            fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
            fclose(recuperar);

            int inodo_carpeta = 0;
            list<string> carpetas;
            if (i == 0) {
                carpetas = separar_carpetas(this->path);
            } else {
                carpetas = separar_carpetas(this->dest);
            }

            list<string>::iterator it;

            //navegar entre las carpetas
            for (it = carpetas.begin(); it != carpetas.end(); it++) {
                inodo_carpeta = get_inodo(*it, carpeta_tmp, discos_montados[disk_pos].path, part_start, 0);
                if (inodo_carpeta == -1) { //si el inodo carpeta es -1 las carpetas no existen
                    cout << "La carpeta " << *it << " no existe " << endl;
                    return;
                }
                recuperar = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(recuperar, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_carpeta, SEEK_SET);
                fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
                fclose(recuperar);
            }

            //si llega hasta aqui significa que las carpetas existen o se crearon con exito
            string old_name;
            if (i == 0) {
                old_name = nombre_archivo(this->path.c_str());
            } else {
                old_name = nombre_archivo(this->dest.c_str());
            }
            int carpeta_buscada = -1;
            if (i == 0) {// 0 regresa el bloque carpeta para borrar referencia, 1 el inodo para agregar la referencia
                //inodo desde el que quiero empezar a copiar
                inodo_origen = get_inodo(old_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 0);
            } else {
                //inodo al cual voy a copiar el contenido
                inodo_destino = get_inodo(old_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 0);
            }
        }
        if (inodo_origen == -1) {
            cout << "ERROR La carpeta/archivo de origen no existe." << endl;
            return;
        } else if (inodo_destino == -1) {
            cout << "ERROR La carpeta destino no existe." << endl;
            return;
        }
        string nombre = nombre_archivo(this->path.c_str());
        //bloque en el que se copiara el contenido, este esta en el inodo destino
        int bloque_destino = conseguir_bloque_carpeta(inodo_destino, inodo_origen, nombre, disk_pos, part_start);
        iniciar_copiado(bloque_destino, disk_pos, part_start, inodo_origen, nombre, inodo_destino);
    } else {
        cout << "La particion no existe." << endl;
    }
}

int CP::get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int operacion) {
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
                    if (operacion == 0) {
                        return carpeta_tmp.b_content[j].b_inodo;
                    } else {
                        return inodo_carpeta.i_block[i];
                    }
                }
            }
        }
    }

    if (inodo_carpeta.i_block[12] != -1) {
        int inodo_indirecto = get_inodo_indirecto(1, inodo_carpeta.i_block[12], nombre_buscado, path,
                                                  part_start, operacion);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    if (inodo_carpeta.i_block[13] != -1) {
        int inodo_indirecto = get_inodo_indirecto(2, inodo_carpeta.i_block[13], nombre_buscado, path,
                                                  part_start, operacion);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    if (inodo_carpeta.i_block[13] != -1) {
        int inodo_indirecto = get_inodo_indirecto(3, inodo_carpeta.i_block[14], nombre_buscado, path,
                                                  part_start, operacion);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    return -1;
}

int CP::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
                            int operacion) {
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
                    if (operacion == 0) {
                        return carpeta_tmp.b_content[j].b_inodo;
                    } else {
                        return apuntadores.b_pointers[i];
                    }
                }
            } else {
                int inodo = get_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_buscado, path,
                                                part_start, operacion);
                if (inodo != -1) return inodo;
            }
        }
    }
    return -1;
}

int
CP::conseguir_bloque_carpeta(int inodo_destino, int bloque_origen, string nombre_origen, int disk_pos, int part_start) {
    //recuperar bloque origen
    FILE *file;

    super_bloque superBloque;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    bloque_carpeta carpeta_tmp;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * bloque_origen, SEEK_SET);
    fread(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    int b_inodo = -1;

    for (int i = 0; i < 4; ++i) {//recupero el inodo que quiero copiar
        if (strcmp(nombre_origen.c_str(), carpeta_tmp.b_content[i].b_name) == 0) {
            b_inodo = carpeta_tmp.b_content[i].b_inodo;
        }
    }

    //recuperar inodo al que se copiara la carpeta o archivo
    tabla_inodos inodo;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            bloque_carpeta carpeta_destino;
            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo.i_block[i], SEEK_SET);
            fread(&carpeta_destino, sizeof(bloque_carpeta), 1, file);
            fclose(file);

            for (int j = 0; j < 4; ++j) {//busco espacio libre en un bloque de carpeta ya creado
                if (carpeta_destino.b_content[j].b_inodo == -1) {
                    return inodo.i_block[i];
                }
            }
        } else {
            int nuevo_bloque = superBloque.s_first_blo;
            inodo.i_block[i] = superBloque.s_first_blo;
            bloque_carpeta carpeta_destino;
            //carpeta_destino.b_content[0].b_inodo = b_inodo;
            //strcpy(carpeta_destino.b_content[0].b_name, nombre_origen.c_str());

            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo.i_block[i], SEEK_SET);
            fwrite(&carpeta_destino, sizeof(bloque_carpeta), 1, file);

            fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
            fwrite("1", 1, 1, file);
            fclose(file);

            superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start,
                                                   discos_montados[disk_pos].path);
            superBloque.s_free_blocks_count--;

            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(super_bloque), 1, file);
            fclose(file);
            return nuevo_bloque;
        }
    }

    int bloque_creado = -1;

    if (inodo.i_block[12] == -1) {//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        inodo.i_block[12] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
        fwrite(&inodo, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                               superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                               discos_montados[disk_pos].path);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    bloque_creado = conseguir_bloque_carpeta_ind(1, inodo.i_block[12], nombre_origen, disk_pos, part_start, inodo,
                                                 b_inodo);
    if (bloque_creado != -1) return bloque_creado;

    if (inodo.i_block[13] == -1) {//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        inodo.i_block[13] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
        fwrite(&inodo, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                               superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                               discos_montados[disk_pos].path);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    bloque_creado = conseguir_bloque_carpeta_ind(1, inodo.i_block[13], nombre_origen, disk_pos, part_start, inodo,
                                                 b_inodo);
    if (bloque_creado != -1) bloque_creado;

    if (inodo.i_block[14] == -1) {//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        inodo.i_block[14] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
        fwrite(&inodo, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                               superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                               discos_montados[disk_pos].path);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    bloque_creado = conseguir_bloque_carpeta_ind(1, inodo.i_block[14], nombre_origen, disk_pos, part_start, inodo,
                                                 b_inodo);
    if (bloque_creado != -1) return bloque_creado;

    return -1;
}

int CP::conseguir_bloque_carpeta_ind(int nivel, int apuntador_ind, string nombre_origen, int disk_pos, int part_start,
                                     tabla_inodos carpeta_actual, int b_inodo) {
    int bloque_creado = -1;
    FILE *file;
    //se trae el superbloque y el inodo indirecto
    super_bloque superBloque;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    bloque_apuntadores apuntadores;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * apuntador_ind, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                bloque_carpeta carpeta_destino;
                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * apuntadores.b_pointers[i], SEEK_SET);
                fread(&carpeta_destino, sizeof(bloque_carpeta), 1, file);
                fclose(file);

                for (int j = 0; j < 4; ++j) {
                    if (carpeta_destino.b_content[j].b_inodo == -1) {
                        return apuntadores.b_pointers[i];
                    }
                }
            } else {
                bloque_creado = conseguir_bloque_carpeta_ind(nivel - 1, apuntadores.b_pointers[i], nombre_origen,
                                                             disk_pos, part_start, carpeta_actual, b_inodo);
                if (bloque_creado != -1) return bloque_creado;
            }
        } else {
            //aqui se crea el bloque de carpeta nuevo
            if (nivel == 1) {
                int nuevo_bloque = superBloque.s_first_blo;
                apuntadores.b_pointers[i] = superBloque.s_first_blo;
                bloque_carpeta carpeta_destino;
                //carpeta_destino.b_content[0].b_inodo = b_inodo;
                //strcpy(carpeta_destino.b_content[0].b_name, nombre_origen.c_str());

                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * apuntadores.b_pointers[i], SEEK_SET);
                fwrite(&carpeta_destino, sizeof(bloque_carpeta), 1, file);

                fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
                fwrite("1", 1, 1, file);
                fclose(file);

                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start,
                                                       discos_montados[disk_pos].path);
                superBloque.s_free_blocks_count--;

                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);
                return nuevo_bloque;

            } else {

                apuntadores.b_pointers[i] = superBloque.s_first_blo;// actualizamos el inodo
                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * apuntador_ind, SEEK_SET);
                fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);//escribimos el inodo actualizado

                bloque_apuntadores apuntadores2;//creamo el bloque de apuntadores
                fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
                fwrite(&apuntadores2, sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

                //marcamos como usado el bitmap del bloque y del inodo
                fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
                fwrite("1", 1, 1, file);
                fclose(file);

                //actualizamos el superbloque y lo escribimos
                superBloque.s_free_blocks_count--;
                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                                       superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                                       discos_montados[disk_pos].path);
                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);

                bloque_creado = conseguir_bloque_carpeta_ind(nivel - 1, apuntadores.b_pointers[i], nombre_origen,
                                                             disk_pos, part_start, carpeta_actual, b_inodo);
                if (bloque_creado != -1) return bloque_creado;
            }
        }
    }
    return -1;
}

void CP::iniciar_copiado(int bloque_origen, int disk_pos, int part_start, int inodo_origen, string nombre_origen,
                         int inodo_destino) {
    FILE *file;
    super_bloque superBloque;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    bloque_carpeta carpeta_tmp;//bloque nuevo o usado en el que va el inodo nuevo, esta en inodo origen
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * bloque_origen, SEEK_SET);
    fread(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    //se busca el espacio libre en el bloque de carpeta para iniciar el copiado
    for (int i = 0; i < 4; ++i) {
        if (carpeta_tmp.b_content[i].b_inodo == -1) {
            //copiar el inodo: inodo origen, metodo recorrer inodo con el inodo origen
            carpeta_tmp.b_content[i].b_inodo = recorrer_inodo(disk_pos, part_start, inodo_origen, nombre_origen,
                                                              inodo_destino);
            strcpy(carpeta_tmp.b_content[i].b_name, nombre_origen.c_str());
            break;
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * bloque_origen, SEEK_SET);
    fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
    fclose(file);
}

int CP::recorrer_inodo(int disk_pos, int part_start, int indice_inodo, string nombre_origen, int inodo_padre) {
    FILE *file;

    file = fopen(discos_montados[disk_pos].path, "rb+");
    super_bloque superBloque;//recupero el superbloque
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);

    int indice_creado = superBloque.s_first_ino;
    fseek(file, superBloque.s_bm_inode_start + indice_creado, SEEK_SET);
    fwrite("1", 1, 1, file);

    tabla_inodos inodo;//recupero el inodo que se va a crear
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);


    tabla_inodos nuevo_inodo;//este inodo sera la copia del inodo que venga en indice inodo, se modifica el nuevo inodo
    nuevo_inodo.i_uid = usuario_loggeado.id;
    nuevo_inodo.i_gid = usuario_loggeado.grupo;
    nuevo_inodo.i_size = inodo.i_size;
    nuevo_inodo.i_atime = time(0);
    nuevo_inodo.i_ctime = time(0);
    nuevo_inodo.i_mtime = time(0);
    nuevo_inodo.i_type = inodo.i_type;
    nuevo_inodo.i_perm = inodo.i_perm;

    //ahora se escribe el inodo para reservar su espacio y se modifica el superbloque
    superBloque.s_free_inodes_count--;
    superBloque.s_first_ino = bitmap_libre(superBloque.s_bm_inode_start,
                                           superBloque.s_bm_inode_start + superBloque.s_inodes_count,
                                           discos_montados[disk_pos].path);
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fwrite(&superBloque, sizeof(super_bloque), 1, file);

    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * indice_creado, SEEK_SET);
    fwrite(&nuevo_inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    int bloque_copiado = -1;
    //ahora recorremos los apuntadores del inodo original, hay que verificar el tipo de inodo para saber que tipo copiar
    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            if (inodo.i_type == '1') {
                bloque_copiado = recorrer_bloque_archivo(disk_pos, part_start, inodo.i_block[i], nombre_origen);
                nuevo_inodo.i_block[i] = bloque_copiado;
            } else {
                bloque_copiado = recorrer_bloque_carpeta(disk_pos, part_start, inodo.i_block[i], nombre_origen,
                                                         indice_creado, inodo_padre);
                nuevo_inodo.i_block[i] = bloque_copiado;
            }
        } else {
            nuevo_inodo.i_block[i] = -1;
        }
    }

    if(inodo.i_block[12] != -1){
        bloque_copiado = recorrer_bloque_apuntadores(disk_pos, part_start, inodo.i_block[12], nombre_origen, indice_creado, inodo_padre, inodo.i_type, 1);
        nuevo_inodo.i_block[12] = bloque_copiado;
    }

    if(inodo.i_block[13] != -1){
        bloque_copiado = recorrer_bloque_apuntadores(disk_pos, part_start, inodo.i_block[13], nombre_origen, indice_creado, inodo_padre, inodo.i_type, 2);
        nuevo_inodo.i_block[13] = bloque_copiado;
    }

    if(inodo.i_block[14] != -1){
        bloque_copiado = recorrer_bloque_apuntadores(disk_pos, part_start, inodo.i_block[14], nombre_origen, indice_creado, inodo_padre, inodo.i_type, 3);
        nuevo_inodo.i_block[14] = bloque_copiado;
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * indice_creado, SEEK_SET);
    fwrite(&nuevo_inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    return indice_creado;
}

int CP::recorrer_bloque_carpeta(int disk_pos, int part_start, int indice_bloque, string nombre_origen, int inodo_padre,
                                int inodo_abuelo) {
    FILE *file;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    super_bloque superBloque;//recupero el superbloque
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);

    int indice_creado = superBloque.s_first_blo;
    fseek(file, superBloque.s_bm_block_start + indice_creado, SEEK_SET);
    fwrite("1", 1, 1, file);

    bloque_carpeta carpeta_original;
    fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * indice_bloque, SEEK_SET);
    fread(&carpeta_original, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    bloque_carpeta carpeta_nueva;//creamos el nuevo bloque y copiamos nombres
    for (int i = 0; i < 4; ++i) {
        if (carpeta_original.b_content[i].b_inodo != -1) {
            strcpy(carpeta_nueva.b_content[i].b_name, carpeta_original.b_content[i].b_name);
            if (strcmp(carpeta_original.b_content[i].b_name, ".") == 0) {
                carpeta_nueva.b_content[i].b_inodo = inodo_padre;
            } else if (strcmp(carpeta_original.b_content[i].b_name, "..") == 0) {
                carpeta_nueva.b_content[i].b_inodo = inodo_abuelo;
            }
        }
    }

    //se excribe la nueva carpeta para guardar su lugar
    superBloque.s_free_blocks_count--;
    superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                           superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                           discos_montados[disk_pos].path);
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fwrite(&superBloque, sizeof(super_bloque), 1, file);

    fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * indice_creado, SEEK_SET);
    fwrite(&carpeta_nueva, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    int inodo_copiado = -1;

    for (int i = 0; i < 4; ++i) {
        if (carpeta_original.b_content[i].b_inodo != -1 && strcmp(carpeta_original.b_content[i].b_name, ".") != 0 &&
            strcmp(carpeta_original.b_content[i].b_name, "..") != 0) {
            inodo_copiado = recorrer_inodo(disk_pos, part_start, carpeta_original.b_content[i].b_inodo, nombre_origen,
                                           inodo_padre);
            carpeta_nueva.b_content[i].b_inodo = inodo_copiado;
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_carpeta) * indice_creado, SEEK_SET);
    fwrite(&carpeta_nueva, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    return indice_creado;
}

int CP::recorrer_bloque_archivo(int disk_pos, int part_start, int indice_bloque, string nombre_origen) {
    FILE *file;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    super_bloque superBloque;//recupero el superbloque
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);

    int indice_creado = superBloque.s_first_blo;
    fseek(file, superBloque.s_bm_block_start + indice_creado, SEEK_SET);// esto es antes de buscar el libre
    fwrite("1", 1, 1, file);

    bloque_archivos archivo_original;
    fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&archivo_original, sizeof(bloque_archivos), 1, file);
    fclose(file);


    bloque_archivos archivo_nuevo;
    memcpy(archivo_nuevo.b_content, archivo_original.b_content, 64);

    superBloque.s_free_blocks_count--;
    superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                           superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                           discos_montados[disk_pos].path);
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fwrite(&superBloque, sizeof(super_bloque), 1, file);


    fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * indice_creado, SEEK_SET);
    fwrite(&archivo_nuevo, sizeof(bloque_archivos), 1, file);
    fclose(file);

    return indice_creado;
}

int
CP::recorrer_bloque_apuntadores(int disk_pos, int part_start, int indice_bloque, string nombre_origen, int inodo_padre,
                                int inodo_abuelo, char tipo, int nivel) {
    //recupero el bloque de apuntadores
    FILE *file;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    super_bloque superBloque;//recupero el superbloque
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);

    int indice_creado = superBloque.s_first_blo;
    fseek(file, superBloque.s_bm_block_start + indice_creado, SEEK_SET);
    fwrite("1", 1, 1, file);

    bloque_apuntadores apuntadores_original;
    fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores_original, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    bloque_apuntadores apuntadores_nuevo;
    //se excribe la nueva carpeta para guardar su lugar
    superBloque.s_free_blocks_count--;
    superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                           superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                           discos_montados[disk_pos].path);
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fwrite(&superBloque, sizeof(super_bloque), 1, file);

    fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * indice_creado, SEEK_SET);
    fwrite(&apuntadores_nuevo, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    int bloque_copiado = -1;
    for (int i = 0; i < 16; ++i) {
        if (apuntadores_original.b_pointers[i] != -1) {
            if (nivel == 1) {
                if (tipo == '1') {
                    bloque_copiado = recorrer_bloque_archivo(disk_pos, part_start, apuntadores_original.b_pointers[i],
                                                             nombre_origen);
                    apuntadores_nuevo.b_pointers[i] = bloque_copiado;
                } else {
                    bloque_copiado = recorrer_bloque_carpeta(disk_pos, part_start, apuntadores_original.b_pointers[i],
                                                             nombre_origen, indice_creado, inodo_padre);
                    apuntadores_nuevo.b_pointers[i] = bloque_copiado;
                }
            } else {
                bloque_copiado = recorrer_bloque_apuntadores(disk_pos, part_start, apuntadores_original.b_pointers[i], nombre_origen, inodo_padre, inodo_abuelo, tipo, nivel - 1);
                apuntadores_nuevo.b_pointers[i] = bloque_copiado;
            }
        }
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * indice_creado, SEEK_SET);
    fwrite(&apuntadores_nuevo, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    return indice_creado;
}

int CP::bitmap_libre(int start, int final, char const *path) {
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

list<string> CP::separar_carpetas(string path) {
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

bool CP::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR CP::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

string CP::nombre_archivo(const char *path) {
    string nombre_char = path;
    string nombre_disco;
    for (int i = nombre_char.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}