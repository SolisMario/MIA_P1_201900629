//
// Created by mario on 2/08/21.
//

#include <iostream>
#include <algorithm>
#include <cstring>
#include "fdisk.h"

void FDISK::setSize(int size_set) {
    this->size = size_set;
}

void FDISK::setUnit(string unit_set) {
    this->unit = toLowerCase(unit_set);
}

void FDISK::setPath(bool comillas, const string &path_set) {
    if (comillas) {
        this->path = path_set.substr(1, path_set.length() - 2);
    } else {
        this->path = path_set;
    }
}

void FDISK::setType(const string &type) {
    this->type = toLowerCase(type);
}

void FDISK::setFit(string fit_set) {
    cout << fit_set << endl;
    this->fit = toLowerCase(fit_set);
    cout << this->fit << endl;
}

void FDISK::setDelete(string delete_) {
    this->delete_ = toLowerCase(delete_);
}

void FDISK::setName(bool comillas, const string &name_set) {
    if (comillas) {
        this->name = name_set.substr(1, name_set.length() - 2);
    } else {
        this->name = name_set;
    }
}

void FDISK::setAdd(int add) {
    this->add = add;
}

void FDISK::fdisk() {
    //comrpobar que los parametros sean correctos
    if (this->path.empty()) {
        cout << "ERROR el parametro path es obligatorio." << endl;
        return;
    } else if (this->name.empty()) {
        cout << "ERROR el parametro name es obligatorio." << endl;
        return;
    } else if (this->unit != "m" && this->unit != "k" && this->unit != "b") {
        cout << "ERROR al crear el disco: el tipo de unit no es correcto." << endl;
        return;
    } else if (this->fit != "bf" && this->fit != "ff" && this->fit != "wf") {
        cout << "ERROR al crear el disco: el tipo de fit no es correcto." << endl;
        return;
    } else if (this->type != "p" && this->type != "e" && this->type != "l") {
        cout << "ERROR al crear el disco: el tipo de particion no es correcto." << endl;
        return;
    } else if (!this->delete_.empty()) {
        delete_partition(this->name, this->path, this->delete_);
        return;
    } else if (this->add != 0) {
        add_size(this->add);
        return;
    } else if (this->size <= 0) {
        cout << "ERROR al crear el disco: el parametro size debe ser positivo y mayor a 0." << endl;
        return;
    }

    //verificar si el disco existe
    char sc[this->path.size() + 1];
    strcpy(sc, this->path.c_str());

    FILE *file_struct;
    file_struct = fopen(sc, "r"); //r= read = si el disco ya existia
    if (file_struct == NULL) {
        cout << "El disco no existe." << endl;
        return;
    }
    fclose(file_struct);

    //si existe el disco se recupera el MBR
    MBR mbr_leido = leer_MBR(sc);

    //se dimensiona el size
    if (this->unit == "k") {
        this->size = this->size * 1024;
    } else if (this->unit == "m") {
        this->size = this->size * 1024 * 1024;
    }
    //si se desea crear una particion primaria se revisa que haya una de las 4 libres
    //si es extendida no debe haber otra extendida ni estar ocupadas las 4
    //si es una logica debe haber una extendida
    if (this->type == "e") {
        for (auto &particione : mbr_leido.particiones) {
            if (particione.part_type == 'e') {
                cout << "ERROR al crear la particion: maximo de particiones primarias alcanzado." << endl;
                return;
            }
        }
    } else if (this->type == "l") {
        bool extendida_encontrada = false;
        for (auto &particione : mbr_leido.particiones) {
            if (particione.part_type == 'e') {
                create_logic_partition(mbr_leido, particione);
                mostrar_particiones(mbr_leido);
                return;
            }
        }
        cout << "ERROR al crear la particion: la particion logica necesita una particion extendida." << endl;
        return;
    }

    int particiones_activas = 0;
    if (this->type == "p" || this->type == "e") {
        for (auto &particione : mbr_leido.particiones) {
            if (particione.part_status == 'a') {
                particiones_activas++;
            }
        }
        if (particiones_activas >= 4) {
            cout << "ERROR al crear la particion: maximo de particiones primarias y/o extendidas alcanzado." << endl;
            return;
        }
    }

    //si se pueden crear las particiones se verifica que haya espacio libre
    int espacio_ocupado = 0;
    for (auto &particione : mbr_leido.particiones) {
        espacio_ocupado = espacio_ocupado + particione.part_size;
    }
    int espacio_libre = mbr_leido.mbr_tamano - espacio_ocupado - sizeof(MBR);
    if (espacio_libre <= 0 && espacio_libre >= this->size) {
        cout << "ERROR al crear la particion: no hay suficiente espacio." << endl;
        return;
    }

    char nombre_particion[16];
    strcpy(nombre_particion, this->name.c_str());
    //si hay espacio suficiente se busca si el nombre existe
    for (auto &particione : mbr_leido.particiones) {
        if (strcmp(nombre_particion, particione.part_name) == 0 && particione.part_status == 'a') {
            cout << "ERROR al crear la particion: ya existe una particion con este nombre." << endl;
            return;
        }
    }

    //se revisa que particion esta libre
    int particion_seleccionada = 0;
    for (int i = 0; i < 4; ++i) {
        if (mbr_leido.particiones[i].part_status == 'i') {
            particion_seleccionada = i;
            break;
        }
    }
    //si el nombre no existe se procede a verificar el fit, si es la primera particion que se crea no se calcula start
    int inicio_particion = 0;

    if (particiones_activas == 0) {
        inicio_particion = sizeof(MBR);
    } else {
        inicio_particion = set_start(mbr_leido);
        if (inicio_particion == -1) {
            cout << "No se encontro un bloque de tamano suficiente para la particion" << endl;
            return;
        }
    }

    //se modifican los atributos de la particion
    mbr_leido.particiones[particion_seleccionada].part_status = 'a';
    if (this->type == "p") {
        mbr_leido.particiones[particion_seleccionada].part_type = 'p';
    } else if (this->type == "e") {
        mbr_leido.particiones[particion_seleccionada].part_type = 'e';
    } else if (this->type == "l") {
        mbr_leido.particiones[particion_seleccionada].part_type = 'l';
    }
    if (this->fit == "ff") {
        mbr_leido.particiones[particion_seleccionada].part_fit = 'f';
    } else if (this->fit == "bf") {
        mbr_leido.particiones[particion_seleccionada].part_fit = 'b';
    } else if (this->fit == "wf") {
        mbr_leido.particiones[particion_seleccionada].part_fit = 'w';
    }
    mbr_leido.particiones[particion_seleccionada].part_start = inicio_particion;
    mbr_leido.particiones[particion_seleccionada].part_size = this->size;
    strcpy(mbr_leido.particiones[particion_seleccionada].part_name, this->name.c_str());

    mover_vacias(mbr_leido);
    ordenar_particiones(mbr_leido);

    //se reescribe el mbr en el archivo con la particion creada
    FILE *reescribir = fopen(this->path.c_str(), "rb+");
    rewind(reescribir);
    fwrite(&mbr_leido, sizeof(MBR), 1, reescribir);
    //si se crea una particion extendida se escribe el ebr inicial
    if (this->type == "e") {
        EBR ebr_inicial;
        fseek(reescribir, inicio_particion, SEEK_SET);
        fwrite(&ebr_inicial, sizeof(EBR), 1, reescribir);
    }
    fclose(reescribir);
    cout << "Particion creada con exito" << endl;
    cout << endl;
    mostrar_particiones(mbr_leido);
}

