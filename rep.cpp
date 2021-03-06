//
// Created by mario on 11/08/21.
//

#include <fstream>
#include <algorithm>
#include "rep.h"
#include "users.h"

void REP::setPath(bool comillas, string path_set) {
    if (comillas) {
        this->path = path_set.substr(1, path_set.length() - 2);
    } else {
        this->path = path_set;
    }
}

void REP::setName(string name_set) {
    this->name = toLowerCase(name_set);
}

void REP::setRoot(int indice) {
    this->root = indice;
}

void REP::setID(string id) {
    this->id = id;
}

void REP::setRuta(bool comillas, string ruta) {
    if (comillas) {
        this->ruta = ruta.substr(1, ruta.length() - 2);
    } else {
        this->ruta = ruta;
    }
}

void REP::rep() {
    if (this->name == "mbr") {
        graficar_MBR();
    } else if (this->name == "disk") {
        graficar_disk();
    } else if (this->name == "tree") {
        graficar_tree();
    } else if (this->name == "inode") {
        graficar_tabla_inodos();
    } else if (this->name == "sb") {
        graficar_super();
    } else if (this->name == "block") {
        graficar_bloque();
    } else if (this->name == "bm_inode") {
        graficar_bitmap("Inodos");
    } else if (this->name == "bm_block") {
        graficar_bitmap("Bloques");
    } else if (this->name == "file") {
        graficar_file();
    } else if (this->name == "journaling") {
        graficar_journal();
    } else if (this->name == "ls") {
        graficar_ls();
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
    string fit(1, mbr_leido.disk_fit);
    graph += "</td></tr><tr><td>Disk_fit:</td><td>" + fit + "</td></tr>";

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
                        int entre_final =
                                mbr_leido.particiones[i].part_start + mbr_leido.particiones[i].part_size - final_actual;
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
    cout << "Reporte Generado con ??xito." << endl;
}

void REP::graficar_tree() {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    char bit_leido;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
    fread(&bit_leido, 1, 1, recuperar);
    fclose(recuperar);

    string graph = "digraph html {\nrankdir=LR;\nranksep=3;";
    if (bit_leido == '1') {
        graph += graficar_inodo(this->root, superBloque.s_inode_start, superBloque.s_block_start,
                                discos_montados[disk_pos].path);
    }
    graph += "}";

    archivo_dot(graph, "tree");
}

string REP::graficar_inodo(int indice, int inode_start, int block_start, char const *path) {
    string graph = "";
    tabla_inodos inodo;
    FILE *recuperar = fopen(path, "rb+");
    fseek(recuperar, inode_start + sizeof(tabla_inodos) * indice, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, recuperar);
    fclose(recuperar);

    string indice_inodo = to_string(indice);
    //dot de inodo
    char fechayhora[16];
    string fecha_hora = "";
    graph += "\ninodo" + indice_inodo + "[shape=none, margin=0, label=<\n";
    graph += R"(<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0" CELLPADDING="4">)";
    graph +=
            "\n<TR><TD BGCOLOR=\"#3498db\">INODO</TD><TD BGCOLOR=\"#3498db\">" + indice_inodo + "</TD></TR>";
    graph += "<TR><TD>UID:</TD><TD>" + to_string(inodo.i_uid) + "</TD></TR>\n";
    graph += "<TR><TD>GUID:</TD><TD>" + to_string(inodo.i_gid) + "</TD></TR>\n";
    graph += "<TR><TD>SIZE:</TD><TD>" + to_string(inodo.i_size) + "</TD></TR>\n";
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&inodo.i_atime));
    fecha_hora = fechayhora;
    graph += "<TR><TD>LECTURA:</TD><TD>" + fecha_hora + "</TD></TR>\n";
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&inodo.i_ctime));
    fecha_hora = fechayhora;
    graph += "<TR><TD>CREACION:</TD><TD>" + fecha_hora + "</TD></TR>\n";
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&inodo.i_mtime));
    fecha_hora = fechayhora;
    graph += "<TR><TD>MODIFICACION:</TD><TD>" + fecha_hora + "</TD></TR>\n";
    //FOR DE BLOQUES
    for (int i = 0; i < 15; ++i) {
        graph += "<TR><TD>BLOCK" + to_string(i) + "</TD><TD PORT=\"port" + to_string(i) + "\">" +
                 to_string(inodo.i_block[i]) + "</TD></TR>\n";
    }
    string tipo(1, inodo.i_type);
    graph += "<TR><TD>TIPO:</TD><TD>" + tipo + "</TD></TR>\n";
    graph += "<TR><TD>PERMISOS:</TD><TD>" + to_string(inodo.i_perm) + "</TD></TR>\n";
    graph += "</TABLE>>];\n";

    //for para graficar blques
    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            if (inodo.i_type == '0') {
                graph += graficar_bloque_carpeta(indice, inodo.i_block[i], inode_start, block_start, path);
            } else {
                graph += graficar_bloque_archivo(indice, inodo.i_block[i], inode_start, block_start, path);
            }
            graph += "inodo" + indice_inodo + ":port" + to_string(i) + "-> bloque" +
                     to_string(inodo.i_block[i]) + "\n";
        }
    }

    if (inodo.i_block[12] != -1) {
        graph += graficar_bloque_apuntadores(indice, inodo.i_block[12], inode_start, block_start, path,
                                             inodo.i_type, 1);
        graph += "inodo" + indice_inodo + ":port" + to_string(12) + "-> bloque" +
                 to_string(inodo.i_block[12]) + "\n";
    }

    if (inodo.i_block[13] != -1) {
        graph += graficar_bloque_apuntadores(indice, inodo.i_block[13], inode_start, block_start, path,
                                             inodo.i_type, 2);
        graph += "inodo" + indice_inodo + ":port" + to_string(13) + "-> bloque" +
                 to_string(inodo.i_block[13]) + "\n";
    }

    if (inodo.i_block[14] != -1) {
        graph += graficar_bloque_apuntadores(indice, inodo.i_block[14], inode_start, block_start, path,
                                             inodo.i_type, 3);
        graph += "inodo" + indice_inodo + ":port" + to_string(13) + "-> bloque" +
                 to_string(inodo.i_block[14]) + "\n";
    }
    return graph;
}

