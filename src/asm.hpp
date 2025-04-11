#ifndef ASM_GEN_HPP
#define ASM_GEN_HPP

#include <iostream>
#include <fstream>

inline void emit_instr(const std::string &instr)
{
    // asm_file << "\t" << instr << "\n";
    std::cout << "\t" << instr << "\n";
}

inline void emit_label(const std::string &label)
{
    // asm_file << label << ":\n";
    std::cout << label << ":\n";
}

inline void emit_data(const std::string &data)
{
    // asm_file << "\t" << data << "\n";
    std::cout << "\t" << data << "\n";
}

inline void emit_section(const std::string &section)
{
    // asm_file << "section " << section << "\n";
    std::cout << "section " << section << "\n";
}

void add_extern_funcs();
void emit_asm(const std::string &);

void next_use_analysis();

#endif