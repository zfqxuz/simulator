
//
// Created by zfq on 2021/3/8.
//

#include "simulartor.h"
#include <stdio.h>
#include <stdlib.h>
long long reg[37];
void parser(int vm[],ifstream &infile, string outfile){

    ///initialize register list
    for (long long & i : reg){
        i=0;
    }
    ///pc points to current text
    int pc=0;
    ///gp  points to static data area
    int gp=0x500000;
    ///sp points to the  end of dynamic memory
    int sp=0xa00000;
    reg[28]=gp;
    reg[29]=sp;
    ///register reserved for ll/sc

    reg[35]=0;
    reg[36]=0;

    while(vm[pc]!=0){
        cout<<pc<<endl;
        string instruct=bitset<32>(vm[pc]).to_string();
        if(instruct.substr(0,6)=="000000"){
            pc=r_type_parser(instruct,pc,infile,outfile,vm);
        }
        else if(instruct.substr(0,6)=="011100"){
            pc=rsp_type_parser(instruct,pc,vm);
        }
        else if(instruct.substr(0,5)=="00001"){
            pc=j_type_parser(instruct,pc);
        }
        else{
            pc=i_type_parser(instruct,pc,vm);
        }
        pc++;
    }
}


int r_type_parser(const string& instruct,int pc,ifstream &infile, string outfile,int* vm){
    int rs=bitset<5>(instruct.substr(6,5)).to_ulong();
    int rt=bitset<5>(instruct.substr(11,5)).to_ulong();
    int rd=bitset<5>(instruct.substr(16,5)).to_ulong();
    int shamt=bitset<5>(instruct.substr(21,5)).to_ulong();
    int funct=bitset<6>(instruct.substr(26,6)).to_ulong();
    bool flag= false;
    int a0,a1,b0,b1,hi,lo,temp;
    switch (funct) {
        case 0:
            ///sll
            reg[rd]=reg[rt]<<shamt;
            break;
        case 2:
            ///srl
            reg[rd]=reg[rt]>>shamt;
            break;
        case 3:
            ///sra
            reg[rd]= ((unsigned int) reg[rt]) >> shamt;
            break;
        case 4:
            ///sllv
            reg[rd]=reg[rt]<<reg[rs];
            break;
        case 6:
            ///srlv
            reg[rd]=reg[rt]>>reg[rs];
            break;
        case 7:
            ///srav
            reg[rd]= ((unsigned int) reg[rt]) >>reg[rs];
            break;
        case 8:
            ///jr
            pc=reg[rs]-1;
            break;
        case 9:
            ///jalr
            reg[rd]=pc;
            pc=reg[rs]-1;
            break;
        case 12:
            ///syscall
            syscall(reg[2],infile,outfile,vm);
            break;
        case 13:
            ///break
            cerr<<"Error in line 8: Runtime exception at 0x00400010: break instruction executed; no code given.";
            throw runtime_error("break instruct exectted");
        case 16:
            ///mfhi
            reg[rd]=reg[33];
            break;
        case 17:
            ///mthi
            reg[33]=reg[rs];
            break;
        case 18:
            ///mflo
            reg[rd]=reg[34];
            break;
        case 19:
            ///mtlo
            reg[34]=reg[rs];
            break;
        case 24:
            ///mult

            flag=((rs/rt)<0||(rt/rs)<0);

            rs=abs(reg[rs]);
            rt=abs(reg[rt]);
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();

            hi=a1*b1+((a1*b0+b1*a0)>>16);
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            cout<<bitset<32>(hi)<<endl;

            if(flag){
                cout<<"t"<<endl;
                hi=0xffffffff-hi+1;
                lo=0xffffffff-lo+1;
            }
            reg[33]=hi;
            reg[34]=lo;
            break;

        case 25:
            ///multu
            rs=reg[rs];
            rt=reg[rt];
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();

            hi=a1*b1+((a1*b0+b1*a0)>>16);
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            reg[33]=hi;
            reg[34]=lo;
            break;
        case 26:
            ///div
            hi=reg[rs]/reg[rt];
            lo=reg[rs]%reg[rt];
            reg[33]=hi;
            reg[34]=lo;
            break;
        case 27:
            ///divu
            hi=(unsigned int)(reg[rs]/reg[rt]);
            lo=(unsigned int)(reg[rs]%reg[rt]);
            reg[33]=hi;
            reg[34]=lo;
            break;
        case 32:
            ///add
            try{
            a0=reg[rs];
            a1=reg[rt];
            temp=a0+a1;
            if(signbit(a0)==signbit(a1)){
                if(signbit(a0)!=signbit(temp)){
                    throw overflow_error("IntegerOverflow");
                }
            }
            reg[rd]=temp;
                break;
            }catch (overflow_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
        case 33:
            ///addu
            a0=reg[rs];
            a1=reg[rt];
            reg[rd]=a0+a1;
            break;
        case 34:
            ///sub
            try{
                a0=reg[rs];
                a1=reg[rt];
                temp=a0-a1;
                if(signbit(a0)==signbit(a1)){
                    if(signbit(a0)!=signbit(temp)){
                        throw overflow_error("IntegerOverflow");
                    }
                }
                reg[rd]=temp;
                break;
            }catch (overflow_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
        case 35:
            ///subu
            a0=reg[rs];
            a1=reg[rt];
            reg[rd]=a0-a1;
            break;
        case 36:
            ///and
            reg[rd]=reg[rs] bitand reg[rt];
            break;
        case 37:
            ///or
            reg[rd]=reg[rs] bitor reg[rt];
            break;
        case 38:
            ///xor
            reg[rd]=reg[rs] ^ reg[rt];
            break;
        case 39:
            ///nor
            reg[rd]=~(reg[rs] bitor reg[rt]);
            break;
        case 42:
            ///slt
            reg[rd]=(reg[rs]<reg[rt]);
            break;
        case 43:
            ///sltu
            reg[rd]=((unsigned int)reg[rs]<(unsigned int)reg[rt]);
            break;
        case 48:
            ///tge
            try{
                a0=reg[rs];
                a1=reg[rt];
                if(a0>=a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        case 49:
            ///  tgeu
            try{
                a0=reg[rs];
                a1=reg[rt];
                if((unsigned int)a0>=(unsigned int)a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        case 50:
            ///tlt
            try{
                a0=reg[rs];
                a1=reg[rt];
                if(a0<a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        case 51:
            ///tltu
            try{
                a0=reg[rs];
                a1=reg[rt];
                if((unsigned int)a0<(unsigned int)a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        case 52:
            ///teq
            try{
                a0=reg[rs];
                a1=reg[rt];
                if(a0==a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        case 53:
            ///tequ
            try{
                a0=reg[rs];
                a1=reg[rt];
                if((unsigned int)a0==(unsigned int)a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        case 54:
            ///tne
            try{
                a0=reg[rs];
                a1=reg[rt];
                if(a0!=a1){
                    throw runtime_error("trap");
                }

            }catch (runtime_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
            break;
        default:
            cout<<"wtf?"<<endl;
            break;
    }
    return pc;
}

void syscall(int i,ifstream &infile,string outfile,int*vm){
    string string1;
    FILE* f;
    unsigned char temp;
    char* src, *c,*buffer= nullptr;
    int fd,sz,size;
    int* a,x,count=0;
    stringstream ss;
    switch (i) {
        case 1:
            //print int
            a=(int*)reg[4];
            f=fopen(outfile.c_str(), "a+");
            fprintf(f,"%d",a);
            fclose(f);

            break;
        case 4:
            //print string
            src=(char*)to_real_address(reg[4],vm);
            f=fopen(outfile.c_str(), "a+");
            do{
                memcpy(&temp,src,1);

                ss<<temp;
                src++;
                count++;
            }while(temp!=0);
            getline(ss,string1,'\0');
            fprintf(f,"%s",string1.c_str());
            fclose(f);
            break;
        case 5:
            //read int
            getline(infile,string1);
            reg[2]=stoi(string1);
            break;
        case 8:
            //read string
            getline(infile,string1);
            c= const_cast<char *>(string1.c_str());
            src=(char*)to_real_address(reg[4],vm);
            memcpy(c,src,reg[5]);
            break;
        case  9:
            //sbrk
            reg[2]=0x600000;
            break;
        case 10:
            //exit
            exit(0);
        case 11:
            //print char
            f=fopen(outfile.c_str(), "a+");
            c=(char*)reg[4];
            memcpy(&temp,c,1);
            ss<<temp;
            ss>>string1;
            fprintf(f,"%s",string1.c_str());
            fclose(f);
            cout<<(char)reg[4];
            break;
        case 12:
            //read char
            getline(infile,string1);
            c= const_cast<char *>(string1.c_str());
            src=(char*)to_real_address(reg[4],vm);
            memcpy(c,src,1);
            break;
        case 13:
            c= const_cast<char *>(string1.c_str());
            src=(char*)to_real_address(reg[4],vm);

            do {
                memcpy(c, src, 1);
                ss<<c;
                src++;
            } while (c!=0);
            ss>>string1;
            if(reg[6]==0){
                fd=open(string1.c_str(), O_RDONLY|O_CREAT, "r");
            }else{
                fd=open(string1.c_str(), O_WRONLY, "w");
            }
            reg[2]=fd;
            break;
        case 14:
            fd=reg[4];
            buffer=(char*)to_real_address(reg[5],vm);
            size=reg[6];
            sz= read(fd,buffer,size);
            reg[2]=sz;
            break;
        case 15:

            fd=reg[4];
            buffer=(char*)to_real_address(reg[5],vm);
            size=reg[6];
            sz= write(fd,buffer,size);
            reg[2]=sz;
            break;
        case 16:
            fd=reg[4];
            close(fd);
            break;
        case 17:
            exit(reg[4]);
        default:
            break;
    }
}

int rsp_type_parser(const string &instruct,int pc,int vm[]){
    int rs=bitset<5>(instruct.substr(6,5)).to_ulong();
    int rt=bitset<5>(instruct.substr(11,5)).to_ulong();
    int rd=bitset<5>(instruct.substr(16,5)).to_ulong();
    int funct=bitset<6>(instruct.substr(26,6)).to_ulong();
    int a0,a1,b0,b1,hi,lo,temp;
    bool flag;
    switch (funct) {
        case 0:
            ///madd
            flag=((rs/rt)<0||(rt/rs)<0);
            rs=abs(reg[rs]);
            rt=abs(reg[rt]);
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();
            hi=a1*b1+((a1*b0+b1*a0)>>16);
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            cout<<bitset<32>(hi)<<endl;
            if(flag){
                cout<<"t"<<endl;
                hi=0xffffffff-hi+1;
                lo=0xffffffff-lo+1;
            }
            reg[33]=reg[33]+hi;
            reg[34]=reg[34]+lo;
            break;
        case 1:
            ///maddu
            rs=reg[rs];
            rt=reg[rt];
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();

            hi=a1*b1+((a1*b0+b1*a0)>>16);
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            reg[33]=reg[33]+hi;
            reg[34]=reg[34]+lo;
            break;
        case 2:
            ///mul
            rs=reg[rs];
            rt=reg[rt];
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            reg[rd]=lo;
            break;
        case 4:
            ///msub
            flag=((rs/rt)<0||(rt/rs)<0);
            rs=abs(reg[rs]);
            rt=abs(reg[rt]);
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();

            hi=a1*b1+((a1*b0+b1*a0)>>16);
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            cout<<bitset<32>(hi)<<endl;

            if(flag){
                cout<<"t"<<endl;
                hi=0xffffffff-hi+1;
                lo=0xffffffff-lo+1;
            }
            reg[33]=reg[33]-hi;
            reg[34]=reg[34]-lo;
            break;
        case 5:
            ///msubu
            rs=reg[rs];
            rt=reg[rt];
            a0=bitset<16>(rs).to_ulong();
            a1=bitset<16>(rs>>16).to_ulong();
            b0=bitset<16>(rt).to_ulong();
            b1=bitset<16>(rt>>16).to_ulong();

            hi=a1*b1+((a1*b0+b1*a0)>>16);
            lo=((a1*b0+b1*a0)<<16)+a0*b0;
            reg[33]=reg[33]-hi;
            reg[34]=reg[34]-lo;
            break;
        case 33:
            ///clo
            temp=reg[rs];
            for (int i=0;i<32;i++){
                if(bitset<32>(temp).to_string().at(i)=='0'){
                    reg[rd]=i;
                    break;
                }
            }
            break;
        case 34:
            ///clz
            temp=reg[rs];
            for (int i=0;i<32;i++){
                if(bitset<32>(temp).to_string().at(i)=='1'){
                    reg[rd]=i;
                    break;
                }
            }
            break;
        default:
            cout<<"wtf?"<<endl;

    }
    return pc;
}

int i_type_parser(const string &instruct,int pc,int* vm){
    int rs=bitset<5>(instruct.substr(6,5)).to_ulong();
    int rt=bitset<5>(instruct.substr(11,5)).to_ulong();
    int opcode=bitset<6>(instruct.substr(0,6)).to_ulong();
    int offset=bitset<16>(instruct.substr(16,16)).to_ulong();
    int temp;
    char* tmp;
    switch (opcode) {
        case  1:
            if(rt==0){
                ///bltz
                if(reg[rs]<0){
                    pc+=offset;
                    break;
                }
            }else if(rt==1){
                ///bgez
                if(reg[rs]>=0){
                    pc+=offset;
                    break;
                }
            }
            break;
        case 4:
            ///beq
            if(reg[rs]==reg[rt]){
                pc+=sign_extend(offset);
            }
            break;
        case 5:
            ///bne
            if(reg[rs]!=reg[rt]){
                pc+=sign_extend(offset);
            }
            break;
        case 6:
            ///blez
            if(reg[rs]<=0){
                pc+=sign_extend(offset);
            }
            break;
        case 7:
            ///
            if(reg[rs]>0){
                pc+=sign_extend(offset);
            }
            break;
        case 8:
            ///addi
            try{
                offset=sign_extend(offset);
            temp=reg[rs]+offset;
            if(signbit(reg[rs])==signbit(offset)){
                if(signbit(reg[rs])!=signbit(temp)){
                    throw overflow_error("IntegerOverflow");
                }
            }
            reg[rt]=temp;
            break;
            }catch (overflow_error &e){
                cerr<<e.what()<<endl;
                exit(-1);
            }
        case 9:
            ///addiu
            reg[rt]=reg[rs]+offset;
            break;
        case 10:
            ///slti
            if (reg[rs]<offset){
                reg[rt]=1;
            }else{
                reg[rt]=0;
            }
            break;
        case 11:
            ///sltiu
            if (reg[rs]<(unsigned int)offset){
                reg[rt]=1;
            }else{
                reg[rt]=0;
            }
            break;
        case 12:
            ///andi
            reg[rt]=(reg[rs] bitand offset);
            break;
        case 13:
            ///ori
            reg[rt]=(reg[rs] bitor offset);
            break;
        case 14:
            ///xori
            reg[rt]=(reg[rs]^offset);
            break;
        case 15:
            ///lui
            reg[rt]=(offset<<16);
            break;
        case 32:
            ///lb
            tmp=(char*)to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(&reg[rt],tmp,1);
            if (reg[rt]>>7==1){
                reg[rt]=0xffffff00 bitor reg[rt];
            }
            break;
        case 33:
            ///lh
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(&reg[rt],tmp,2);
            if (reg[rt]>>15==1){
                reg[rt]=0xffffff00 bitor reg[rt];
            }
            break;
        case 35:
            ///lw
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(&reg[rt],tmp,4);

            break;
        case 36:
            ///lbu
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(&reg[rt],tmp,1);
            break;
        case 37:
            ///lhu
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(&reg[rt],tmp,2);
            break;
        case 40:
            ///sb
            tmp=(char*)to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(tmp,&reg[rt],1);
            break;
        case 41:
            ///sh
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=offset;
            memcpy(tmp,&reg[rt],2);
            break;
        case 42:
            ///swl
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=4*(offset/4);
            check_ll(tmp);
            memcpy(tmp,&reg[rt],offset%4+1);
            break;
        case 43:
            ///sw
            tmp=(char*)to_real_address(reg[rs],vm);
            tmp+=offset;
            check_ll(tmp);
            memcpy(tmp,&reg[rt],4);
            break;
        case 46:
            ///swr
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=4*(offset/4);
            check_ll(tmp);
            memcpy(tmp,&reg[rt],4);
            *tmp=*tmp<<(8*(offset%4));
            break;
        case 48:
            ///ll
            if(!LL){
                LL= true;
                tmp=(char*)to_real_address(reg[rs],vm);
                tmp+=offset;
                memcpy(&reg[rt],tmp,4);
                reg[35]=(unsigned long)tmp;
                break;
            }
            break;
        case 56:
            ///sc
            if(LL){
                tmp=(char*)to_real_address(reg[rs],vm);
                tmp+=offset;

                memcpy(tmp,&reg[rt],4);
                reg[rt]=1;
                LL= false;
            }else{
                reg[rt]=0;
            }
            break;
        case 50:
            ///lwl
            tmp=(char*) to_real_address(reg[rs],vm);
            tmp+=4*(offset/4);
            check_ll(tmp);
            memcpy(&reg[rt],tmp,offset%4+1);
            break;
        case 51:
            ///lwr
            tmp=(char*) to_real_address(reg[rs],vm);
            check_ll(tmp);
            tmp+=4*(offset/4);
            memcpy(&reg[rt],tmp,4);
            *tmp=*tmp<<(8*(offset%4));
            break;


    }
    return pc;
}

void check_ll(void* x){
    if((unsigned long) x==reg[35]){
        LL= false;
    }
}

int j_type_parser(const string &instruct,int pc){
    int opcode=bitset<6>(instruct.substr(0,6)).to_ulong();
    int jump=bitset<26>(instruct.substr(6,26)).to_ulong();
    switch (opcode) {
        case 2:
        ///j
            pc=jump-1;
            break;
        case 3:
            ///jal
            reg[31]=pc+1;
            pc=jump-1;
            break;
        default:
            break;
    }
    return pc;
}

void* to_real_address(int v_add,int* vm){

    void* ptr= (void *) ((ulong) (&vm[0]) +v_add-0x400000);
    return ptr;
}


long sign_extend(int a){
    if(a>>15==0){
        return (0xffffffff bitand a);
    }else{
        return (0xffff0000 bitor a);
    }
}