string
REP::graficar_bloque_carpeta(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path) {
    bloque_carpeta carpeta;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&carpeta, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    string graph = "";
    graph += "bloque" + to_string(indice_bloque) + "[shape=none, margin=0, label=<";
    graph += "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += R"(<TR><TD BGCOLOR="green">BLOCK</TD><TD BGCOLOR="green">)" + to_string(indice_bloque) + "</TD></TR>\n";
    graph += "<TR><TD>B_NAME</TD><TD>B_INODO</TD></TR>\n";

    for (int i = 0; i < 4; ++i) {
        string b_name = carpeta.b_content[i].b_name;
        if (b_name.empty() || carpeta.b_content[i].b_inodo == -1) {
            b_name = "-";
        } else if (b_name.length() > 12) {
            b_name = b_name.substr(0, 12);
        }
        graph += "<TR><TD>" + b_name + "</TD><TD PORT=\"port" + to_string(i) + "\">" +
                 to_string(carpeta.b_content[i].b_inodo) + "</TD></TR>\n";
    }
    graph += "</TABLE>>];\n";

    for (int i = 0; i < 4; ++i) {
        if (carpeta.b_content[i].b_inodo != -1) {
            if (strcmp(carpeta.b_content[i].b_name, ".") != 0 && strcmp(carpeta.b_content[i].b_name, "..") != 0) {
                graph += graficar_inodo(carpeta.b_content[i].b_inodo, inode_start, block_start, path);
                graph += "bloque" + to_string(indice_bloque) + ":port" + to_string(i) + "-> inodo" +
                         to_string(carpeta.b_content[i].b_inodo) + "\n";
            }
        }
    }
    return graph;
}

string
REP::graficar_bloque_archivo(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path) {
    bloque_archivos contenido;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&contenido, sizeof(bloque_archivos), 1, file);
    fclose(file);

    string graph = "";
    graph += "bloque" + to_string(indice_bloque) + "[shape=none, margin=0, label=<\n";
    graph += "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += R"(<TR><TD BGCOLOR="yellow">BLOCK</TD><TD BGCOLOR="yellow">)" + to_string(indice_bloque) + "</TD></TR>\n";
    string content = contenido.b_content;
    if (content.length() > 64) {
        content = content.substr(0, 64);
    }
    graph += "<TR><TD>" + content + "</TD><TD></TD></TR>\n";
    graph += "</TABLE>>];\n";
    return graph;
}

