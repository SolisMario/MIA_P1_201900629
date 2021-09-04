//
// Created by mario on 29/08/21.
//

#include <fstream>
#include "ren.h"

void REN::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void REN::set_name(string name) {
    this->name = name;
}

void REN::ren() {
    if (this->path.empty()) {
        cout << "El parametro path es obligatorio." << endl;
        return;
    } else if(this->name.empty()) {
        cout << "El parametro name es obligatorio." << endl;
        return;
    } else if (usuario_loggeado.activo == 0) {
        cout << "No se encuentra ningun usuario loggeado." << endl;
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

        int inodo_carpeta = 0;
        list<string> carpetas = separar_carpetas(this->path);
        list<string>::iterator it;
        bool carpeta_inexistente = false;

        //navegar entre las carpetas
        for (it = carpetas.begin(); it != carpetas.end(); it++) {
            inodo_carpeta = get_inodo(*it, this->name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 0);
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
        string old_name = nombre_archivo(this->path.c_str());
        int inodo_archivo = -1;
        inodo_archivo = get_inodo(this->name, this->name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 0);
        if(inodo_archivo != -1){
            cout << "ERROR ya existe un archivo/carpeta con este nombre." << endl;
            return;
        }
        //renombrar
        inodo_archivo = get_inodo(old_name, this->name, carpeta_tmp, discos_montados[disk_pos].path, part_start, 1);
        if(inodo_archivo == -1) {
            cout << "ERROR El archivo no existe." << endl;
            return;
        }
        cout << "EL archivo se renombro exitosamente." << endl;
        if(superBloque.s_filesystem_type == 3) add_to_journal(discos_montados[disk_pos].path, part_start);
    }
    else{
        cout << "La particion no existe." << endl;
    }
}

int REN::get_inodo(string nombre_buscado, string new_name, tabla_inodos inodo_carpeta, char const *path, int part_start, int operacion) {
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
                    if(operacion == 0){
                        return carpeta_tmp.b_content[j].b_inodo;
                    } else {
                        strcpy(carpeta_tmp.b_content[j].b_name, new_name.c_str());
                        file = fopen(path, "rb+");
                        fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * inodo_carpeta.i_block[i], SEEK_SET);
                        fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
                        fclose(file);
                        return 0;
                    }
                }
            }
        }
    }

    if (inodo_carpeta.i_block[12] != -1) {
        int inodo_indirecto = get_inodo_indirecto(1, inodo_carpeta.i_block[12], nombre_buscado, new_name, path, part_start, operacion);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    if (inodo_carpeta.i_block[13] != -1) {
        int inodo_indirecto = get_inodo_indirecto(2, inodo_carpeta.i_block[13], nombre_buscado, new_name, path, part_start, operacion);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    if (inodo_carpeta.i_block[13] != -1) {
        int inodo_indirecto = get_inodo_indirecto(3, inodo_carpeta.i_block[14], nombre_buscado, new_name, path, part_start,operacion);
        if (inodo_indirecto != -1) return inodo_indirecto;
    }

    return -1;
}

//metodo para traer el inodo en el que se encuentra el archivo o carpeta
int REN::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, string new_name, char const *path, int part_start, int operacion) {
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
                        if(operacion == 0){
                            return carpeta_tmp.b_content[j].b_inodo;
                        } else {
                            int apuntaa = carpeta_tmp.b_content[j].b_inodo;
                            strcpy(carpeta_tmp.b_content[j].b_name, new_name.c_str());
                            carpeta_tmp.b_content[j].b_inodo = apuntaa;
                            file = fopen(path, "rb+");
                            fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * apuntadores.b_pointers[i], SEEK_SET);
                            fwrite(&carpeta_tmp, sizeof(bloque_carpeta), 1, file);
                            fclose(file);
                            return 0;
                        }
                    }
                }
            } else {
                int inodo = get_inodo_indirecto(nivel - 1, apuntadores.b_pointers[i], nombre_buscado, new_name, path, part_start, operacion);
                if (inodo != -1) return inodo;
            }
        }
    }
    return -1;
}

list<string> REN::separar_carpetas(string path) {
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

bool REN::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR REN::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

string REN::nombre_archivo(const char *path) {
    string nombre_char = path;
    string nombre_disco;
    for (int i = nombre_char.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}

int REN::posicion_journal(char const *path, int partStart) {
    journal journal_actual;
    FILE *file;
    file = fopen(path, "rb+");
    fseek(file, partStart + sizeof(super_bloque), SEEK_SET);
    fread(&journal_actual, sizeof(journal), 1, file);
    fclose(file);

    int posicion_actual = 0;
    while (true) {
        posicion_actual = journal_actual.posicion;
        if(posicion_actual >=35) return -1;
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

void REN::add_to_journal(char const *path, int partStart) {
    journal nuevo;
    nuevo.posicion = posicion_journal(path, partStart);
    if(nuevo.posicion == -1) return;
    strcpy(nuevo.tipo_operacion, "ren");
    strcpy(nuevo.path, this->path.c_str());
    nuevo.log_fecha = time(0);
    nuevo.tipo = '5';
    FILE *file;
    file = fopen(path, "rb+");
    fseek(file, partStart + sizeof(super_bloque) + sizeof(journal) * nuevo.posicion, SEEK_SET);
    fwrite(&nuevo, sizeof(journal), 1, file);
    fclose(file);
}