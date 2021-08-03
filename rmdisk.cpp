//
// Created by mario on 2/08/21.
//

#include "rmdisk.h"
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

void RMDISK::setPath(bool comillas, string path_set) {
    if (comillas) {
        this->path = path_set.substr(1, path_set.length() - 2);
    } else {
        this->path = path_set;
    }
}

void RMDISK::rmdisk() {
    if (this->path.empty()) {
        cout << "ERROR al eliminar el disco: el parametro path es obligatorio." << endl;
    } else {
        char sc[this->path.size() + 1];
        strcpy(sc, this->path.c_str());

        FILE *verificar_existente;
        verificar_existente = fopen(sc, "r"); //r= read = si el disco ya existia
        if (verificar_existente == nullptr) {
            cout << "ERROR al eliminar el disco: el disco no existe." << endl;
            return;
        }

        string confirmacion;
        cout << "Confirme si desea eliminar el disco[s/n]" << endl;
        getline(cin, confirmacion);

        if (toLowerCase(confirmacion) == "s") {
            string command = "rm \'" + this->path + "\'";
            system(command.c_str());

            FILE *existe = fopen(this->path.c_str(), "r");
            if (existe != NULL) {
                cout << "ERROR al eliminar el disco." << endl;
                fclose(existe);
                return;
            }
            cout << "El disco se elimino exitosamente." << endl;
        } else if (toLowerCase(confirmacion) == "n") {
            cout << "Eliminacion cancelada" << endl;
        }
    }
}

string RMDISK::toLowerCase(string sl) {
    string low_str = sl;
    transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
    return low_str;
}