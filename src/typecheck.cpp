#include "typecheck.h"


string primaryExpression(string id) {
    sym_entry* n = lookup(id);
    if(n) {
        string s = n->type;
        return s;     
    }
    return "";
}
string constantEx(int numType ){
    switch(numType){
        case 1: return "int";
        case 2: return "long";
        case 3: return "long long";
        case 4: return "float";
        case 5: return "double";
        case 6: return "long double";
        default : return "default";
    }
}

string postfixExpression(string type_name, int rule_num) {
    string result_type;
    switch(rule_num) {
        case 1: {
            if(type_name.back()=='*')return type_name.substr(0,type_name.length()-1);
            return "";
        }
        case 2:{
            if(type_name.substr(0, 5)=="FUNC_")return type_name.substr(5, type_name.length() - 5); //TODO
            return "";
        }
        case 3:{
            if(type_name.substr(0, 5)=="FUNC_")return type_name.substr(5, type_name.length() - 5); //TODO
            return "";
        }
        case 6:{
            if(checkInt(type_name))return type_name;
            return "";
        }
        case 7:{
            if(checkInt(type_name))return type_name;
            return "";
        }
        default : return "";
        
    }
    
}

string checkType(string a, string b){
    if(a == b)return "ok";
    if((a == "void*" && b.back()=='*')||(a.back()=='*' && b == "void"))return "ok";
    if(a.back()=='*' && b.back()=='*')return "warning";
    if((checkInt(a) && b.back()=='*')||(checkInt(b)&&a.back()=='*'))return "warning";
    if(a == "char" || checkInt(a)) a = "long double";
    if(b == "char" || checkInt(b)) b = "long double";
    if(isFloat(a) && isFloat(b)) return "ok";
    return "";
}

string argExp(string a, string b, int  rule_num){
    if(rule_num == 1){
        if(a == "void") return a;
        return "";
    }
    else{
        if(a == "void" && b=="void") return a;
        return "";
    }
}

string unaryExp(string op, string type){
    if(op=="*") return postfixExpression(type, 1);
    else if(op == "&") type+="*";
    else if(op=="+" || op=="-"){
        if(!(checkInt(type) || isFloat(type))) return "";
    }
    else if(op=="~"){
        if(!(checkInt(type) || type != "bool")) return "";
    }
    else if(op=="!" && type!="bool") return "";
    return type;
}

string mulExp(string a, string b, char op){
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

string addExp(string a, string b, char op){
    if(checkInt(a) && checkInt(b)) return "int";
    else if((checkInt(a) || isFloat(a)) && (checkInt(b) || isFloat(b))) return "real";
    else if((checkInt(a) && b=="char") || (a=="char" && checkInt(b))) return "char";
    else if(checkInt(a) && b.back()=='*') return b;
    else if(a.back()=='*' && checkInt(b)) return a;
    return "";
}

string shiftExp(string a, string b){
    if(checkInt(a) && checkInt(b)) return "ok";
    return "";
}

string relExp(string a, string b){
    if((checkInt(a) || isFloat(a) || a=="char") && (checkInt(b) || isFloat(b) ||b=="char")) return "bool";
    if((checkInt(a) || a=="char") && b.back()=='*') return "Bool";
    if((checkInt(b) || b=="char") && a.back()=='*') return "Bool";
    return "";
}

string eqExp(string a, string b){
    if(a==b) return "Ok";
    if((checkInt(a) || isFloat(a) || a=="char") && (checkInt(b) || isFloat(b) ||b=="char")) return "Ok";
    if((checkInt(a) && b.back()=='*') || (a.back()=='*' && checkInt(b))) return "ok";
    return "";
}

string bitExp(string a, string b){
    if(a=="bool" && b=="bool") return "ok";
    if((a=="bool" || checkInt(a)) && (b=="bool" || checkInt(b))) return "Ok";
    return "";
}

string assignExp(string a, string b, string op){
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

string condExp(string a,string b){
    if(a == b)return a;
    if(a == "char"|| checkInt(a)) a = "long double";
    if(b == "char"|| checkInt(b)) b = "long double";
    if(isFloat(a) && isFloat(b)) return "long double";
    if(a.back() == '*' && b.back() == '*')return "void*" ;
    return "";

}
int checkInt(string type1){
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


bool isFloat (string type){
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

// bool isSignedInt (string type){
//    if(type=="int") return 1;
//    if(type=="long") return 1;
//    if(type=="long long") return 1;
//    if(type=="long int") return 1;
//    if(type=="long long int") return 1;
//    if(type=="signed int") return 1;
//    if(type=="signed long") return 1;
//    if(type=="signed long long") return 1;
//    if(type=="signed long int") return 1;
//    if(type=="signed long long int") return 1;
//    if(type=="short") return 1;
//    if(type=="short int") return 1;
//    if(type=="signed short") return 1;
//    if(type=="signed short int") return 1;
//    return 0;
// }
// bool isSignedFloat (string type){
//    if(type=="float") return 1;
//    if(type=="double") return 1;
//    if(type=="long double") return 1;
//    if(type=="signed float") return 1;
//    if(type=="signed double") return 1;
//    if(type=="signed long double") return 1;
//    return 0;
// }
