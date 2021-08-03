//
// Created by mario on 1/08/21.
//
#include <iostream>
#include "partition.h"
using namespace std;

#ifndef MIA_P1_201900629_MBR_H
#define MIA_P1_201900629_MBR_H

typedef struct {
    int mbr_tamano = 0;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit = 'F';
    partition mbr_partition1;
    partition mbr_partition2;
    partition mbr_partition3;
    partition mbr_partition4;
} MBR;

#endif //MIA_P1_201900629_MBR_H