string REP::graficar_bloque_apuntadores(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                        char const *path, char tipo, int nivel) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    string graph = "";
    graph += "bloque" + to_string(indice_bloque) + "[shape=none, margin=0, label=<";
    graph += "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += R"(<TR><TD BGCOLOR="orange">BLOCK</TD><TD BGCOLOR="orange">)" + to_string(indice_bloque) + "</TD></TR>\n";
    graph += "<TR><TD></TD><TD>B_POINTER</TD></TR>\n";

    for (int i = 0; i < 16; ++i) {
        graph += "<TR><TD></TD><TD PORT=\"port" + to_string(i) + "\">" + to_string(apuntadores.b_pointers[i]) +
                 "</TD></TR>\n";
    }

    graph += "</TABLE>>];\n";

    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                if (tipo == '0') {
                    graph += graficar_bloque_carpeta(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                     path);
                } else {
                    graph += graficar_bloque_archivo(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                     path);
                }
                graph += "bloque" + to_string(indice_bloque) + ":port" + to_string(i) + "-> bloque" +
                         to_string(apuntadores.b_pointers[i]) + "\n";
            } else {
                graph += graficar_bloque_apuntadores(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                     path, tipo, nivel - 1);
                graph += "bloque" + to_string(indice_bloque) + ":port" + to_string(i) + "-> bloque" +
                         to_string(apuntadores.b_pointers[i]) + "\n";
            }
        }
    }
    return graph;
}

void REP::graficar_tabla_inodos() {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    char bit_leido;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
    fread(&bit_leido, 1, 1, recuperar);
    fclose(recuperar);

    string graph = "digraph{\ntabla[shape=none, margin=0, label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD BGCOLOR=\"lightblue\">INDEX</TD><TD BGCOLOR=\"lightblue\">TIPO</TD><TD BGCOLOR=\"lightblue\">NOMBRE</TD></TR>\n";

    if (bit_leido == '1') {
        graph += recorrer_inodo(0, superBloque.s_inode_start, superBloque.s_block_start, discos_montados[disk_pos].path,
                                "");
    }

    graph += "</TABLE>>]\n}";
    archivo_dot(graph, "inode");
}

string REP::recorrer_inodo(int indice_inodo, int inode_start, int block_start, char const *path, char const *name) {
    string graph = "";
    tabla_inodos inodo;
    FILE *recuperar = fopen(path, "rb+");
    fseek(recuperar, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, recuperar);
    fclose(recuperar);
    string nombre = name;

    if (inodo.i_type == '0') {
        graph += "<TR><TD>" + to_string(indice_inodo) + "</TD><TD>Folder</TD><TD>/" + nombre + "</TD></TR>\n";
        for (int i = 0; i < 12; ++i) {
            if (inodo.i_block[i] != -1) {
                graph += recorrer_bloque_carpeta(indice_inodo, inodo.i_block[i], inode_start, block_start, path);
            }
        }

        if (inodo.i_block[12] != -1) {
            graph += recorrer_apuntadores_inodos(indice_inodo, inodo.i_block[12], inode_start, block_start, path, 1);
        }

        if (inodo.i_block[13] != -1) {
            graph += recorrer_apuntadores_inodos(indice_inodo, inodo.i_block[13], inode_start, block_start, path, 2);
        }

        if (inodo.i_block[13] != -1) {
            graph += recorrer_apuntadores_inodos(indice_inodo, inodo.i_block[13], inode_start, block_start, path, 3);
        }
    } else {
        graph += "<TR><TD>" + to_string(indice_inodo) + "</TD><TD>File</TD><TD>/" + nombre + "</TD></TR>\n";
    }
    return graph;
}

string
REP::recorrer_bloque_carpeta(int indice_inodo, int indice_bloque, int inode_start, int block_start, char const *path) {
    bloque_carpeta carpeta;//recuperar bloque carpeta
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&carpeta, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    string graph = "";

    for (int i = 0; i < 4; ++i) {//agregar el nombre e indice de inodo al que apunta
        if (carpeta.b_content[i].b_inodo != -1) {
            if (strcmp(carpeta.b_content[i].b_name, ".") != 0 && strcmp(carpeta.b_content[i].b_name, "..") != 0) {
                graph += recorrer_inodo(carpeta.b_content[i].b_inodo, inode_start, block_start, path,
                                        carpeta.b_content[i].b_name);
            }
        }
    }
    return graph;
}

string REP::recorrer_apuntadores_inodos(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                        char const *path, int nivel) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);
    string graph = "";
    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                graph += recorrer_bloque_carpeta(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                 path);
            } else {
                graph += recorrer_apuntadores_inodos(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                     path, nivel - 1);
            }
        }
    }
    return graph;
}

