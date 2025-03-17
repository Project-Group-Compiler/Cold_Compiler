#ifndef TAC_HPP
#define TAC_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include "data_structures.hpp"
typedef std::pair<std::string, SymbolTableEntry*> qid;

class quad
{
public:
    int Label;
    qid op;
    qid arg1;
    qid arg2;
    qid result;
    int gotoLabel = -1;

    quad(int Label, qid op, qid arg1, qid arg2, qid result, int gotoLabel) : Label(Label), op(op), arg1(arg1), arg2(arg2), result(result), gotoLabel(gotoLabel) {}
};

// extern std::vector<quad> tac_code;

void run_optimisations();

qid getTempVariable(std::string );
void print_tac_code(const std::string &inputFile);
int emit(qid , qid , qid , qid , int);
void backpatch(std::vector<int>& , int);
void casepatch(std::vector<int>& bplist, qid target);
void remainingBackpatch();

#endif
