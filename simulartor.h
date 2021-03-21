//
// Created by zfq on 2021/3/8.
//

#ifndef UNTITLED_SIMULARTOR_H
#define UNTITLED_SIMULARTOR_H

#include "main.h"
#include "oplist.h"
#include <iostream>
#include<sys/types.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
int r_type_parser(const string& s,int pc,ifstream &infile, string outfile,int*vm);
int i_type_parser(const string& s,int pc,int *vm);
int j_type_parser(const string& s,int pc);
int rsp_type_parser(const string &s,int pc,int* vm);
void parser(int vm[],ifstream &infile,string outfile);
void syscall(int i,ifstream &infile,string outfile ,int* vm);
void* to_real_address(int v_add,int* vm);
static bool LL= false;
void check_ll(void*x);
long sign_extend(int a);

#endif //UNTITLED_SIMULARTOR_H