void REP::graficar_super() {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    char primer_bit;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
    fread(&primer_bit, 1, 1, recuperar);
    fclose(recuperar);

    if (primer_bit != '1') {
        superBloque.s_free_blocks_count = superBloque.s_blocks_count;
        superBloque.s_free_inodes_count = superBloque.s_inodes_count;
        superBloque.s_first_blo = 0;
        superBloque.s_first_ino = 0;
    }

    char fechayhora[16];
    string fecha_hora = "";

    string graph = "";
    graph += "digraph{\n";
    graph += "tabla[shape=none, margin=0, label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD ALIGN=\"left\" BGCOLOR=\"#3498db\">Nombre</TD><TD ALIGN=\"left\" BGCOLOR=\"#3498db\">Valor</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_inodes_count</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_inodes_count) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_blocks_count</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_blocks_count) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_free_blocks_count</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_free_blocks_count) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_free_inodes_count</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_free_inodes_count) + "</TD></TR>\n";
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&superBloque.s_mtime));
    fecha_hora = fechayhora;
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_mtime</TD><TD ALIGN="left">)" + fecha_hora + "</TD></TR>\n";
    fecha_hora = "";
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&superBloque.s_umtime));
    fecha_hora = fechayhora;
    if (superBloque.s_umtime == 0) fecha_hora = "--";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_umtime</TD><TD ALIGN="left">)" + fecha_hora + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_mnt_count</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_mnt_count) + "</TD></TR>\n";
    graph += "<TR><TD ALIGN=\"left\" BGCOLOR=\"lightblue\">s_magic</TD><TD ALIGN=\"left\">0xEF53</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_inode_size</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_inode_size) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_block_size</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_block_size) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_first_ino</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_first_ino) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_first_blo</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_first_blo) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_bm_inode_start</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_bm_inode_start) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_bm_block_start</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_bm_block_start) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_inode_start</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_inode_start) + "</TD></TR>\n";
    graph += R"(<TR><TD ALIGN="left" BGCOLOR="lightblue">s_block_start</TD><TD ALIGN="left">)" +
             to_string(superBloque.s_block_start) + "</TD></TR>\n";
    graph += "</TABLE>>];\n}";

    archivo_dot(graph, "sb");
}

void REP::graficar_bloque() {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    char bit_leido;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
    fread(&bit_leido, 1, 1, recuperar);
    fclose(recuperar);

    string graph = "digraph{\nrankdir=LR;";
    if (bit_leido == '1') {
        graph += graficar_inodo_bloque(0, superBloque.s_inode_start, superBloque.s_block_start,
                                       discos_montados[disk_pos].path);
    }
    graph += "\n}";
    archivo_dot(graph, "inode");
}

string REP::graficar_inodo_bloque(int indice_inodo, int inode_start, int block_start, char const *path) {
    string graph = "";
    tabla_inodos inodo;
    FILE *recuperar = fopen(path, "rb+");
    fseek(recuperar, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, recuperar);
    fclose(recuperar);
    string nombre = name;

    //for para graficar blques
    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            if (inodo.i_type == '0') {
                graph += graficar_bloque_carpeta_bloque(indice_inodo, inodo.i_block[i], inode_start, block_start, path);
            } else {
                graph += graficar_bloque_archivo_bloque(indice_inodo, inodo.i_block[i], inode_start, block_start, path);
            }
        }
    }

    if (inodo.i_block[12] != -1) {
        graph += graficar_bloque_apuntadores_bloque(indice_inodo, inodo.i_block[12], inode_start, block_start, path,
                                                    inodo.i_type, 1);
    }

    if (inodo.i_block[13] != -1) {
        graph += graficar_bloque_apuntadores_bloque(indice_inodo, inodo.i_block[13], inode_start, block_start, path,
                                                    inodo.i_type, 2);
    }

    if (inodo.i_block[14] != -1) {
        graph += graficar_bloque_apuntadores_bloque(indice_inodo, inodo.i_block[14], inode_start, block_start, path,
                                                    inodo.i_type, 3);
    }

    return graph;
}