MBR FDISK::leer_MBR(char const *ruta) {
    FILE *file_struct;
    file_struct = fopen(ruta, "r");
    MBR mbr_recuperado;
    fread(&mbr_recuperado, sizeof(MBR), 1, file_struct);
    fclose(file_struct);
    return mbr_recuperado;
}

int FDISK::set_start(MBR &mbr) {
    int start = -1;
    int espacio_tmp = 0;
    int espacio_inicio = mbr.particiones[0].part_start - sizeof(MBR);
    bool ultima_part = false;

    //si el espacio inicio es mayor a 0 sinifica que hay espacio entre el MBR y la primera particion
    if (espacio_inicio >= this->size) {
        espacio_tmp = espacio_inicio;
        start = sizeof(MBR);
    }
    //si el fit es el first se asigna este espacio de una vez
    if (mbr.disk_fit == 'F' && espacio_tmp > 0) {
        return start;
    }

    for (int i = 0; i < 4; ++i) {
        int fin_part_actual = mbr.particiones[i].part_start + mbr.particiones[i].part_size;
        int espacio_libre = 0;
        //si la siguiente particion esta activa se calcula el espacio libre entre la particion actual y siguiente
        //de no ser asi significa que es el final de la lista, se calcula el espacio de esta al final del disco
        if (mbr.particiones[i + 1].part_status == 'a') {
            espacio_libre = mbr.particiones[i + 1].part_start - fin_part_actual;
        } else {
            espacio_libre = mbr.mbr_tamano - fin_part_actual;
            ultima_part = true;
        }
        if (espacio_libre >= this->size) {
            //segun el ajuste
            if (mbr.disk_fit == 'B') {
                if (espacio_tmp > espacio_libre || espacio_tmp <= 0) {
                    espacio_tmp = espacio_libre;
                    start = fin_part_actual;
                }
            } else if (mbr.disk_fit == 'W') {
                if (espacio_tmp < espacio_libre || espacio_tmp <= 0) {
                    espacio_tmp = espacio_libre;
                    start = fin_part_actual;
                }
            } else if (mbr.disk_fit == 'F') {
                start = fin_part_actual;
                break;
            }

            if (ultima_part) {
                break;
            }
        }
    }
    return start;
}

