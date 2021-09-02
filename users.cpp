//
// Created by mario on 1/09/21.
//

#include "users.h"

void USERS::set_id(string id) {
    this->id = id;
}

void USERS::set_pwd(string parte1, string parte2) {
    this->password = parte1 + parte2;
}

void USERS::set_user(string user) {
    this->user = user;
}

void USERS::set_name(bool comillas, string name) {
    if (comillas) {
        this->name = name.substr(1, name.length() - 2);
    } else {
        this->name = name;
    }
}

void USERS::login() {
    if (this->id.empty()) {
        cout << "El parametro id es obligatorio." << endl;
        return;
    } else if (this->password.empty()) {
        cout << "El parametro pwd es obligatorio." << endl;
        return;
    } else if (this->user.empty()) {
        cout << "El parametro user es obligatorio." << endl;
        return;
    }

    if (usuario_loggeado.activo == 1) {
        cout << "Actualmente existe un usuario loggeado.\nDebe realizar un logout." << endl;
        return;
    }

    int disk_pos = get_disk_pos(this->id);
    int part_pos = get_part_pos(this->id, disk_pos);
    if (disk_pos == -1 || part_pos == -1) return;

    int part_start = get_part_start(this->id, disk_pos, part_pos);

    if (part_start == -1) {
        cout << "La particion requerida no existe." << endl;
        return;
    }

    //tengo el inicio de particion, ahora traigo el users.txt
    super_bloque super;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    string txt = get_txt(part_start, discos_montados[disk_pos].path, super.s_inode_start, super.s_block_start);
    list<string> lines = get_lines_txt(txt);
    list<string> usrs = get_grps_usrs(4, lines);//4 comas usrs, 2 comas grps

    //se tiene la lista de todos los usuarios, se recorre esa lista buscando el ususario y comprobando la pwd
    list<string>::iterator it;
    for (it = usrs.begin(); it != usrs.end(); it++) {
        vector<string> parts = get_parts(*it);
        if (this->user == parts[3] && parts[0] != "0") {//se encuentra el nombre
            if (this->password == parts[4]) {//se verifica la password
                usuario_loggeado.user = this->user;
                usuario_loggeado.id = atoi(parts[0].c_str());
                //usuario_loggeado.grupo = parts[2];
                usuario_loggeado.particion_loggeada = this->id;
                usuario_loggeado.activo = 1;
                usuario_loggeado.pwd = this->password;
                cout << "Bienvenido " << this->user << " iniciaste sesion en " << this->id << endl;
                return;
            } else {
                cout << "La contrasena no es correcta." << endl;
                return;
            }
        }
    }
    cout << "El usuario ingresado no existe en esta particion." << endl;
}

void USERS::logout() {
    if(usuario_loggeado.activo == 0) {
        cout << "Actualmente no existe un usuario loggeado. No se puede realizar logout." << endl;
        return;
    }
    cout << "Se cerro la sesion de " << usuario_loggeado.user << " en " << usuario_loggeado.particion_loggeada << endl;
    usuario_loggeado.activo = 0;
}