string
REP::graficar_bloque_carpeta_bloque(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                    char const *path) {
    bloque_carpeta carpeta;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&carpeta, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    string graph = "";
    graph += "bloque" + to_string(indice_bloque) + "\n";
    graph += "bloque" + to_string(indice_bloque) + "[shape=none, margin=0, label=<";
    graph += "<TABLE BORDER=\"1\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD COLSPAN=\"2\">Bloque de Carpetas " + to_string(indice_bloque) + "</TD></TR>\n";
    graph += "<TR><TD>B_NAME</TD><TD>B_INODO</TD></TR>\n";

    for (int i = 0; i < 4; ++i) {
        string b_name = carpeta.b_content[i].b_name;
        if (b_name.empty() || carpeta.b_content[i].b_inodo == -1) {
            b_name = "-";
        } else if (b_name.length() > 12) {
            b_name = b_name.substr(0, 12);
        }
        graph += "<TR><TD>" + b_name + "</TD><TD PORT=\"port" + to_string(i) + "\">" +
                 to_string(carpeta.b_content[i].b_inodo) + "</TD></TR>\n";
    }
    graph += "</TABLE>>];\n";
    graph += "bloque" + to_string(indice_bloque);

    for (int i = 0; i < 4; ++i) {
        if (carpeta.b_content[i].b_inodo != -1) {
            if (strcmp(carpeta.b_content[i].b_name, ".") != 0 && strcmp(carpeta.b_content[i].b_name, "..") != 0) {
                graph += graficar_inodo_bloque(carpeta.b_content[i].b_inodo, inode_start, block_start, path);
            }
        }
    }

    if (indice_bloque != 0) {
        graph = "-> bloque" + to_string(indice_bloque) + "\n" + graph;
    }

    return graph;
}

string
REP::graficar_bloque_archivo_bloque(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                    char const *path) {
    bloque_archivos contenido;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&contenido, sizeof(bloque_archivos), 1, file);
    fclose(file);

    string graph = "";
    graph += "bloque" + to_string(indice_bloque) + "[shape=none, margin=0, label=<\n";
    graph += "<TABLE BORDER=\"1\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD>Bloque de Archivos " + to_string(indice_bloque) + "</TD></TR>\n";
    string content = contenido.b_content;
    if (content.length() > 64) {
        content = content.substr(0, 64);
    }
    graph += "<TR><TD>" + content + "</TD></TR>\n";
    graph += "</TABLE>>];\n";
    graph += "bloque" + to_string(indice_bloque);
    graph = "-> bloque" + to_string(indice_bloque) + "\n" + graph;


    return graph;
}

string REP::graficar_bloque_apuntadores_bloque(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                               char const *path,
                                               char tipo, int nivel) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    string pointers = "";
    string graph = "";
    graph += "bloque" + to_string(indice_bloque) + "[shape=none, margin=0, label=<";
    graph += "<TABLE BORDER=\"1\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD>Bloque de Apuntadores " + to_string(indice_bloque) + "</TD></TR>\n";

    for (int i = 0; i < 15; ++i) {
        pointers += to_string(apuntadores.b_pointers[i]) + ", ";
        if (i == 7) {
            pointers += "\n";
        }
    }
    pointers += to_string(apuntadores.b_pointers[15]);

    graph += "<TR><TD>" + pointers + "</TD></TR>\n";
    graph += "</TABLE>>];\n";
    graph += "bloque" + to_string(indice_bloque);


    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                if (tipo == '0') {
                    graph += graficar_bloque_carpeta_bloque(indice_inodo, apuntadores.b_pointers[i], inode_start,
                                                            block_start,
                                                            path);
                } else {
                    graph += graficar_bloque_archivo_bloque(indice_inodo, apuntadores.b_pointers[i], inode_start,
                                                            block_start,
                                                            path);
                }
            } else {
                graph += graficar_bloque_apuntadores(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                     path, tipo, nivel - 1);
            }
        }
    }
    graph = "-> bloque" + to_string(indice_bloque) + "\n" + graph;
    return graph;
}

void REP::graficar_bitmap(string bitmap) {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    int bm_start = 0;
    int bm_end = 0;

    if (bitmap == "Inodos") {
        bm_start = superBloque.s_bm_inode_start;
        bm_end = superBloque.s_bm_inode_start + superBloque.s_inodes_count;
    } else {
        bm_start = superBloque.s_bm_block_start;
        bm_end = superBloque.s_bm_block_start + superBloque.s_blocks_count;
    }

    char primer_bit;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
    fread(&primer_bit, 1, 1, recuperar);
    fclose(recuperar);

    string graph = "digraph html {\nrankdir=LR;\ninodo0[shape=none, margin=0, label=<\n";
    graph += "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD>Bitmap de " + bitmap + "</TD></TR>";
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    int cont_linea = 0;
    string linea_bits;
    char bit_leido;

    if (primer_bit == '1') {
        for (int i = bm_start; i < bm_end; ++i) {
            fseek(recuperar, i, SEEK_SET);
            fread(&primer_bit, 1, 1, recuperar);
            linea_bits += primer_bit;
            cont_linea++;
            if (cont_linea >= 64) {
                graph += "<TR><TD>" + linea_bits + "</TD></TR>";
                cont_linea = 0;
                linea_bits = "";
            }
        }
        graph += "<TR><TD>" + linea_bits + "</TD></TR>";
    }
    graph += "</TABLE>>];\n}";

    archivo_dot(graph, bitmap);
}

