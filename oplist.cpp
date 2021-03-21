//
// Created by zfq on 2021/3/6.
//
#include "oplist.h"

int select(string input){
    if(rRDRSRT.find(input)->second!=0){return 1;}
    if(rRSRT.find(input)->second!=0){return 2;}
    if(rRDRTSA.find(input)->second!=0){return 3;}
    if(rRS.find(input)->second!=0){return 4;}
    if(rRD.find(input)->second!=0){return 5;}
    if(naidesu.find(input)->second!=0){return 6;}
    if(rRDRTRS.find(input)->second!=0){return 7;}
    if(rRDRS.find(input)->second!=0){return 8;}
    if(rSP2.find(input)->second!=0){return 9;}
    if(rSP1.find(input)->second!=0){return 10;}
    if(iRSLB.find(input)->second!=0){return 11;}//
    if(iRSIM.find(input)->second!=0){return 12;}
    if(iRTIMRS.find(input)->second!=0){return 13;}
    if(iRSRTIM.find(input)->second!=0){return 14;}
    if(branch.find(input)->second!=0){return 15;}//
    if(input=="lui"){return 16;}
    if(input=="j"){return 17;}//
    if(input=="jal"){return 18;}//
    if(input=="jalr"){return 19;}
    if(input=="jr"){return 20;}
    return -1;
}

string r_type(int func,string rd,string  rs,string  rt,string shamt,string op){
    stringstream ss;
    ss<<op;
    ss<<bitset<5>(regno.find(rs)->second);
    ss<<bitset<5>(regno.find(rt)->second);
    ss<<bitset<5>(regno.find(rd)->second);
    ss<<bitset<5>(stoi(shamt));
    ss<<bitset<6>(func);
    string result;
    ss>>result;
    return result;
}

string i_type(int opcode,string rs,string rt,int imm){
    stringstream ss;
    ss<<bitset<6>(opcode);
    ss<<bitset<5>(regno.find(rs)->second);
    ss<<bitset<5>(regno.find(rt)->second);
    ss<<bitset<16>(imm);
    string result;
    ss>>result;
    return result;
}

