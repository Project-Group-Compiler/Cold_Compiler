#ifndef ASM_GEN_HPP
#define ASM_GEN_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "tac.hpp"

class x86_instr{
    public:
    std::string op;
    std::string size;//dword, byte, qword etc
    std::string arg1;
    std::string arg2;
    std::string arg3;
    std::string comment;
    std::string label;//jump label
    std::string printing;
};
extern std::ofstream asm_file;
extern bool print_comments;
extern std::vector<x86_instr> asm_instr;
inline void emit_instr(const std::string &instr)
{
    asm_file << "\t" << instr << std::endl;
}

inline void emit_label(const std::string &label)
{
    x86_instr instr;
    instr.label = label;
    instr.printing = label + " :";
    asm_instr.push_back(instr);
    asm_file << label << " :\n";
}

inline void emit_data(const std::string &data)
{
    x86_instr instr;
    instr.comment = data;
    instr.printing = "\t" + data;
    asm_instr.push_back(instr);
    asm_file << "\t" << data << "\n";
}

inline void emit_section(const std::string &section)
{
    x86_instr instr;
    instr.comment = section;
    instr.printing = "\nsection " + section;
    asm_instr.push_back(instr);
    asm_file << "\nsection " << section << "\n";
}

inline void emit_comment(const std::string &comment)
{
    x86_instr instr;
    instr.comment = comment;
    instr.printing = "\t\t\t; " + comment;
    asm_instr.push_back(instr);
    asm_file << "\t\t\t; " << comment << "\n";
}

inline void add_extern_funcs()
{
    for (auto &func : called_lib_funcs){
        x86_instr instr;
        instr.comment = "extern " + func;
        instr.printing = "extern " + func;
        asm_instr.push_back(instr);
        asm_file << "extern " << func << "\n";
    }
        
}

template <typename T>
inline void __print__(const T &x){
    std::cerr << x;
}

template <typename T, typename... Args>
inline void __print__(const T &x, const Args &...rest){
    std::cerr << x << ", ";
    __print__(rest...);
}
#define _debug_(x...)                                             \
    std::cerr << "(Line " << __LINE__ << "): [" << #x << "] => "; \
    __print__(x);                                                 \
    std::cerr << std::endl;

void emit_asm(const std::string &);
void emit_fload(operand &op);
void emit_fstore(operand &op);
void emit_unary_star(quad &instr);
void emit_unary_and(quad &instr);
void emit_fn_defn(quad &instr);
void emit_fn_epilogue();
void emit_param(quad &instr);
void emit_fparam(operand op);
void emit_fn_call(quad &instr);
void emit_return(quad &instr);
void emit_goto(quad &instr);
void emit_assign(quad &instr);
void emit_fassign(quad &instr);
void emit_add(quad &instr);
void emit_fadd(quad &instr);
void emit_sub(quad &instr);
void emit_fsub(quad &instr);
void emit_mul(quad &instr);
void emit_fmul(quad &instr);
void emit_div(quad &instr);
void emit_fdiv(quad &instr);
void emit_mod(quad &instr);
void emit_or(quad &instr);
void emit_and(quad &instr);
void emit_xor(quad &instr);
void emit_unary_plus(quad &instr);
void emit_unary_minus(quad &instr);
void emit_not(quad &instr);
void emit_right_shift(quad &instr);
void emit_left_shift(quad &instr);
void emit_cmp(quad &instr);
void emit_fcmp(quad &instr);
void emit_logical_and(quad &instr);
void emit_logical_or(quad &instr);
void emit_logical_not(quad &instr);
void emit_intToFloat(quad &instr);
void emit_floatToInt(quad &instr);
void emit_intToChar(quad &instr);
void emit_charToInt(quad &instr);
void emit_cassign(quad &instr);
void emit_logical_ptr_add(quad &instr);
void emit_logical_ptr_sub(quad &instr);
void emit_copy_to_offset(quad &instr);

void next_use_analysis(std::vector<quad> &block);
void print_next_use();

int getReg(operand &op, bool willYouModify, std::vector<int> resReg);
int getBestReg(std::vector<int> resReg);

void update_ir();
void fixgotoLabels();
void emit_data_section();
void emit_bss_section();

//Optimization functions
void optimize_asm(const std::string &inputFile);
void print_asm_instr();


#endif
