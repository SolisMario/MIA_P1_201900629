//
// Created by mario on 29/08/21.
//

#include "exec.h"
#include "parser.h"
#include "scanner.h"

void EXEC::set_path(bool comillas, string path) {
    if (comillas) {
        this->path = path.substr(1, path.length() - 2);
    } else {
        this->path = path;
    }
}

void EXEC::exec() {
    if (this->path.empty()){
        cout << "ERROR el  parametro path es obligatorio." << endl;
        return;
    }

    FILE *file = NULL;
    file = fopen(this->path.c_str(), "r");
    if(file == NULL){
        cout << "ERROR La ruta del archivo es incorrecta o no existe." << endl;
        fclose(file);
        return;
    }
    fclose(file);

    ifstream script(this->path);
    string str;
    while (getline(script, str)) {
        cout << "----------------------------------------------------------------------------" << endl;
        std::cout << str << endl;
        if (str != "exit") {
            if (str != "") {
                YY_BUFFER_STATE buffer = yy_scan_string(str.c_str());

                if (yyparse() == 0) {
                    printf("\nComando ejecutado correctamente\n\n");
                } else {
                    printf("\nNo se pudo ejecutar el comando\n\n");
                }
                //yy_delete_buffer(buffer);
            }
        } else {
            exit(1);
        }
    }
}