string inst2bin(string a,  map<string,int> &func_address_set,  map<string,char*> &data_address_set,int i, func &j,int* p){
    stringstream ss(a);
    stringstream out;
    string op;
    string rst;
    string label;
    int imm,addf,addd;
    string rs;
    string rt;
    string rd;
    string shamt;
    while(true){
        getline(ss,op,',');
        if(!op.empty()){break;}
    }
    switch(select(op)) {
        case 1:
            ///{"xor",  0b100110},
            //        {"sub",  0b100010},
            //        {"subu", 0b100011},
            //        {"slt",  0b101010},
            //        {"sltu", 0b101011},
            //        {"nor",  0b100111},
            //        {"or",   0b100101},
            //        {"add",  0b100000},
            //        {"addu", 0b100001},
            //        {"and",  0b100100}
            getline(ss, rd, ',');
            getline(ss, rs, ',');
            getline(ss, rt, ',');
            rst = r_type(rRDRSRT.find(op)->second, rd, rs, rt, "00000");
            break;
        case 2:
            ///  {"teq",0b110100},
            //        {"tne",0b110110},
            //        {"tge",0b110000},
            //        {"tgeu",0b110001},
            //        {"tlt",0b110010},
            //        {"tltu",0b110011},
            //        {"div",   0b011010},
            //        {"divu",  0b011011},
            //        {"mult",  0b011000},
            //        {"multu", 0b011001},
            getline(ss, rs, ',');
            getline(ss, rt, ',');
            rst = r_type(rRSRT.find(op)->second, "$zero", rs, rt, "00000");
            break;
        case 3:
            ///{"sll",0b000000},
            //        {"sra",0b000011},
            //        {"srl",0b000010}
            getline(ss, rd, ',');
            getline(ss, rt, ',');
            getline(ss, shamt, ',');
            rst = r_type(rRDRTSA.find(op)->second, rd, "$zero", rt, shamt);
            break;
        case 4:
            //{"jr",   0b001000},
            //        {"mthi", 0b010001},
            //        {"mtlo", 0b010011},
            getline(ss, rs, ',');
            rst = r_type(rRS.find(op)->second, "$zero", rs, "$zero", "00000");
            break;
        case 5:
            ///{"mfhi", 0b010000},
            //        {"mflo", 0b010010},
            getline(ss, rd, ',');
            rst = r_type(rRD.find(op)->second, rd, "$zero", "$zero", "00000");
            break;
        case 6:
            rst = r_type(naidesu.find(op)->second, "$zero", "$zero", "$zero", "00000");
            break;
        case 7:
            ///{"sllv", 0b000100},
            //        {"srav", 0b000111},
            //        {"srlv",0b000110},
            getline(ss, rd, ',');
            getline(ss, rt, ',');
            getline(ss, rs, ',');
            rst = r_type(rRDRTRS.find(op)->second, rd, rs, rt, "00000");
            break;
        case 8:
            ///jalr
            getline(ss, rd, ',');
            getline(ss, rs, ',');
            rst = r_type(rRDRS.find(op)->second, rd, rs, "$zero", "00000");
            break;
        case 9:
            /// {"mul", 0b000010},
            //        {"clo", 0b100001},
            //        {"clz", 0b100000},
            getline(ss, rd, ',');
            getline(ss, rs, ',');
            getline(ss, rt, ',');
            rst = r_type(rSP2.find(op)->second, rd, rs, rt, "00000", "011100");
            break;
        case 10:
            /// {"madd",0b000000},
            //        {"maddu",0b000001},
            //        {"msub",0b000100},
            //        {"msubu",0b000101},
            getline(ss, rs, ',');
            getline(ss, rt, ',');
            rst = r_type(rSP1.find(op)->second, "$zero", rs, rt, "00000", "011100");
            break;
        case 11:
            ///{"bgez",0b000001},
            //        {"bgtz",0b000111},
            //        {"blez",0b000110},
            //        {"bltz",0b000001},
            getline(ss, rs, ',');
            getline(ss, label, ',');

            addf = func_address_set.find(label)->second;

            imm = (-j.address+addf)-i-1;
            rt = (op == "bgez" ? "00001" : "00000");
            rst = i_type(iRSLB.find(op)->second, rs, rt, imm);
            break;
        case 12:
            ///{"teqi",0b01100},
            //        {"tnei",0b01110},
            //        {"tgei",0b01000},
            //        {"tgeiu",0b01001},
            //        {"tlti",0b01010},
            //        {"tlti",0b01011},
            //        {"bltzal",0b10000},
            getline(ss, rs, ',');
            getline(ss, label, ',');
            imm = stoi(label);
            rt = to_string(iRSIM.find(op)->second);
            rst = i_type(1, rs, rt, imm);
            break;
        case 13:
            ///{"lb",0b100000},
            //        {"lbu",0b100100},
            //        {"lh",0b100001},
            //        {"lhu",0b100101},
            //        {"lw",0b100011},
            //        {"lwl",0b110010},
            //        {"lwr",0b110110},
            //        {"sb",0b101000},
            //        {"sh",0b101001},
            //        {"sw",0b101011},
            //        {"swl",0b101010},
            //        {"swr",0b101110},
            //        {"ll",0b110000},
            //        {"sc",0b111000},
            getline(ss, rt, ',');
            getline(ss, label, ',');
            imm = stoi(label.substr(0, label.find('(')));
            rs = label.substr(label.find('(') + 1, 3);
            rst = i_type(iRTIMRS.find(op)->second, rs, rt, imm);
            break;
        case 14:
            ///{"xori",0b001110},
            //        {"slti",0b001010},
            //        {"sltiu",0b001011},
            //        {"ori",0b001101},
            //        {"addi",0b001000},
            //        {"addiu",0b001001},
            //        {"andi",0b001100},
            getline(ss, rt, ',');
            getline(ss, rs, ',');
            getline(ss, label, ',');
            imm = stoi(label);
            rst = i_type(iRSRTIM.find(op)->second, rs, rt, imm);
            break;
        case 15:
            ///{"beq",0b000100},
            //        {"bne",0b000101},
            getline(ss, rs, ',');
            getline(ss, rt, ',');
            getline(ss, label, ',');
            imm = func_address_set.find(label)->second;
            imm=((imm-j.address-i-1));
            rst = i_type(branch.find(op)->second, rs, rt, imm);
            break;
        case 16:///lui
            getline(ss, rt, ',');
            getline(ss, label, ',');
            imm = stoi(label);
            rst = i_type(0b001111, "$zero", rt, imm);
            break;
        case 17:///j
            getline(ss, label, ',');

            imm = func_address_set.find(label)->second;

            rst="000010"+bitset<26>(imm).to_string();

            break;
        case 18:///jal
            getline(ss, label, ',');
            imm = func_address_set.find(label)->second;
            rst="000011"+bitset<26>(imm).to_string();
            break;
        case 19:///jalr
            getline(ss, rd, ',');
            getline(ss, rs, ',');
            if (rs.empty()) {
                rst = r_type(0b001001, "$ra", rd, "$zero", "$zero");
            } else {
                rst = r_type(0b001001, rd, rs, "$zero", "$zero");
            }
            break;
        default:
            rst = "err";
    }
    //cout<<"posinarray"<<hex<<&p[j.address+i]<<endl;
    p[j.address+i]=stol(rst,0,2);

    //cout<<rst<<endl;
    return rst;
}


