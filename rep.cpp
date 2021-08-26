//
// Created by mario on 11/08/21.
//

#include <fstream>
#include <algorithm>
#include "rep.h"

void REP::setPath(bool comillas, string path_set) {
    if (comillas) {
        this->path = path_set.substr(1, path_set.length() - 2);
    } else {
        this->path = path_set;
    }
}

void REP::setName(string name_set) {
    this->name = name_set;
}

void REP::setRoot(int indice) {
    this->root = indice;
}

void REP::setID(string id) {
    this->id = id;
}

void REP::setRuta(string ruta) {
    this->ruta = ruta;
}

void REP::rep() {
    if (this->name == "mbr") {
        graficar_MBR();
    } else if (this->name == "disk") {
        graficar_disk();
    }
}

void REP::graficar_MBR() {
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

    if (discos_montados[disk_pos].estado == 0 || discos_montados[disk_pos].particiones[part_pos].estado == 0) {
        cout << "ERROR: el disco indicado no esta montado." << endl;
        return;
    }

    //se verifica que el disco exista
    if (!verificar_disco(discos_montados[disk_pos].path)) return;

    FILE *recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    MBR mbr_leido;
    fread(&mbr_leido, sizeof(MBR), 1, recuperar);

    int nodo = 0; //se utiliza para darle nombre a los nodos tabla

    int posicion_extendida = -1;
    string nombre_d = nombre_disco(discos_montados[disk_pos].path);
    string graph = "digraph html {\n" + to_string(nodo) +
                   " [shape=box, margin=0, label=<\n"
                   "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">";

    char fechayhora[16];
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&mbr_leido.mbr_fecha_creacion));
    string fecha_hora = fechayhora;
    graph += "<tr><td>mbr_tamano</td><td>" + to_string(mbr_leido.mbr_tamano) + "\n";
    graph += "</td></tr><tr><td>mbr_fecha_creacion:</td><td>" + fecha_hora + "\n";
    graph += "</td></tr><tr><td>mbr_disk_signture:</td><td>" + to_string(mbr_leido.mbr_disk_signature) + "\n";
    graph += "</td></tr><tr><td>Disk_fit:</td><td>" + mbr_leido.disk_fit;
    graph += "</td></tr>";

    for (int i = 0; i < 4; ++i) {
        if (mbr_leido.particiones[i].part_status == 'a') {
            //se guarda
            if (mbr_leido.particiones[i].part_type == 'e') posicion_extendida = i;
            string num = to_string(i + 1);
            graph += "<tr><td>part_name_" + num + "</td><td>" + mbr_leido.particiones[i].part_name + "\n";
            graph += "</td></tr><tr><td>part_status_" + num + "</td><td>" + mbr_leido.particiones[i].part_status + "\n";
            graph += "</td></tr><tr><td>part_type_" + num + "</td><td>" + mbr_leido.particiones[i].part_type + "\n";
            graph += "</td></tr><tr><td>part_fit_" + num + "</td><td>" + mbr_leido.particiones[i].part_fit + "\n";
            graph += "</td></tr><tr><td>part__start_" + num + "</td><td>" +
                     to_string(mbr_leido.particiones[i].part_start) + "\n";
            graph +=
                    "</td></tr><tr><td>part_size_" + num + "</td><td>" + to_string(mbr_leido.particiones[i].part_size) +
                    "</td></tr>";

            if (mbr_leido.particiones[i].part_type == 'e') posicion_extendida = i;
        }
    }
    nodo++;
    graph = graph + "\n</TABLE>> xlabel=\"MBR " + nombre_d + "\"]" + "\n";

    if (posicion_extendida != -1) {
        //si es extendida se busca entre las logicas
        EBR ebr_actual;
        ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[posicion_extendida].part_start);
        int cont_ebr = 1;
        while (true) {
            graph += to_string(nodo) +
                     " [shape=box, margin=0, label=<\n"
                     "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">";
            graph = graph + "<tr><td>Nombre</td><td>Valor</td></tr>";
            graph = graph + "<tr><td>part_status_" + to_string(cont_ebr) + "</td><td>" + ebr_actual.part_status +
                    "</td></tr>" + "\n";
            graph = graph + "<tr><td>part_fit_" + to_string(cont_ebr) + "</td><td>" + ebr_actual.part_fit +
                    "</td></tr>" + "\n";
            graph = graph + "<tr><td>part_size_" + to_string(cont_ebr) + "</td><td>" + to_string(ebr_actual.part_size) +
                    "</td></tr>" + "\n";
            graph = graph + "<tr><td>part_start_" + to_string(cont_ebr) + "</td><td>" +
                    to_string(ebr_actual.part_start) +
                    "</td></tr>" + "\n";
            graph = graph + "<tr><td>part_next_" + to_string(cont_ebr) + "</td><td>" + to_string(ebr_actual.part_next) +
                    "</td></tr>" + "\n";
            graph = graph + "<tr><td>part_name_" + to_string(cont_ebr) + "</td><td>" + ebr_actual.part_name +
                    "</td></tr>";
            graph = graph + "</TABLE>> xlabel=\"EBR_" + to_string(cont_ebr) + "\"]" + "\n";

            if (ebr_actual.part_next == -1) break;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
            nodo++;
            cont_ebr++;
        }
    }

    if (nodo > 1) {
        for (int i = 0; i < nodo; i++) {
            graph += to_string(i) + "->" + to_string(i + 1) + "[style=invisible arrowhead=none];\n";
        }
    }

    graph += "}";
    archivo_dot(graph, "MBR");
}

