#include <fstream>
#include <iostream>

// #include "data_structures.hpp"
#include "tac.hpp"


extern std::string outputDir;
std::vector<quad> tac_code;
long long counter = 0;
void print_error(const std::string &message);

void print_tac_code(const std::string &inputFile) {
    std::ofstream out(outputDir + inputFile + "_IR.txt");
    if (!out) {
        print_error("cannot open " + outputDir + inputFile + "_IR.txt");
        return;
    }

    out << "Three Address Code:\n\n";
    std::cout << std::setw(5) << "Label" << std::setw(10) << "Op" << std::setw(10) << "Arg1" 
              << std::setw(10) << "Arg2" << std::setw(10) << "Result" << std::setw(10) << "Goto" << "\n";
    std::cout << "---------------------------------------------------------\n";

    for (size_t i = 0; i < tac_code.size(); i++) {
        const auto& q = tac_code[i];
        std::cout << std::setw(5) << q.Label << std::setw(10) << q.op.first
                  << std::setw(10) << q.arg1.first << std::setw(10) << q.arg2.first
                  << std::setw(10) << q.result.first
                  << std::setw(10) << (q.gotoLabel == -1 ? "" : std::to_string(q.gotoLabel))
                  << "\n";
    }
}

int emit(qid op, qid arg1, qid arg2, qid result, int gotoLabel) {
    tac_code.emplace_back(tac_code.size() ,op, arg1, arg2, result, gotoLabel);
    return (int)tac_code.size() - 1;
}

void backpatch(std::vector<int> &nextList, int gotoLabel) {
    for(auto i : nextList) {
        tac_code[i].gotoLabel = gotoLabel;
    }
}

void casepatch(std::vector<int> &bplist, qid target) {
    for(auto i : bplist) {
        tac_code[i].arg1 = target;
    }    
}

qid getTempVariable(std::string type) {
    std::string tempName = "_t_" + std::to_string(counter++);

    addToSymbolTable(tempName, type);
    SymbolTableEntry* entry = new SymbolTableEntry; 
    entry->token = tempName;
    entry->data_type = type;

    return qid(tempName, entry);
}

void remainingBackpatch(){
    int i,j;
    i = j = (int)tac_code.size()-1;
    i--;
    while(tac_code[i].op.first.substr(0,5)!="FUNC_"){
        if(tac_code[i].op.first =="GOTO" && tac_code[i].gotoLabel==0)tac_code[i].gotoLabel = j;
        i--;
    }
}
