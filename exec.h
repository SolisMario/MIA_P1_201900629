//
// Created by mario on 29/08/21.
//

#ifndef MIA_P1_201900629_EXEC_H
#define MIA_P1_201900629_EXEC_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class  EXEC{
private:
    string path;
public:
    void set_path(bool comillas, string path);

    void exec();
};

#endif //MIA_P1_201900629_EXEC_H