void USERS::mkgrp() {
    if (this->name.empty()) {
        cout << "El parametro name es obligatorio." << endl;
        return;
    } else if (usuario_loggeado.activo == 0){
        cout << "Para realizar esta operacion necesita estar logeado en una particion." << endl;
        return;
    } else if (usuario_loggeado.id != 1){
        cout << "Este usuario no tiene los permisos para realizar esta operacion.\n Inicie sesion con root." << endl;
        return;
    } else if (this->name.length() > 10){
        cout << "El nombre del grupo excede el maximo de caracteres permitido." << endl;
        return;
    }

    int disk_pos = get_disk_pos(usuario_loggeado.particion_loggeada);
    int part_pos = get_part_pos(usuario_loggeado.particion_loggeada, disk_pos);
    if (disk_pos == -1 || part_pos == -1) return;

    int part_start = get_part_start(usuario_loggeado.particion_loggeada, disk_pos, part_pos);

    if (part_start == -1) {
        cout << "La particion requerida no existe." << endl;
        return;
    }

    //tengo el inicio de particion, ahora traigo el users.txt
    super_bloque super;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    string txt = get_txt(part_start, discos_montados[disk_pos].path, super.s_inode_start, super.s_block_start);
    list<string> lines = get_lines_txt(txt);
    list<string> grps = get_grps_usrs(2, lines);//4 comas usrs, 2 comas grps

    list<string>::iterator it;
    for (it = grps.begin(); it != grps.end(); it++) {//se busca si el grupo no existe
        vector<string> parts = get_parts(*it);
        if (this->name == parts[2] && parts[0] != "0" && parts[1] == "G") {
            cout << "Ya existe un grupo creado bajo este nombre." << endl;
            return;
        }
    }
    int num_grupo = 0;
    //se ve que numero tendra el grupo recorriendo la lista
    for (it = grps.begin(); it != grps.end(); it++) {//se busca si el grupo no existe
        vector<string> parts = get_parts(*it);
        if(atoi(parts[0].c_str()) != 0) num_grupo = atoi(parts[0].c_str());
    }
    num_grupo += 1;
    string nuevo_grp = to_string(num_grupo) + ",G," + this->name + "\n";
    txt += nuevo_grp;

    //se edita el archivo users.txt
    recorrer_inodo(1, disk_pos, super.s_inode_start, super.s_block_start,super.s_bm_inode_start, super.s_bm_block_start, part_start);
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    super.s_first_blo = bitmap_libre(super.s_bm_block_start, super.s_bm_block_start + super.s_blocks_count, discos_montados[disk_pos].path);
    super.s_first_ino = bitmap_libre(super.s_bm_inode_start,super.s_bm_inode_start + super.s_inodes_count,discos_montados[disk_pos].path);

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fwrite(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    escribir_archivo(discos_montados[disk_pos].path, part_start, 1, txt);
}

void USERS::rmgrp() {
    if (this->name.empty()) {
        cout << "El parametro name es obligatorio." << endl;
        return;
    } else if (usuario_loggeado.activo == 0){
        cout << "Para realizar esta operacion necesita estar logeado en una particion." << endl;
        return;
    } else if (usuario_loggeado.id != 1){
        cout << "Este usuario no tiene los permisos para realizar esta operacion.\n Inicie sesion con root." << endl;
        return;
    }

    int disk_pos = get_disk_pos(usuario_loggeado.particion_loggeada);
    int part_pos = get_part_pos(usuario_loggeado.particion_loggeada, disk_pos);
    if (disk_pos == -1 || part_pos == -1) return;

    int part_start = get_part_start(usuario_loggeado.particion_loggeada, disk_pos, part_pos);

    if (part_start == -1) {
        cout << "La particion requerida no existe." << endl;
        return;
    }

    //tengo el inicio de particion, ahora traigo el users.txt
    super_bloque super;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    string txt = get_txt(part_start, discos_montados[disk_pos].path, super.s_inode_start, super.s_block_start);
    list<string> lines = get_lines_txt(txt);

    bool encontrado = false;
    list<string>::iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {//se busca si el grupo no existe
        vector<string> parts = get_parts(*it);
        if (this->name == parts[2] && parts[0] != "0" && parts[1] == "G") {
            string grupo = *it;
            *it = "0" + grupo.substr(1, grupo.length());
            encontrado = true;
            break;
        }
    }

    if(encontrado){
        string nuevo_txt;
        for (it = lines.begin(); it != lines.end(); it++) {//se busca si el grupo no existe
            nuevo_txt += *it;
        }

        //se edita el archivo users.txt
        recorrer_inodo(1, disk_pos, super.s_inode_start, super.s_block_start,super.s_bm_inode_start, super.s_bm_block_start, part_start);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fread(&super, sizeof(super_bloque), 1, file);
        fclose(file);

        super.s_first_blo = bitmap_libre(super.s_bm_block_start, super.s_bm_block_start + super.s_blocks_count, discos_montados[disk_pos].path);
        super.s_first_ino = bitmap_libre(super.s_bm_inode_start,super.s_bm_inode_start + super.s_inodes_count,discos_montados[disk_pos].path);

        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&super, sizeof(super_bloque), 1, file);
        fclose(file);

        escribir_archivo(discos_montados[disk_pos].path, part_start, 1, nuevo_txt);
    } else {
        cout << "El grupo indicado no existe en la particion." << endl;
    }
}

