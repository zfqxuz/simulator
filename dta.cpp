//
// Created by zfq on 2021/3/6.
//
#include "dta.h"

void reformat(string & k){

    int pos=k.find('\\');
    if(pos<k.length()){
        switch (k.at(pos+1)) {
            case 'n':
                k.replace(k.find('\\'),2,1,'\n');
                break;
            case 0:
                k.replace(k.find('\\'),2,1,'\0');
                break;
            default:
                break;
        }
}
}

map<string,char*>  write_data(ifstream & infile,int*vm){
    dta datas;
    string line,sub1,sub2,sub3;
    static int address=0;
    static bool status= false;
    char * data_seg= (char *) ((ulong) (&vm[0])+0x100000 );
    map<string,char*> data_address_set;
    while(getline(infile,line)){
        line=line.substr(0,line.find('#'));
        if ((!line.empty())&&status){
            if(line.find(".text")<line.length()){break;}

            stringstream word(line) ;
            getline(word,sub1,' ');
            getline(word,sub2,' ');
            getline(word,sub3);
            char x=sub2.at(sub2.length()-1);
            reformat(sub3);

            int dtype;
            int length=0,align=0;///      indicating the size of data (byte)

//            switch (x) {
//                case 'f':
//                    dtype=0;
//                    length=2;
//                    break;
//                case 'd':
//                    dtype=1;
//                    length=4;
//                    break;
//                case 'z':
//                    sub3=sub3.substr(1,sub3.length()-2);
//                    dtype=2;
//                    length=sub3.length()+1;
//                    while(length%4!=0){
//                        length++;
//                    }
//                    break;
//                default:
//                    sub3=sub3.substr(1,sub3.length()-2);
//                    dtype=3;
//                    length=sub3.length()+1;
//                    while(length%4!=0){
//                        length++;
//                    }
//                    break;
//            }
            if(sub2.at(1)=='a'){
                sub3=sub3.substr(1,sub3.length()-2);
                char* a= const_cast<char *>(sub3.c_str());
                while(*(a+length)!=0){
                    length++;
                }
                if(sub2.find('z')<sub2.length()){
                    length++;
                }
                memcpy((char*)data_seg,a,length);
                data_seg+=length;
                while(length%4!=0){
                    length++;
                    align++;
                }
                data_seg+=align;
            } else if(sub2.at(1=='w')){
                int* a= (int*)(stoi(sub3));
                memcpy((int*)data_seg,&a,1);
                data_seg+=1;
            } else{
                char* a= const_cast<char *>(sub3.c_str());
                memcpy((char*)data_seg,a,2);
                data_seg+=2;
            }

            tuple<string,int,int> tp(sub3,length,address);
            data_address_set.insert(pair<string,char*>(sub1,data_seg));
            //address+=length;
            //datas.content.insert(pair<string,tuple<string,int,int>>(sub1,tp));


        }
        else if(line.find(".data")<line.length()){
            status= true;
            continue;
        }
    }
    return data_address_set;

}