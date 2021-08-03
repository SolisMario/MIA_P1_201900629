//
// Created by mario on 1/08/21.
//

#ifndef MIA_P1_201900629_PARTITION_H
#define MIA_P1_201900629_PARTITION_H

typedef struct {
    char part_status;
    char part_type;
    char part_fit = 'W';
    int part_start;
    int part_size;
    char part_name[16];
} partition;

#endif //MIA_P1_201900629_PARTITION_H
