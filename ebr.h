//
// Created by mario on 7/08/21.
//

#ifndef MIA_P1_201900629_EBR_H
#define MIA_P1_201900629_EBR_H

#include <iostream>
using namespace std;

typedef struct {
    char part_status = 'i';
    char part_fit = 'w';
    int part_start = 0;
    int part_size = 0;
    int part_next = -1;
    char part_name[16];
} EBR;

#endif //MIA_P1_201900629_EBR_H
