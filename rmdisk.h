//
// Created by mario on 2/08/21.
//

#ifndef MIA_P1_201900629_RMDISK_H
#define MIA_P1_201900629_RMDISK_H

#include <iostream>
using namespace std;

class RMDISK {
    private:
        string path;
    public:
        void setPath(bool comillas, string path);
        void rmdisk();
        string toLowerCase(string str);
};

#endif //MIA_P1_201900629_RMDISK_H