void REP::graficar_file() {

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
            int posicion_eliminada = 0;
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
                posicion_eliminada++;
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

        char bit_leido;
        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
        fread(&bit_leido, 1, 1, recuperar);
        fclose(recuperar);


        tabla_inodos carpeta_tmp;
        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(recuperar, superBloque.s_inode_start, SEEK_SET);
        fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
        fclose(recuperar);

        int inodo_carpeta = -1;
        list<string> carpetas = separar_carpetas(this->ruta);
        list<string>::iterator it;

        //navegar entre las carpetas
        for (it = carpetas.begin(); it != carpetas.end(); it++) {
            inodo_carpeta = get_inodo(*it, carpeta_tmp, discos_montados[disk_pos].path, part_start, '0');
            if (inodo_carpeta == -1) { //si el inodo carpeta es -1 las carpetas no existen
                cout << "La carpeta " << *it << " no existe " << endl;
                break;
            }
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_carpeta, SEEK_SET);
            fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
            fclose(recuperar);
        }

        string contenido = "";
        string file_name = nombre_archivo(this->ruta);
        int inodo_archivo = -1;
        inodo_archivo = get_inodo(file_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, '1');
        if (inodo_archivo == -1) {
            cout << "El archivo " << file_name << " no existe." << endl;
            return;
        } else {
            contenido += contenido_archivo_file(inodo_archivo, disk_pos, superBloque.s_inode_start,
                                                superBloque.s_block_start);
            string graph = "digraph html {\nrankdir=LR;\ninodo0[shape=none, margin=0, label=<\n";
            if (bit_leido == '1') {

                graph += "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
                graph += "<TR><TD BGCOLOR=\"yellow\">" + file_name + "</TD></TR>";
                graph += "<TR><TD>" + contenido + "</TD></TR>";
                graph += "</TABLE>>];\n}";
            }

            archivo_dot(graph, "file");
            return;
        }
    } else {
        cout << "No existe la particion." << endl;
    }
}

int REP::get_inodo(string nombre_buscado, tabla_inodos inodo_carpeta, char const *path, int part_start, int tipo) {
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
                    return carpeta_tmp.b_content[j].b_inodo;
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

int REP::get_inodo_indirecto(int nivel, int apuntador_ind, string nombre_buscado, char const *path, int part_start,
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
                        return carpeta_tmp.b_content[j].b_inodo;
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

string REP::contenido_archivo_file(int indice_inodo, int disk_pos, int inode_start, int block_start) {
    FILE *file;
    tabla_inodos inodo;// se recupera el inodo
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    string contenido = "";

    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            contenido += contenido_bloque_file(inodo.i_block[i], disk_pos, inode_start, block_start);
        }
    }

    if (inodo.i_block[12] != -1) {
        contenido += contenido_indirectos_file(inodo.i_block[12], disk_pos, inode_start, block_start, 1);
    }

    if (inodo.i_block[13] != -1) {
        contenido += contenido_indirectos_file(inodo.i_block[13], disk_pos, inode_start, block_start, 2);
    }

    if (inodo.i_block[14] != -1) {
        contenido += contenido_indirectos_file(inodo.i_block[14], disk_pos, inode_start, block_start, 3);
    }
    return contenido;
}

string REP::contenido_bloque_file(int indice_bloque, int disk_pos, int inode_start, int block_start) {
    bloque_archivos bloque;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&bloque, sizeof(bloque_archivos), 1, file);
    fclose(file);

    string contenido(bloque.b_content);
    if (contenido.length() > 64) {
        contenido = contenido.substr(0, 64);
    }
    return contenido;
}

string REP::contenido_indirectos_file(int indice_bloque, int disk_pos, int inode_start, int block_start, int nivel) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);

    string contenido = "";

    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                contenido += contenido_bloque_file(apuntadores.b_pointers[i], disk_pos, inode_start, block_start);
            } else {
                contenido += contenido_indirectos_file(apuntadores.b_pointers[i], disk_pos, inode_start, block_start,
                                                       nivel - 1);
            }
        }
    }
    return contenido;
}

