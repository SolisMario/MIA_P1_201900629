//
// Created by mario on 22/08/21.
//

#include <fstream>
#include "touch.h"

void TOUCH::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void TOUCH::set_r(bool r) {
    this->r = r;
}

void TOUCH::set_size(int size) {
    this->size = size;
}

void TOUCH::set_cont(bool comillas, std::string cont) {
    if (comillas) {
        this->cont = cont.substr(1, cont.length() - 2);
    } else {
        this->cont = cont;
    }
}

void TOUCH::set_stdin(bool stdin_) {
    this->stdin_ = stdin_;
}

void TOUCH::touch() {
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
        bool carpeta_inexistente = false;

        //navegar entre las carpetas o crearlas si es permitido
        for (it = carpetas.begin(); it != carpetas.end(); it++) {
            inodo_carpeta = get_inodo(*it, carpeta_tmp, discos_montados[disk_pos].path, part_start, '0');
            if (inodo_carpeta == -1) { //si el inodo carpeta es -1 las carpetas no existen
                if (this->r) {//si r esta activa se crea la carpeta y se vuelve a llamar al siguiente inodo
                    inodo_carpeta = crear_inodo(*it, inodo_padre, inodo_abuelo, carpeta_tmp,
                                                discos_montados[disk_pos].path, part_start, '0');
                    carpeta_inexistente = true;
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

        //si llega hasta aqui significa que las carpetas existen o se crearon con exito
        string file_name = nombre_archivo(this->path.c_str());
        int inodo_archivo = -1;
        bool archivo_existente;
        if (!carpeta_inexistente) {//si carpeta inexistente es true no hay necesidad de revisar que el archivo exista
            //search archivo
            inodo_archivo = get_inodo(file_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, '1');
        }

        if (inodo_archivo != -1) {//si se devolvio un inodo el archivo existe
            cout << "El archivo que se quiere crear ya existe. \n ¿Desea sobreescribirlo?" << endl;
            string confirmacion;
            getline(cin, confirmacion);
            if (confirmacion != "s" && confirmacion != "S") {
                cout << "El archivo no fue creado ni sobreescrito." << endl;
                return;
            } else {
                //sobreescribir archivo
                int borrado = recorrer_inodo(inodo_archivo, disk_pos, superBloque.s_inode_start,
                                             superBloque.s_block_start,
                                             superBloque.s_bm_inode_start, superBloque.s_bm_block_start, part_start);
                if (borrado != 0) {
                    cout << "ERROR no se tiene permiso de edicion en uno de los archivos/carpetas." << endl;
                }
                recuperar = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(recuperar, part_start, SEEK_SET);
                fread(&superBloque, sizeof(super_bloque), 1, recuperar);
                fclose(recuperar);

                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                                       superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                                       discos_montados[disk_pos].path);
                superBloque.s_first_ino = bitmap_libre(superBloque.s_bm_inode_start,
                                                       superBloque.s_bm_inode_start + superBloque.s_inodes_count,
                                                       discos_montados[disk_pos].path);

                recuperar = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(recuperar, part_start, SEEK_SET);
                fwrite(&superBloque, sizeof(super_bloque), 1, recuperar);
                fclose(recuperar);

                escribir_archivo(discos_montados[disk_pos].path, part_start, inodo_archivo);
                return;
            }
        } else {
            //crear el inodo del archivo
            inodo_archivo = crear_inodo(file_name, inodo_padre, inodo_abuelo, carpeta_tmp,
                                        discos_montados[disk_pos].path, part_start, '1');
            if (inodo_archivo == -1) {
                cout << "Ocurrio un error inesperado al crear el archivo" << endl;
                return;
            } else {
                escribir_archivo(discos_montados[disk_pos].path, part_start, inodo_archivo);
                cout << "Archivo creado exitosamente." << endl;
                if(superBloque.s_filesystem_type == 3) add_to_journal(discos_montados[disk_pos].path, part_start);
            }
        }
    } else {
        cout << "La particion no existe" << endl;
    }
}

void TOUCH::escribir_archivo(const char *path, int part_start, int inodo_archivo) {
    FILE *file;
    string contenido;
    if (!this->cont.empty()) {
        ifstream in(this->cont);
        string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        contenido = contents;
        this->cont = contenido;
    } else if (this->stdin_) {
        cout << "Ingrese el texto que desea que el archivo contenga." << endl;
        getline(cin, contenido);
        this->cont = contenido;
    } else {
        int numero = 0;
        for (int i = 0; i < this->size; ++i) {
            contenido += to_string(numero);
            numero++;
            if (numero >= 10) {
                numero = 0;
            }
        }
    }


    super_bloque superBloque;
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);

    tabla_inodos archivo;
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
    fread(&archivo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    archivo.i_size = contenido.length();
    for (int i = 0; i < 12; ++i) {
        archivo.i_block[i] = superBloque.s_first_blo;

        bloque_archivos contenido_archivo;
        memset(&contenido_archivo, '\0', 64);
        for (int j = 0; j < 64; ++j) {
            if (contenido.length() == 0) {
                break;
            }
            contenido_archivo.b_content[j] = contenido[0];
            contenido.erase(0, 1);
        }
        //escribir el bloque de contenido
        fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
        fwrite(&archivo, sizeof(tabla_inodos), 1, file);

        fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&contenido_archivo, sizeof(bloque_archivos), 1, file);
        //marcar el bloque como utilizado en el bitmap
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);
        //buscar el bloque libre en el bitmap, actualizar el superBloque
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        superBloque.s_free_blocks_count--;

        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(superBloque), 1, file);
        fclose(file);
        if (contenido.length() == 0) break;
    }
    if (contenido.length() == 0) return;

    //si el contenido no es 0 se sigue con apuntadores indirectos
    archivo.i_block[12] = superBloque.s_first_blo;
    file = fopen(path, "rb+");
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
    fwrite(&archivo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    contenido = escribir_indirecto(1, contenido, path, part_start);
    if (contenido.length() == 0) return;

    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);
    archivo.i_block[13] = superBloque.s_first_blo;

    file = fopen(path, "rb+");
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
    fwrite(&archivo, sizeof(tabla_inodos), 1, file);
    fclose(file);
    contenido = escribir_indirecto(2, contenido, path, part_start);
    if (contenido.length() == 0) return;

    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    archivo.i_block[14] = superBloque.s_first_blo;
    file = fopen(path, "rb+");
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
    fwrite(&archivo, sizeof(tabla_inodos), 1, file);
    fclose(file);
    contenido = escribir_indirecto(3, contenido, path, part_start);
    if (contenido.length() == 0) return;

    if (contenido.length() != 0) {
        cout << "No se escribio el archivo completo." << endl;
    }
}

