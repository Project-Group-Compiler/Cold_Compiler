#ifndef TYPECHECK_HPP
#define TYPECHECK_HPP

#include "symbol_table.hpp"
#include <string>
#include <cctype> // for isxdigit
#include <iostream>

std::string searchIdentifierType(std::string id);
std::string postfixExpression(std::string type_name, int rule_num);
std::string checkType(std::string a, std::string b);
std::string argExp(std::string a, std::string b, int rule_num);
std::string unaryExp(std::string op, std::string type);
std::string mulExp(std::string a, std::string b, char op);
std::string addExp(std::string a, std::string b, char op);
std::string shiftExp(std::string a, std::string b);
std::string relExp(std::string a, std::string b);
std::string eqExp(std::string a, std::string b);
std::string bitExp(std::string a, std::string b);
std::string assignExp(std::string a, std::string b, std::string op);
std::string condExp(std::string a, std::string b);
int checkInt(std::string type1);
bool isFloat(std::string type);
bool checkChar(std::string type);

#endif // TYPECHECK_HPP