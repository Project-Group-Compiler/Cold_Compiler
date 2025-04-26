#include "asm.hpp"
#include <regex>
#include <sstream>

std::vector<x86_instr> optim_asm_instr;
std::vector<std::vector<std::string>> opt_blocks_asm;
std::vector<std::string> opt_curr_block_asm;


//TODO: what about other section
void print_block_wise_asm(std::vector<std::vector<std::string>>& blocks_asm){
    for(auto &it:blocks_asm){
        asm_file << "\t\t;NEW BLOCK" << std::endl;
        for(auto &instr:it){
            asm_file << instr << std::endl;
        }
    }
}


void pattern_one_opt(std::vector<std::string> &opt){
    if(opt.size() == 0) return;
    /*add rg 0 
    sub rg 0
    imul rg rg 1
    xor rg 0
    or rg 0 - remove this instr 
    */

    std::regex add_regex(R"(^add (eax|ebx|ecx|edx|esi|edi), 0$)");
    std::regex sub_regex(R"(^sub (eax|ebx|ecx|edx|esi|edi), 0$)");
    std::regex xor_regex(R"(^xor (eax|ebx|ecx|edx|esi|edi), 0$)");
    std::regex or_regex(R"(^or (eax|ebx|ecx|edx|esi|edi), 0$)");
    std::regex imul_regex(R"(^imul (eax), \1, 1$|^imul (ebx), \1, 1$|^imul (ecx), \1, 1$|^imul (edx), \1, 1$|^imul (esi), \1, 1$|^imul (edi), \1, 1$)");

    std::string instr = opt.back();
    if (std::regex_match(instr, add_regex)) {
        opt.pop_back();
    } else if (std::regex_match(instr, sub_regex)) {
        opt.pop_back();
    } else if (std::regex_match(instr, xor_regex)) {
        opt.pop_back();
    } else if (std::regex_match(instr, or_regex)) {
        opt.pop_back();
    } else if (std::regex_match(instr, imul_regex)) {
        opt.pop_back();
    }
}

void pattern_two_opt(std::vector<std::string> &opt){
    /*
        eax -> al
        ebx -> bl
        ecx -> cl
        edx -> dl
    */
    /*
        [ebp - x] <- eax/int
        eax <- [ebp - x]
         [ebp - x] <- eax/int
    */

    /*   
    mov ecx, [ebp-8]  dst mem/reg
    ...no use of ecx
    ... no jumps and call
    ...edx stored then check use of dl
    
    mov ecx, eax
    */

    


}

void check_pattern_and_optimize(std::vector<std::string> &opt){
    if(opt.size() == 0) return;
    pattern_one_opt(opt);
}

void optimize_block_asm(std::vector<std::string> block_asm){
    if(block_asm.size() == 0) return;

    std::vector<std::string> opt;
    int j = 0;
    int i = 0;
    while(i != block_asm.size()){
        opt.push_back(block_asm[i]);
        check_pattern_and_optimize(opt);
        i++;
    }
    opt_curr_block_asm = opt;
}

void optimize_asm(const std::string &inputFile)
{
    asm_file.close();
    asm_file.open( inputFile + "_optimized.asm");

    // std::cout << "---------Original ASM-------------" << std::endl;
    // print_block_wise_asm(blocks_asm);
    
    for(auto it:blocks_asm){
        opt_curr_block_asm.clear();
        optimize_block_asm(it);
        if(opt_curr_block_asm.size()){
            opt_blocks_asm.push_back(opt_curr_block_asm);
        }
    }
    
    std::cout << "---------Optimized ASM-------------" << std::endl;
    print_block_wise_asm(opt_blocks_asm);
    asm_file.close();
}

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