string TOUCH::escribir_indirecto(int nivel, string contenido, char const *path, int part_start) {
    FILE *file;
    super_bloque superBloque;
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    //int bloque_libre = bitmap_libre(superBloque.s_block_start, superBloque.s_block_start + superBloque.s_blocks_count,path);
    int posicion_apuntadores = superBloque.s_first_blo;

    bloque_apuntadores apuntadores;
    file = fopen(path, "rb+");
    fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * superBloque.s_first_blo, SEEK_SET);
    fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);

    fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
    fwrite("1", 1, 1, file);
    fclose(file);

    superBloque.s_free_blocks_count--;
    superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                           superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fwrite(&superBloque, sizeof(superBloque), 1, file);
    fclose(file);

    for (int i = 0; i < 16; ++i) {
        apuntadores.b_pointers[i] = superBloque.s_first_blo;
        fopen(path, "rb+");
        fseek(file, superBloque.s_block_start + sizeof(bloque_apuntadores) * posicion_apuntadores, SEEK_SET);
        fwrite(&apuntadores, sizeof(bloque_apuntadores), 1, file);
        fclose(file);

        if (nivel == 1) {

            bloque_archivos contenido_archivo;
            memset(&contenido_archivo, '\0', 64);

            for (int j = 0; j < 64; ++j) {
                if (contenido.length() == 0) {
                    break;
                }
                contenido_archivo.b_content[j] = contenido[0];
                contenido.erase(0, 1);
            }
            //escribir el bloque de contenido
            fopen(path, "rb+");
            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * superBloque.s_first_blo, SEEK_SET);
            fwrite(&contenido_archivo, sizeof(bloque_archivos), 1, file);

            //marcar el bloque como utilizado en el bitmap
            fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
            fwrite("1", 1, 1, file);
            fclose(file);
            //buscar el bloque libre en el bitmap, actualizar el superBloque
            superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                                   superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
            superBloque.s_free_blocks_count--;
            file = fopen(path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(superBloque), 1, file);
            fclose(file);

            if (contenido.length() == 0) break;
        } else {
            contenido = escribir_indirecto(nivel - 1, contenido, path, part_start);
            if (contenido.length() == 0) break;
            superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                                   superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
            file = fopen(path, "rb+");
            fseek(file, part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(superBloque), 1, file);
            fclose(file);
        }
    }
    return contenido;
}

