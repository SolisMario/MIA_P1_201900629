//
// Created by mario on 13/08/21.
//

#include <cmath>
#include "mkfs.h"
#include <cstring>

void MKFS::set_type(string type_set) {
    this->type = type_set;
}

void MKFS::set_id(string id_set) {
    this->id = id_set;
}

void MKFS::set_fs(int fs_set) {
    this->fs = fs_set;
}

void MKFS::mkfs() {
    if (this->id.empty()) {
        cout << "Error: el parametro id es obligatorio." << endl;
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

    int n = 0;

    for (auto &particione : mbr_leido.particiones) {
        if (strcmp(nombre_particion, particione.part_name) == 0 && particione.part_status == 'a') {
            //se obtiene el numero de inodos
            n = get_n(particione.part_size);

            //se llena el superbloque
            super_bloque SB;
            recuperar = fopen(discos_montados[disk_pos].path, "rb+");
            fseek(recuperar, particione.part_start, SEEK_SET);
            fread(&SB, sizeof(super_bloque), 1, recuperar);

            SB = editar_super(n, SB, particione.part_start);
            fseek(recuperar, particione.part_start, SEEK_SET);
            fwrite(&SB, sizeof(super_bloque), 1, recuperar);

            int inicio_bm = 0;
            inicio_bm = particione.part_start + sizeof(super_bloque);
            if (this->fs == 3) inicio_bm += 100 * sizeof(bloque_archivos);

            //seteando 0 bitmap de inodos
            for (int i = inicio_bm; i < inicio_bm + n; i++) {
                fseek(recuperar, i, SEEK_SET);
                fwrite("0", 1, 1, recuperar);
                fflush(recuperar);
            }

            //seteando 0 bitmap de bloques
            inicio_bm = inicio_bm + n;
            for (int i = inicio_bm; i < inicio_bm + 3 * n; i++) {
                fseek(recuperar, i, SEEK_SET);
                fwrite("0", 1, 1, recuperar);
                fflush(recuperar);
            }

            //verificar tipo de formateo
            if (this->type == "full") {
                for (int i = inicio_bm + 3 * n; i < particione.part_start + particione.part_size; ++i) {
                    fseek(recuperar, i, SEEK_SET);
                    fwrite("\0", 1, 1, recuperar);
                    fflush(recuperar);
                }
            }

            //inodo de carpeta raiz
            tabla_inodos inodo_raiz;
            inodo_raiz.i_atime = time(0);
            inodo_raiz.i_ctime = time(0);
            inodo_raiz.i_mtime = time(0);
            inodo_raiz.i_block[0] = 0;
            fseek(recuperar, SB.s_inode_start, SEEK_SET);
            fwrite(&inodo_raiz, sizeof(tabla_inodos), 1, recuperar);
            fflush(recuperar);

            //bloque de carpeta raiz
            bloque_carpeta bloque_raiz;
            strcpy(bloque_raiz.b_content[0].b_name, ".");
            bloque_raiz.b_content[0].b_inodo = 0;
            strcpy(bloque_raiz.b_content[1].b_name, "..");
            bloque_raiz.b_content[1].b_inodo = 0;
            strcpy(bloque_raiz.b_content[2].b_name, "users.txt");
            bloque_raiz.b_content[2].b_inodo = 1;
            fseek(recuperar, SB.s_block_start, SEEK_SET);
            fwrite(&bloque_raiz, sizeof(bloque_carpeta), 1, recuperar);
            fflush(recuperar);

            //inodo archivo users.txt
            tabla_inodos inodo_usuarios;
            inodo_usuarios.i_atime = time(0);
            inodo_usuarios.i_ctime = time(0);
            inodo_usuarios.i_mtime = time(0);
            inodo_usuarios.i_block[0] = 1;
            inodo_usuarios.i_type = '1';
            inodo_usuarios.i_size = sizeof("1,G,root\n1,U,root,root,123\n");
            fseek(recuperar, SB.s_inode_start + sizeof(tabla_inodos), SEEK_SET);
            fwrite(&inodo_usuarios, sizeof(tabla_inodos), 1, recuperar);
            fflush(recuperar);

            //bloque con contenido de usuarios.txt
            bloque_archivos bloque_usuarios;
            strcpy(bloque_usuarios.b_content, "1,G,root\n1,U,root,root,123\n");
            fseek(recuperar, SB.s_block_start + sizeof(bloque_carpeta), SEEK_SET);
            fwrite(&bloque_usuarios, sizeof(bloque_archivos), 1, recuperar);
            fflush(recuperar);

            //marcando como utilizados los bitmap
            fseek(recuperar, SB.s_bm_inode_start, SEEK_SET);//inodo carpeta raiz
            fwrite("1", 1, 1, recuperar);
            fseek(recuperar, SB.s_bm_inode_start + 1, SEEK_SET);//inodo archivo users.txt
            fwrite("1", 1, 1, recuperar);
            fseek(recuperar, SB.s_bm_block_start, SEEK_SET);//bloque carpeta raiz
            fwrite("1", 1, 1, recuperar);
            fseek(recuperar, SB.s_bm_block_start + 1, SEEK_SET);//bloque archivo users.txt
            fwrite("1", 1, 1, recuperar);

            if (this->fs == 3) {
                llenar_journal(particione.part_start, recuperar);
            }

            fclose(recuperar);

        } else if (particione.part_type == 'e') {
            //si es extendida se busca entre las logicas
            EBR ebr_actual;
            ebr_actual = leer_ebr(discos_montados[disk_pos].path, particione.part_start);

            while (true) {
                if (strcmp(nombre_particion, ebr_actual.part_name) == 0 && ebr_actual.part_status == 'a') {
                    n = get_n(ebr_actual.part_size);

                    //se llena el superbloque
                    super_bloque SB;
                    recuperar = fopen(discos_montados[disk_pos].path, "rb+");
                    fseek(recuperar, particione.part_start, SEEK_SET);
                    fread(&SB, sizeof(EBR), 1, recuperar);

                    SB = editar_super(n, SB, particione.part_start);
                    fseek(recuperar, particione.part_start, SEEK_SET);
                    fwrite(&SB, sizeof(super_bloque), 1, recuperar);

                    int inicio_bm = 0;
                    inicio_bm = ebr_actual.part_start + sizeof(super_bloque);
                    if (this->fs == 3) inicio_bm += 100 * sizeof(bloque_archivos);

                    //seteando 0 bitmap de inodos
                    for (int i = inicio_bm; i < inicio_bm + n; i++) {
                        fseek(recuperar, i, SEEK_SET);
                        fwrite("0", 1, 1, recuperar);
                        fflush(recuperar);
                    }

                    //seteando 0 bitmap de bloques
                    inicio_bm = inicio_bm + n;
                    for (int i = inicio_bm; i < inicio_bm + 3 * n; i++) {
                        fseek(recuperar, i, SEEK_SET);
                        fwrite("0", 1, 1, recuperar);
                        fflush(recuperar);
                    }

                    //verificar tipo de formateo
                    if (this->type == "full") {
                        for (int i = inicio_bm + 3 * n; i < ebr_actual.part_start + ebr_actual.part_size; ++i) {
                            fseek(recuperar, i, SEEK_SET);
                            fwrite("\0", 1, 1, recuperar);
                            fflush(recuperar);
                        }
                    }

                    //inodo de carpeta raiz
                    tabla_inodos inodo_raiz;
                    inodo_raiz.i_atime = time(0);
                    inodo_raiz.i_ctime = time(0);
                    inodo_raiz.i_mtime = time(0);
                    inodo_raiz.i_block[0] = 0;
                    fseek(recuperar, SB.s_inode_start, SEEK_SET);
                    fwrite(&inodo_raiz, sizeof(tabla_inodos), 1, recuperar);
                    fflush(recuperar);

                    //bloque de carpeta raiz
                    bloque_carpeta bloque_raiz;
                    strcpy(bloque_raiz.b_content[0].b_name, ".");
                    bloque_raiz.b_content[0].b_inodo = 0;
                    strcpy(bloque_raiz.b_content[1].b_name, "..");
                    bloque_raiz.b_content[1].b_inodo = 0;
                    strcpy(bloque_raiz.b_content[2].b_name, "users.txt");
                    bloque_raiz.b_content[2].b_inodo = 1;
                    fseek(recuperar, SB.s_block_start, SEEK_SET);
                    fwrite(&bloque_raiz, sizeof(bloque_raiz), 1, recuperar);
                    fflush(recuperar);

                    //inodo archivo users.txt
                    tabla_inodos inodo_usuarios;
                    inodo_usuarios.i_atime = time(0);
                    inodo_usuarios.i_ctime = time(0);
                    inodo_usuarios.i_mtime = time(0);
                    inodo_usuarios.i_block[0] = 1;
                    inodo_usuarios.i_type = '1';
                    inodo_usuarios.i_size = sizeof("1,G,root\n1,U,root,root,123\n");
                    fseek(recuperar, SB.s_inode_start + sizeof(tabla_inodos), SEEK_SET);
                    fwrite(&inodo_usuarios, sizeof(inodo_usuarios), 1, recuperar);
                    fflush(recuperar);

                    //bloque con contenido de usuarios.txt
                    bloque_archivos bloque_usuarios;
                    strcpy(bloque_usuarios.b_content, "1,G,root\n1,U,root,root,123\n");

                    //marcando como utilizados los bitmap
                    fseek(recuperar, SB.s_bm_inode_start, SEEK_SET);//inodo carpeta raiz
                    fwrite("1", 1, 1, recuperar);
                    fseek(recuperar, SB.s_bm_inode_start + 1, SEEK_SET);//inodo archivo users.txt
                    fwrite("1", 1, 1, recuperar);
                    fseek(recuperar, SB.s_bm_block_start, SEEK_SET);//bloque carpeta raiz
                    fwrite("1", 1, 1, recuperar);
                    fseek(recuperar, SB.s_bm_block_start + 1, SEEK_SET);//bloque archivo users.txt
                    fwrite("1", 1, 1, recuperar);

                    if (this->fs == 3) {
                        llenar_journal(ebr_actual.part_start, recuperar);
                    }

                    fclose(recuperar);
                }
                if (ebr_actual.part_next == -1) break;
                ebr_actual = leer_ebr(discos_montados[disk_pos].path, ebr_actual.part_next);
            }
        }
    }

    if (n <= 0) {
        cout << "No se puede crear el sistema de archivos. La particion no se encuentra activa" << endl;
        return;
    }
}

bool MKFS::verificar_disco(char const *path) {

    FILE *verificar = fopen(path, "r"); //r= read = si el disco ya existia
    if (verificar == NULL) {
        cout << "El disco no existe." << endl;
        return false;
    }
    fclose(verificar);
    return true;
}

int MKFS::get_n(int tam_particion) {
    float numerador;
    float denominador;
    float n = 0;
    if (this->fs == 2) {
        numerador = tam_particion - sizeof(super_bloque);
        denominador = 4 + sizeof(tabla_inodos) + (3 * sizeof(bloque_archivos));
    } else {
        numerador = tam_particion - sizeof(super_bloque) - 100 * sizeof(bloque_archivos);
        denominador = 4 + sizeof(tabla_inodos) + 3 * sizeof(bloque_archivos);
    }

    n = numerador / denominador;

    return floor(n);
}

EBR MKFS::leer_ebr(char const *sc, int seek) {
    EBR ebr_aux;
    FILE *rec_aux;
    rec_aux = fopen(sc, "rb+");
    fseek(rec_aux, seek, SEEK_SET);
    fread(&ebr_aux, sizeof(EBR), 1, rec_aux);
    fclose(rec_aux);
    return ebr_aux;
}

super_bloque MKFS::editar_super(int n, super_bloque &SB, int part_start) {
    super_bloque super_aux = SB;

    if (this->fs == 2) {
        super_aux.s_filesystem_type = 2;
    } else {
        super_aux.s_filesystem_type = 3;
    }
    super_aux.s_inodes_count = n;
    super_aux.s_blocks_count = 3 * n;
    super_aux.s_free_blocks_count = 3 * n - 2;
    super_aux.s_free_inodes_count = n - 2;
    super_aux.s_inode_size = sizeof(tabla_inodos);
    super_aux.s_block_size = sizeof(bloque_archivos);
    super_aux.s_first_ino = 2;
    super_aux.s_first_blo = 2;
    if (this->fs == 2) {
        super_aux.s_bm_inode_start = part_start + sizeof(super_bloque);
        super_aux.s_bm_block_start = part_start + sizeof(super_bloque) + n;
        super_aux.s_inode_start = part_start + sizeof(super_bloque) + n + n * 3;
        super_aux.s_block_start = part_start + sizeof(super_bloque) + n + n * 3 + n * sizeof(tabla_inodos);
    } else {
        super_aux.s_bm_inode_start = part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos);
        super_aux.s_bm_block_start = part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos) + n;
        super_aux.s_inode_start = part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos) + n + n * 3;
        super_aux.s_block_start = part_start + sizeof(super_bloque) + 100 * sizeof(bloque_archivos) + n + n * 3 +
                                  n * sizeof(tabla_inodos);
    }

    return super_aux;
}

void MKFS::llenar_journal(int part_start, FILE *file) {
    journal diario_inicial;
    diario_inicial.size = 0;
    diario_inicial.tipo = '1';
    strcpy(diario_inicial.path, "/");
    diario_inicial.log_fecha = time(0);
    strcpy(diario_inicial.tipo_operacion, "mkdir");
    strcpy(diario_inicial.contenido, "");
    diario_inicial.next = 1;
    fseek(file, part_start + sizeof(super_bloque) + 0 * sizeof(journal), SEEK_SET);
    fwrite(&diario_inicial, sizeof(journal), 1, file);
    fflush(file);
    journal segundo;
    segundo.size = 0;
    segundo.posicion = 1;
    segundo.tipo = '2';
    strcpy(segundo.path, "/users.txt");
    segundo.log_fecha = time(0);
    strcpy(segundo.tipo_operacion, "touch");
    strcpy(segundo.contenido, "1,G,root\n1,U,root,root,123\n");
    fseek(file, part_start + sizeof(super_bloque) + 1 * sizeof(journal), SEEK_SET);
    fwrite(&segundo, sizeof(journal), 1, file);
    fflush(file);
}