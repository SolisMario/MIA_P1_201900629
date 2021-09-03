//
// Created by mario on 29/08/21.
//

#include "mv.h"

void MV::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void MV::set_dest(bool comillas, string dest) {
    if (comillas) {
        this->dest = dest.substr(1, dest.length() - 2);
    } else {
        this->dest = dest;
    }
}

void MV::mv() {
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

        int bloque_actual;
        int inodo_destino;

        for (int i = 0; i < 2; ++i) {

            tabla_inodos carpeta_tmp;
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, superBloque.s_inode_start, SEEK_SET);
            fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
            fclose(recuperar);

            int inodo_carpeta = 0;
            list<string> carpetas;
            if(i == 0){
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
            if(i == 0) {
                old_name = nombre_archivo(this->path.c_str());
            } else {
                old_name = nombre_archivo(this->dest.c_str());
            }
            int carpeta_buscada = -1;
            if(i == 0){// 0 regresa el bloque carpeta para borrar referencia, 1 el inodo para agregar la referencia
                bloque_actual = get_inodo(old_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 1);
            } else {
                inodo_destino = get_inodo(old_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 0);
            }
        }
        if(bloque_actual == -1){
            cout << "ERROR La carpeta/archivo de origen no existe." << endl;
            return;
        } else if(inodo_destino == -1){
            cout << "ERROR La carpeta destino no existe." << endl;
            return;
        }

        string nombre = nombre_archivo(this->path.c_str());
        //cambiar la referencia
        cambiar_referencia(inodo_destino, bloque_actual, nombre, disk_pos, part_start);

        cout << "Contenido movido exitosamente." << endl;
        if(superBloque.s_filesystem_type == 3) add_to_journal(discos_montados[disk_pos].path, part_start);
    } else {
        cout << "La particion no exite." << endl;
    }
}

int MV::get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start,
                  int operacion) {
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

//metodo para traer el inodo en el que se encuentra el archivo o carpeta
int MV::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path,
                            int part_start, int operacion) {
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

void MV::actualizar_abuelo(int indice_inodo, int disk_pos, int inode_start, int block_start, int inodo_abuelo){
    //recuperar inodo
    FILE *file;
    tabla_inodos inodo;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    if (inodo.i_type == '0') {
        bloque_carpeta carpeta_tmp;
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, block_start + sizeof(bloque_archivos) * inodo.i_block[0], SEEK_SET);
        fread(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
        fclose(file);

        carpeta_tmp.b_content[1].b_inodo = inodo_abuelo;

        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, block_start + sizeof(bloque_archivos) * inodo.i_block[0], SEEK_SET);
        fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
        fclose(file);
    }

}

