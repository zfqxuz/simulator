//
// Created by zfq on 2021/3/6.
//
#include "func.h"
#include <iostream>
bool func::is_empty() {
    return size==0;
}

void func::clear(){
    label="";
    content=vector<string>();
    size=0;
    address=0;
}

vector<func> process_text(ifstream &infile){

    vector <func> funcs=write_func(infile);
    for(func i : funcs){
        if (i.label=="main"){
            swap(i,funcs[0]);
        }
    }
    return funcs;
}


vector<func> write_func(ifstream &  infile){
    string line;
    vector<func> functions;
    func f;
    f.label="plain_text";
    static int count=0;
    while (true) {

        if(count==0) {
           if( getline(infile,line)) {
               line = modify(line);
           } else{break;}
        }

        ///status decides if this is a data segment or text segment
        static bool status = true;

        ///read nonempty line
        if (!line.empty()) {
            if (line.find(".text") < line.length()) {
                status = true;
                continue;
            }
            else if (line.find(".data") < line.length()) {
                if (count>0){break;}
                status = false;
                continue;
            }

            if (status){
                if(line.find(':') < line.length()) {
    if(!f.is_empty()){
        functions.push_back(f);
        f.clear();
    }
    line=modify(line);
    if (line.find(':') == line.length() - 1) {
        f.label = line.substr(0, line.length() - 1);
    } else {
        f.label = line.substr(0, line.find(':'));
        f.content.push_back(line.substr(line.find(':') + 1, line.length() - 1));
        f.size++;
    }
    tuple<func,string>tp = write(f, infile);
    f=get<0>(tp);
//    functions.push_back(f);
    count+=1;
    line=get<1>(tp);
    if (line.empty()){
//        f.clear();
        break;
    }
}
                else{
                    f.content.push_back(line);
                    f.size++;
                }
            }
        }
    }
    if(!f.is_empty()){functions.push_back(f);}
    return functions;
}

tuple<func,string> write(func f,ifstream &infile){
    string line;

    while (getline(infile,line)){
        line=modify(line);
        if(line.find(".data")<line.length()){break;}
        if(line.find(':')<line.length()){
            tuple<func,string>tp(f,line);
            return tp;
        }
        if (line.empty()){continue;}
        f.content.push_back(line);
        f.size++;
    }
    tuple<func,string>tp(f,"");
    return tp;
}

string modify(string line){

    line=line.substr(0,line.find(35));
    if(line.empty()){return line;}
    stringstream word(line);
    string temp;
    string k;
    while (word>>k){
        temp+=k;
        if(!k.empty()&&k.at(k.length()-1)!=','){
            temp+=',';
        }

    }
    if (!temp.empty()&&temp.at(temp.length()-1)==','){
        temp=temp.substr(0,temp.length()-1);
    }
    return temp;
}

map<string,int> addalloc(vector<func> &funcs){
    map<string,int> func_address_set;
    for(int i=0;i<funcs.size();i++){
        if (i==0){funcs[i].address=0;}
        else{
            funcs[i].address=funcs[i-1].address+funcs[i-1].size;
        }
        func_address_set.insert(pair<string,int>(funcs[i].label,funcs[i].address));
    }
    return func_address_set;
}