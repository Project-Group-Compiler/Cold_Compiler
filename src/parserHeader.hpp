#ifndef PARSERHEADER_HPP
#define PARSERHEADER_HPP

using namespace std;
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include "AST.hpp"
#include "types.hpp"
#include "data_structures.hpp"
#include "tac.hpp"

std::string currentDataType="";
std::string currentAccess = "", tdstring="", tdstring2="";//for classes
int noArgs = 0;
int flag = 0, flag2 = 0, flag3 = 0;
int enum_ctr=0;

extern int yyleng;
extern char* yytext;
extern int column;
extern int line;
extern std::string inputFilename;
extern std::string outputDir;
extern bool has_error;

//Semantics
bool isStaticDecl = false;
bool isConstDecl = false;
bool array_decl = 0;
bool enum_decl = 0;
string funcName = "";
string structName = "";
string className="";
string funcType = "";
int block_count = 0;
stack<int> block_stack;
bool fn_decl = 0;
int func_flag = 0;

string type = "";
int Anon_StructCounter=0;
int Anon_ClassCounter = 0;
std::vector<std::string> funcArgs;
std::vector<std::string> classMethodArgs; 
std::vector<std::string> varargsFuncs; 
bool inMethodBody = false;  // Set true when inside a method body
int classMethodSize=0;
std::vector<std::string> idList;
std::vector<std::string> currArgs;
std::vector<operand> actualArgs;
std::vector<int>previousCaseList;
std::vector<int>CaseContinueList;

// Debug tracking
extern bool debug_enabled; // Flag to enable or disable debugging
std::ofstream out;
void DBG(const std::string&rule){
	 if (debug_enabled) 
		out<<"DEBUG: Processing rule "<<rule<<" at line "<<line<<"\n";
}

void semantic_error(const char* s, const std::string &errorType="semantic error");
void yyerror(const char* s, const std::string &errorType = "syntax error");
int yylex();
int warning(const char*);

//3 - AC 
bool is_arr = false;
int rValue = 0;
int if_found = 0; //TODO : Rename to inside a selection stmt/also in while 
int previous_if_found = 0; // TODO: May need later
std::vector<operand> list_values;
std::map<std::string, std::vector<int>> gotolablelist;
std::map<std::string, int> gotolabel;
template <typename T>
void _print_(const T& x) {
    if (debug_enabled) out << x;
}

template <typename T, typename... Args>
void _print_(const T& x, const Args&... rest) {
    if (debug_enabled) out << x << ", ";
    _print_(rest...);
}
#define debug(x...) if (debug_enabled) out << "(Line " << __LINE__ << "): [" << #x << "] => "; _print_(x); if (debug_enabled) out << std::endl;

#endif // PARSERHEADER_HPP