void REP::graficar_disk() {
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

    if (discos_montados[disk_pos].estado == 0 || discos_montados[disk_pos].particiones[part_pos].estado == 0) {
        cout << "ERROR: el disco indicado no esta montado." << endl;
        return;
    }

    //se verifica que el disco exista
    if (!verificar_disco(discos_montados[disk_pos].path)) return;

    FILE *recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    MBR mbr_leido;
    fread(&mbr_leido, sizeof(MBR), 1, recuperar);

    float porcentaje_ocupado = 0;
    int cont_ebr = 0;
    float espacio_disco = mbr_leido.mbr_tamano - sizeof(MBR);
    string tr_logicas;

    string graph = "digraph html {\nabc [shape=none, margin=0, label=<\n<TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLPADDING=\"4\">\n<TR><TD ROWSPAN=\"6\">MBR</TD>\n";

    //revisar si existe espacio libre al inicion
    int espacio_inicio = mbr_leido.particiones[0].part_start - sizeof(MBR);
    if (espacio_inicio > 0) {
        //se calcula cuanto del deisco es espacio libre
        porcentaje_ocupado = espacio_inicio * 100 / espacio_disco;
        graph += "<TD ROWSPAN=\"6\">Libre<BR/>" + to_string(porcentaje_ocupado) + "% del disco</TD>\n";
    }

    //for para recuperar la info de las particiones y graficarlas
    for (int i = 0; i < 4; ++i) {
        if (mbr_leido.particiones[i].part_status == 'a') {
            if (mbr_leido.particiones[i].part_type == 'p') {
                porcentaje_ocupado = mbr_leido.particiones[i].part_size * 100 / espacio_disco;
                graph += "<TD ROWSPAN=\"6\">Primaria<BR/>" + to_string(porcentaje_ocupado) + "% del disco</TD>\n";
            } else if (mbr_leido.particiones[i].part_type == 'e') {
                EBR ebr_actual;
                ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[i].part_start);
                while (true) {
                    tr_logicas += "<TD ROWSPAN=\"5\">EBR</TD>\n";
                    cont_ebr++;
                    int final_actual = ebr_actual.part_start + ebr_actual.part_size;
                    porcentaje_ocupado = (ebr_actual.part_size + sizeof(EBR)) * 100 / espacio_disco;
                    tr_logicas +=
                            "<TD ROWSPAN=\"5\">Logica<BR/>" + to_string(porcentaje_ocupado) + "% del disco</TD>\n";
                    cont_ebr++;
                    int espacio_entre = ebr_actual.part_next - final_actual;
                    if (espacio_entre > 0) {
                        porcentaje_ocupado = espacio_entre * 100 / espacio_disco;
                        tr_logicas +=
                                "<TD ROWSPAN=\"6\">Libre<BR/>" + to_string(porcentaje_ocupado) + "% del disco</TD>\n";
                        cont_ebr++;
                    }
                    if (ebr_actual.part_next == -1) {
                        int entre_final = mbr_leido.particiones[i].part_size - final_actual;
                        if (entre_final > 0) {
                            porcentaje_ocupado = entre_final * 100 / espacio_disco;
                            tr_logicas +=
                                    "<TD ROWSPAN=\"5\">Libre<BR/>" + to_string(porcentaje_ocupado) +
                                    "% del disco</TD>\n";
                            cont_ebr++;
                        }
                        break;
                    }
                    ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
                }
                graph += "<TD COLSPAN=\"" + to_string(cont_ebr) + "\">Extendida</TD>\n";
            }

            //se reviasa espacio libre entre particiones o final
            if (mbr_leido.particiones[i + 1].part_status == 'a') {
                int final_actual = mbr_leido.particiones[i].part_start + mbr_leido.particiones[i].part_size;
                int entre_particiones = mbr_leido.particiones[i + 1].part_start - final_actual;
                if (entre_particiones > 0) {
                    porcentaje_ocupado = entre_particiones * 100 / espacio_disco;
                    graph += "<TD ROWSPAN=\"6\">Libre<BR/>" + to_string(porcentaje_ocupado) + "% del disco</TD>\n";
                }
            } else {
                int final_actual = mbr_leido.particiones[i].part_start + mbr_leido.particiones[i].part_size;
                int final = mbr_leido.mbr_tamano - final_actual;
                if (final > 0) {
                    porcentaje_ocupado = final * 100 / espacio_disco;
                    graph += "<TD ROWSPAN=\"6\">Libre<BR/>" + to_string(porcentaje_ocupado) + "% del disco</TD>\n";
                }
                break;
            }
        }
    }
    if (!tr_logicas.empty()) {
        graph += "</TR>\n<TR>\n" + tr_logicas + "</TR>\n</TABLE>>];\n}";
    } else {
        graph += "</TR>\n</TABLE>>];\n}";
    }

    archivo_dot(graph, "disk");
}

