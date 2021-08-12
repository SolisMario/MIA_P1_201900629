//
// Created by mario on 10/08/21.
//

#include <string>
#include <iostream>
#include <cstring>
#include "mount.h"

void MOUNT::setPath(bool comillas, const string &path_set) {
    if (comillas) {
        this->path = path_set.substr(1, path_set.length() - 2);
    } else {
        this->path = path_set;
    }
}

void MOUNT::setName(bool comillas, const string &name_set) {
    if (comillas) {
        this->name = name_set.substr(1, name_set.length() - 2);
    } else {
        this->name = name_set;
    }
}

void MOUNT::setID(string ID) {
    this->id = ID;
}

void MOUNT::mount() {
    if (this->path.empty()) {
        cout << "ERROR: El parametro path es obligatorio." << endl;
        return;
    } else if (this->path.empty()) {
        cout << "ERROR: El parametro name es obligatorio." << endl;
        return;
    }

    //se verifica que el disco exista
    if (!verificar_disco(this->path.c_str())) return;

    int posicion_montar;
    char path_cstr[this->path.length() + 1];
    strcpy(path_cstr, this->path.c_str());
    for (int i = 0; i < 99; ++i) {
        //se revisa si existe espacio vacio o ya esta montado el disco
        if (discos_montados[i].estado == 0 || strcmp(discos_montados[i].path, path_cstr) == 0) {
            posicion_montar = i;
            discos_montados[i].estado = 1;
            strcpy(discos_montados[i].path, this->path.c_str());
            discos_montados[i].numero = i + 1;
            break;
        }

        if (i == 25) {
            cout << "No es posible montar la particion. Se alcanzo el maximo de discos montados.";
        }
    }

    //se revisa si la particion no esta montada
    char nombre_part[this->name.length() + 1];
    strcpy(nombre_part, this->name.c_str());
    for (auto &particione : discos_montados[posicion_montar].particiones) {
        if (strcmp(particione.nombre, nombre_part) == 0) {
            cout << "ERROR: esta particion se encuentra actualmente montada" << endl;
            return;
        }
    }

    FILE *recuperar = fopen(this->path.c_str(), "rb+");
    MBR mbr_leido;
    fread(&mbr_leido, sizeof(MBR), 1, recuperar);

    //se busca la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_part) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            //particion encontrada
            for (int j = 0; j < 26; ++j) {
                if (discos_montados[posicion_montar].particiones[j].estado == 0) {//si esta posicion no esta montada
                    strcpy(discos_montados[posicion_montar].particiones[j].nombre, this->name.c_str());
                    discos_montados[posicion_montar].particiones[j].estado = 1;
                    discos_montados[posicion_montar].particiones[j].letra = char(65 + j);
                    super_bloque superBloque;
                    fseek(recuperar, mbr_leido.particiones[i].part_start, SEEK_SET);
                    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
                    superBloque.s_mtime = time(nullptr);
                    fseek(recuperar, mbr_leido.particiones[i].part_start, SEEK_SET);
                    fwrite(&superBloque, sizeof(super_bloque), 1, recuperar);
                    fflush(recuperar);
                    cout << "La particion 29" << discos_montados[posicion_montar].numero
                         << discos_montados[posicion_montar].particiones[j].letra << " fue montada exitosamente."
                         << endl;
                    mostrar_montadas();
                    fclose(recuperar);
                    return;
                }
            }
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_part, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    for (int j = 0; j < 99; ++j) {
                        if (discos_montados[posicion_montar].particiones[j].estado == 0) {
                            //si esta posicion no esta montada
                            strcpy(discos_montados[posicion_montar].particiones[j].nombre, this->name.c_str());
                            discos_montados[posicion_montar].particiones[j].estado = 1;
                            discos_montados[posicion_montar].particiones[j].letra = char(65 + j);
                            super_bloque superBloque;
                            fseek(recuperar, ebr_actual.part_start, SEEK_SET);
                            fread(&superBloque, sizeof(super_bloque), 1, recuperar);
                            superBloque.s_mtime = time(0);
                            fseek(recuperar, ebr_actual.part_start, SEEK_SET);
                            fwrite(&superBloque, sizeof(super_bloque), 1, recuperar);
                            fflush(recuperar);
                            cout << "La particion 29" << discos_montados[posicion_montar].numero
                                 << discos_montados[posicion_montar].particiones[j].letra
                                 << " fue montada exitosamente." << endl;
                            mostrar_montadas();
                            fclose(recuperar);
                            return;
                        }
                    }
                }
                if (ebr_actual.part_next == -1) break;
                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
            }
        }
    }
    cout << "ERROR: La particion que se quiere montar no existe." << endl;
}

void MOUNT::umount() {
    if (this->id.empty()) {
        cout << "ERROR: El parametro id es obligatorio";
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

    //se verifica que el disco exista
    if (!verificar_disco(discos_montados[disk_pos].path)) return;

    FILE *recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    MBR mbr_leido;
    fread(&mbr_leido, sizeof(MBR), 1, recuperar);

    //se busca la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, discos_montados[disk_pos].particiones[part_pos].nombre) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            //particion encontrada
            discos_montados[disk_pos].particiones[part_pos].estado = 0;
            super_bloque superBloque;
            fseek(recuperar, mbr_leido.particiones[i].part_start, SEEK_SET);
            fread(&superBloque, sizeof(super_bloque), 1, recuperar);
            superBloque.s_umtime = time(0);
            fseek(recuperar, mbr_leido.particiones[i].part_start, SEEK_SET);
            fwrite(&superBloque, sizeof(super_bloque), 1, recuperar);
            fflush(recuperar);
            cout << "La particion " << this->id << " fue desmontada exitosamente." << endl;
            mostrar_montadas();
            fclose(recuperar);
            return;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(discos_montados[disk_pos].particiones[part_pos].nombre, ebr_actual.part_name) == 0 &&
                    ebr_actual.part_status == 'a') {
                            //si esta posicion no esta montada
                            discos_montados[disk_pos].particiones[part_pos].estado = 0;
                            super_bloque superBloque;
                            fseek(recuperar, ebr_actual.part_start, SEEK_SET);
                            fread(&superBloque, sizeof(super_bloque), 1, recuperar);
                            superBloque.s_umtime = time(0);
                            fseek(recuperar, ebr_actual.part_start, SEEK_SET);
                            fwrite(&superBloque, sizeof(super_bloque), 1, recuperar);
                            fflush(recuperar);
                            cout << "La particion " << this->id << " fue montada exitosamente." << endl;
                            mostrar_montadas();
                            fclose(recuperar);
                            return;
                }
                if (ebr_actual.part_next == -1) break;
                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
            }
        }
    }
    cout << "ERROR: La particion que se quiere montar no existe." << endl;
}

bool MOUNT::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == NULL) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR MOUNT::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

void MOUNT::mostrar_montadas() {
    for (auto &discos_montado : discos_montados) {
        if (discos_montado.estado == 1) {
            for (auto &particione : discos_montado.particiones) {
                if (particione.estado == 1) {
                    cout << discos_montado.path << "|" << particione.nombre << "|29"
                         << discos_montado.numero << particione.letra << endl;
                }
            }
        }
    }
}