void USERS::mkusr() {
    if (this->user.empty()) {
        cout << "El parametro name es obligatorio." << endl;
        return;
    } else if (usuario_loggeado.activo == 0){
        cout << "Para realizar esta operacion necesita estar logeado en una particion." << endl;
        return;
    } else if (usuario_loggeado.id != 1){
        cout << "Este usuario no tiene los permisos para realizar esta operacion.\n Inicie sesion con root." << endl;
        return;
    } else if (this->name.length() > 10){
        cout << "El nombre del grupo excede el maximo de caracteres permitido." << endl;
        return;
    } else if (this->user.length() > 10){
        cout << "El nombre del usuario excede el maximo de caracteres permitido." << endl;
        return;
    } else if (this->password.length() > 10){
        cout << "La contrasena excede el maximo de caracteres permitido." << endl;
        return;
    }

    int disk_pos = get_disk_pos(usuario_loggeado.particion_loggeada);
    int part_pos = get_part_pos(usuario_loggeado.particion_loggeada, disk_pos);
    if (disk_pos == -1 || part_pos == -1) return;

    int part_start = get_part_start(usuario_loggeado.particion_loggeada, disk_pos, part_pos);

    if (part_start == -1) {
        cout << "La particion requerida no existe." << endl;
        return;
    }

    //tengo el inicio de particion, ahora traigo el users.txt
    super_bloque super;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    string txt = get_txt(part_start, discos_montados[disk_pos].path, super.s_inode_start, super.s_block_start);
    list<string> lines = get_lines_txt(txt);
    list<string> usrs = get_grps_usrs(4, lines);//4 comas usrs, 2 comas grps
    list<string> grps = get_grps_usrs(2, lines);//4 comas usrs, 2 comas grps

    list<string>::iterator it;
    for (it = usrs.begin(); it != usrs.end(); it++) {//se busca si el usuario no existe
        vector<string> parts = get_parts(*it);
        if (this->name == parts[3] && parts[0] != "0" && parts[1] == "U") {
            cout << "Ya existe un grupo creado bajo este nombre." << endl;
            return;
        }
    }

    bool encontrado = false;
    for (it = grps.begin(); it != grps.end(); it++) {//se busca si el grupo no existe
        vector<string> parts = get_parts(*it);
        if (this->name == parts[2] && parts[0] != "0" && parts[1] == "G") {
            encontrado = true;
            break;
        }
    }

    if(encontrado){
        int num_usr = 0;
        //se ve que numero tendra el grupo recorriendo la lista
        for (it = usrs.begin(); it != usrs.end(); it++) {//se busca si el grupo no existe
            vector<string> parts = get_parts(*it);
            if(atoi(parts[0].c_str()) != 0) num_usr = atoi(parts[0].c_str());
        }
        num_usr += 1;
        string nuevo_usr = to_string(num_usr) + ",U," + this->name + "," + this->user + "," + this->password + "\n";
        txt += nuevo_usr;

        //se edita el archivo users.txt
        recorrer_inodo(1, disk_pos, super.s_inode_start, super.s_block_start,super.s_bm_inode_start, super.s_bm_block_start, part_start);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fread(&super, sizeof(super_bloque), 1, file);
        fclose(file);

        super.s_first_blo = bitmap_libre(super.s_bm_block_start, super.s_bm_block_start + super.s_blocks_count, discos_montados[disk_pos].path);
        super.s_first_ino = bitmap_libre(super.s_bm_inode_start,super.s_bm_inode_start + super.s_inodes_count,discos_montados[disk_pos].path);

        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&super, sizeof(super_bloque), 1, file);
        fclose(file);

        escribir_archivo(discos_montados[disk_pos].path, part_start, 1, txt);
    }else{
        cout << "El grupo indicado no existe." << endl;
    }
}

