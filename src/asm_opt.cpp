#include "asm.hpp"
#include <regex>
#include <sstream>


std::vector<x86_instr> optim_asm_instr;
bool optimize_asm = false;

void print_instr_asm(std::vector<x86_instr> &v){
    for(x86_instr &instr : v){
         if(instr.op == ""){
             asm_file << instr.printing << "\n";
         }else{
            asm_file << "\t"<< instr.printing << "\n";
         }
    }
}

/**
 * Optimizes simple arithmetic instructions with neutral elements
 * Removes instructions like "add reg, 0", "sub reg, 0", "xor reg, 0", etc.
*/
void pattern_one_opt(std::vector<x86_instr> &opt){
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

    std::string instr = opt.back().printing;
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

/**
 * Performs pattern-based optimization to eliminate redundant register/memory moves
 *
 * This function optimizes code by eliminating redundant move instructions. It analyzes
 * the instruction sequence to find cases where:
 * 1. A register is loaded with a value but is overwritten before being used
 * 2. A memory location is written to and then overwritten before being read
 *
 * The function preserves program semantics by:
 * - Checking for any intervening instructions that might use the register or memory location
 * - Avoiding optimization across labels, jumps, or function calls
 * - Handling special cases like partial register usage (e.g., al vs eax)
 */
void pattern_two_opt(std::vector<x86_instr> &opt){
    if(opt.size() <= 1) return;
    
    x86_instr last_instr = opt.back();
    if(last_instr.op != "mov")return;
    
    std::map<std::string,std::string> mp = {{"eax","al"}, {"ebx","bl"}, {"ecx","cl"}, {"edx","dl"}, {"esi", ""}, {"edi", ""},{"al","eax"}, {"bl","ebx"}, {"cl","ecx"}, {"dl","edx"},};
    std::set<std::string> stops = {"idiv", "call", "jmp", "je", "jne", "jz", "jnz", "jg", "jge", "jl", "jle", "ja", "jae", "jb", "jbe"};
    int sz = opt.size();
    if(mp.find(last_instr.arg1) != mp.end()){ //dest is reg
        std::string destReg = last_instr.arg1;
        std::string subReg = mp[destReg];
        int j = -1;
        for(int i = sz-2;i>=0;i--){
            if(opt[i].label != "" || stops.find(opt[i].op) != stops.end()){
                return;
            }
            if(opt[i].op == "mov" && opt[i].arg1 == destReg){
                j = i;
                break;
            }
            if(opt[i].arg1.find(destReg) != std::string::npos || opt[i].arg2.find(destReg) != std::string::npos || opt[i].arg3.find(destReg) != std::string::npos)
            {
                return;
            }
            if(opt[i].arg1.find(subReg) != std::string::npos || opt[i].arg2.find(subReg) != std::string::npos || opt[i].arg3.find(subReg) != std::string::npos)
            {
                return;
            }
        }
        if(j != -1){
            opt.erase(opt.begin()+j);
        }
    }else if(last_instr.arg1.substr(0,4) == "[ebp"){
        std::string destMem = last_instr.arg1;
        int j = -1;
        for(int i = sz-2;i>=0;i--){
            if(opt[i].op == "movzx" || opt[i].label != "" || stops.find(opt[i].op) != stops.end()){
                return;
            }
            if(opt[i].op == "mov" && opt[i].arg1 == destMem){
                j = i;
                break;
            }
            if(opt[i].arg1 == destMem || opt[i].arg2 == destMem || opt[i].arg3 == destMem){
                return;
            }
        }
        if(j != -1){
            opt.erase(opt.begin()+j);
        }
    }else{
        return;
    }
    // last instr se dekho
    /* eax -> al ebx -> bl ecx -> cl edx -> dl */
    /*
        [ebp - x] <- eax/int
        eax <- [ebp - x]
         [ebp - x] <- eax/int
    */

    /*   
    mov ecx, [ebp-8]  dst mem/reg
    ...no use of ecx,dl..
    ... no jumps and call
    ...edx stored then check use of dl

    mov ecx, eax
    */
   /*   return "mov " + dest_reg + ", " + src_reg;
        return "mov " + dest_reg + ", " + mem(src_addr);
        return "mov " + dest_reg + ", " + imm;
        return "mov " + mem(dest_addr) + ", " + src_reg;
    */

}


void optimize_last_asm(std::vector<x86_instr> &optim_asm_instr){
    pattern_one_opt(optim_asm_instr);
    pattern_two_opt(optim_asm_instr);
}

void opt_asm(const std::string &inputFile)
{
    for(auto it:asm_instr){
        optim_asm_instr.push_back(it);
        optimize_last_asm(optim_asm_instr);
    }
    print_instr_asm(optim_asm_instr);
}

// void register_move_coalescing(){
//     for(auto &instr : asm_instr)
//     {
//         if(instr.op == "mov" )
//         {
//             continue;
//         }
//         else optim_asm_instr.push_back(instr);
//     }
// }
