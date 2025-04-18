#ifndef ASM_GEN_HPP
#define ASM_GEN_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "tac.hpp"

extern std::ofstream asm_file;

inline void emit_instr(const std::string &instr)
{
    asm_file << "\t" << instr << "\n";
}

inline void emit_label(const std::string &label)
{
    asm_file << label << ":\n";
}

inline void emit_data(const std::string &data)
{
    asm_file << "\t" << data << "\n";
}

inline void emit_section(const std::string &section)
{
    asm_file << "\nsection " << section << "\n";
}

inline void add_extern_funcs()
{
    for (auto &func : called_lib_funcs)
        asm_file << "extern " << func << "\n";
}

void emit_asm(const std::string &);

void emit_fn_defn(quad &instr);
void emit_fn_epilogue();
void emit_param(quad &instr);
void emit_fn_call(quad &instr);
void emit_return(quad &instr);
void emit_goto(quad &instr);
void emit_assign(quad &instr);
void emit_add(quad &instr);
void emit_sub(quad &instr);
void emit_mul(quad &instr);
void emit_div(quad &instr);
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
void emit_logical_and(quad &instr);
void emit_logical_or(quad &instr);
void emit_logical_not(quad &instr);

void next_use_analysis(std::vector<quad> &block);
void print_next_use();

int getReg(operand &op, bool willYouModify, std::vector<int> resReg);
int getBestReg(std::vector<int> resReg);

void update_ir();
void fixgotoLabels();
void emit_data_section();
void emit_bss_section();

#endif