void USERS::rmusr() {
    if (this->name.empty()) {
        cout << "El parametro name es obligatorio." << endl;
        return;
    } else if (usuario_loggeado.activo == 0){
        cout << "Para realizar esta operacion necesita estar logeado en una particion." << endl;
        return;
    } else if (usuario_loggeado.id != 1){
        cout << "Este usuario no tiene los permisos para realizar esta operacion.\n Inicie sesion con root." << endl;
        return;
    }

    int disk_pos = get_disk_pos(usuario_loggeado.particion_loggeada);
    int part_pos = get_part_pos(usuario_loggeado.particion_loggeada, disk_pos);
    if (disk_pos == -1 || part_pos == -1) return;

    int part_start = get_part_start(usuario_loggeado.particion_loggeada, disk_pos, part_pos);

    if (part_start == -1) {
        cout << "La particion requerida no existe." << endl;
        return;
    }

    //tengo el inicio de particion, ahora traigo el users.txt
    super_bloque super;
    FILE *file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&super, sizeof(super_bloque), 1, file);
    fclose(file);

    string txt = get_txt(part_start, discos_montados[disk_pos].path, super.s_inode_start, super.s_block_start);
    list<string> lines = get_lines_txt(txt);

    bool encontrado = false;
    list<string>::iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {//se busca si el grupo no existe
        vector<string> parts = get_parts(*it);
        if (this->name == parts[3] && parts[0] != "0" && parts[1] == "U") {
            string grupo = *it;
            *it = "0" + grupo.substr(1, grupo.length());
            encontrado = true;
            break;
        }
    }

    if(encontrado){
        string nuevo_txt;
        for (it = lines.begin(); it != lines.end(); it++) {//se busca si el grupo no existe
            nuevo_txt += *it;
        }

        //se edita el archivo users.txt
        recorrer_inodo(1, disk_pos, super.s_inode_start, super.s_block_start,super.s_bm_inode_start, super.s_bm_block_start, part_start);
        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fread(&super, sizeof(super_bloque), 1, file);
        fclose(file);

        super.s_first_blo = bitmap_libre(super.s_bm_block_start, super.s_bm_block_start + super.s_blocks_count, discos_montados[disk_pos].path);
        super.s_first_ino = bitmap_libre(super.s_bm_inode_start,super.s_bm_inode_start + super.s_inodes_count,discos_montados[disk_pos].path);

        file = fopen(discos_montados[disk_pos].path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&super, sizeof(super_bloque), 1, file);
        fclose(file);

        escribir_archivo(discos_montados[disk_pos].path, part_start, 1, nuevo_txt);
    } else {
        cout << "El grupo indicado no existe en la particion." << endl;
    }
}

string USERS::get_txt(int part_start, char const *path, int inode_start, int block_start) {
    //el archivo siempre va a estar en el inodo 1 hay que traerlo
    tabla_inodos inodo;
    FILE *file = fopen(path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos), SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    string txt;

    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            bloque_archivos contenido_bloque;
            file = fopen(path, "rb+");
            fseek(file, block_start + sizeof(bloque_archivos) * inodo.i_block[i], SEEK_SET);
            fread(&contenido_bloque, sizeof(bloque_archivos), 1, file);
            fclose(file);
            string contenido(contenido_bloque.b_content);
            if(contenido.length() > 64) {
                contenido = contenido.substr(0, 64);
            }
            txt += contenido;
        }
    }

    return txt;
}

list<string> USERS::get_lines_txt(string txt) {
    list<string> lines;
    string line;
    for (int i = 0; i < txt.length(); ++i) {
        line += txt[i];
        if (txt[i] == '\n') {
            lines.push_back(line);
            line = "";
        }
    }
    return lines;
}

list<string> USERS::get_grps_usrs(int cant_coma, list<string> lines) {
    list<string> usrs_grps;
    list<string>::iterator it;
    for (it = lines.begin(); it != lines.end(); it++) {
        int comas_encontradas = 0;
        string line = *it;
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == ',') {
                comas_encontradas++;
            }
        }
        if (cant_coma == comas_encontradas) {
            usrs_grps.push_back(line);
        }
    }
    return usrs_grps;
}

vector<string> USERS::get_parts(string line) {
    vector<string> parts;
    string part;
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == ',' || line[i] == '\n') {
            parts.push_back(part);
            part = "";
        } else {
            part += line[i];
        }
    }
    return parts;
}

int USERS::get_disk_pos(string id) {
    //se recupera la posicion del dico y particion, se verifica si estan montados
    string disk_number = id.substr(2, id.length() - 3);
    int disk_pos = stoi(disk_number) - 1;
    if (disk_pos < 0 || disk_pos > 98 || discos_montados[disk_pos].estado == 0) {
        cout << "ERROR: El disco indicado no esta montado." << endl;
        return -1;
    }
    return disk_pos;
}

int USERS::get_part_pos(string id, int disk_pos) {
    char part_letter = id.at(id.length() - 1);
    int part_pos = int(part_letter - 65);
    if (part_pos < 0 || part_pos > 25 || discos_montados[disk_pos].particiones[part_pos].estado == 0) {
        cout << "ERROR: La particion indicada no esta montada." << endl;
        return -1;
    }
    return part_pos;
}

