#ifndef TAC_HPP
#define TAC_HPP

#include <iostream>
#include <vector>

class quad
{
    // not sure if it should be strings or what
public:
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

extern std::vector<quad> tac_code;

void run_optimisations();

void print_tac_code(const std::string &inputFile);

#endif