void FDISK::mostrar_particiones(MBR &mbr) {
    cout << "lista de particiones" << endl;
    for (auto &particion: mbr.particiones) {
        if (particion.part_status == 'a') {
            cout << "\nType: "
                 << particion.part_type << "\nFit: "
                 << particion.part_fit << "\nStart: "
                 << particion.part_start << "\nSize: "
                 << particion.part_size << "\nName: "
                 << particion.part_name << "\nStatus: "
                 << particion.part_status << endl;

            if (particion.part_type == 'e') {
                EBR ebr_inicial;
                char sc[this->path.size() + 1];
                strcpy(sc, this->path.c_str());
                FILE *file_struct;
                file_struct = fopen(sc, "rb+");
                fseek(file_struct, particion.part_start, SEEK_SET);
                fread(&ebr_inicial, sizeof(EBR), 1, file_struct);
                while (true) {
                    if (ebr_inicial.part_status == 'a') {
                        cout << "\nFit: "
                             << ebr_inicial.part_fit << "\nStart: "
                             << ebr_inicial.part_start << "\nSize: "
                             << ebr_inicial.part_size << "\nName: "
                             << ebr_inicial.part_name << "\nStatus: "
                             << ebr_inicial.part_status << "\nNext: "
                             << ebr_inicial.part_next << endl;
                    }

                    if (ebr_inicial.part_next == -1) break;
                    fseek(file_struct, ebr_inicial.part_next, SEEK_SET);
                    fread(&ebr_inicial, sizeof(EBR), 1, file_struct);
                }
                fclose(file_struct);
            }
        }
    }
}

string FDISK::toLowerCase(string sl) {
    string low_str = sl;
    transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
    return low_str;
}

void FDISK::mover_vacias(MBR &mbr) {
    partition_ aux_part;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (mbr.particiones[j].part_start == 0) {
                aux_part = mbr.particiones[j];
                mbr.particiones[j] = mbr.particiones[j + 1];
                mbr.particiones[j + 1] = aux_part;
            }
        }
    }
}

void FDISK::ordenar_particiones(MBR &mbr) {
    partition_ aux_part;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if ((mbr.particiones[j].part_start > 0) && (mbr.particiones[j + 1].part_start > 0)) {
                if (mbr.particiones[j].part_start > mbr.particiones[j + 1].part_start) {
                    aux_part = mbr.particiones[j];
                    mbr.particiones[j] = mbr.particiones[j + 1];
                    mbr.particiones[j + 1] = aux_part;
                }
            }
        }
    }
}