bool REP::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == NULL) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

void REP::archivo_dot(string content, string name) {
    string path_carpeta = this->path.substr(0, this->path.length() - 4) + ".dot";
    FILE *searchFile = fopen(path_carpeta.c_str(), "wb+");

    if (searchFile == NULL) {
        string create_command = "mkdir -p \"" + this->path + "\"";
        system(create_command.c_str());
        create_command = "rmdir \'" + this->path + "\'";
        system(create_command.c_str());
    }

    ofstream graphFile;
    graphFile.open(path_carpeta);
    graphFile << content;
    graphFile.close();

    string filePath;
    string extension = toLowerCase(this->path.substr(this->path.length() - 3, this->path.length()));
    string salida = this->path.substr(0, this->path.length() - 3) + extension;
    if (extension == "png") {
        filePath = "dot -Tpng \"" + path_carpeta + "\" -o \"" + salida + "\"";
    } else if (extension == "jpg") {
        filePath = "dot -Tjpg \"" + path_carpeta + "\" -o \"" + salida + "\"";
    } else if (extension == "pdf") {
        filePath = "dot -Tpdf \"" + path_carpeta + "\" -o \"" + salida + "\"";
    }
    system(filePath.c_str());
    cout << "Reporte Generado con éxito." << endl;
}

string REP::toLowerCase(string sl) {
    string low_str = sl;
    transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
    return low_str;
}

EBR REP::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

string REP::nombre_disco(const char *path) {
    string nombre_char = path;
    string nombre_disco;
    for (int i = nombre_char.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}