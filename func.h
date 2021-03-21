//
// Created by zfq on 2021/3/4.
//

#ifndef UNTITLED_FUNC_H
#define UNTITLED_FUNC_H
#include <string>
#include <vector>
#include <vector>
#include <tuple>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;
class func{
public:
    std::string label;
    std::vector<std::string> content;
    int size;
    int address;
    func(){
        label="";
        content=std::vector<std::string>();
        size=0;
        address=0;
    }
    void clear();
    bool is_empty();

};
vector<func> process_text(ifstream &infile);
vector<func> write_func(ifstream & infile);
tuple<func,string> write(func f,ifstream &infile);
string modify(string tar);
map<string,int> addalloc(vector<func>& func);
#endif //UNTITLED_FUNC_H