void FDISK::delete_partition(string name, string path, string delete_type) {
    char sc[this->path.size() + 1];
    strcpy(sc, this->path.c_str());

    FILE *file_struct;
    file_struct = fopen(sc, "rb+"); //r= read = si el disco ya existia
    if (file_struct == NULL) {
        cout << "El disco no existe." << endl;
        return;
    }
    fclose(file_struct);

    //si existe el disco se recupera el MBR
    MBR mbr_leido = leer_MBR(sc);

    char nombre_particion[16];
    strcpy(nombre_particion, this->name.c_str());
    for (auto &particione : mbr_leido.particiones) {
        if (strcmp(nombre_particion, particione.part_name) == 0) {
            cout << "Se encontro la particion " << this->name << " en el disco " << this->path << endl;
            cout << "Por favor confirme que desea eliminar la particion[S/n]." << endl;
            string confirmacion;
            getline(cin, confirmacion);
            if (confirmacion == "s" || confirmacion == "S") {
                if (delete_type == "full") {
                    int inicio_part = particione.part_start;
                    int final_part = particione.part_start + particione.part_size;
                    //se llena de 0 el espacio del archivo que ocupaba la particion
                    file_struct = fopen(sc, "rb+");
                    for (int i = inicio_part; i < final_part; ++i) {
                        fseek(file_struct, i, SEEK_SET);
                        fwrite("\0", 1, 1, file_struct);
                        fflush(file_struct);
                    }
                    fclose(file_struct);
                }
                //se marca como vacio el pacio en la tabla de particiones
                particione.part_status = 'i';
                particione.part_start = 0;
                particione.part_size = 0;

                mover_vacias(mbr_leido);
                ordenar_particiones(mbr_leido);
                mostrar_particiones(mbr_leido);

                // se reescribe el archivo actualizado
                file_struct = fopen(sc, "rb+");
                rewind(file_struct);
                fwrite(&mbr_leido, sizeof(MBR), 1, file_struct);
                fclose(file_struct);
                return;
            } else {
                cout << "La eliminacion de la particion se ha cancelado." << endl;
                return;
            }
        } else if (particione.part_type == 'e') {
            //si es extendida se busca entre las logicas
            int posicion_eliminada = 0;
            EBR ebr_actual;
            EBR ebr_anterior;
            EBR ebr_siguiente;
            char sc[this->path.size() + 1];
            strcpy(sc, this->path.c_str());
            ebr_actual = leer_ebr(sc, particione.part_start);
            ebr_anterior = ebr_actual;

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0) {
                    cout << "Se encontro la particion " << this->name << " en el disco " << this->path << endl;
                    cout << "Por favor confirme que desea eliminar la particion[S/n]." << endl;
                    string confirmacion;
                    getline(cin, confirmacion);
                    if (confirmacion == "s") {
                        //debemos llamar ebr siguiente
                        if (ebr_actual.part_next != -1) {
                            //fseek(file_struct, ebr_actual.part_next, SEEK_SET);
                            //fread(&ebr_siguiente, sizeof(EBR), 1, file_struct);
                            ebr_siguiente = leer_ebr(sc, ebr_actual.part_next);
                        }
                        if (delete_type == "full") {
                            int inicio_part = ebr_actual.part_start - sizeof(EBR);
                            int final_part = inicio_part + ebr_actual.part_size;
                            //se llena de 0 el espacio del archivo que ocupaba la particion
                            file_struct = fopen(sc, "rb+");
                            for (int i = inicio_part; i < final_part; ++i) {
                                fseek(file_struct, i, SEEK_SET);
                                fwrite("\0", 1, 1, file_struct);
                                fflush(file_struct);
                            }
                            fclose(file_struct);
                        }
                        if (ebr_siguiente.part_status != 'i') {
                            ebr_anterior.part_next = ebr_siguiente.part_start - sizeof(EBR);
                        } else {
                            ebr_anterior.part_next = -1;
                        }
                        if (posicion_eliminada == 0) {
                            ebr_anterior.part_status = 'i';
                        }

                        // se reescribe el archivo actualizado
                        file_struct = fopen(sc, "rb+");
                        fseek(file_struct, ebr_anterior.part_start - sizeof(EBR), SEEK_SET);
                        fwrite(&ebr_anterior, sizeof(MBR), 1, file_struct);
                        fclose(file_struct);
                        return;
                    } else {
                        cout << "La eliminacion de la particion se ha cancelado." << endl;
                        return;
                    }
                }
                if (ebr_actual.part_next == -1) break;

                ebr_anterior = ebr_actual;
                ebr_actual = leer_ebr(sc, ebr_actual.part_next);
                //fseek(file_struct, ebr_actual.part_next, SEEK_SET);
                //fread(&ebr_actual, sizeof(EBR), 1, file_struct);
                posicion_eliminada++;
            }
            //fclose(file_struct);
        }
    }
    cout << "La particion requerida no existe." << endl;
}

