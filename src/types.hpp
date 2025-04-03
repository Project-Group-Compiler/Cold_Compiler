#ifndef TYPECHECK_HPP
#define TYPECHECK_HPP

#include "symbol_table.hpp"
#include <string>
#include <cctype> // for isxdigit
#include <iostream>

int checkInt(std::string type);
bool isFloat(std::string type);
bool checkChar(std::string type);
std::string searchIdentifierType(std::string id);
std::string postfixExpression(std::string type_name, int rule_num);
std::string checkType(std::string a, std::string b);
std::string unaryExp(std::string op, std::string type);
std::string Exp(std::string a, std::string b, std::string op);
std::string shiftExp(std::string a, std::string b);
std::string relExp(std::string a, std::string b);
std::string eqExp(std::string a, std::string b);
std::string bitExp(std::string a, std::string b);
std::string assignExp(std::string a, std::string b, std::string op);
std::string condExp(std::string a, std::string b);


#endif // TYPECHECK_HPP