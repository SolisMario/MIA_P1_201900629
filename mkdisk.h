//
// Created by mario on 1/08/21.
//

#ifndef MIA_P1_201900629_MKDISK_H
#define MIA_P1_201900629_MKDISK_H

#include <iostream>
using namespace std;

class MKDISK {
private:
    int size = 0;
    string fit = "bf";
    string unit = "m";
    string path;
public:
    void setSize(int size_set);

    void setFit(string fit_set);

    void setUnit(string unit_set);

    void setPath(bool comillas, const string &path_set);

    /*
    int getSize();

    string getFit();

    string getUnit();

    string getPath();
    */

    void mkdisk();

    string toLowerCase(string str);
};

#endif //MIA_P1_201900629_MKDISK_H