void FDISK::create_logic_partition(MBR &mbr_leido, partition_ &part_extendida) {
    int espacio_inicio = 0; //espacio disponible entre el ebr inicial y la primer particion activa
    int espacio_tmp = -1; //espacio para comparar segun el fit
    int start = -1; //representa el inicio del nuevo ebr
    int part_next = -1;

    //se recupera el ebr inicial
    char sc[this->path.size() + 1];
    strcpy(sc, this->path.c_str());

    EBR ebr_inicial;
    ebr_inicial = leer_ebr(sc, part_extendida.part_start);

    //se busca el nombre para ver si existe
    char nombre_particion[16];
    strcpy(nombre_particion, this->name.c_str());
    EBR ebr_aux;
    ebr_aux = ebr_inicial;

    while (true) {
        if (strcmp(nombre_particion, ebr_aux.part_name) == 0) {
            cout << "Ya existe una particion logica con este nombre." << endl;
            return;
        }
        if (ebr_aux.part_next == -1) break;

        ebr_aux = leer_ebr(sc, ebr_aux.part_next);
    }

    //se revisa si el ebr inicial esta activo, si no se revisa el siguiente, si no hay es la primera logica
    if (ebr_inicial.part_status == 'i') {
        if (ebr_inicial.part_next == -1) {
            int final_ebr = part_extendida.part_start + sizeof(EBR);
            int espacio_logica_disponible = part_extendida.part_start + part_extendida.part_size - final_ebr;
            //si alcanza el espacio se modifica el ebr inicial
            if (espacio_logica_disponible >= this->size) {
                ebr_inicial.part_status = 'a';
                if (this->fit == "bf") {
                    ebr_inicial.part_fit = 'b';
                } else if (this->fit == "wf") {
                    ebr_inicial.part_fit = 'w';
                } else if (this->fit == "ff") {
                    ebr_inicial.part_fit = 'f';
                }
                ebr_inicial.part_start = final_ebr;
                ebr_inicial.part_size = this->size;
                ebr_inicial.part_next = -1;
                strcpy(ebr_inicial.part_name, this->name.c_str());

                FILE *reescribir = fopen(this->path.c_str(), "rb+");
                fseek(reescribir, part_extendida.part_start, SEEK_SET);
                fwrite(&ebr_inicial, sizeof(EBR), 1, reescribir);
                fclose(reescribir);

                cout << "se creo la particion con exito." << endl;
                return;
            } else {
                cout << "No hay suficiente espacio para crear la particion." << endl;
            }
        } else {
            //existe espacio vacio entre la primera logica activa y el ebr inicial
            int final_ebr = part_extendida.part_start + sizeof(EBR);
            espacio_inicio = part_extendida.part_start + part_extendida.part_size - final_ebr;
            if (espacio_inicio >= this->size) {
                ebr_inicial.part_size = 0;
                ebr_inicial.part_start = ebr_inicial.part_start - sizeof(EBR);
            }
        }
    }

    //si llega aqui es que hay al menos 1 particion logica creada
    EBR ebr_actual = ebr_inicial;
    EBR ebr_siguiente;
    EBR ebr_anterior; //ebr que contendra como part_next el inicio del nuevo ebr

    while (true) {
        if (ebr_actual.part_next == -1) {
            //significa que es el ultimo ebr
            cout << "es el ultimo ebr" << endl;
            int fin_part_actual = ebr_actual.part_start + ebr_actual.part_size;
            int entre_final = part_extendida.part_start + part_extendida.part_size - fin_part_actual;
            int espacio_util = entre_final - sizeof(EBR);

            if (this->size <= espacio_util) {
                if (mbr_leido.disk_fit == 'B') {
                    if (espacio_tmp > entre_final || espacio_tmp <= 0) {
                        espacio_tmp = entre_final;
                        start = fin_part_actual;
                        ebr_anterior = ebr_actual;
                        part_next = ebr_actual.part_next;
                    }
                } else if (mbr_leido.disk_fit == 'W') {
                    if (espacio_tmp < entre_final || espacio_tmp <= 0) {
                        espacio_tmp = entre_final;
                        start = fin_part_actual;
                        ebr_anterior = ebr_actual;
                        part_next = ebr_actual.part_next;
                    }
                } else if (mbr_leido.disk_fit == 'F') {
                    espacio_tmp = entre_final;
                    start = fin_part_actual;
                    ebr_anterior = ebr_actual;
                    part_next = ebr_actual.part_next;
                }
            }
            break;
        } else {
            //no es el ultimo ebr
            //se recupera el siguiente ebr

            ebr_siguiente = leer_ebr(sc, ebr_actual.part_next);

            //se revisa el espacio entre particiones
            int fin_part_actual = ebr_actual.part_start + ebr_actual.part_size;
            int entre_particiones = ebr_siguiente.part_start - sizeof(EBR) - fin_part_actual;

            int tam_part = this->size;
            int espacio_util = entre_particiones - sizeof(EBR);
            if (tam_part <= espacio_util) {
                if (mbr_leido.disk_fit == 'B') {
                    if (espacio_tmp > entre_particiones || espacio_tmp <= 0) {
                        espacio_tmp = entre_particiones;
                        start = fin_part_actual;
                        ebr_anterior = ebr_actual;
                        part_next = ebr_actual.part_next;
                    }

                } else if (mbr_leido.disk_fit == 'W') {
                    if (espacio_tmp < entre_particiones || espacio_tmp <= 0) {
                        espacio_tmp = entre_particiones;
                        start = fin_part_actual;
                        ebr_anterior = ebr_actual;
                        part_next = ebr_actual.part_next;
                    }
                } else if (mbr_leido.disk_fit == 'F') {
                    espacio_tmp = entre_particiones;
                    start = fin_part_actual;
                    ebr_anterior = ebr_actual;
                    part_next = ebr_actual.part_next;
                    break;
                }
            }
        }
        ebr_actual = ebr_siguiente;
    }

    if (start > 0 && espacio_tmp > 0) {
        //se crea el nuevo ebr y se modifican sus datos
        EBR nuevo_ebr;
        nuevo_ebr.part_status = 'a';
        if (this->type == "bf") {
            nuevo_ebr.part_fit = 'b';
        } else if (this->type == "wf") {
            nuevo_ebr.part_fit = 'w';
        } else if (this->type == "ff") {
            nuevo_ebr.part_fit = 'f';
        }

        nuevo_ebr.part_start = start + sizeof(EBR);
        nuevo_ebr.part_size = this->size;
        nuevo_ebr.part_next = part_next;
        strcpy(nuevo_ebr.part_name, this->name.c_str());

        ebr_anterior.part_next = start;

        FILE *escribir_ebr = fopen(this->path.c_str(), "rb+");
        //reescribir el ebr anterior
        fseek(escribir_ebr, ebr_anterior.part_start - sizeof(EBR), SEEK_SET);
        fwrite(&ebr_anterior, sizeof(EBR), 1, escribir_ebr);
        //escribir el nuevo ebr
        fseek(escribir_ebr, ebr_anterior.part_next, SEEK_SET);
        fwrite(&nuevo_ebr, sizeof(EBR), 1, escribir_ebr);
        fclose(escribir_ebr);
        cout << "Particion creada con exito" << endl;
        return;
    }
    cout << "No fue posible crear la particion, no se encontro un bloque con espacio suficiente." << endl;
}

