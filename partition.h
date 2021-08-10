//
// Created by mario on 1/08/21.
//
#include <iostream>
using namespace std;

#ifndef MIA_P1_201900629_PARTITION_H
#define MIA_P1_201900629_PARTITION_H

typedef struct {
    char part_status = 'i';
    char part_type = 'p';
    char part_fit = 'w';
    int part_start = 0;
    int part_size = 0;
    char part_name[16];
} partition_;

#endif //MIA_P1_201900629_PARTITION_H
