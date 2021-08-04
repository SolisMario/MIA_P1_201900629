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
    this->fit = toLowerCase(fit_set);
}

void FDISK::setDelete(string type) {
    this->delete_ = toLowerCase(type);
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
    if (this->size <= 0) {
        cout << "ERROR al crear el disco: el parametro size debe ser positivo y mayor a 0." << endl;
        return;
    } else if (this->path.empty()) {
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
        cout << "Se quiere borrar la particion, aun no listo." << endl;
    } else if (this->add != 0) {
        cout << "Se quiere expandir o reducir una particion, aun no listo." << endl;
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
                cout << "se encontro una extendida";
                extendida_encontrada = true;
            }
        }
        if (!extendida_encontrada) {
            cout << "ERROR al crear la particion: la particion logica necesita una particion extendida." << endl;
            return;
        }
    }

    int particiones_activas = 0;
    if (this->type == "p" || this->type == "e") {
        for(auto & particione : mbr_leido.particiones) {
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
    for(auto & particione : mbr_leido.particiones) {
        espacio_ocupado = espacio_ocupado + particione.part_size;
    }
    int espacio_libre = mbr_leido.mbr_tamano - espacio_ocupado - sizeof(MBR);
    if(espacio_libre <= 0 && espacio_libre >= this->size){
        cout << "ERROR al crear la particion: no hay suficiente espacio." << endl;
        return;
    }

    //-----------------------------FALTA CALCULAR ESPACIO DE LAS LOGICAS-----------------------------------------------

    char nombre_particion[16];
    strcpy(nombre_particion, this->name.c_str());
    //si hay espacio suficiente se busca si el nombre existe
    for (auto & particione : mbr_leido.particiones) {
        cout << nombre_particion << endl;
        cout << particione.part_name << endl;
        if (nombre_particion == particione.part_name){
            cout << "ERROR al crear la particion: ya existe una particion con este nombre." << endl;
            return;
        }
    }

    //si el nombre no existe se procede a verificar el fit
    int particion_seleccionada = 0;
    int inicio_particion = 0;
    if(particiones_activas == 0){
        inicio_particion = sizeof(MBR);
    }
    mbr_leido.particiones[particion_seleccionada].part_status = 'a';
    if(this->type=="p"){
        mbr_leido.particiones[particion_seleccionada].part_type = 'p';
    } else if(this->type=="e"){
        mbr_leido.particiones[particion_seleccionada].part_type = 'e';
    } else if(this->type=="l"){
        mbr_leido.particiones[particion_seleccionada].part_type = 'l';
    }
    if(this->fit=="ff"){
        mbr_leido.particiones[particion_seleccionada].part_type = 'f';
    } else if(this->type=="bf"){
        mbr_leido.particiones[particion_seleccionada].part_type = 'b';
    } else if(this->type=="wf"){
        mbr_leido.particiones[particion_seleccionada].part_type = 'w';
    }
    mbr_leido.particiones[particion_seleccionada].part_start = inicio_particion;
    mbr_leido.particiones[particion_seleccionada].part_size = this->size;
    strcpy(mbr_leido.particiones[particion_seleccionada].part_name, this->name.c_str());
    cout << mbr_leido.particiones[particion_seleccionada].part_name << endl;

    FILE *bfile = fopen(this->path.c_str(), "rb+");
    rewind(bfile);
    fwrite(&mbr_leido, sizeof(MBR), 1, bfile);
    fclose(bfile);
    cout << "Particion creada con exito" << endl;
    cout << "Nueva particion: \nSize: " << mbr_leido.particiones[particion_seleccionada].part_size << "\nType: " << mbr_leido.particiones[particion_seleccionada].part_type << "\nFit: " << mbr_leido.particiones[particion_seleccionada].part_fit << "\nStart: " << mbr_leido.particiones[particion_seleccionada].part_start << "\nSize: " << mbr_leido.particiones[particion_seleccionada].part_size << "\nName: " << mbr_leido.particiones[particion_seleccionada].part_name << endl;

    char fechayhora[16];
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&mbr_leido.mbr_fecha_creacion));
    cout << "\n-------------DISCO RECUPERADO--------------------" << endl;
    cout << "Fecha de creacion: " << fechayhora << endl;
    cout << "Signature: " << mbr_leido.mbr_disk_signature << endl;
    cout << "Tamaño: " << mbr_leido.mbr_tamano << endl;
    cout << "Fit: " << mbr_leido.disk_fit << endl;
}

MBR FDISK::leer_MBR(char const *ruta) {
    FILE *file_struct;
    file_struct = fopen(ruta, "r");
    MBR mbr_recuperado;
    fread(&mbr_recuperado, sizeof(MBR), 1, file_struct);
    fclose(file_struct);
    return mbr_recuperado;
}

string FDISK::toLowerCase(string sl) {
    string low_str = sl;
    transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
    return low_str;
}