list<string> REP::separar_carpetas(string path) {
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

string REP::nombre_archivo(string path) {
    string nombre_disco;
    for (int i = path.length() - 1; i > 0; --i) {
        if (path[i] == '/') break;
        nombre_disco = path[i] + nombre_disco;
    }
    return nombre_disco;
}

void REP::graficar_journal() {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    char fechayhora[16];
    string fecha_hora = "";

    string graph = "";
    graph += "digraph{\n";
    graph += "tabla[shape=none, margin=0, label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    graph += "<TR><TD COLSPAN=\"4\" BGCOLOR=\"#145a32\">Journal</TD></TR>\n";
    graph += "<TR><TD BGCOLOR=\"#196f3d\">Operacion</TD><TD BGCOLOR=\"#196f3d\">Path</TD><TD BGCOLOR=\"#196f3d\">Contenido</TD><TD BGCOLOR=\"#196f3d\">Fecha</TD></TR>\n";

    journal journal_actual;
    FILE *file;
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start + sizeof(super_bloque), SEEK_SET);
    fread(&journal_actual, sizeof(journal), 1, file);
    fclose(file);

    while (true) {
        string op = journal_actual.tipo_operacion;
        string path = journal_actual.path;
        string contenido = journal_actual.contenido;
        strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&journal_actual.log_fecha));
        fecha_hora = fechayhora;
        graph += "<TR><TD BGCOLOR=\"#52be80\">" + op + "</TD><TD BGCOLOR=\"#52be80\">" + path +
                 "</TD><TD BGCOLOR=\"#52be80\">" + contenido + "</TD><TD BGCOLOR=\"#52be80\">" + fecha_hora +
                 "</TD></TR>\n";
        if (journal_actual.next == -1) {
            break;
        }
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start + sizeof(super_bloque) + sizeof(journal) * journal_actual.next, SEEK_SET);
        fread(&journal_actual, sizeof(journal), 1, file);
        fclose(file);
    }

    graph += "</TABLE>>];\n}";

    archivo_dot(graph, "sb");
}

void REP::graficar_ls() {
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
    fclose(recuperar);

    int part_start = -1;
    char nombre_particion[16];
    strcpy(nombre_particion, discos_montados[disk_pos].particiones[part_pos].nombre);

    //for para buscar el inicio de la particion
    for (int i = 0; i < 4; ++i) {
        if (strcmp(mbr_leido.particiones[i].part_name, nombre_particion) == 0 &&
            mbr_leido.particiones[i].part_status == 'a') {
            part_start = mbr_leido.particiones[i].part_start;
            break;
        } else if (mbr_leido.particiones[i].part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(this->path.c_str(), mbr_leido.particiones[i].part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    part_start = ebr_actual.part_start;
                }
                if (ebr_actual.part_next == -1) break;

                ebr_actual = leer_ebr(this->path.c_str(), ebr_actual.part_next);
                break;
            }
        }
    }

    if (part_start == -1) {
        cout << "No se encontro la particion solicitada." << endl;
        return;
    }

    //se llama al super bloque
    super_bloque superBloque;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, recuperar);
    fclose(recuperar);

    char bit_leido;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_bm_inode_start, SEEK_SET);
    fread(&bit_leido, 1, 1, recuperar);
    fclose(recuperar);

    tabla_inodos carpeta_tmp;
    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(recuperar, superBloque.s_inode_start, SEEK_SET);
    fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
    fclose(recuperar);

    int inodo_carpeta = -1;
    list<string> carpetas = separar_carpetas(this->ruta);
    list<string>::iterator it;

    //navegar entre las carpetas
    for (it = carpetas.begin(); it != carpetas.end(); it++) {
        inodo_carpeta = get_inodo(*it, carpeta_tmp, discos_montados[disk_pos].path, part_start, '0');
        if (inodo_carpeta == -1) { //si el inodo carpeta es -1 las carpetas no existen
            cout << "La carpeta " << *it << " no existe " << endl;
            break;
        }
        recuperar = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(recuperar, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_carpeta, SEEK_SET);
        fread(&carpeta_tmp, sizeof(tabla_inodos), 1, recuperar);
        fclose(recuperar);
    }

    string contenido = "";
    string file_name = nombre_archivo(this->ruta);
    int inodo_archivo = -1;
    if (inodo_carpeta != -1) {
        inodo_archivo = get_inodo(file_name, carpeta_tmp, discos_montados[disk_pos].path, part_start, '0');
    } else {
        inodo_archivo = 0;
    }
    if (inodo_archivo == -1) {
        cout << "El archivo " << file_name << " no existe." << endl;
        return;
    } else {

        string graph = "digraph{\ntabla[shape=none, margin=0, label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
        graph += "<TR><TD BGCOLOR=\"lightblue\">Permisos</TD><TD BGCOLOR=\"lightblue\">Owner</TD><TD BGCOLOR=\"lightblue\">Grupo</TD><TD BGCOLOR=\"lightblue\">Size(bytes)</TD><TD BGCOLOR=\"lightblue\">Fecha y Hora</TD><TD BGCOLOR=\"lightblue\">Tipo</TD><TD BGCOLOR=\"lightblue\">Name</TD></TR>\n";

        if (bit_leido == '1') {
            graph += recorrer_inodo_ls(inodo_archivo, superBloque.s_inode_start, superBloque.s_block_start,
                                       discos_montados[disk_pos].path,
                                       (file_name + "/").c_str());
        }

        graph += "</TABLE>>]\n}";
        archivo_dot(graph, "inode");
    }

}