EBR FDISK::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

void FDISK::escribir_MBR(MBR &mbr) {
    FILE *reescribir = fopen(this->path.c_str(), "rb+");
    rewind(reescribir);
    fwrite(&mbr, sizeof(MBR), 1, reescribir);
    fclose(reescribir);
}

void FDISK::escribir_EBR(EBR &ebr) {
    FILE *reescribir = fopen(this->path.c_str(), "rb+");
    fseek(reescribir, ebr.part_start - sizeof(EBR), SEEK_SET);
    fwrite(&ebr, sizeof(EBR), 1, reescribir);
    fclose(reescribir);
}

void FDISK::add_size(int size_change) {
    //se reivsa si el disco existe
    char sc[this->path.size() + 1];
    strcpy(sc, this->path.c_str());
    FILE *file_struct;
    file_struct = fopen(sc, "rb+");
    if (file_struct == NULL) {
        cout << "El disco no existe." << endl;
        return;
    }
    fclose(file_struct);

    //si existe el disco se recupera el MBR
    MBR mbr_leido = leer_MBR(sc);

    char nombre_particion[16];
    strcpy(nombre_particion, this->name.c_str());

    for (int i = 0; i < 4; i++) {
        if (strcmp(nombre_particion, mbr_leido.particiones[i].part_name) == 0) {
            if (size_change < 0) {
                int espacio_sobrante = mbr_leido.particiones[i].part_size + size_change;
                if (espacio_sobrante > 0) {
                    cout << "Por favor confirme que desea reducir la particion[S/n]." << endl;
                    string confirmacion;
                    getline(cin, confirmacion);
                    if (confirmacion == "s" || confirmacion == "S") {
                        mbr_leido.particiones[i].part_size = espacio_sobrante;
                        escribir_MBR(mbr_leido);
                        cout << "Reduccion realizada con exito." << endl;
                        return;
                    } else {
                        cout << "Se ha cancelado la reduccion." << endl;
                    }
                } else {
                    cout << "No se puede proceder con la reduccion: espacio insuficiente." << endl;
                }
            } else {
                int espacio_entre = 0;
                if (mbr_leido.particiones[i + 1].part_status == 'a') {
                    int final_particion = mbr_leido.particiones[i].part_start + mbr_leido.particiones[i].part_size;
                    espacio_entre = mbr_leido.particiones[i + 1].part_start - final_particion;
                } else {
                    int final_particion = mbr_leido.particiones[i].part_start + mbr_leido.particiones[i].part_size;
                    espacio_entre = mbr_leido.mbr_tamano - final_particion;
                }
                if (espacio_entre >= size_change) {
                    cout << "Por favor confirme que desea expandir la particion[S/n]." << endl;
                    string confirmacion;
                    getline(cin, confirmacion);
                    if (confirmacion == "s" || confirmacion == "S") {
                        mbr_leido.particiones[i].part_size = mbr_leido.particiones[i].part_size + size_change;
                        escribir_MBR(mbr_leido);
                        cout << "Expandion realizada con exito." << endl;
                        return;
                    } else {
                        cout << "Se ha cancelado la reduccion." << endl;
                    }
                } else {
                    cout << "No se puede realizar la expansion: espacio insuficiente." << endl;
                }
            }
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            int posicion_eliminada = 0;
            EBR ebr_actual;
            //EBR ebr_anterior;
            EBR ebr_siguiente;
            ebr_actual = leer_ebr(sc, mbr_leido.particiones[i].part_start);
            //ebr_anterior = ebr_actual;

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0) {
                    if (size_change < 0) {
                        int espacio_sobrante = ebr_actual.part_size + size_change;
                        if (espacio_sobrante > 0) {
                            cout << "Por favor confirme que desea reducir la particion[S/n]." << endl;
                            string confirmacion;
                            getline(cin, confirmacion);
                            if (confirmacion == "s" || confirmacion == "S") {
                                ebr_actual.part_size = espacio_sobrante;
                                escribir_EBR(ebr_actual);
                                cout << "Reduccion realizada con exito." << endl;
                                return;
                            } else {
                                cout << "Se ha cancelado la reduccion." << endl;
                            }
                        } else {
                            cout << "No se puede proceder con la reduccion: espacio insuficiente." << endl;
                        }
                    } else {
                        if (ebr_actual.part_next != -1) {
                            ebr_siguiente = leer_ebr(sc, ebr_actual.part_next);
                        }

                        int espacio_entre = 0;
                        if (ebr_actual.part_next != -1) {
                            int final_particion = ebr_actual.part_start + ebr_actual.part_size;
                            espacio_entre = ebr_siguiente.part_start - sizeof(EBR) - final_particion;
                        } else {
                            int final_particion = ebr_actual.part_start + ebr_actual.part_size;
                            espacio_entre = mbr_leido.particiones[i].part_start + mbr_leido.particiones[i].part_size - final_particion;
                        }
                        if (espacio_entre >= size_change) {
                            cout << "Por favor confirme que desea expandir la particion[S/n]." << endl;
                            string confirmacion;
                            getline(cin, confirmacion);
                            if (confirmacion == "s" || confirmacion == "S") {
                                ebr_actual.part_size = ebr_actual.part_size + size_change;
                                escribir_EBR(ebr_actual);
                                cout << "Expandion realizada con exito." << endl;
                                return;
                            } else {
                                cout << "Se ha cancelado la reduccion." << endl;
                            }
                        } else {
                            cout << "No se puede realizar la expansion: espacio insuficiente." << endl;
                        }
                    }
                }
                if (ebr_actual.part_next == -1) break;

                //ebr_anterior = ebr_actual;
                ebr_actual = leer_ebr(sc, ebr_actual.part_next);
                posicion_eliminada++;
            }
        }
    }
}