int TOUCH::get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo) {
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

int TOUCH::crear_inodo(string nombre_carpeta, int inodo_padre, int inodo_abuelo, tabla_inodos carpeta_actual,
                       char const *path, int part_start, char tipo) {
    int inodo_creado = -1;

    FILE *file;
    super_bloque superBloque;
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);
    fclose(file);

    //for que itera los 12 apuntadores directos del inodo
    for (int i = 0; i < 12; ++i) {
        if (carpeta_actual.i_block[i] != -1) {//se ve si hay un bloque de carpeta ya creado con espacio libre
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

                    if (tipo == '0') {//crear inodo carpeta
                        crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start,
                                            superBloque.s_bm_inode_start, superBloque.s_bm_block_start,
                                            superBloque.s_first_ino, inodo_padre, superBloque.s_first_blo, path);
                        superBloque.s_free_blocks_count--;
                    } else {//crear inodo archivo
                        create_file_inode(superBloque.s_inode_start, superBloque.s_bm_inode_start,
                                          superBloque.s_first_ino, path);
                    }


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

            if (tipo == '0') {
                //se busca el siguiente bloque de carpeta libre
                int bloque_carpeta_libre = bitmap_libre(superBloque.s_bm_block_start,
                                                        superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                                        path);

                crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start, superBloque.s_bm_inode_start,
                                    superBloque.s_bm_block_start, superBloque.s_first_ino, inodo_padre,
                                    bloque_carpeta_libre, path);
                superBloque.s_free_blocks_count--;

            } else {
                create_file_inode(superBloque.s_inode_start, superBloque.s_bm_inode_start, superBloque.s_first_ino,
                                  path);
            }

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

    if (carpeta_actual.i_block[12] == -1) {//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        carpeta_actual.i_block[12] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
        fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

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
                                               superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(1, carpeta_actual.i_block[12], nombre_carpeta, path, part_start, tipo,
                                         inodo_padre, carpeta_actual);
    if (inodo_creado != -1) return inodo_creado;

    if (carpeta_actual.i_block[13] == -1) {//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        carpeta_actual.i_block[13] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
        fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

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
                                               superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(super_bloque), 1, file);
        fclose(file);
    }

    inodo_creado = crear_inodo_indirecto(2, carpeta_actual.i_block[13], nombre_carpeta, path, part_start, tipo,
                                         inodo_padre, carpeta_actual);
    if (inodo_creado != -1) return inodo_creado;

    if (carpeta_actual.i_block[14] == -1) {//si es -1 significa que no hay ningun bloque de punteros creado, se crea
        carpeta_actual.i_block[14] = superBloque.s_first_blo;// actualizamos el inodo
        file = fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_padre, SEEK_SET);
        fwrite(&carpeta_actual, sizeof(tabla_inodos), 1, file);//escribimos el inodo actualizado

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
                                               superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
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

int TOUCH::crear_inodo_indirecto(int nivel, int apuntador_ind, string nombre_carpeta, char const *path, int part_start,
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

                        if (tipo == '0') {//crear inodo carpeta
                            crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start,
                                                superBloque.s_bm_inode_start, superBloque.s_bm_block_start,
                                                superBloque.s_first_ino, inodo_padre, superBloque.s_first_blo, path);
                            superBloque.s_free_blocks_count--;
                        } else {//crear inodo archivo
                            create_file_inode(superBloque.s_inode_start, superBloque.s_bm_inode_start,
                                              superBloque.s_first_ino, path);
                        }

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

                if (tipo == '0') {
                    //se busca el siguiente bloque de carpeta libre
                    int bloque_carpeta_libre = bitmap_libre(superBloque.s_bm_block_start,
                                                            superBloque.s_bm_block_start + superBloque.s_blocks_count,
                                                            path);

                    crear_inodo_carpeta(superBloque.s_inode_start, superBloque.s_block_start,
                                        superBloque.s_bm_inode_start,
                                        superBloque.s_bm_block_start, superBloque.s_first_ino, inodo_padre,
                                        bloque_carpeta_libre, path);
                    superBloque.s_free_blocks_count--;

                } else {
                    create_file_inode(superBloque.s_inode_start, superBloque.s_bm_inode_start, superBloque.s_first_ino,
                                      path);
                }

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
                fwrite(&apuntadores2, sizeof(bloque_apuntadores), 1, file);//escribimos el nuevo bloque

                //marcamos como usado el bitmap del bloque y del inodo
                fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
                fwrite("1", 1, 1, file);
                fclose(file);

                //actualizamos el superbloque y lo escribimos
                superBloque.s_free_blocks_count--;
                superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start,
                                                       superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
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
void TOUCH::crear_inodo_carpeta(int inode_start, int block_start, int bm_inode, int bm_block, int inodo_actual,
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

//meotod para crear inodos tipo archivo
void TOUCH::create_file_inode(int inode_start, int bm_inode, int inodo_actual, char const *path) {
    FILE *file;
    //se debe crear el inodo carpeta
    tabla_inodos nuevo_archivo;
    nuevo_archivo.i_atime = time(0);
    nuevo_archivo.i_ctime = time(0);
    nuevo_archivo.i_mtime = time(0);
    nuevo_archivo.i_type = '1';
    file = fopen(path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * inodo_actual, SEEK_SET);
    fwrite(&nuevo_archivo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    file = fopen(path, "rb+");
    fseek(file, bm_inode + inodo_actual, SEEK_SET);
    fwrite("1", 1, 1, file);
    fclose(file);
}

//metodo para traer el inodo en el que se encuentra el archivo o carpeta
int TOUCH::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
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

int TOUCH::recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block,
                          int part_start) {
    FILE *file;
    tabla_inodos inodo;// se recupera el inodo
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    int resultado = -1;

    //******************************************AQUI VA EL IF DE PERIMISOS********************************************//

    file = fopen(discos_montados[disk_pos].path, "rb+");
    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            fseek(file, bm_block + inodo.i_block[i], SEEK_SET);
            fwrite("0", 1, 1, file);
            inodo.i_block[i] = -1;
        }
    }
    fclose(file);

    if (inodo.i_block[12] != -1) {
        recorrer_apuntadores(inodo.i_block[12], disk_pos, inode_start, block_start, 1, '1', bm_inode, bm_block,
                             part_start);
        inodo.i_block[12] = -1;
    }

    if (inodo.i_block[13] != -1) {
        recorrer_apuntadores(inodo.i_block[13], disk_pos, inode_start, block_start, 2, '1', bm_inode, bm_block,
                             part_start);
        inodo.i_block[13] = -1;
    }

    if (inodo.i_block[14] != -1) {
        recorrer_apuntadores(inodo.i_block[14], disk_pos, inode_start, block_start, 3, '1', bm_inode, bm_block,
                             part_start);
        inodo.i_block[14] = -1;
    }

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fwrite(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    return 0;
}

int TOUCH::recorrer_apuntadores(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel, char tipo,
                                int bm_inode, int bm_block, int part_start) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    for (int i = 0; i < 16; ++i) {
        if (nivel == 1) {
            if (apuntadores.b_pointers[i] != -1) {
                file = fopen(discos_montados[disk_pos].path, "rb+");
                fseek(file, bm_block + apuntadores.b_pointers[i], SEEK_SET);
                fwrite("0", 1, 1, file);
                fclose(file);
            }
        } else {
            recorrer_apuntadores(apuntadores.b_pointers[i], disk_pos, inode_start, block_start, nivel - 1, tipo,
                                 bm_inode, bm_block, part_start);
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

//metodo que busca el primer bitmap libre
int TOUCH::bitmap_libre(int start, int final, char const *path) {
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

list<string> TOUCH::separar_carpetas(string path) {
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

string TOUCH::nombre_archivo(const char *path) {
    string nombre_char = path;
    string nombre_disco;
    for (int i = nombre_char.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}

bool TOUCH::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR TOUCH::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

int TOUCH::posicion_journal(char const *path, int partStart) {
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

void TOUCH::add_to_journal(char const *path, int partStart) {
    journal nuevo;
    nuevo.posicion = posicion_journal(path, partStart);
    strcpy(nuevo.tipo_operacion, "touch");
    strcpy(nuevo.path, this->path.c_str());
    string contenido;
    if(this->cont[0] != '/'){
        contenido = this->cont;
        if(contenido.length() > 100){
            contenido = contenido.substr(0, 100);
        }
    }
    strcpy(nuevo.contenido, contenido.c_str());
    nuevo.log_fecha = time(0);
    nuevo.size = this->size;

    FILE *file;
    file = fopen(path, "rb+");
    fseek(file, partStart + sizeof(super_bloque) + sizeof(journal) * nuevo.posicion, SEEK_SET);
    fwrite(&nuevo, sizeof(journal), 1, file);
    fclose(file);
}