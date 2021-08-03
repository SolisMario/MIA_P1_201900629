//
// Created by mario on 2/08/21.
//

#ifndef MIA_P1_201900629_FDISK_H
#define MIA_P1_201900629_FDISK_H

#include <iostream>

using namespace std;

class FDISK {
private:
    int size = 0;
    string unit = "k";
    string path;
    string type = "p";
    string fit = "wf";
    string delete_;
    string name;
    int add;
public:
    void setSize(int size_set);

    void setUnit(string unit_set);

    void setPath(bool comillas, const string &path_set);

    void setType(const string &type);

    void setFit(string fit_set);

    void setDelete(string delete_type);

    void setName(bool comillas, const string &name_set);

    void setAdd(int add);

    void fdisk();

    string toLowerCase(string str);
};

#endif //MIA_P1_201900629_FDISK_H
