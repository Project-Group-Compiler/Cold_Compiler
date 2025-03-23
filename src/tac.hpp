#ifndef TAC_HPP
#define TAC_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include "data_structures.hpp"
typedef std::pair<std::string, SymbolTableEntry *> qid;

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

qid getTempVariable(std::string);
void print_tac_code(const std::string &inputFile);
int getCurrentSize();
std::vector<int> mergeList(std::vector<int> &list1, std::vector<int> &list2);
int emit(qid, qid, qid, qid, int);
void backpatch(std::vector<int> &, int);
void casepatch(std::vector<int> &bplist, qid target);
void remainingBackpatch();

void setArg1(int ind, qid value);
void setResult(int ind, qid value);
int assign_exp(std::string op, std::string type, std::string type1, std::string type2, qid arg1, qid arg2);

extern std::vector<quad> tac_code;
extern std::vector<std::vector<quad>> basic_blocks;

void generate_ir();

void compute_basic_blocks();
void print_basic_blocks();

inline std::string stringify(const quad &instr)
{
    std::string s = std::to_string(instr.Label) + ":  ";
    const std::string &curr_op = instr.op.first;
    const char fi = curr_op[0];

    if (curr_op.back() == ':' || curr_op.substr(0, 5) == "FUNC_")
        s += curr_op;
    else if (curr_op.substr(0, 2) == "++" || curr_op.substr(1, 2) == "++" || curr_op.substr(0, 2) == "--" || curr_op.substr(1, 2) == "--" || curr_op == "!" || curr_op == "~" || curr_op == "unary-" || curr_op == "unary+" || curr_op == "unary&" || curr_op == "unary*" || curr_op == "SIZEOF")
        s += instr.result.first + " = " + curr_op + " " + instr.arg1.first;
    else if (curr_op == "=")
        s += instr.result.first + " = " + instr.arg1.first;
    else if (fi == '+' || fi == '-' || fi == '*' || fi == '/' || fi == '%')
        s += instr.result.first + " = " + instr.arg1.first + " " + fi + " " + instr.arg2.first + "\t(" + curr_op.substr(1) + ")";
    else if (curr_op == "==" || curr_op == "!=" || curr_op == "<" || curr_op == ">" || curr_op == "<=" || curr_op == ">=" || curr_op == "&&" || curr_op == "||" || curr_op == ">>" || curr_op == "<<" || curr_op == "&" || curr_op == "|" || curr_op == "^" || curr_op == "PTR_OP" || curr_op == "member_access")
        s += instr.result.first + " = " + instr.arg1.first + " " + curr_op + " " + instr.arg2.first;
    else if (curr_op == "RETURN" || curr_op == "param")
        s += curr_op + " " + instr.arg1.first;
    else if (curr_op == "CALL")
        s += instr.result.first + " = " + curr_op + " " + instr.arg1.first + " " + instr.arg2.first;
    else if (curr_op == "[ ]")
        s += instr.result.first + " = " + instr.arg1.first + "[" + instr.arg2.first + "]";
    else if (curr_op == "GOTO")
    {
        if (instr.arg1.first == "IF")
            s += "IF " + instr.arg2.first + " GOTO " + std::to_string(instr.gotoLabel);
        else
            s += "GOTO " + std::to_string(instr.gotoLabel);
    }
    return s;
}

#endif
