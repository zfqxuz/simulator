//
// Created by zfq on 2021/3/5.
//
#include <string>
#include <map>
#include <tuple>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string.h>
#include "func.h"
using namespace std;


#ifndef UNTITLED_DTA_H
#define UNTITLED_DTA_H

class dta{
public:
    map<string,tuple<string,int ,int>> content;
};
map<string,char*> write_data(ifstream &infile,int* vm);
 //addalloc(dta & datas,int vm[]);

#endif //UNTITLED_DTA_H
