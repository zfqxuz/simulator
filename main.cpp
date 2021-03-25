#include "main.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
using namespace std;



int main(int argc, char* argv[]) {
    cout<<argv[1]<<argv[2]<<argv[3]<<endl;
    if (argc<4){cout<<"wrong input"<<endl;}
    string path=get_current_dir_name();
    path+="/";
    string in1,in2,out;
    in1=(path+argv[1]).c_str();
    in2=(path+argv[2]).c_str();
    out=(path+argv[3]).c_str();
    ifstream FILEIN1,FILEIN2;
    cout<<in1<<endl;
    cout<<out<<endl;
    FILEIN1.open(in1);//argv[1]);
    FILEIN2.open(in2);//argv[2]);

    vector<func> funcs=process_text(FILEIN1);

    ///alloc address for text
    static map<string,int> func_address_set=addalloc(funcs);

    ///read .data
    FILEIN1.close();
    int vm[0x180000];

    FILEIN1.open(in1);//argv[1]);
    ///alloc address for static data
    map<string,char*> data_address_set=write_data(FILEIN1,vm);
    ///generate binary file


        for(func i:funcs){
        for (int j=0;j<i.size;j++){
            cout<<i.content[j]<<endl;
            string temp=inst2bin(i.content[j],func_address_set,data_address_set,j,i,vm);
        }
    }
        parser(vm,FILEIN2,out);//argv[3]);
    ///just for debug
    //display(funcs,func_address_set,datas,data_address_set)


    return 0;

}




void display(const vector<func>& funcs,map<string,int> func_address_set,dta datas,map<string,int> data_address_set){
    for (func i:funcs){
        cout<<"\n"<<i.label<<"\n"<<endl;
        for (string j:i.content){
            cout<<j<<endl;
        }
    }
    cout<<"-------------------------------------------------"<<endl;
    map<string,int>::iterator it;
    it=func_address_set.begin();
    while(it!=func_address_set.end()){
        string a=it->first;
        int b=it->second;
        cout<<"func "<<a<<" address ";
        cout<<hex<<b<<endl;
        it++;
    }
    cout<<"------------------------------------------------"<<endl;
    map<string,tuple<string,int,int>>::iterator iter;
    iter=datas.content.begin();
    while(iter!=datas.content.end()){
        string a=iter->first;
        int b=get<1>(iter->second);
        int c=get<2>(iter->second);
        cout<<"label "<<a<<" type "<<b<<" address ";
        cout<<hex<<c<<endl;
        iter++;
    }
    cout<<"-------------------------------------------------"<<endl;
    it=data_address_set.begin();
    while(it!=data_address_set.end()){
        string a=it->first;
        int b=it->second;
        cout<<"data "<<a<<" address ";
        cout<<hex<<b<<endl;
        it++;
    }
}