void MV::cambiar_referencia(int inodo_destino, int bloque_origen, string nombre_origen, int disk_pos, int part_start){
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

    for (int i = 0; i < 4; ++i) {
        if (strcmp(nombre_origen.c_str(), carpeta_tmp.b_content[i].b_name) == 0) {
            b_inodo = carpeta_tmp.b_content[i].b_inodo;

            memset(carpeta_tmp.b_content[i].b_name, '\0', 12);
            carpeta_tmp.b_content[i].b_inodo = -1;

            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * bloque_origen, SEEK_SET);
            fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
            fclose(file);
        }
    }

    actualizar_abuelo(b_inodo, disk_pos, superBloque.s_inode_start, superBloque.s_block_start, inodo_destino);

    //recuperar inodo
    tabla_inodos inodo;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    for (int i = 0; i < 12; ++i) {
        if(inodo.i_block[i] != -1){
            bloque_carpeta carpeta_destino;
            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo.i_block[i], SEEK_SET);
            fread(&carpeta_destino, sizeof(bloque_carpeta), 1, file);
            fclose(file);

            for (int j = 0; j < 4; ++j) {
                if(carpeta_destino.b_content[j].b_inodo == -1){
                    carpeta_destino.b_content[j].b_inodo = b_inodo;
                    strcpy(carpeta_destino.b_content[j].b_name, nombre_origen.c_str());
                    file = fopen(discos_montados[disk_pos].path, "rb+");
                    fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo.i_block[i], SEEK_SET);
                    fwrite(&carpeta_destino, sizeof(bloque_carpeta), 1, file);
                    fclose(file);
                    return;
                }
            }
        } else {
            inodo.i_block[i] = superBloque.s_first_blo;
            bloque_carpeta carpeta_destino;
            carpeta_destino.b_content[0].b_inodo = b_inodo;
            strcpy(carpeta_destino.b_content[0].b_name, nombre_origen.c_str());

            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo.i_block[i], SEEK_SET);
            fwrite(&carpeta_destino, sizeof(bloque_carpeta), 1, file);

            fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
            fwrite("1", 1, 1, file);
            fclose(file);

            superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start, discos_montados[disk_pos].path);
            superBloque.s_free_blocks_count--;

            file = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(super_bloque), 1, file);
            fclose(file);
            return;
        }
    }

    int inodo_creado = -1;

    if(inodo.i_block[12] == -1){//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        inodo.i_block[12] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
        fwrite(&inodo, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, discos_montados[disk_pos].path);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(1, inodo.i_block[12], nombre_origen, disk_pos, part_start, inodo, b_inodo);
    if (inodo_creado != -1) return;

    if(inodo.i_block[13] == -1){//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        inodo.i_block[13] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
        fwrite(&inodo, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, discos_montados[disk_pos].path);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(1, inodo.i_block[13], nombre_origen, disk_pos, part_start, inodo, b_inodo);
    if (inodo_creado != -1) return;

    if(inodo.i_block[14] == -1){//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        inodo.i_block[14] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_destino, SEEK_SET);
        fwrite(&inodo, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

        bloque_apuntadores apuntadores;//creamo el bloque de apuntadores
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&apuntadores , sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

        //marcamos como usado el bitmap del bloque y del inodo
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);

        //actualizamos el superbloque y lo escribimos
        superBloque.s_free_blocks_count--;
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, discos_montados[disk_pos].path);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(1, inodo.i_block[14], nombre_origen, disk_pos, part_start, inodo, b_inodo);

}

int MV::crear_inodo_indirecto(int nivel, int apuntador_ind, string nombre_origen, int disk_pos, int part_start, tabla_inodos carpeta_actual, int b_inodo) {
    int inodo_creado = -1;
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
                    if(carpeta_destino.b_content[j].b_inodo == -1){
                        carpeta_destino.b_content[j].b_inodo = b_inodo;
                        strcpy(carpeta_destino.b_content[j].b_name, nombre_origen.c_str());
                        file = fopen(discos_montados[disk_pos].path, "rb+");
                        fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * apuntadores.b_pointers[i], SEEK_SET);
                        fwrite(&carpeta_destino, sizeof(bloque_carpeta), 1, file);
                        fclose(file);
                        return 0;
                    }
                }
            } else {
                inodo_creado = crear_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_origen, disk_pos, part_start, carpeta_actual, b_inodo);
                if (inodo_creado != -1) return inodo_creado;
            }
        } else {
            //aqui se crea el bloque de carpeta nuevo
            if (nivel == 1) {
                apuntadores.b_pointers[i] = superBloque.s_first_blo;
                bloque_carpeta carpeta_destino;
                carpeta_destino.b_content[0].b_inodo = b_inodo;
                strcpy(carpeta_destino.b_content[0].b_name, nombre_origen.c_str());

                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * apuntadores.b_pointers[i], SEEK_SET);
                fwrite(&carpeta_destino, sizeof(bloque_carpeta), 1, file);

                fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
                fwrite("1", 1, 1, file);
                fclose(file);

                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start, discos_montados[disk_pos].path);
                superBloque.s_free_blocks_count--;

                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);
                return 0;

            } else {

                apuntadores.b_pointers[i] = superBloque.s_first_blo;// actualizamos el inodo
                file = fopen(discos_montados[disk_pos].path, "rb+");
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
                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, discos_montados[disk_pos].path);
                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, file);
                fclose(file);

                inodo_creado = crear_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_origen, disk_pos, part_start, carpeta_actual, b_inodo);
                if (inodo_creado != -1) return inodo_creado;
            }
        }
    }
    return -1;
}

int MV::bitmap_libre(int start, int final, char const *path) {
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

list<string> MV::separar_carpetas(string path) {
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

bool MV::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR MV::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

string MV::nombre_archivo(const char *path) {
    string nombre_char = path;
    string nombre_disco;
    for (int i = nombre_char.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}

int MV::posicion_journal(char const *path, int partStart) {
    journal journal_actual;
    FILE *file;
    file = fopen(path, "rb+");
    fseek(file, partStart + sizeof(super_bloque), SEEK_SET);
    fread(&journal_actual, sizeof(journal), 1, file);
    fclose(file);

    int posicion_actual = 0;
    while (true) {
        posicion_actual = journal_actual.posicion;
        if (journal_actual.next == -1) {
            break;
        }
        file = fopen(path, "rb+");
        fseek(file, partStart + sizeof(super_bloque) + sizeof(journal) * journal_actual.next, SEEK_SET);
        fread(&journal_actual, sizeof(journal), 1, file);
        fclose(file);
    }

    journal_actual.next = posicion_actual + 1;
    file = fopen(path, "rb+");
    fseek(file, partStart + sizeof(super_bloque) + sizeof(journal) * posicion_actual, SEEK_SET);
    fwrite(&journal_actual, sizeof(journal), 1, file);
    fclose(file);

    return posicion_actual + 1;
}

void MV::add_to_journal(char const *path, int partStart) {
    journal nuevo;
    nuevo.posicion = posicion_journal(path, partStart);
    strcpy(nuevo.tipo_operacion, "mv");
    strcpy(nuevo.path, this->path.c_str());
    nuevo.log_fecha = time(0);
    nuevo.tipo = '8';
    FILE *file;
    file = fopen(path, "rb+");
    fseek(file, partStart + sizeof(super_bloque) + sizeof(journal) * nuevo.posicion, SEEK_SET);
    fwrite(&nuevo, sizeof(journal), 1, file);
    fclose(file);
}