string REP::recorrer_inodo_ls(int indice_inodo, int inode_start, int block_start, char const *path, char const *name) {
    string graph = "";
    tabla_inodos inodo;
    FILE *recuperar = fopen(path, "rb+");
    fseek(recuperar, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, recuperar);
    fclose(recuperar);
    string nombre = name;
    if (nombre.length() > 12) {
        nombre = nombre.substr(0, 12);
    }
    USERS *user = new USERS();
    string usr = user->get_name(inodo.i_uid, "U");
    string grp = user->get_name(inodo.i_gid, "G");
    char fechayhora[16];
    string fecha_hora = "";
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M", localtime(&inodo.i_ctime));
    fecha_hora = fechayhora;

    if (inodo.i_type == '0') {
        graph += "<TR><TD>-rw-rw-</TD><TD>" + usr + "</TD><TD>" + grp + "</TD><TD>" + to_string(inodo.i_size) +
                 "</TD><TD>" + fecha_hora + "</TD><TD>Carpeta</TD><TD>" + nombre + "</TD></TR>\n";;
        for (int i = 0; i < 12; ++i) {
            if (inodo.i_block[i] != -1) {
                graph += recorrer_bloque_carpeta_ls(indice_inodo, inodo.i_block[i], inode_start, block_start, path);
            }
        }

        if (inodo.i_block[12] != -1) {
            graph += recorrer_apuntadores_inodos_ls(indice_inodo, inodo.i_block[12], inode_start, block_start, path, 1);
        }

        if (inodo.i_block[13] != -1) {
            graph += recorrer_apuntadores_inodos_ls(indice_inodo, inodo.i_block[13], inode_start, block_start, path, 2);
        }

        if (inodo.i_block[13] != -1) {
            graph += recorrer_apuntadores_inodos_ls(indice_inodo, inodo.i_block[13], inode_start, block_start, path, 3);
        }
    } else {
        graph += "<TR><TD>-rw-rw-</TD><TD>" + usr + "</TD><TD>" + grp + "</TD><TD>" + to_string(inodo.i_size) +
                 "</TD><TD>" + fecha_hora + "</TD><TD>Archivo</TD><TD>" + nombre + "</TD></TR>\n";;
    }
    return graph;
}

string
REP::recorrer_bloque_carpeta_ls(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                char const *path) {
    bloque_carpeta carpeta;//recuperar bloque carpeta
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_archivos) * indice_bloque, SEEK_SET);
    fread(&carpeta, sizeof(bloque_carpeta), 1, file);
    fclose(file);

    string graph = "";

    for (int i = 0; i < 4; ++i) {//agregar el nombre e indice de inodo al que apunta
        if (carpeta.b_content[i].b_inodo != -1) {
            if (strcmp(carpeta.b_content[i].b_name, ".") != 0 && strcmp(carpeta.b_content[i].b_name, "..") != 0) {
                graph += recorrer_inodo_ls(carpeta.b_content[i].b_inodo, inode_start, block_start, path,
                                           carpeta.b_content[i].b_name);
            }
        }
    }
    return graph;
}

string REP::recorrer_apuntadores_inodos_ls(int indice_inodo, int indice_bloque, int inode_start, int block_start,
                                           char const *path, int nivel) {
    //recupero el bloque de apuntadores
    bloque_apuntadores apuntadores;
    FILE *file = fopen(path, "rb+");
    fseek(file, block_start + sizeof(bloque_apuntadores) * indice_bloque, SEEK_SET);
    fread(&apuntadores, sizeof(bloque_apuntadores), 1, file);
    fclose(file);
    string graph = "";
    for (int i = 0; i < 16; ++i) {
        if (apuntadores.b_pointers[i] != -1) {
            if (nivel == 1) {
                graph += recorrer_bloque_carpeta_ls(indice_inodo, apuntadores.b_pointers[i], inode_start, block_start,
                                                    path);
            } else {
                graph += recorrer_apuntadores_inodos_ls(indice_inodo, apuntadores.b_pointers[i], inode_start,
                                                        block_start,
                                                        path, nivel - 1);
            }
        }
    }
    return graph;
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