#ifndef TAC_HPP
#define TAC_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include "data_structures.hpp"
#include <regex>
struct operand
{
    std::string value;
    sym_entry *entry;
    int nextUse = -1; // TODO
    int isLive = 0;

    operand()
    {
        value = "";
        entry = nullptr;
    }
    operand(std::string value)
    {
        this->value = value;
        this->entry = nullptr;
    }
    operand(std::string value, sym_entry *entry)
    {
        this->value = value;
        this->entry = entry;
    }
    bool operator==(const operand &other) const
    {
        return value == other.value && entry == other.entry;
    }
    bool operator<(const operand &other) const
    {
        if (value != other.value)
            return value < other.value;
        return entry < other.entry; // pointer comparison
    }
    bool operator>(const operand &other) const
    {
        if (value != other.value)
            return value > other.value;
        return entry > other.entry; // pointer comparison
    }
};

class quad
{
public:
    int Label;
    std::string op;
    operand arg1;
    operand arg2;
    operand result;
    int gotoLabel = -1;

    quad(int Label, std::string op, operand arg1, operand arg2, operand result, int gotoLabel) : Label(Label), op(op), arg1(arg1), arg2(arg2), result(result), gotoLabel(gotoLabel) {}
};

void run_optimisations();

operand getTempVariable(std::string);
operand getTempVariableForArray(std::string, std::vector<int>&);
void print_tac_code(const std::string &inputFile, bool modifygotoLabels = false);
int getCurrentSize();
void extendList(std::vector<int> &list1, std::vector<int> &list2);
int emit(std::string, operand, operand, operand, int);
void backpatch(std::vector<int> &, int);
void singlePatch(int label, int gotoLabel);
void casepatch(std::vector<int> &bplist, operand target);
void remainingBackpatch();

void setArg1(int ind, operand value);
void setResult(int ind, operand value);
int assign_exp(std::string op, std::string type, std::string type1, std::string type2, operand arg1, operand arg2, int isLocalStaticInit = 0);

extern std::vector<quad> tac_code;
extern std::vector<std::vector<quad>> basic_blocks;
extern std::vector<operand> static_vars;

extern bool optimize_ir;

void generate_ir();

void addgotoLabels();

void compute_basic_blocks();
void print_basic_blocks(bool modifygotoLabels = false);

inline std::string stringify(const quad &instr, bool modifygotoLabels = false)
{
    std::string s = std::to_string(instr.Label) + ":  ";
    const std::string &curr_op = instr.op;
    const char fi = curr_op[0];

    if (curr_op.back() == ':' || curr_op.substr(0, 5) == "FUNC_")
        s += curr_op;
    else if (curr_op == "!" || curr_op == "~")
        s += instr.result.value + " = " + curr_op + " " + instr.arg1.value;
    else if (curr_op.substr(0, 5) == "unary")
    {
        if (curr_op.back() == '&')
        {
            if (instr.arg1.value.size() > 0 && instr.arg1.value[0] == '*')
                s += instr.result.value + " = " + instr.arg1.value.substr(1); //&*
            else
                s += instr.result.value + " = & " + instr.arg1.value;
        }
        else
            s += instr.result.value + " = " + curr_op.back() + " " + instr.arg1.value;
    }
    else if (curr_op.substr(0, 3) == "(f)")
    {
        if (curr_op.back() == '=')
            s += instr.result.value + " = " + instr.arg1.value + "\t\t(float)";
        else
            s += instr.result.value + " = " + instr.arg1.value + " " + curr_op.back() + " " + instr.arg2.value + "\t\t(float)";
    }
    else if (curr_op == "=" || curr_op == "(c)=")
        s += instr.result.value + " " + curr_op + " " + instr.arg1.value;
    else if (curr_op == "+" || curr_op == "-" || curr_op == "*" || curr_op == "/" || curr_op == "%")
        s += instr.result.value + " = " + instr.arg1.value + " " + curr_op + " " + instr.arg2.value;
    else if (curr_op == "==" || curr_op == "!=" || curr_op == "<" || curr_op == ">" || curr_op == "<=" || curr_op == ">=" || curr_op == "&&" || curr_op == "||" || curr_op == ">>" || curr_op == "<<" || curr_op == "&" || curr_op == "|" || curr_op == "^" || curr_op == "ptr+" || curr_op == "ptr-")
        s += instr.result.value + " = " + instr.arg1.value + " " + curr_op + " " + instr.arg2.value;
    else if (curr_op == "RETURN" || curr_op == "param")
        s += curr_op + " " + instr.arg1.value;
    else if (curr_op == "CALL")
    {
        s += curr_op + " " + instr.arg1.value + " " + instr.arg2.value;
        auto pos = s.find(":  ");
        if (instr.result.value != "")
            s = s.substr(0, pos + 3) + instr.result.value + " = " + s.substr(pos + 3);
    }
    else if (curr_op == "[ ]")
        s += instr.result.value + " = " + instr.arg1.value + "[" + instr.arg2.value + "]";
    else if (curr_op == "GOTO")
    {
        std::string print_goto = " GOTO ";
        if (modifygotoLabels)
            print_goto = " GOTO L";
        if (instr.arg1.value == "IF")
            s += "IF " + instr.arg2.value + print_goto + std::to_string(instr.gotoLabel);
        else
            s += print_goto.substr(1) + std::to_string(instr.gotoLabel);
    }
    else if (curr_op == "CopyToOffset")
        s += instr.result.value + " offset " + instr.arg2.value + " <- " + instr.arg1.value;
    else if (curr_op == "intToFloat" || curr_op == "floatToInt" || curr_op == "charToInt" || curr_op == "intToChar" || curr_op == "SIZEOF")
        s += instr.result.value + " = " + curr_op + "(" + instr.arg1.value + ")";
    else if (curr_op.substr(0, 5) == "CAST_")
        s += instr.result.value + " = CAST " + instr.arg1.value + " to " + curr_op.substr(5);
    return s;
}

inline std::regex hex_integer("[-]?0[xX][0-9a-fA-F]+");
inline std::regex octal_integer("[-]?0[0-7]+");
inline std::regex decimal_integer("[-]?[0-9]+");
inline std::regex scientific_float("[-]?[0-9]+[Ee][+-]?[0-9]+[fF]?");
inline std::regex float_leading_decimal("[-]?[0-9]+\\.[0-9]*([Ee][+-]?[0-9]+)?[fF]?");
inline std::regex float_trailing_decimal("[-]?[0-9]*\\.[0-9]+([Ee][+-]?[0-9]+)?[fF]?");

inline bool is_int_constant(const std::string &s)
{
    return std::regex_match(s, hex_integer) || std::regex_match(s, octal_integer) || std::regex_match(s, decimal_integer);
}

inline bool is_float_constant(const std::string &s)
{
    return std::regex_match(s, scientific_float) || std::regex_match(s, float_leading_decimal) || std::regex_match(s, float_trailing_decimal);
}

inline bool is_num_constant(const std::string &s)
{
    return is_int_constant(s) || is_float_constant(s);
}

#endif
