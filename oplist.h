//
// Created by zfq on 2021/3/4.
//

#ifndef UNTITLED_OPLIST_H
#define UNTITLED_OPLIST_H
#include "main.h"
#include "dta.h"
#include "func.h"
#include <map>
#include <sstream>
#include <iostream>
#include <string>
#include <bitset>
using  namespace std;

int select(string input);
string inst2bin(string a, map<string,int> &func_add_set,map<string,char*> &data_add_set,int i,func &j,int* p);
string r_type(int func,string rd,string  rs,string  rt,string shamt,string op="000000");
string i_type(int op,string rs,string rt,int imm);

static  map<string ,int> regno={
        {"$zero",0},
        {"$at",1},
        {"$v0",2},
        {"$v1",3},
        {"$a0",4},
        {"$a1",5},
        {"$a2",6},
        {"$a3",7},
        {"$t0",8},
        {"$t1",9},
        {"$t2",10},
        {"$t3",11},
        {"$t4",12},
        {"$t5",13},
        {"$t6",14},
        {"$t7",15},
        {"$s0",16},
        {"$s1",17},
        {"$s2",18},
        {"$s3",19},
        {"$s4",20},
        {"$s5",21},
        {"$s6",22},
        {"$s7",23},
        {"$t8",24},
        {"$t9",25},
        {"$k0",26},
        {"$k1",27},
        {"$gp",28},
        {"$sp",29},
        {"$fp",30},
        {"$ra",31}
};
static map<string,int> rRDRSRT={
        {"xor",  0b100110},
        {"sub",  0b100010},
        {"subu", 0b100011},
        {"slt",  0b101010},
        {"sltu", 0b101011},
        {"nor",  0b100111},
        {"or",   0b100101},
        {"add",  0b100000},
        {"addu", 0b100001},
        {"and",  0b100100}
};

static map<string,int> rRDRTRS={
        {"sllv", 0b000100},
        {"srav", 0b000111},
        {"srlv",0b000110},
};

static map<string,int> rRDRTSA={
        {"sll",0b000000},
        {"sra",0b000011},
        {"srl",0b000010}
};
static map<string,int> rRSRT={
        {"teq",0b110100},
        {"tne",0b110110},
        {"tge",0b110000},
        {"tgeu",0b110001},
        {"tlt",0b110010},
        {"tltu",0b110011},
        {"div",   0b011010},
        {"divu",  0b011011},
        {"mult",  0b011000},
        {"multu", 0b011001},
};
static map<string,int> rRDRS={
        {"jalr", 0b001001},
};
static map<string,int> rRS={
        {"jr",   0b001000},
        {"mthi", 0b010001},
        {"mtlo", 0b010011},
};

static map<string,int> rRD={
        {"mfhi", 0b010000},
        {"mflo", 0b010010},
};

static map<string,int> naidesu={
        {"break",0b001101},
        {"syscall",0b001100},

};

static map<string,int> rSP1={
        {"madd",0b000000},
        {"maddu",0b000001},
        {"msub",0b000100},
        {"msubu",0b000101},
};
static map<string,int> rSP2= {
        {"mul", 0b000010},
        {"clo", 0b100001},
        {"clz", 0b100000},
};

static map<string,int> iRSLB={
        {"bgez",0b000001},
        {"bgtz",0b000111},
        {"blez",0b000110},
        {"bltz",0b000001},
};
static map<string,int> iRSIM={
        {"teqi",0b01100},
        {"tnei",0b01110},
        {"tgei",0b01000},
        {"tgeiu",0b01001},
        {"tlti",0b01010},
        {"tlti",0b01011},
        {"bltzal",0b10000},
};

static map<string,int> iRTIMRS={
        {"lb",0b100000},
        {"lbu",0b100100},
        {"lh",0b100001},
        {"lhu",0b100101},
        {"lw",0b100011},
        {"lwl",0b110010},
        {"lwr",0b110110},
        {"sb",0b101000},
        {"sh",0b101001},
        {"sw",0b101011},
        {"swl",0b101010},
        {"swr",0b101110},
        {"ll",0b110000},
        {"sc",0b111000},
};

static map<string,int> iRSRTIM={
        {"xori",0b001110},
        {"slti",0b001010},
        {"sltiu",0b001011},
        {"ori",0b001101},
        {"addi",0b001000},
        {"addiu",0b001001},
        {"andi",0b001100},
};

static map<string,int> branch={
        {"beq",0b000100},
        {"bne",0b000101},

};
#endif //UNTLED_MAIN_H
