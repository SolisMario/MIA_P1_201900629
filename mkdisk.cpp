//
// Created by mario on 1/08/21.
//

#include "mkdisk.h"
#include "mbr.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

void MKDISK::setSize(int size_set) {
    this->size = size_set;
}

void MKDISK::setFit(string fit_set) {
    this->fit = toLowerCase(fit_set);
}

void MKDISK::setUnit(string unit_set) {
    this->unit = toLowerCase(unit_set);
}

void MKDISK::setPath(bool comillas, const string &path_set) {
    if (comillas) {
        this->path = path_set.substr(1, path_set.length() - 2);
    } else {
        this->path = path_set;
    }
}

/*
int MKDISK::getSize() {
    return this->size;
}

string MKDISK::getFit() {
    return this->fit;
}

string MKDISK::getUnit() {
    return this->unit;
}

string MKDISK::getPath() {
    return this->path;
}
*/

void MKDISK::mkdisk() {

    if (this->size <= 0) {
        cout << "ERROR al crear el disco: el parametro size debe ser positivo y mayor a 0." << endl;
        return;
    } else if (this->path.empty()) {
        cout << "ERROR al crear el disco: el parametro path es obligatorio." << endl;
        return;
    } else if (this->fit != "bf" && this->fit != "ff" && this->fit != "wf") {
        cout << "ERROR al crear el disco: el tipo de fit no es correcto." << endl;
        return;
    } else if (this->unit != "m" && this->unit != "k") {
        cout << "ERROR al crear el disco: el tipo de unit no es correcto." << endl;
        return;
    } else {
        char sc[this->path.size() + 1];
        strcpy(sc, this->path.c_str());

        FILE *verificar_existente;
        verificar_existente = fopen(sc, "r"); //r= read = si el disco ya existia
        if (verificar_existente != nullptr) {
            cout << "ERROR al crear el disco: disco ya existente." << endl;
            return;//error
        }

        if (this->unit == "k") { //kb
            this->size = this->size * 1024;
        } else { //mb
            this->size = this->size * 1024 * 1024;
        }

        MBR mbr;
        mbr.mbr_tamano = this->size;
        mbr.mbr_fecha_creacion = time(nullptr);
        mbr.mbr_disk_signature = rand() % 99999999 + 10000000;

        if (this->fit == "bf") {
            mbr.disk_fit = 'B';
        } else if (this->fit == "ff") {
            mbr.disk_fit = 'F';
        } else if (this->fit == "wf") {
            mbr.disk_fit = 'W';
        }

        FILE *file = fopen(sc, "wb+");
        if (file != nullptr) {
            fwrite("\0", 1, 1, file);
            fseek(file, this->size - 1, SEEK_SET); // objeto file, size de cuanto se quiere mover, al inicio
            fwrite("\0", 1, 1, file);
            rewind(file);
            fseek(file, 0, SEEK_SET);
            fwrite(&mbr, sizeof(MBR), 1, file);
            cout << "Disco creado con exito" << endl;
        } else {
            string command = "mkdir -p -m=777 \""+this->path+"\"";
            system(command.c_str());
            command = "rmdir \'" + this->path + "\'";
            system(command.c_str());

            file = fopen(sc, "wb+");
            fwrite("\0", 1, 1, file);
            fseek(file, this->size - 1, SEEK_SET); // objeto file, size de cuanto se quiere mover, al inicio
            fwrite("\0", 1, 1, file);
            rewind(file);
            fseek(file, 0, SEEK_SET);
            fwrite(&mbr, sizeof(MBR), 1, file);
            cout << "Disco creado con exito" << endl;
        }
        fclose(file);

        char fechayhora[16];
        strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&mbr.mbr_fecha_creacion));
        cout << "\n-------------DISCO CREADO--------------------" << endl;
        cout << "Fecha de creacion: " << fechayhora << endl;
        cout << "Signature: " << mbr.mbr_disk_signature << endl;
        cout << "Tamaño: " << mbr.mbr_tamano << endl;
        cout << "Fit: " << mbr.disk_fit << endl;

    }
}

string MKDISK::toLowerCase(string sl) {
    string low_str = sl;
    transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
    return low_str;
}