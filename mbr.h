//
// Created by mario on 1/08/21.
//

#ifndef MIA_P1_201900629_MBR_H
#define MIA_P1_201900629_MBR_H

#include <iostream>
#include "partition.h"

using namespace std;

typedef struct {
    int mbr_tamano = 0;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit = 'F';
    partition_ particiones[4];
} MBR;

#endif //MIA_P1_201900629_MBR_H
