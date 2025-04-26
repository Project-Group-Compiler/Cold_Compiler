#include "asm.hpp"
std::vector<x86_instr> optim_asm_instr;
void register_move_coalescing(){
    for(auto &instr : asm_instr)
    {
        if(instr.op == "mov" )
        {
            continue;
        }
        else optim_asm_instr.push_back(instr);
    }
}
void print_asm_instr(const std::string &inputFile)
{
        // Close the current file
    asm_file.close();

    // Reopen with truncation to clear contents
    asm_file.open( inputFile + "_optimized.asm", std::ios::trunc);
    
    // Check if file opened successfully
    if (!asm_file) {
        std::cerr << "Error: Unable to reopen assembly file for writing" << std::endl;
        return;
    }

    for(x86_instr &instr : optim_asm_instr)
    {
        asm_file << instr.printing << "\n";
    }

}
void optimize_asm(const std::string &inputFile)
{
    optim_asm_instr= asm_instr;
    //Register Move Coalescing
    register_move_coalescing();
    //Print the optimized assembly code
    print_asm_instr(inputFile);
    
}