int USERS::get_part_start(string logged_partition, int disk_pos, int part_pos) {
    if (discos_montados[disk_pos].estado == 0 || discos_montados[disk_pos].particiones[part_pos].estado == 0) {
        cout << "ERROR: el disco indicado no esta montado." << endl;
        return -1;
    }

    //se verifica que el disco exista
    if (!verificar_disco(discos_montados[disk_pos].path)) return -1;

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
    return part_start;
}

bool USERS::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == nullptr) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

EBR USERS::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

void USERS::recorrer_inodo(int indice_inodo, int disk_pos, int inode_start, int block_start, int bm_inode, int bm_block, int part_start) {
    FILE *file;
    tabla_inodos inodo;// se recupera el inodo
    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fread(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    int resultado = -1;

    //******************************************AQUI VA EL IF DE PERIMISOS********************************************//

    file = fopen(discos_montados[disk_pos].path, "rb+");
    for (int i = 0; i < 12; ++i) {
        if (inodo.i_block[i] != -1) {
            fseek(file, bm_block + inodo.i_block[i], SEEK_SET);
            fwrite("0", 1, 1, file);
            inodo.i_block[i] = -1;
        }
    }
    fclose(file);

    file = fopen(discos_montados[disk_pos].path, "rb+");
    fseek(file, inode_start + sizeof(tabla_inodos) * indice_inodo, SEEK_SET);
    fwrite(&inodo, sizeof(tabla_inodos), 1, file);
    fclose(file);
}

void USERS::escribir_archivo(const char *path, int part_start, int inodo_archivo, string txt) {
    FILE *file;
    string contenido =  txt;
    super_bloque superBloque;
    file = fopen(path, "rb+");
    fseek(file, part_start, SEEK_SET);
    fread(&superBloque, sizeof(super_bloque), 1, file);

    tabla_inodos archivo;
    fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
    fread(&archivo, sizeof(tabla_inodos), 1, file);
    fclose(file);

    archivo.i_size = contenido.length();
    for (int i = 0; i < 12; ++i) {
        archivo.i_block[i] = superBloque.s_first_blo;

        bloque_archivos contenido_archivo;
        memset(&contenido_archivo, '\0', 64);
        for (int j = 0; j < 64; ++j) {
            if (contenido.length() == 0) {
                break;
            }
            contenido_archivo.b_content[j] = contenido[0];
            contenido.erase(0, 1);
        }
        //escribir el bloque de contenido
        fopen(path, "rb+");
        fseek(file, superBloque.s_inode_start + sizeof(tabla_inodos) * inodo_archivo, SEEK_SET);
        fwrite(&archivo, sizeof(tabla_inodos), 1, file);

        fseek(file, superBloque.s_block_start + sizeof(bloque_archivos) * superBloque.s_first_blo, SEEK_SET);
        fwrite(&contenido_archivo, sizeof(bloque_archivos), 1, file);
        //marcar el bloque como utilizado en el bitmap
        fseek(file, superBloque.s_bm_block_start + superBloque.s_first_blo, SEEK_SET);
        fwrite("1", 1, 1, file);
        fclose(file);
        //buscar el bloque libre en el bitmap, actualizar el superBloque
        superBloque.s_first_blo = bitmap_libre(superBloque.s_bm_block_start, superBloque.s_bm_block_start + superBloque.s_blocks_count, path);
        superBloque.s_free_blocks_count--;

        file = fopen(path, "rb+");
        fseek(file, part_start, SEEK_SET);
        fwrite(&superBloque, sizeof(superBloque), 1, file);
        fclose(file);
        if (contenido.length() == 0) break;
    }
    if (contenido.length() == 0) return;

    if (contenido.length() != 0) {
        cout << "No se escribio el archivo completo." << endl;
    }
}

int USERS::bitmap_libre(int start, int final, char const *path) {
    FILE *file = fopen(path, "rb+");
    int libre = -1;
    char bitmap_leido;
    for (int i = start; i < final; i++) {
        fseek(file, i, SEEK_SET);
        fread(&bitmap_leido, sizeof(char), 1, file);
        if (bitmap_leido == '0') {
            libre = i - start;
            fclose(file);
            return libre;
        }
    }
    fclose(file);
    return libre;
}