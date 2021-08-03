//
// Created by mario on 2/08/21.
//

#include <iostream>
#include <algorithm>
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
    this->type = toLowerCase(fit_set);
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
    if (this->size <= 0) {
        cout << "ERROR al crear el disco: el parametro size debe ser positivo y mayor a 0." << endl;
    } else if (this->path.empty()) {
        cout << "ERROR el parametro path es obligatorio." << endl;
    } else if (this->name.empty()) {
        cout << "ERROR el parametro name es obligatorio." << endl;
    } else if (this->unit != "m" && this->unit != "k" && this->unit != "b") {
        cout << "ERROR al crear el disco: el tipo de unit no es correcto." << endl;
    } else if (this->fit != "bf" && this->fit != "ff" && this->fit != "wf") {
        cout << "ERROR al crear el disco: el tipo de fit no es correcto." << endl;
    } else if (this->type != "p" && this->type != "e" && this->type != "l") {
        cout << "ERROR al crear el disco: el tipo de fit no es correcto." << endl;
    }

    //recuperar info del mbr

}

string FDISK::toLowerCase(string sl) {
    string low_str = sl;
    transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
    return low_str;
}