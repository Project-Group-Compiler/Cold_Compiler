#include "types.hpp"

std::string searchIdentifierType(std::string id) {
    sym_entry* n = lookup(id);
    if(n) {
        std::string s = n->type;
        return s;     
    }
    return "";
}

std::string postfixExpression(std::string type, int rule_num) {
    if (type.empty()) {
        return "";
    }
    
    switch(rule_num) {
        case 1: {
            if(type.back()=='*') return type.substr(0, type.length()-1);
            return "";
        }
        case 2: {
            if(type.size() >= 5 && type.substr(0, 5)=="FUNC_") 
                return type.substr(5, type.length() - 5);
            return "";
        }
        case 3: {
            if(checkInt(type)) return type;
            return "";
        }
        default: 
            return "";
    }
}

std::string checkType(std::string a, std::string b){
    std::cout << "checkType: " << a << " " << b << std::endl;
    if(a.empty() || b.empty()) return "";
    if(a == b) return "ok";
    if(b == "void") return "";
    if((a == "void*" && b.back()=='*')||(a.back()=='*' && b == "void*")) return "ok";
    if(a.back()=='*' && b.back()=='*') return "warning";
    if((checkInt(a) && b.back()=='*')||(checkInt(b) && a.back()=='*')) return "";
    if(a == "char" || checkInt(a)) a = "float";
    if(b == "char" || checkInt(b)) b = "float";
    if(isFloat(a) && isFloat(b)) return "ok";
    return "";
}

std::string argExp(std::string a, std::string b, int rule_num){
    if(rule_num == 1){
        if(a == "void") return a;
        return "";
    }
    else{
        if(a == "void" && b=="void") return a;
        return "";
    }
}

std::string unaryExp(std::string op, std::string type){
    if(op=="*") return postfixExpression(type, 1);
    else if(op == "&") type+="*";
    else if(op=="+" || op=="-"){
        if(!(checkInt(type) || isFloat(type))) return "";
    }
    else if(op=="~"){
        if(!(checkInt(type) || type != "bool")) return "";
    }
    else if(op=="!"){
        if(!(checkInt(type) || type != "bool")) return "";
    } 
    return type;
}

std::string mulExp(std::string a, std::string b, char op){
    if(op=='*' || op =='/'){
        if(checkInt(a) && checkInt(b)) return "int";
        else if((checkInt(a) || isFloat(a)) && (checkInt(b) || isFloat(b))) return "float";
        return "";
    }
    else if(op=='%'){
        if(checkInt(a) && checkInt(b)) return "int";
    }
    return "";
}

std::string addExp(std::string a, std::string b, char op){
    if(checkInt(a) && checkInt(b)) return "int";
    else if((checkInt(a) || isFloat(a)) && (checkInt(b) || isFloat(b))) return "float";
    else if((checkInt(a) && b=="char") || (a=="char" && checkInt(b))) return "char";
    else if(checkInt(a) && b.back()=='*') return b;
    else if(a.back()=='*' && checkInt(b)) return a;
    return "";
}

std::string shiftExp(std::string a, std::string b){
    if(checkInt(a) && checkInt(b)) return "ok";
    return "";
}

std::string relExp(std::string a, std::string b){
    if((checkInt(a) || isFloat(a) || a=="char") && (checkInt(b) || isFloat(b) ||b=="char")) return "bool";
    if((checkInt(a) || a=="char") && b.back()=='*') return "Bool";
    if((checkInt(b) || b=="char") && a.back()=='*') return "Bool";
    return "";
}

std::string eqExp(std::string a, std::string b){
    if(a==b) return "Ok";
    if((checkInt(a) || isFloat(a) || a=="char") && (checkInt(b) || isFloat(b) ||b=="char")) return "Ok";
    if((checkInt(a) && b.back()=='*') || (a.back()=='*' && checkInt(b))) return "ok";
    return "";
}

std::string bitExp(std::string a, std::string b){
    if(a=="bool" && b=="bool") return "ok";
    if((a=="bool" || checkInt(a)) && (b=="bool" || checkInt(b))) return "Ok";
    return "";
}

std::string assignExp(std::string a, std::string b, std::string op){
    if(op=="="){
        return checkType(a,b);
    }
    if(op == "*=" || op == "/=" || op == "%="){
        if(mulExp(a, b, op[0])=="") return "";
        else return "ok";
    }
    if(op == "+=" || op == "-="){
        if(addExp(a, b, op[0])=="") return "";
        else return "ok";
    }
    if(op == ">>=" || op == "<<="){
        if(shiftExp(a, b)=="") return "";
        else return "ok";
    }
    if(op == "&=" || op == "|=" || op == "^="){
        if(bitExp(a, b)=="") return "";
        else return "ok";
    }
    return "";
}

std::string condExp(std::string a, std::string b){
    if(a == b) return a;
    if(a == "char"|| checkInt(a)) a = "long double";
    if(b == "char"|| checkInt(b)) b = "long double";
    if(isFloat(a) && isFloat(b)) return "long double";
    if(a.back() == '*' && b.back() == '*') return "void*";
    return "";
}

int checkInt(std::string type1){
   if(type1=="int") return 1;
   if(type1=="long") return 1;
   if(type1=="long long") return 1;
   if(type1=="long int") return 1;
   if(type1=="long long int") return 1;
   if(type1=="unsigned int") return 1;
   if(type1=="unsigned long") return 1;
   if(type1=="unsigned long long") return 1;
   if(type1=="unsigned long int") return 1;
   if(type1=="unsigned long long int") return 1;
   if(type1=="signed int") return 1;
   if(type1=="signed long") return 1;
   if(type1=="signed long long") return 1;
   if(type1=="signed long int") return 1;
   if(type1=="signed long long int") return 1;
   if(type1=="short") return 1;
   if(type1=="short int") return 1;
   if(type1=="signed short") return 1;
   if(type1=="unsigned short") return 1;
   if(type1=="unsigned short int") return 1;
   if(type1=="signed short int") return 1;
   return 0;
}

bool isFloat(std::string type){
   if(type=="float") return 1;
   if(type=="double") return 1;
   if(type=="long double") return 1;
   if(type=="unsigned float") return 1;
   if(type=="unsigned double") return 1;
   if(type=="unsigned long double") return 1;
   if(type=="signed float") return 1;
   if(type=="signed double") return 1;
   if(type=="signed long double") return 1;
   return 0;
}

bool checkChar(std::string temp){
   // Validate character constant
   bool valid = false;
   // Simple character: 'x'
   if (temp.size() == 3 && temp[0] == '\'' && temp[2] == '\'') {
       return true;
   }
   // Escape sequence: '\n', '\t', etc.
   else if (temp.size() == 4 && temp[0] == '\'' && temp[3] == '\'' && temp[1] == '\\') {
       char esc = temp[2];
       return (esc == 'n' || esc == 't' || esc == 'r' || esc == '0' || 
               esc == '\\' || esc == '\'' || esc == '\"' || esc == 'a' || 
               esc == 'b' || esc == 'f' || esc == 'v');
   }
   // Hex escape sequence: '\xhh'
   else if (temp.size() >= 5 && temp.size() <= 8 && temp[0] == '\'' && temp.back() == '\'' && temp[1] == '\\' && temp[2] == 'x') {
        valid = true;
        for (size_t i = 3; i < temp.size() - 1; i++) {
            if (!std::isxdigit(temp[i])) {
                valid = false;
                return false;
                break;
            }
        }
    }
    return valid;
}