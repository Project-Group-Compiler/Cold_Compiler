%{
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
#include <algorithm>

std::string currentDataType="";
std::string currentAccess = "", tdstring="", tdstring2="";//for classes
int noArgs = 0;
int flag = 0, flag2 = 0, flag3 = 0;

extern int yyleng;
extern char* yytext;
extern int column;
extern int line;
extern std::string inputFilename;
extern std::string outputDir;
extern bool has_error;

//Semantics
bool array_decl = 0;
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
vector<string> funcArgs;
vector<string> classMethodArgs; 
bool inMethodBody = false;  // Set true when inside a method body
vector<string> idList;
vector<string> currArgs;
vector<string> actualArgs;

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
int rValue = 0;
int if_found = 0; //TODO : Rename to inside a selection stmt/also in while 
int previous_if_found = 0; // TODO: May need later
std::vector<std::string> list_values;
std::map<std::string, std::vector<int>> gotolablelist;
std::map<std::string, int> gotolabel;
template <typename T>void debug(T x){std::cerr<<x<<'\n';}template <typename T>void debugsp(T x) {std::cerr << x << ' ';}
template <typename T1, typename... T2>void debug(T1 x, T2... y){debugsp(x);if (sizeof...(y) == 1) debug(y...);}

std::vector<int>previousCaseList;
std::vector<int>CaseContinueList;
%}

%define parse.error detailed
%define parse.lac full

%union{
	char* str;
	Node* ptr;
	constants* num;
	int Int;
}

%token<str> IDENTIFIER STRING_LITERAL SIZEOF
%token<str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP INHERITANCE_OP
%token<str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token<str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token<str> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token<str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token<str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID FILE_MAN VA_LIST
%token<str> STRUCT UNION ENUM ELLIPSIS
%token<str> CLASS PUBLIC PRIVATE PROTECTED

%token<str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token<str> UNTIL /*** Added UNTIL token ***/
//Semantic
%token<num> CONSTANT
%type<str> F G G_C
%type<str> CHANGE_TABLE 

//3AC 
%type<Int> NEXT_QUAD WRITE_GOTO
%type<ptr> GOTO_COND CASE_CODE IF_CODE EXPR_CODE EXPR_STMT_CODE DEFAULT_CODE
%type<ptr> N


%start translation_unit

%type<ptr> primary_expression postfix_expression argument_expression_list unary_expression unary_operator cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression
%type<str> assignment_operator 
%type<ptr> and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression 
%type<ptr> assignment_expression expression constant_expression declaration declaration_specifiers init_declarator_list
%type<ptr> declarator direct_declarator pointer type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list type_name abstract_declarator direct_abstract_declarator initializer
%type<ptr> init_declarator type_specifier struct_or_union_specifier	struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator enum_specifier enumerator_list enumerator type_qualifier
%type<ptr> statement labeled_statement compound_statement declaration_list statement_list expression_statement selection_statement iteration_statement jump_statement translation_unit external_declaration function_definition initializer_list
%type<ptr> storage_class_specifier
%type<str> struct_or_union
%type<ptr> class_definition inheritance_specifier inheritance_specifier_list access_specifier class class_definition_head class_internal_definition_list class_internal_definition	class_member_list class_member

%left ';'
%%

primary_expression
    : IDENTIFIER {
        DBG("primary_expression -> IDENTIFIER");
    	$$ = getNode(std::string($1));
		
		// Semantics
		string temp = searchIdentifierType(string($1));
		if(temp.empty()){
			semantic_error(("Undeclared Identifier " + string($1)).c_str(), "scope error");
		}
		else{
			if(temp.substr(0, 5) == "FUNC_"){
				$$->expType = 3;
			}
			else if(temp.back() == '*'){
				$$->expType = 2; 
			}
			else $$->expType = 1;
			$$->type = temp;
			$$->isInit = lookup(string($1))->init;
			$$->size = getSize(temp);
			$$->temp_name = string($1); 

			//3AC
			if(temp.substr(0, 5) == "FUNC_") {//remove
				$$->place = "CALL " + std::string($1);
			} 
			else {
				$$->place = std::string($1);
			}
			$$->nextlist.clear();
		}
    }
	| CONSTANT IDENTIFIER {
		DBG("primary_expression -> CONSTANT IDENTIFIER");
		yyerror("invalid identifier", "syntax error");
		$$ = getNode($2);
	}
	| CONSTANT CONSTANT {
		DBG("primary_expression -> CONSTANT CONSTANT");
		yyerror("invalid constant", "syntax error");
		std::string tp = std::string($1->str);
		$$ = getNode(tp);
	}
	| CONSTANT {
		std::string tp = std::string($1->str);
		$$ = getNode(tp);
		
		// Semantics for constants
		$$->type = $1->type;
		$$->intVal = $1->intVal;
		$$->realVal = $1->realVal;
		$$->expType = 4;
		$$->temp_name = $1->str;
		
		if($$->type=="char"){
			DBG("DEBUG: Constant type = " + std::string($1->str));
			std::string temp = std::string($1->str);
			if (!checkChar(temp)) {
				yyerror(("Invalid character constant: " + temp + ". Character constants must be a single character.").c_str(),"syntax error");
				// Set to a default value to prevent further errors
			}
		}
		//3AC
		$$->place = $$->temp_name;
		$$->nextlist.clear();

	}
	| STRING_LITERAL {
        DBG("primary_expression -> STRING_LITERAL");
		$$ = getNode($1);
		
		// Semantics for string literals
		$$->type = string("char*");
		$$->temp_name = string($1);
		$$->strVal = string($1);

		//3AC
		$$->place = $$->temp_name;
		$$->nextlist.clear();
	}
	| '(' expression ')' {
        DBG("primary_expression -> '(' expression ')'");
		$$ = $2;
	}
	;

postfix_expression
	: primary_expression {
        DBG("postfix_expression -> primary_expression");
		$$ = $1;
	}
	| postfix_expression '[' expression ']' {
        DBG("postfix_expression -> postfix_expression '[' expression ']'");
		$$ = getNode("postfix_expression", mergeAttrs($1, $3));
		
		// Semantics
		if($1->isInit && $3->isInit){
			$$->isInit = 1;
		}
		string temp = postfixExpression($1->type, 1);
		if(!temp.empty()){	
			$$->type = temp;
			//3AC
			if($$->type == "int"){
				std::string q = getTempVariable("int*");
				emit("=", $1->place,"", q, -1);
				std::string q2 = getTempVariable("int");
				emit("*", $3->place, getSizeOfType("int"), q2, -1);
				std::string q3 = getTempVariable("int*");
				emit("ptr+", q, q2, q3, -1);
				$$->place = "*" + q3;
			}else{
				std::string q = getTempVariable($$->type);
				$$->place = q;
				emit("[ ]", $1->place, $3->place, q, -1);
			}
			$$->temp_name = $1->temp_name;
		}
		else{
			//semantic_error(("Array " + $1->temp_name +  " Index out of bound").c_str(), "semantic error"); ->TODO
		}
	}
	| postfix_expression '(' ')' {
        DBG("postfix_expression -> postfix_expression '(' ')'");
		$$ = $1;
		
		// Semantics
		$$->isInit = 1;
		string temp = postfixExpression($1->type, 2);
		if(!temp.empty()){	
			$$->type = temp;
			if($1->expType == 3){
				std::string mangledName = mangleFunctionName($1->temp_name,currArgs);
				vector<string> funcArg = getFuncArgs(mangledName);
				
				if(currArgs.size()!=funcArg.size()){
					semantic_error(("Incorrect signature while calling function " + $1->temp_name).c_str(), "semantic error");
				}
				else{
					//3AC
					$$->temp_name = $1->temp_name;
					if($$->type != "void")
					{
						std::string q2 = getTempVariable($$->type);
						emit("CALL", mangledName, "0", q2, -1);
        				$$->place = q2;
					}
					else
					{
						emit("CALL", mangledName, "0", "", -1);
					}
					$$->nextlist.clear();
				}
			}
		}
		else{
			semantic_error(("Function " + $1->temp_name + " not declared in this scope").c_str(), "scope error");
		}
		currArgs.clear(); 
		actualArgs.clear();
	}
	| postfix_expression '(' argument_expression_list ')' {//for function calls
        DBG("postfix_expression -> postfix_expression '(' argument_expression_list ')'");
		$$ = getNode("postfix_expression", mergeAttrs($1, $3));
		
		// Semantics
		$$->isInit = $3->isInit;
		
		// Create mangled name with current arguments
	    std::string mangledName = mangleFunctionName($1->temp_name, currArgs);
		string temp = searchIdentifierType(mangledName);
		if(temp.empty()){
			//semantic_error(("Undeclared Identifier " + $1->temp_name +" .Incorrect Function overloading.").c_str(), "scope error");//->repetive error msg
		}
		else{
			if(temp.substr(0, 5) == "FUNC_"){
				$1->expType = 3;
			}
			else if(temp.back() == '*'){
				$1->expType = 2; 
			}
			else $1->expType = 1;
			$1->type = temp;
			DBG("DEBUG: Function call type = " + $1->type);
			$1->isInit = lookup(mangledName)->init;
			$1->size = getSize(temp);
		}


		temp = postfixExpression($1->type, 2);
		
		if(!temp.empty()){	
			$$->type = temp;
			DBG("DEBUG: Function call type = " + $$->type);
			if($1->expType == 3){
				vector<string> funcArgs = getFuncArgs(mangledName);
				if (funcArgs.back() == "...") { //if "..." is present, remove it and do not check for size
					funcArgs.pop_back();
				}
				else if(currArgs.size()!=funcArgs.size()){
					semantic_error(("Incorrect signature while calling function " + $1->temp_name).c_str(), "semantic error");
				}
				else{
					for(int i=0; i<funcArgs.size(); i++){
					if(funcArgs[i]=="...")break;
					string msg = checkType(funcArgs[i],currArgs[i]);
					if(msg.empty()){
						semantic_error(("Incorrect signature while calling function " + $1->temp_name).c_str(), "semantic error");
						break;
					}
				}
				}
				//3AC
				$$->temp_name = $1->temp_name;
				reverse(actualArgs.begin(), actualArgs.end());
				for(auto&x : actualArgs)
					emit("param", x, "", "", -1);
				if($$->type != "void")
				{
					std::string q2 = getTempVariable($$->type);
					emit("CALL", mangledName, std::to_string(currArgs.size()), q2, -1);
					$$->place = q2;
				}
				else
				{
					emit("CALL", mangledName, std::to_string(currArgs.size()), "", -1);
				}
				$$->nextlist.clear();
			}
		}
		else{
			semantic_error("Invalid function call", "semantic error");
		}
	    currArgs.clear();
		actualArgs.clear();
	}
	| postfix_expression '.' IDENTIFIER {
    	DBG("postfix_expression -> postfix_expression '.' IDENTIFIER");
    	$$ = getNode("expression.id", mergeAttrs($1, getNode($3)));
	
    	// Semantics - check if it's a class or struct
    	string temp = string($3);
    	string memberName = $1->temp_name;
    	string type = $1->type;
		
		// Debug the type to see what's reaching this rule

		if ($1->temp_name == "this" || ($1->type.substr(0, 6) == "CLASS_" && !className.empty() && $1->type.substr(6) == className)) {
        	// We're inside a class method accessing a member through 'this'
        	// Check if the member exists in the current class structure
        	if (curr_class_structure && (*curr_class_structure).find(temp) != (*curr_class_structure).end()) {
        	    $$->type = (*curr_class_structure)[temp]->type;
        	    $$->temp_name = $1->temp_name + "." + temp;
        	} else {
        	    semantic_error(("Member '" + temp + "' not found in class '" + className + "'").c_str(), "scope error");
        	}
    	}
    	// First check if it's a regular class
		else if (type.substr(0, 6) == "CLASS_") {
    	    int ret = lookupClass(type, temp);
    	    if (ret == 1) {
    	        string memberAccess = ClassAttrAccess(type, temp);
        
        		// If outside class trying to access private/protected member
        		if ((memberAccess == "private" || memberAccess == "protected") && 
        		    (className.empty() || className != type.substr(6))) {
        		    semantic_error(("Cannot access " + memberAccess + " member '" + temp + 
        		            "' of class '" + type.substr(6) + "'").c_str(), "scope error");
        		} else {
        		    // Member found and accessible
        		    $$->type = ClassAttrType(type, temp);
        		    $$->temp_name = $1->temp_name + "." + temp;
        		}
    	    }
    	    else if (ret == 0) {
    	        semantic_error(("Member '" + temp + "' not found in class").c_str(), "scope error");
    	    }
    	    else {
    	        semantic_error(("Class '" + memberName + "' not defined").c_str(), "scope error");
    	    }
    	}
    	// If not a class, try struct
    	else {
    	    int ret = lookupStruct($1->type, temp);
    	    if (ret == -1) {
    	        semantic_error(("Struct or class '" + memberName + "' not defined").c_str(), "scope error");
    	    }
    	    else if (ret == 0) {
    	        semantic_error(("Member '" + temp + "' not found in struct").c_str(), "scope error");
    	    }
    	    else {
    	        $$->type = StructAttrType($1->type, temp);
    	        $$->temp_name = $1->temp_name + "." + temp;
    	    }
    	}
		//3AC
		std::string q = getTempVariable($1->type+'*'); 
		emit("unary&", $1->place, "", q, -1);
		if(type.substr(0, 6) != "UNION_")
			emit("ptr+", q, std::string($3), q, -1); //TODO: REPLACE $3 with $3->offset 
		q = "*" + q;
        $$->place = q;
		$$->nextlist.clear();

	}
	| postfix_expression '.' IDENTIFIER '(' ')'  {
	    DBG("postfix_expression -> postfix_expression '.' IDENTIFIER '(' ')'");
	    
		$$ = getNode("method_call", mergeAttrs($1, getNode($3)));
	
	    // Semantics - check if it's a class method call
	    string classType = $1->type;
	    string methodName = string($3);
		std::string manglemethod;
    	// Special case for 'this' pointer access within a class definition
    	if ($1->temp_name == "this" || 
    	    ($1->type.substr(0, 6) == "CLASS_" && !className.empty() && 
    	     $1->type.substr(6) == className)) {
			
			manglemethod=mangleFunctionName(methodName,currArgs);//need to skip 'this'
			manglemethod="FUNC_" + std::to_string(className.size()) + className + "_" + manglemethod.substr(5);

    	    // We're inside a class method calling another method through 'this'
    	    if (curr_class_structure && (*curr_class_structure).find(manglemethod) != (*curr_class_structure).end()) {
    	        string methodType = (*curr_class_structure)[manglemethod]->type;
	
    	        if (methodType.substr(0, 5) == "FUNC_") {
    	            $$->type = methodType.substr(5); // Extract return type
    	            $$->temp_name = $1->temp_name + "." + methodName;
    	            $$->isInit = 1;
	
    	            // Check arguments
    	            vector<string> methodArgs = getFuncArgs(manglemethod);
    	            if (methodArgs.size() > 1) { // More than 1 because of implicit 'this'
						semantic_error(("Incorrect signature while calling method " + methodName).c_str(), "semantic error");
    	            }
    	        } else {
    	            semantic_error(("Member '" + methodName + "' is not a method").c_str(), "semantic error");
    	        }
    	    } else {
    	        semantic_error(("Method '" + methodName + "' not found in class '" + className + "' with this signature").c_str(), "scope error");
    	    }
    	}
	    // First check if it's a class
	    else if (classType.substr(0, 6) == "CLASS_") {
	        // Look up method in class
			manglemethod=mangleFunctionName(methodName,currArgs);
			manglemethod="FUNC_" + std::to_string((classType.substr(6)).size()) + classType.substr(6) + "_" + manglemethod.substr(5);
	        int ret = lookupClass(classType, manglemethod);

	        if (ret == 1) {
				string memberAccess = ClassAttrAccess(classType, manglemethod);
				
        		// If outside class trying to access private/protected member
        		if ((memberAccess == "private" || memberAccess == "protected") && 
        		    (className.empty() || className != classType.substr(6))) {
        		    semantic_error(("Cannot access " + memberAccess + " member '" + methodName + 
        		            "' of class '" + classType.substr(6) + "'").c_str(), "scope error");
        		}
				else{
	            	// Get method type (should be FUNC_returnType)
	            	string methodType = ClassAttrType(classType, manglemethod);

	            	// Check if it's a function
	            	if (methodType.substr(0, 5) == "FUNC_") {
	            	    string returnType = methodType.substr(5); // Extract return type
	            	    $$->type = returnType;
	            	    $$->temp_name = $1->temp_name + "." + methodName;
	            	    $$->isInit = 1;

	            	    // Check arguments (none for this rule)
	            	    vector<string> methodArgs = getFuncArgs(manglemethod);
	            	    if (methodArgs.size() > 1) {  // More than 1 because the first is the implicit 'this'
							semantic_error(("Incorrect signature while calling method " + methodName).c_str(), "semantic error");
                		}
	            	} 
					else {
	            	    semantic_error(("Member '" + methodName + "' is not a method").c_str(), "semantic error");
	            	}
				}
	        } else if (ret == 0) {
	            semantic_error(("Method '" + methodName + "' not found in class with this signature").c_str(), "scope error");
	        } else {
	            semantic_error(("Class '" + $1->temp_name + "' not defined").c_str(), "scope error");
	        }
	    } else {
	        semantic_error("Cannot call method on non-class type", "type error");
	    }
		//3AC
		std::string q = getTempVariable($1->type+'*'); 
		emit("unary&", $1->place, "", q, -1);
		emit("param", q, "", "", -1);
		if($$->type != "void")
		{
			std::string q2 = getTempVariable($$->type);
			emit("CALL", manglemethod, "1", q2, -1);
			$$->place = q2;
		}
		else
		{
			emit("CALL", manglemethod, "1", "", -1);
		}
	    currArgs.clear();
		actualArgs.clear();
		$$->nextlist.clear();
	}
	| postfix_expression '.' IDENTIFIER '(' argument_expression_list ')' {
	    DBG("postfix_expression -> postfix_expression '.' IDENTIFIER '(' argument_expression_list ')'");
		$$ = getNode("method_call_args", mergeAttrs($1, getNode($3), $5));
	
	    // Semantics - check if it's a class method call with arguments
	    string classType = $1->type;
	    string methodName = string($3);
		std::string manglemethod;
    	// Special case for 'this' pointer access within a class definition
    	if (($1->temp_name == "this") || 
    	    (classType.substr(0, 6) == "CLASS_" && !className.empty() && 
    	     classType.substr(6) == className)) {
			manglemethod=mangleFunctionName(methodName,currArgs);//need to skip 'this'
			manglemethod="FUNC_" + std::to_string(className.size()) + className + "_" + manglemethod.substr(5);

    	    // We're inside a class method calling another method through 'this'
    	    if (curr_class_structure && (*curr_class_structure).find(manglemethod) != (*curr_class_structure).end()) {
    	        string methodType = (*curr_class_structure)[manglemethod]->type;
				
    	        if (methodType.substr(0, 5) == "FUNC_") {
    	            string returnType = methodType.substr(5); // Extract return type
    	            $$->type = returnType;
    	            $$->temp_name = $1->temp_name + "." + methodName;
    	            $$->isInit = $5->isInit;

    	            // Check arguments
    	            vector<string> methodArgs = getFuncArgs(manglemethod);
	
    	            // Check number of arguments (account for implicit 'this')
    	            if (currArgs.size() != methodArgs.size() - 1) {
    	                semantic_error(("Incorrect signature while calling method " + methodName).c_str(), "semantic error");
    	            } else {
    	                // Type check arguments
    	                for (int i = 1; i < methodArgs.size(); i++) { // Start from 1 to skip 'this'
    	                    if (methodArgs[i] == "...") break;
	
    	                    string msg = checkType(methodArgs[i], currArgs[i-1]);
							if (msg.empty()) {
    	                        semantic_error(("Incorrect signature while calling method " + methodName).c_str(), "semantic error");
    	                        break;
    	                    }
    	                }
    	            }
    	        } 
				else {
    	            semantic_error(("Member '" + methodName + "' is not a method").c_str(), "semantic error");
    	        }
    	    } else {
    	        semantic_error(("Method '" + methodName + "' not found in class '" + className + "' with this signature ").c_str(), "scope error");
    	    }
    	}
	    // First check if it's a class
	    else if (classType.substr(0, 6) == "CLASS_") {
	        // Look up method in class
			manglemethod=mangleFunctionName(methodName,currArgs);
			manglemethod="FUNC_" + std::to_string((classType.substr(6)).size()) + classType.substr(6) + "_" + manglemethod.substr(5);
	        int ret = lookupClass(classType, manglemethod);
	        if (ret == 1) {
				string memberAccess = ClassAttrAccess(classType, manglemethod);

        		// If outside class trying to access private/protected member
        		if ((memberAccess == "private" || memberAccess == "protected") && 
        		    (className.empty() || className != classType.substr(6))) {
        		    semantic_error(("Cannot access " + memberAccess + " member '" + methodName + 
        		            "' of class '" + classType.substr(6) + "'").c_str(), "scope error");
        		}
				else{
	            	// Get method type (should be FUNC_returnType)
	            	string methodType = ClassAttrType(classType, manglemethod);

	            	// Check if it's a function
	            	if (methodType.substr(0, 5) == "FUNC_") {
	            	    string returnType = methodType.substr(5); // Extract return type

	            	    // Check arguments against parameter types
	            	    vector<string> methodArgs = getFuncArgs(manglemethod);//gives className_func ->className empty right now
						if(currArgs.size() != methodArgs.size()-1)
							semantic_error(("Incorrect signature while calling method " + methodName).c_str(), "semantic error");
					else{

	            	    for (int i = 1; i < methodArgs.size(); i++) {
	            	        if (methodArgs[i] == "...") break;
							string msg = checkType(methodArgs[i], currArgs[i-1]);
							if (msg.empty()) {
	            	            semantic_error(("Incorrect signature while calling method " + methodName).c_str(), "semantic error");
	            	            break;
	            	        }
	            	    }
					}
	            	    $$->type = returnType;
	            	    $$->temp_name = $1->temp_name + "." + methodName;
	            	    $$->isInit = $5->isInit;
	            	} else {
	            	    semantic_error(("Member '" + methodName + "' is not a method").c_str(), "semantic error");
	            	}
				}
	        } else if (ret == 0) {
	            semantic_error(("Method '" + methodName + "' not found in class with this signature").c_str(), "scope error");
	        } else {
	            semantic_error(("Class '" + $1->temp_name + "' not defined").c_str(), "scope error");
	        }
	    } else {
	        semantic_error("Cannot call method on non-class type", "type error");
	    }
		//3AC
		std::string q = getTempVariable($1->type+'*'); 
		emit("unary&", $1->place, "", q, -1);
		reverse(actualArgs.begin(), actualArgs.end());
		for(auto&x : actualArgs)
			emit("param", x, "", "", -1);
		emit("param", q, "", "", -1); 
		if($$->type != "void")
		{
			std::string q2 = getTempVariable($$->type);
			emit("CALL", manglemethod, std::to_string(currArgs.size()+1), q2, -1);
			$$->place = q2;
		}
		else
		{
			emit("CALL", manglemethod, std::to_string(currArgs.size()+1), "", -1);
		}
		$$->nextlist.clear();
	    currArgs.clear();
		actualArgs.clear();
		$$->nextlist.clear();
	}
	| postfix_expression PTR_OP IDENTIFIER {
        DBG("postfix_expression -> postfix_expression PTR_OP IDENTIFIER");
		$$ = getNode($2, mergeAttrs($1, getNode($3)));
		
		// Semantics
		string temp = string($3);
		string temp1 = ($1->type);
		if(temp1.back() != '*'){
			semantic_error(($1->node_name + " is not a pointer, did you mean to use '.' ").c_str(), "type error");
		}
		else temp1.pop_back();
		
		int ret = lookupStruct(temp1, temp);
		if(ret == -1){
			semantic_error("Struct not defined", "scope error");
		}
		else if (ret == 0){
			semantic_error("Attribute of Struct not defined", "scope error");
		}
		else{
			$$->type = StructAttrType(temp1, temp);
			$$->temp_name = $1->temp_name + "->" + temp;
			//3AC
			std::string q = getTempVariable($1->type); 
			emit("ptr+", $1->place, std::string($3), q, -1); //TODO: REPLACE $3 with $3->offset 
			q = "*" + q;
			$$->place = q;
			//$$->nextlist.clear();
		}
	}
	| postfix_expression INC_OP {
        DBG("postfix_expression -> postfix_expression INC_OP");
		$$ = getNode($2, mergeAttrs($1, nullptr));
		
		// Semantics
		$$->isInit = $1->isInit;
		string temp = postfixExpression($1->type, 3);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $1->intVal + 1;
			//3AC
			std::string q = getTempVariable($$->type);
			$$->place = q;
			$$->nextlist.clear();
			emit("S++", $1->place, "", q, -1);
		}
		else{
			semantic_error("Increment not defined for this type", "type error");
		}
	}
	| postfix_expression DEC_OP {
        DBG("postfix_expression -> postfix_expression DEC_OP");
		$$ = getNode($2, mergeAttrs($1, nullptr));
		
		// Semantics
		$$->isInit = $1->isInit;
		string temp = postfixExpression($1->type, 3);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $1->intVal - 1;
			//3AC
			std::string q = getTempVariable($$->type);
			$$->place = q;
			$$->nextlist.clear();
			emit("S--", $1->place, "", q, -1);
		}
		else{
			semantic_error("Decrement not defined for this type", "type error");
		}
	}
	;

argument_expression_list
	: assignment_expression {
        DBG("argument_expression_list -> assignment_expression");
		$$ = $1;
		
		// Semantics
		$$->isInit = $1->isInit;
		currArgs.push_back($1->type);
		actualArgs.push_back($$->place);
		$$->type = "void";
		//3AC
		$$->nextlist.clear();
        // emit("param", $$->place, "", "", -1);
	}
	| argument_expression_list ',' assignment_expression {
        DBG("argument_expression_list -> argument_expression_list ',' assignment_expression");
		$$ = getNode("argument_list", mergeAttrs($1, $3));
		
		// Semantics
		
		if($1->isInit && $3->isInit) $$->isInit=1;
		currArgs.push_back($3->type);
		actualArgs.push_back($3->place);
		$$->type = "void";
		//3AC
		$$->nextlist.clear();
	
        // int Label = emit("param", $3->place, "", "", -1);
		// backpatch($3->nextlist, Label);
	}
	;

unary_expression
	: postfix_expression {
        DBG("unary_expression -> postfix_expression");
		$$ = $1;
	}
	| INC_OP unary_expression {
        DBG("unary_expression -> INC_OP unary_expression");
		$$ = getNode($1, mergeAttrs($2, nullptr));
		
		// Semantics
		$$->isInit = $2->isInit;
		string temp = postfixExpression($2->type, 3);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal + 1;
			//3AC
			std::string q = getTempVariable($$->type);
			$$->place = q;
			$$->nextlist.clear();
			emit("++P", $2->place, "", q, -1);
		}
		else{
			semantic_error("Increment not defined for this type", "type error");
		}
	}
	| DEC_OP unary_expression {
        DBG("unary_expression -> DEC_OP unary_expression");
		$$ = getNode($1, mergeAttrs($2, nullptr));

		
		// Semantics
		$$->isInit = $2->isInit;
		string temp = postfixExpression($2->type, 3);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal - 1;
			//3AC
			std::string q = getTempVariable($$->type);
			$$->place = q;
			$$->nextlist.clear();
			emit("--P", $2->place, "", q, -1);
		}
		else{
			semantic_error("Decrement not defined for this type", "type error");
		}
	}
	| unary_operator cast_expression {
		//TODO: l value .. Here
        DBG("unary_expression -> unary_operator cast_expression");
		$$ = getNode("unary_exp", mergeAttrs($1, $2));
		
		// Semantics
		$$->isInit = $2->isInit;
		
    	// Normal unary expression handling
    	string temp = unaryExp($1->node_name, $2->type);
    	if(!temp.empty()){
    	    $$->type = temp;
    	    $$->intVal = $2->intVal;
			if(rValue == 0 && $1->place == "unary*" && $2->type == "int*"){ // (*ptr) = 10 -> ptr store 10 
				$$->temp_name = $2->temp_name;
				$$->place = "*" + $2->place;
				$$->nextlist.clear();
			}
			else{
				std::string q = getTempVariable(temp);
				$$->temp_name = $2->temp_name;
				$$->place = q;
				$$->nextlist.clear();
				emit($1->place, $2->place, "", q, -1);
			}
    	}
    	else{
    	    semantic_error("Type inconsistent with operator", "type error");
    	}
	}
	| SIZEOF unary_expression {
        DBG("unary_expression -> SIZEOF unary_expression");
		$$ = getNode($1, mergeAttrs($2, nullptr));
		
		// Semantics
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $2->size;
		//3AC
        std::string q = getTempVariable("int");
        $$->place = q;
        $$->nextlist.clear();
        emit("SIZEOF", $2->place, "", q, -1);
	}
	| SIZEOF '(' type_name ')' {
        DBG("unary_expression -> SIZEOF '(' type_name ')'");
		$$ = getNode($1, mergeAttrs($3, nullptr));
		
		// Semantics
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $3->size;
		//3AC
        std::string q = getTempVariable("int");
        $$->place = q;
        $$->nextlist.clear();
        emit("SIZEOF", $3->type, "", q, -1);
	}
	;

unary_operator
	: '&' {
		DBG("unary_operator -> '&'");
		$$ = getNode("&");
		//3AC
        $$->place = "unary&";
	}
	| '*' {
		DBG("unary_operator -> '*'");
		$$ = getNode("*");
		//3AC
        $$->place = "unary*";
	}
	| '+' {
		DBG("unary_operator -> '+'");
		$$ = getNode("+");
		//3AC
        $$->place = "unary+";
	}
	| '-' {
		DBG("unary_operator -> '-'");
		$$ = getNode("-");
		//3AC
        $$->place = "unary-";
	}
	| '~' {
		DBG("unary_operator -> '~'");
		$$ = getNode("~");
		//3AC
        $$->place = "~";
	}
	| '!' {
		DBG("unary_operator -> '!'");
		$$ = getNode("!");
		//3AC
        $$->place = "!";
	}
	;

cast_expression
	: unary_expression {
		DBG("cast_expression -> unary_expression");
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		DBG("cast_expression -> '(' type_name ')' cast_expression");
		$$ = getNode("cast_expression", mergeAttrs($2, $4));
		
		// Semantic
		$$->type = $2->type;
		$$->isInit = $4->isInit;
		//3AC
		//TODO: Try to do CAST_typename
		DBG("DEBUG:type = " + $2->type);
		DBG("DEBUG:place = " + $4->type);
		if(checkType($2->type, $4->type) == ""){
			semantic_error(("Incompatible conversion of " + $4->type + " to type " + $2->type).c_str(), "type error");
		}
		std::string q = getTempVariable($2->type);
        $$->place = q;
		$4->nextlist.clear();
        emit("CAST_" +$2->type, $4->place, "", q, -1);
	}
	;

multiplicative_expression
	: cast_expression {
		DBG("multiplicative_expression -> cast_expression");
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
		DBG("multiplicative_expression -> multiplicative_expression '*' cast_expression");
		$$ = getNode("*", mergeAttrs($1, $3));
		
		// Semantic
		$$->intVal = $1->intVal * $3->intVal;
		
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = Exp($1->type, $3->type, "*");
		
		if(!temp.empty()){
			if(temp == "int"){
				$$->type = "int";
			}
			else if(isFloat(temp)){
				$$->type = "float";
			}

			//3AC
			if(isFloat(temp)){
				std::string q = getTempVariable($$->type);
				if(checkInt($1->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$1->place,"",q1,-1);					
					emit("(f)*", q1, $3->place, q, -1);
				}else if(checkInt($3->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$3->place,"",q1,-1);
					emit("(f)*", $1->place, q1, q, -1);
				}else{
					emit("(f)*", $1->place, $3->place, q, -1);
				}
				$$->place = q;
				$$->temp_name = q;
			}else{ 
				std::string q = getTempVariable($$->type); //TODO not always int
				$$->place = q;
				$$->temp_name = q;
				emit("*", $1->place, $3->place, q, -1);
			}
			$$->nextlist.clear();
		}
		else{
			semantic_error("Incompatible type for * operator", "type error");
		}
	}
	| multiplicative_expression '/' cast_expression {
		DBG("multiplicative_expression -> multiplicative_expression '/' cast_expression");
		$$ = getNode("/", mergeAttrs($1, $3));
		
		// Semantic
		if($3->intVal != 0) $$->intVal = $1->intVal / $3->intVal;
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = Exp($1->type, $3->type,"/");
		if(!temp.empty()){
			if(temp == "int"){
				$$->type = "int";
			}
			else if(isFloat(temp)){
				$$->type = "float";
			}
			//3AC
			if(isFloat(temp)){
				std::string q = getTempVariable($$->type);
				if(checkInt($1->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$1->place,"",q1,-1);					
					emit("(f)/", q1, $3->place, q, -1);
				}else if(checkInt($3->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$3->place,"",q1,-1);
					emit("(f)/", $1->place, q1, q, -1);
				}else{
					emit("(f)/", $1->place, $3->place, q, -1);
				}
				$$->place = q;
				$$->temp_name = q;
			}else{ 
				std::string q = getTempVariable($$->type); //TODO not always int
				$$->place = q;
				$$->temp_name = q;
				emit("/", $1->place, $3->place, q, -1);
			}
			$$->nextlist.clear();
		}
		else{
			semantic_error("Incompatible type for / operator", "type error");
		}
	}
	| multiplicative_expression '%' cast_expression {
		DBG("multiplicative_expression -> multiplicative_expression '%' cast_expression");
		$$ = getNode("%", mergeAttrs($1, $3));
		
		// Semantic
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		if($3->intVal != 0) $$->intVal = $1->intVal % $3->intVal;
		string temp = Exp($1->type, $3->type, "%");
		if(temp == "int"){
			$$->type = "int";
			//3AC
			if(isFloat(temp)){
				std::string q = getTempVariable($$->type);
				if(checkInt($1->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$1->place,"",q1,-1);					
					emit("(f)%", q1, $3->place, q, -1);
				}else if(checkInt($3->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$3->place,"",q1,-1);
					emit("(f)%", $1->place, q1, q, -1);
				}else{
					emit("(f)%", $1->place, $3->place, q, -1);
				}
				$$->place = q;
				$$->temp_name = q;
			}else{ 
				std::string q = getTempVariable($$->type); //TODO not always int
				$$->place = q;
				$$->temp_name = q;
				emit("%", $1->place, $3->place, q, -1);
			}
			$$->nextlist.clear();
		}
		else{
			semantic_error("Incompatible type for % operator", "type error");
		}
	}
	;

additive_expression
    : multiplicative_expression {
        DBG("additive_expression -> multiplicative_expression");
        $$ = $1;
    }
    | additive_expression '+' multiplicative_expression {
        DBG("additive_expression -> additive_expression '+' multiplicative_expression");
        $$ = getNode("+", mergeAttrs($1, $3));
        
        // Semantic
        if($1->isInit && $3->isInit) $$->isInit = 1;
        $$->intVal = $1->intVal + $3->intVal;
        string temp = Exp($1->type, $3->type, "+");
        if(!temp.empty()){
            if(temp == "int") $$->type = "int";
			else if(temp == "real") $$->type = "float";
			else $$->type = temp;
			//3AC
			std::string q = getTempVariable($$->type);//TODO not always int
			$$->place = q;
			$$->temp_name = q;
			if(($1->type).back() == '*' && (($3->type == "int") || ($3->type == "Integer Constant"))){  //int** + ...
				std::string q2 = getTempVariable($3->type);
				emit("*", $3->place, getSizeOfType($1->type.substr(0, $1->type.size()-1)), q2, -1);
				emit("ptr+", $1->place, q2, q, -1);
			}else{
				//TODO : Handle float pointer 
				if(isFloat(temp)){
					if(checkInt($1->type)){
						std::string q1 = getTempVariable($$->type);
						emit("intToFloat",$1->place,"",q1,-1);					
						emit("(f)+", q1, $3->place, q, -1);
					}else if(checkInt($3->type)){
						std::string q1 = getTempVariable($$->type);
						emit("intToFloat",$3->place,"",q1,-1);
						emit("(f)+", $1->place, q1, q, -1);
					}else{
						emit("(f)+", $1->place, $3->place, q, -1);
					}
				}else{ 
					emit("+", $1->place, $3->place, q, -1);
				}
			}
			$$->nextlist.clear();
        } else {
            semantic_error("Incompatible type for + operator", "type error");
        }
    }
    | additive_expression '-' multiplicative_expression {
        DBG("additive_expression -> additive_expression '-' multiplicative_expression");
        $$ = getNode("-", mergeAttrs($1, $3));
        
        // Semantic
        if($1->isInit && $3->isInit) $$->isInit = 1;
        $$->intVal = $1->intVal - $3->intVal;
        string temp = Exp($1->type, $3->type, "-");
        if(!temp.empty()){
            if(temp == "int") $$->type = "int";
			else if(temp == "real") $$->type = "float";
			else $$->type = temp;
			//3AC
			std::string q = getTempVariable($$->type);//TODO not always int
			$$->place = q;
			$$->temp_name = q;
			if(isFloat(temp)){
				if(checkInt($1->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$1->place,"",q1,-1);					
					emit("(f)-", q1, $3->place, q, -1);
				}else if(checkInt($3->type)){
					std::string q1 = getTempVariable($$->type);
					emit("intToFloat",$3->place,"",q1,-1);
					emit("(f)-", $1->place, q1, q, -1);
				}else{
					emit("(f)-", $1->place, $3->place, q, -1);
				}
			}else{ 
				emit("-", $1->place, $3->place, q, -1);
			}
			$$->nextlist.clear();
        } else {
            semantic_error("Incompatible type for - operator", "type error");
        }
    }
    ;

shift_expression
    : additive_expression {
        DBG("shift_expression -> additive_expression");
        $$ = $1;
    }
    | shift_expression LEFT_OP additive_expression {
        DBG("shift_expression -> shift_expression LEFT_OP additive_expression");
        $$ = getNode($2, mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = shiftExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = $1->type;
			//3AC
			std::string q = getTempVariable($$->type);//TODO not always int
			$$->place = q;
			$$->nextlist.clear();
			emit("<<", $1->place, $3->place, q, -1);
        } else {
            semantic_error("Invalid operands to binary <<", "type error");
        }
    }
    | shift_expression RIGHT_OP additive_expression {
        DBG("shift_expression -> shift_expression RIGHT_OP additive_expression");
        $$ = getNode($2, mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = shiftExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = $1->type;
			//3AC
			std::string q = getTempVariable($$->type);//TODO not always int
			$$->place = q;
			$$->nextlist.clear();
			emit(">>", $1->place, $3->place, q, -1);
        } else {
            semantic_error("Invalid operands to binary >>", "type error");
        }
    }
    ;

relational_expression
    : inclusive_or_expression {
        DBG("relational_expression -> inclusive_or_expression");
        $$ = $1;
    }
    | relational_expression '<' inclusive_or_expression {
        DBG("relational_expression -> relational_expression '<' inclusive_or_expression");
        $$ = getNode("<", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "warning") warning("Comparison between pointer and integer");
			// 3AC 
			std::string q = getTempVariable($$->type);
			emit("<", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        } else {
            semantic_error("Invalid operands to binary <", "type error");
        }
    }
    | relational_expression '>' inclusive_or_expression {
        DBG("relational_expression -> relational_expression '>' inclusive_or_expression");
        $$ = getNode(">", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "warning") warning("Comparison between pointer and integer");
			// 3AC
			std::string q = getTempVariable($$->type);
			emit(">", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        } else {
            semantic_error("Invalid operands to binary >", "type error");
        }
    }
    | relational_expression LE_OP inclusive_or_expression {
        DBG("relational_expression -> relational_expression LE_OP inclusive_or_expression");
        $$ = getNode("<=", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "warning") warning("Comparison between pointer and integer");
			//3AC
			std::string q = getTempVariable($$->type);
			emit("<=", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        } else {
            semantic_error("Invalid operands to binary <=", "type error");
        }
    }
    | relational_expression GE_OP inclusive_or_expression {
        DBG("relational_expression -> relational_expression GE_OP inclusive_or_expression");
        $$ = getNode(">=", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "warning") warning("Comparison between pointer and integer");
			// 3AC
			std::string q = getTempVariable($$->type);
			emit(">=", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        } else {
            semantic_error("Invalid operands to binary >=", "type error");
        }
    }
    ;

equality_expression
    : relational_expression { 
        DBG("equality_expression -> relational_expression");
        $$ = $1;
    }
    | equality_expression EQ_OP relational_expression {
        DBG("equality_expression -> equality_expression EQ_OP relational_expression");
        $$ = getNode($2, mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = eqExp($1->type, $3->type);
        if(!temp.empty()){
            if(temp == "warning"){
                semantic_error("Comparison between pointer and integer", "type error");
            }
            $$->type = "bool";
			//3AC
			std::string q = getTempVariable($$->type);
			emit("==", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        }
        else{
            semantic_error("Invalid operands to binary ==", "type error");
        }
    }
    | equality_expression NE_OP relational_expression {
        DBG("equality_expression -> equality_expression NE_OP relational_expression");
        $$ = getNode($2, mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = eqExp($1->type, $3->type);
        if(!temp.empty()){
            if(temp == "warning"){
                semantic_error("Comparison between pointer and integer", "type error");
            }
            $$->type = "bool";
			//3AC
			std::string q = getTempVariable($$->type);
			emit("!=", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        }
        else{
            semantic_error("Invalid operands to binary !=", "type error");
        }
    }
    ;

and_expression
    : shift_expression {	
        DBG("and_expression -> shift_expression");
        $$ = $1;
    }
    | and_expression '&' shift_expression {
        DBG("and_expression -> and_expression '&' shift_expression");
        $$ = getNode("&", mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = bitExp($1->type, $3->type);
        if(!temp.empty()){
            if(temp == "ok"){
                $$->type = "bool";
            }
            else $$->type = "int";
			// 3AC
			std::string q = getTempVariable($$->type);
			emit("&", $1->place, $3->place, q, -1);
			$$->place = q;
        }
        else{
            semantic_error("Invalid operands to binary &", "type error");
        }
    }
    ;

exclusive_or_expression
    : and_expression {
        DBG("exclusive_or_expression -> and_expression");
        $$ = $1;
    }
    | exclusive_or_expression '^' and_expression {
        DBG("exclusive_or_expression -> exclusive_or_expression '^' and_expression");
        $$ = getNode("^", mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = bitExp($1->type, $3->type);
        if(!temp.empty()){
            if(temp == "ok"){
				$$->type = "bool";
			}
			else {
				$$->type = "int";
			}

			// -3AC ---
			std::string q = getTempVariable($$->type);  
			emit("^", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        } else {
            semantic_error("Invalid operands to binary ^", "type error");
        }
    }
    ;

inclusive_or_expression
    : exclusive_or_expression {
        DBG("inclusive_or_expression -> exclusive_or_expression");
        $$ = $1;
    }
    | inclusive_or_expression '|' exclusive_or_expression {
        DBG("inclusive_or_expression -> inclusive_or_expression '|' exclusive_or_expression");
        $$ = getNode("|", mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = bitExp($1->type, $3->type);
        if(!temp.empty()){
            $$->type = (temp == "ok") ? "bool" : "int";
			// -3AC ---
			std::string q = getTempVariable($$->type); 
			emit("|", $1->place, $3->place, q, -1);
			$$->place = q;
			$$->nextlist.clear();
        } else {
            semantic_error("Invalid operands to binary |", "type error");
        }
    }
    ;

logical_and_expression
    : equality_expression {
        DBG("logical_and_expression -> equality_expression");
        $$ = $1;
    }
    | logical_and_expression AND_OP equality_expression {
        DBG("logical_and_expression -> logical_and_expression AND_OP equality_expression");
        $$ = getNode("&&", mergeAttrs($1, $3));

        // Semantics
        $$->type = "bool";
        $$->isInit = ($1->isInit & $3->isInit);   
        $$->intVal = $1->intVal && $3->intVal;
		// 3AC
		std::string q = getTempVariable($$->type);
		emit("&&", $1->place, $3->place, q, -1);
		$$->place = q;
		$$->nextlist.clear();
    }
    ;

logical_or_expression
    : logical_and_expression {
        DBG("logical_or_expression -> logical_and_expression");
        $$ = $1;
    }
    | logical_or_expression OR_OP logical_and_expression {
        DBG("logical_or_expression -> logical_or_expression OR_OP logical_and_expression");
        $$ = getNode("||", mergeAttrs($1, $3));
        
        // Semantics
        $$->type = "bool";
        $$->isInit = $1->isInit & $3->isInit;
        $$->intVal = $1->intVal || $3->intVal;
		// 3AC
		std::string q = getTempVariable($$->type);
		emit("||", $1->place, $3->place, q, -1);
		$$->place = q;
		$$->nextlist.clear();
    }
    ;
NEXT_QUAD
	: %empty {
		$$ = getCurrentSize();
	}
	;

conditional_expression
    : logical_or_expression {
        DBG("conditional_expression -> logical_or_expression");
        $$ = $1;
    }
	| GOTO_COND NEXT_QUAD expression WRITE_GOTO ':' NEXT_QUAD conditional_expression {
        DBG("conditional_expression -> logical_or_expression '?' expression ':' conditional_expression");
        $$ = getNode("ternary operator", mergeAttrs($1, $3, $7));
        
        // Semantics
        string temp = condExp($3->type, $7->type);
        if (!temp.empty()) {
            $$->type = "int";
			// 3AC
			std::string q = getTempVariable($$->type);

			backpatch($1->truelist, $2);
			backpatch($1->falselist, $6);
			backpatch($3->nextlist, $4-1);
			backpatch($3->truelist, $4-1);
			backpatch($3->falselist, $4-1);

			setArg1($4-1,$3->place);
			setResult($4-1,q);

			backpatch($7->nextlist, getCurrentSize());
			backpatch($7->falselist, getCurrentSize());
			backpatch($7->truelist, getCurrentSize());

			emit("=", $7->place, "", q, -1);
			singlePatch($4, getCurrentSize());
			$$->place = q;
        } else {
            semantic_error("Type mismatch in Conditional Expression", "type error");
        }
        $$->isInit = $1->isInit & $3->isInit & $7->isInit;
    }
    ;

GOTO_COND
	: logical_or_expression '?' {
		previous_if_found = if_found;
		if_found = 0;
		$$ = $1;

		// 3AC
		if ($1->truelist.empty()) {
			backpatch($1->nextlist, getCurrentSize());
			int label = emit("GOTO", "IF", $1->place, "", 0);
			$1->truelist.push_back(label);
			label = emit("GOTO", "", "", "", 0);
			$1->falselist.push_back(label);
		}
	}
	;

WRITE_GOTO
	: %empty {
		// 3AC
		emit("=", "", "", "", -1);
		int label = emit("GOTO", "", "", "", 0);
		$$ = label;
	}
	;


assignment_expression
    : conditional_expression {
        DBG("assignment_expression -> conditional_expression");
        $$ = $1;
    }
    | unary_expression assignment_operator { if_found = 0; rValue = 1;} assignment_expression {
        DBG("assignment_expression -> unary_expression assignment_operator assignment_expression");
        $$ = getNode($2, mergeAttrs($1, $4));
        
        // Semantics
        string temp = assignExp($1->type, $4->type, string($2));
        if (!temp.empty()) {
            if (temp == "ok") {
                $$->type = $1->type;
            } else if (temp == "warning") {//doesn't happen yet
                $$->type = $1->type;
                warning("Assignment with incompatible pointer type");
            }
			// 3AC
			int num = assign_exp($2, $$->type, $1->type, $4->type, $1->place, $4->place);
			$$->place = $1->place;
			backpatch($4->nextlist, num);
			rValue = 0;
        } else {
            semantic_error("Incompatible types when assigning type", "type error");
        }
        if ($1->expType == 3 && $4->isInit) {
            updInit($1->temp_name);
        }
    }
    ;

assignment_operator
    : '=' {
        $$ = strdup("=");
    }
    | MUL_ASSIGN { $$ = $1; }
    | DIV_ASSIGN { $$ = $1; }
    | MOD_ASSIGN { $$ = $1; }
    | ADD_ASSIGN { $$ = $1; }
    | SUB_ASSIGN { $$ = $1; }
    | LEFT_ASSIGN { $$ = $1; }
    | RIGHT_ASSIGN { $$ = $1; }
    | AND_ASSIGN { $$ = $1; }
    | XOR_ASSIGN { $$ = $1; }
    | OR_ASSIGN { $$ = $1; }
    ;

expression
	: assignment_expression {
		DBG("expression -> assignment_expression");
		$$ = $1;
	}
	| expression ',' NEXT_QUAD assignment_expression {
		DBG("expression -> expression ',' assignment_expression");
		$$ = getNode("expression", mergeAttrs($1, $4));
		$$->type = "void"; // Semantic

		//3AC
		backpatch($1->nextlist, $3);
        backpatch($1->truelist, $3);
        backpatch($1->falselist, $3);
        $$->nextlist = $4->nextlist;
        $$->place = $4->place;
	}
	;

constant_expression
	: conditional_expression {
		DBG("constant_expression -> conditional_expression");
		$$ = $1;
	}
	;

declaration 
    : declaration_specifiers ';' { 
        DBG("declaration -> declaration_specifiers ';'");
          $$ = $1; 
      }
    | declaration_specifiers init_declarator_list ';' {
        DBG("declaration -> declaration_specifiers init_declarator_list ';'");
          $$ = getNode("declaration", mergeAttrs($1, $2));

			//changes made for classes
		  $$->temp_name = $2->temp_name;
        $$->type = $2->type;
        $$->size = $2->size;
        
        type = "";

        // 3AC
		$$->nextlist = $2->nextlist;
      }
    ;


declaration_specifiers
	: storage_class_specifier {
		DBG("declaration_specifiers -> storage_class_specifier");
		$$ = $1;
	}
	| storage_class_specifier declaration_specifiers {
		DBG("declaration_specifiers -> storage_class_specifier declaration_specifiers");
		$$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
	}
	| type_specifier {
		DBG("declaration_specifiers -> type_specifier");
		$$ = $1;
	}
	| type_specifier declaration_specifiers {
		DBG("declaration_specifiers -> type_specifier declaration_specifiers");
		$$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
	}
	| type_qualifier {
		DBG("declaration_specifiers -> type_qualifier");
		$$ = $1;
	}
	| type_qualifier declaration_specifiers {
		DBG("declaration_specifiers -> type_qualifier declaration_specifiers");
		$$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
	}
	;

init_declarator_list
	: init_declarator {
		DBG("init_declarator_list -> init_declarator");
		$$ = $1;
		array_decl = 0;
	}
	| init_declarator_list ',' NEXT_QUAD init_declarator {
		DBG("init_declarator_list -> init_declarator_list ',' init_declarator");
		$$ = getNode("init_declarator_list", mergeAttrs($1, $4));
		// 3AC
        backpatch($1->nextlist, $3);
        $$->nextlist = $4->nextlist;
	}
	;

init_declarator
	: declarator {
		DBG("init_declarator -> declarator");
		//$$=getNode($1);
		//
		$$ = $1;  // Just pass the node directly
		// Semantics
		if(currLookup($1->temp_name)){
			semantic_error(($1->temp_name + " is already declared").c_str(), "scope error");
		}
		else if($1->expType == 3){
			if(fn_decl){
				semantic_error("A parameter list with types is only allowed in a function definition", "semantic error");
				fn_decl = 0;
			}
			removeFuncProto();
		}
		else{
			// Insert into symbol table based on context:
            // 1. No class context - normal insertion
            // 2. In class but not in method body - class member (handled in insertClassAttr)
            // 3. In class and in method body - local variable
			if((className.empty() || inMethodBody) && !flag && !flag3){
				insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 0, NULL);
			}
		}
		if(flag3){
			typedefTable.push_back(make_pair(tdstring2,tdstring));
			flag3 = 0;
		}
		if(flag){
			flag = 0;
		}
		//3AC
		$$->place = $1->temp_name;
	}
	| declarator '=' {rValue = 1;} NEXT_QUAD initializer {
		DBG("init_declarator -> declarator '=' initializer");
		$$ = getNode("=", mergeAttrs($1, $5));
		
		// Semantics
		if(currLookup($1->temp_name)){
			semantic_error(($1->temp_name + " is already declared").c_str(), "scope error");
		}
		else{
			DBG("Inserting into symbol table: " + $1->temp_name);
			insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 1, NULL);
			std::string type = $1->type;
			DBG("Type of variable: " + $1->type);
			DBG("Type of initializer: " + $5->type);
			// Check if types are compatible for initialization
			string compatible = assignExp($1->type, $5->type, "=");
			if (compatible.empty()) {
				semantic_error(("Cannot initialize variable of type '" + $1->type + 
							"' with expression of type '" + $5->type + "'").c_str(), "type error");
			}
			//3AC
			if(array_decl){
				DBG("Array declaration  ");
				for(int i = 0; i<list_values.size();i++){
					emit("CopyToOffset", list_values[i], std::to_string(i*4), $1->temp_name, -1);
				}
				array_decl = 0;
			}else{
				assign_exp("=", $1->type,$1->type, $5->type, $1->place, $5->place);
			}

			$$->place = $1->temp_name;
			$$->nextlist = $5->nextlist;
			backpatch($1->nextlist, $4);

			rValue = 0;
			list_values.clear();
		}
	}
	;

storage_class_specifier
	: TYPEDEF {
		DBG("storage_class_specifier -> TYPEDEF");
		$$ = getNode($1);
		flag = 1;
		
	}
	| EXTERN {
		DBG("storage_class_specifier -> EXTERN");
		$$ = getNode($1);
		currentDataType = "extern ";
		flag2 = 1;
	}
	| STATIC {
		DBG("storage_class_specifier -> STATIC");
		$$ = getNode($1);
		currentDataType = "static ";
		flag2 = 1;
	}
	| AUTO {
		DBG("storage_class_specifier -> AUTO");
		$$ = getNode($1);
		currentDataType = "auto ";
		flag2 = 1;
	}
	| REGISTER {
		DBG("storage_class_specifier -> REGISTER");
		$$ = getNode($1);
		currentDataType = "register ";
		flag2 = 1;
	}
	;

type_specifier
		: VOID {
			DBG("type_specifier -> VOID");
			$$ = getNode($1);
			$$->type = "void";
			currentDataType = "void";
			if(type.empty()) type = "void";
			else type += " void";
		}	
		| CHAR {
			DBG("type_specifier -> CHAR");
			$$ = getNode($1);
			$$->type = "char";
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = flag2 ? currentDataType + " char" : "char";
			flag2 = 0;
			if(type.empty()) type = "char";
			else type += " char";
		}	
		| SHORT {
			DBG("type_specifier -> SHORT");
			$$ = getNode($1);
			$$->type = "short";
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = flag2 ? currentDataType + " short" : "short";
			flag2 = 0;
			if(type.empty()) type = "short";
			else type += " short";
		}	
		| INT {
			DBG("type_specifier -> INT");
			$$ = getNode($1);
			$$->type = "int";
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = flag2 ? currentDataType + " int" : "int";
			flag2 = 0;
			if(type.empty()) type = "int";
			else type += " int";
		}
		| LONG {
			DBG("type_specifier -> LONG");
			$$ = getNode($1);
			$$->type = "long";
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = flag2 ? currentDataType + " long" : "long";
			flag2 = 0;
			if(type.empty()) type = "long";
			else type += " long";
		}
		| FLOAT {
			DBG("type_specifier -> FLOAT");
			$$ = getNode($1);
			$$->type = "float";
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = flag2 ? currentDataType + " float" : "float";
			flag2 = 0;
			if(type.empty()) type = "float";
			else type += " float";
		}
		| DOUBLE {
			DBG("type_specifier -> DOUBLE");
			$$ = getNode($1);
			$$->type = "double";
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = flag2 ? currentDataType + " double" : "double";
			flag2 = 0;
			if(type.empty()) type = "double";
			else type += " double";
		}
		| SIGNED {
			DBG("type_specifier -> SIGNED");
			$$ = getNode($1);
			currentDataType = "signed";
			flag2 = 1;
			if(type.empty()) type = "signed";
			else type += " signed";
		}
		| UNSIGNED {
			DBG("type_specifier -> UNSIGNED");
			$$ = getNode($1);
			currentDataType = "unsigned";
			flag2 = 1;
			if(type.empty()) type = "unsigned";
			else type += " unsigned";
		}
		| FILE_MAN {
			DBG("type_specifier -> FILE_MAN");
			$$ = getNode($1);
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = "file";
			if(type.empty()) type = "file";
			else type += " file";
		}
		| VA_LIST {
			DBG("type_specifier -> VA_LIST");
			$$ = getNode($1);
			if(flag==1){ 
				flag++;
				tdstring = std::string($1);
			}
			currentDataType = "va_list";
			if(type.empty()) type = "va_list";
			else type += " va_list";
		}
		| struct_or_union_specifier {
			DBG("type_specifier -> struct_or_union_specifier");
			$$ = $1;
		}	
		| class_definition {
			DBG("type_specifier -> class_definition");
			$$ = $1;
		}
		| enum_specifier {
			DBG("type_specifier -> enum_specifier");
			$$ = $1;
		}
		| TYPE_NAME {
			DBG("type_specifier -> TYPE_NAME");
			$$ = getNode($1);
			type = getType($1);
		}	
		;

inheritance_specifier
	: IDENTIFIER {
		DBG("inheritance_specifier -> IDENTIFIER");
		std::vector<Data> attr;
		insertAttr(attr, getNode($1), "", 1);
		$$ = getNode("inheritance_specifier",&attr);
		$$->temp_name=string($1); //to propagate the class name of parent
	}
	;

inheritance_specifier_list
	: inheritance_specifier {
        DBG("inheritance_specifier_list -> inheritance_specifier");
        $$ = $1;
    }
	| inheritance_specifier_list ',' inheritance_specifier{
        DBG("inheritance_specifier_list -> inheritance_specifier_list ',' inheritance_specifier");
        $$ = getNode("inheritance_specifier_list", mergeAttrs($1, $3));
		yyerror("Multiple inheritance is not supported","syntax error");
    }
	;

access_specifier 
	: PRIVATE {
		DBG("access_specifier -> PRIVATE");
		$$ = getNode($1);
		currentAccess="private";
	}
	| PUBLIC {
		DBG("access_specifier -> PUBLIC");
		$$ = getNode($1);
		currentAccess="public";
	}
	| PROTECTED {
		DBG("access_specifier -> PROTECTED");
		$$ = getNode($1);
		currentAccess="protected";
	}
	;

class
	: CLASS {
		DBG("class -> CLASS");
		$$ = getNode($1);
	}
	;

class_definition_head 
	: class S_C INHERITANCE_OP inheritance_specifier_list {
        DBG("class_definition_head -> class INHERITANCE_OP inheritance_specifier_list");
		$$ = getNode("class_definition_head", mergeAttrs($1, $4));
         // Semantics: For inherited classes without an explicit name, mark as anonymous.
        $$->type = currentDataType;
        Anon_ClassCounter++; 
		$$->temp_name = to_string(Anon_ClassCounter);  
    }
	| class G_C S_C {
        DBG("class_definition_head -> class IDENTIFIER");
		$$ = getNode("class_definition_head", mergeAttrs($1, getNode($2)));
        currentDataType = "Class " + std::string($2);
		$$->temp_name = std::string($2); 
        // Semantics: Save the class name for later symbol table insertion.
    }
	| class G_C S_C INHERITANCE_OP inheritance_specifier_list {
        DBG("class_definition_head -> class IDENTIFIER INHERITANCE_OP inheritance_specifier_list");
		$$ = getNode("class_definition_head", mergeAttrs($1, getNode($2), $5));
        currentDataType = "Class " + std::string($2);
		$$->temp_name = std::string($2); 
        // Semantics: Save the class name for later symbol table insertion.
        // Process inheritance
		Node* inheritanceNode = $5;
        // Extract parent class name(s)
		if (inheritanceNode->node_name == "inheritance_specifier") {
            // Single parent
            string parentClassName = "CLASS_" + inheritanceNode->temp_name;
            if (!inheritFromClass("CLASS_" + std::string($2), parentClassName)) {
                semantic_error(("Cannot inherit from undefined class " + 
                        parentClassName.substr(6)).c_str(), "scope error");
            }
        }
    }
	;

class_definition 
	: class_definition_head '{' class_internal_definition_list '}' {
        DBG("class_definition -> class_definition_head '{' class_internal_definition_list '}'");
		$$ = getNode("class_definition", mergeAttrs($1, $3));
		$$->temp_name = $1->temp_name;
		// Semantics
		if(printClassTable("CLASS_" + $1->temp_name) == 1){
			if(type == "") type = "CLASS_" + $1->temp_name;
			else type += " CLASS_" + $1->temp_name; //won't occur but need to confirm
			//size not getting registered correctly
			DBG("Inserting into symbol table: " + $1->temp_name);
			insertSymbol(*curr_table, "CLASS_" + $1->temp_name, "class", getSize("CLASS_" + $1->temp_name), true, nullptr);
		}
		else{
			semantic_error(("Class " + $1->temp_name + " is already defined").c_str(), "scope error");
		}
		className = "";
		inClassContext = false;
		type = ""; //clearing after definition of class
    }
	| class IDENTIFIER {
        DBG("class_definition -> class IDENTIFIER");
		$$ = $1;

		// Semantics
		if(findClass("CLASS_" + string($2)) == 1){
			if(type == "") type = "CLASS_" + string($2);
			else type += " CLASS_" + string($2);
		}
		else if(className == string($2)){
			// We are inside a class
			type = "#INSIDE";
		}
		else{
			semantic_error(("Class " + string($2) + " is not defined").c_str(), "scope error");
		}
    }
	;

class_internal_definition_list
	: class_internal_definition {
		DBG("class_internal_definition_list -> class_internal_definition");
		$$ = $1;
	}
	| class_internal_definition_list class_internal_definition {
		DBG("class_internal_definition_list -> class_internal_definition_list class_internal_definition");
		$$ = getNode("class_internal_definition_list", mergeAttrs($1, $2));
	}
	; 

class_internal_definition	
	: access_specifier '{' class_member_list '}' ';' {
		DBG("class_internal_definition -> access_specifier '{' class_member_list '}' ';'");
		$$ = getNode("class_internal_definition", mergeAttrs($1, $3));
		currentAccess = $1->strVal;
		// Semantics: Propagate the access specifier from $1 to every member in $3.
	}
	;

class_member_list
    : class_member {
        DBG("class_member_list -> class_member");
        $$ = $1;
    }
    | class_member_list class_member {
        DBG("class_member_list -> class_member_list class_member");
        $$ = getNode("class_member_list", mergeAttrs($1, $2));
    }
    ;

class_member
	: function_definition { 
        DBG("class_member -> function_definition");
		 $1->strVal = currentAccess;
		 // Add function as a class member with proper access specifier

        std::string manglemethod;
		if (classMethodArgs.empty()) {
		    // Function with no arguments - create with empty parameter types
		    manglemethod = mangleFunctionName($1->temp_name, std::vector<string>());
		} else {
		    // Function with arguments - skip 'this' parameter if present
		    manglemethod = mangleFunctionName($1->temp_name, std::vector<string>(
		        classMethodArgs.size() > 0 ? classMethodArgs.begin() + 1 : classMethodArgs.begin(), 
		        classMethodArgs.end()
		    ));
		}
		manglemethod="FUNC_" + std::to_string(className.size()) + className + "_" + manglemethod.substr(5);
        insertClassAttr(manglemethod, "FUNC_"+$1->type, $1->size, 0,currentAccess);
		classMethodArgs.clear();
		$$ = $1; 
	}
	| declaration { 
        DBG("class_member -> declaration");
        $1->strVal = currentAccess;
		// Add declaration as a class member with proper access specifier
        insertClassAttr($1->temp_name, $1->type, $1->size, 0,currentAccess);
		$$ = $1; 
	}
	;

G_C 
    : IDENTIFIER {
        DBG("G_C -> IDENTIFIER");
        $$ = $1;
        className = $1;
		inClassContext = true;
    }
    ;

S_C 
    : %empty {
        DBG("S_C -> %empty");
        createClassTable();
    }
    ;

struct_or_union_specifier
	: struct_or_union G S '{' struct_declaration_list '}'	{
        DBG("struct_or_union_specifier -> struct_or_union G S '{' struct_declaration_list '}'");
        $$ = getNode($1, mergeAttrs(getNode($2), $5));
		std::string check=std::string($2);
		// Semantics
		std::cout<<"struct or union "<<$1<<std::endl;
		if(std::string($1)=="struct" && printStructTable("STRUCT_" + string($2)) == 1 ){
			if(findStruct("UNION_" + string($2)) == 1){
				semantic_error(("UNION " + string($2) + " is already defined").c_str(), "scope error");
			}
			else{
				if(type == "") type = "STRUCT_" + string($2);
				else type += " STRUCT_" + string($2);
				DBG("Inserting into symbol table: " + string($2));
				insertSymbol(*curr_table, "STRUCT_" + string($2), "struct", getSize("STRUCT_" + string($2)), true, nullptr);
			}
		}
		else if(std::string($1)=="union" && printStructTable("UNION_" + string($2)) == 1){
			if(findStruct("STRUCT_" + string($2)) == 1){
				semantic_error(("Struct " + string($2) + " is already defined").c_str(), "scope error");
			}
			else{
				if(type == "") type = "UNION_" + string($2);
				else type += " UNION_" + string($2);
				DBG("Inserting into symbol table: " + string($2));
				insertSymbol(*curr_table, "UNION_" + string($2), "union", getSize("UNION_" + string($2)), true, nullptr);
			}
		}
		else{
			if(std::string($1)=="struct")
				semantic_error(("Struct " + string($2) + " is already defined").c_str(), "scope error");
			else
				semantic_error(("Union " + string($2) + " is already defined").c_str(), "scope error");
		}
		type = ""; //clearing after definition of struct
		if (flag == 1) {
            // We're in a typedef, so register the struct type with tdstring
            tdstring=std::string($2);
            flag++;
        }	
		if(flag3){
			tdstring=std::string($2);
		}
	}
	| struct_or_union S '{' struct_declaration_list '}'		{
        DBG("struct_or_union_specifier -> struct_or_union S '{' struct_declaration_list '}'");
        $$ = getNode($1, mergeAttrs($4, nullptr));
		
		// Semantics
		Anon_StructCounter++;
		if(printStructTable("STRUCT_" + to_string(Anon_StructCounter)) == 1){
			if(type == "") type = "STRUCT_" + to_string(Anon_StructCounter);
			else type += " STRUCT_" + to_string(Anon_StructCounter);
			DBG("Inserting into symbol table: " + to_string(Anon_StructCounter));
			insertSymbol(*curr_table, "STRUCT_" + to_string(Anon_StructCounter), "struct", getSize("STRUCT_" + to_string(Anon_StructCounter)), true, nullptr);
		}
		else{
			semantic_error("Struct is already defined", "scope error");
		}
		type = ""; //clearing after definition of struct
	}
	| struct_or_union IDENTIFIER 	{
        DBG("struct_or_union_specifier -> struct_or_union IDENTIFIER");
        $$ = getNode($1, mergeAttrs(getNode($2), nullptr));
		currentDataType += " " + string($2);
		
		// Clear type before processing struct members
        type = "";
		// Semantics
		DBG("Struct or Union: " + std::string($1));
		DBG("Struct or Union name: " + std::string($2));
		DBG("find struct "+std::to_string(findStruct("STRUCT_" + string($2))));
		DBG("find union "+std::to_string(findStruct("UNION_" + string($2))));
		if(findStruct("STRUCT_" + string($2)) == 1 && findStruct("UNION_" + string($2)) == 1){
			semantic_error((string($2) + " defined as wrong kind of tag").c_str(), "scope error");
		}
		if(std::string($1)=="struct" && findStruct("STRUCT_" + string($2)) == 1){
			if(type == "") type = "STRUCT_" + string($2);
			else type += " STRUCT_" + string($2);
		}
		else if(std::string($1)=="union" && findStruct("UNION_" + string($2)) == 1){
			if(type == "") type = "UNION_" + string($2);
			else type += " UNION_" + string($2);
		}
		else if(structName == string($2)){
			// We are inside a struct or union ->but i don't think it matters which one :)
			// We are inside a struct to handle declaration of structs in structs
			type = "#INSIDE";
		}
		else if(std::string($1)=="struct"){
			semantic_error(("Struct " + string($2) + " is not defined").c_str(), "scope error");
		}
		else if(std::string($1)=="union"){
			semantic_error(("Union " + string($2) + " is not defined").c_str(), "scope error");
		}
		else{//will never reach
			semantic_error(("Struct " + string($2) + " is not defined").c_str(), "scope error");
		}
	}
	;

G 
	: IDENTIFIER {
        DBG("G -> IDENTIFIER");
		$$ = $1;
		structName = $1;
		if (flag==1){
			tdstring = std::string($1);
			//flag++;
		}
		else if (flag==2) {
			//typedefTable.push_back(make_pair(check, tdstring));
			flag = 0;
		}
	}
	;

S 
	: %empty {
        DBG("S -> %empty");
		createStructTable();
	}
	;

struct_or_union
	: STRUCT {
        DBG("struct_or_union -> STRUCT");
        $$ = $1; currentDataType="struct";
		if(flag==1){
			flag3 = 1;
			flag = 0;
		}
    }
	| UNION {
        DBG("struct_or_union -> UNION");
        $$ = $1; currentDataType="union";
    }
	;

struct_declaration_list
	: struct_declaration {
		DBG("struct_declaration_list -> struct_declaration");
		$$ = getNode("struct_declaration_list", mergeAttrs($1));
	}
	| struct_declaration_list struct_declaration {
		DBG("struct_declaration_list -> struct_declaration_list struct_declaration");
		$$ = getNode("struct_declaration_list", mergeAttrs($1, $2));
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' {
		DBG("struct_declaration -> specifier_qualifier_list struct_declarator_list ';'");
		$$ = getNode("struct_declaration", mergeAttrs($1, $2));
		
		// Semantics
		type = "";
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list {
		DBG("specifier_qualifier_list -> type_specifier specifier_qualifier_list");
		$$ = getNode("specifier_qualifier_list", mergeAttrs($1, $2));
	}
	| type_specifier {
		DBG("specifier_qualifier_list -> type_specifier");
		$$ = $1;
	}
	| type_qualifier specifier_qualifier_list {
		DBG("specifier_qualifier_list -> type_qualifier specifier_qualifier_list");
		$$ = getNode("specifier_qualifier_list", mergeAttrs($1, $2));
	}
	| type_qualifier {
		DBG("specifier_qualifier_list -> type_qualifier");
		$$ = $1;
	}
	;

struct_declarator_list
	: struct_declarator {
		DBG("struct_declarator_list -> struct_declarator");
		$$ = $1;
	}
	| struct_declarator_list ',' struct_declarator {
		DBG("struct_declarator_list -> struct_declarator_list ',' struct_declarator");
		$$ = getNode("struct_declarator_list", mergeAttrs($1, $3));
	}
	;

struct_declarator
	: declarator {
		DBG("struct_declarator -> declarator");
		$$ = $1;
		
		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $1->size, 0) != 1) {
			semantic_error(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str(), "scope error");
		}
	}
	| ':' constant_expression {
		DBG("struct_declarator -> ':' constant_expression");
		$$ = $2;
	}
	| declarator ':' constant_expression {
		DBG("struct_declarator -> declarator ':' constant_expression");
		$$ = getNode(":", mergeAttrs($1, $3));
		
		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $3->intVal, 0) != 1) {
			semantic_error(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str(), "scope error");
		}
	}
	;


enum_specifier
	: ENUM '{' enumerator_list '}' {
		DBG("enum_specifier -> ENUM '{' enumerator_list '}'");
		$$ = getNode($1, mergeAttrs($3));
		// TODO: Add enum semantics
	}
	| ENUM IDENTIFIER '{' enumerator_list '}' {
		DBG("enum_specifier -> ENUM IDENTIFIER '{' enumerator_list '}'");
		$$ = getNode($1, mergeAttrs(getNode($2), $4));
	}
	| ENUM IDENTIFIER {
		DBG("enum_specifier -> ENUM IDENTIFIER");
		$$ = getNode($1, mergeAttrs(getNode($2)));
		currentDataType = "Enum " + std::string($2);
	}
	;

enumerator_list
	: enumerator {
		DBG("enumerator_list -> enumerator");
		$$ = $1;
	}
	| enumerator_list ',' enumerator {
		DBG("enumerator_list -> enumerator_list ',' enumerator");
		$$ = getNode("enumerator_list", mergeAttrs($1, $3));
	}
	;

enumerator
	: IDENTIFIER {
		DBG("enumerator -> IDENTIFIER");
		$$ = getNode($1);
	}
	| IDENTIFIER '=' constant_expression {
		DBG("enumerator -> IDENTIFIER '=' constant_expression");
		$$ = getNode("=", mergeAttrs(getNode($1), $3));
	}
	;

type_qualifier
	: CONST {
		DBG("type_qualifier -> CONST");
		$$ = getNode($1);
		currentDataType = "const ";
		flag2 = 1;
	}
	| VOLATILE {
		DBG("type_qualifier -> VOLATILE");
		$$ = getNode($1);
		currentDataType = "volatile ";
		flag2 = 1;
	}
	;

declarator
	: pointer direct_declarator {
		DBG("declarator -> pointer direct_declarator");
		$$ = getNode("declarator", mergeAttrs($1, $2));
		// Semantics
		if (type == "#INSIDE") {
			$$->type = "STRUCT_" + structName + $1->type;
			$$->temp_name = $2->temp_name;
			$$->size = 4;
			$$->expType = 2;
		} else {
			$$->type = $2->type + $1->type;
			type=$$->type;
			$$->temp_name = $2->temp_name;
			$$->size = 4;
			$$->expType = 2;
		}
		//3AC
		$$->place = $$->temp_name;
	}
	| direct_declarator {
		DBG("declarator -> direct_declarator");
		$$ = $1;
		//3AC
		$$->place = $$->temp_name;
	}
	;

direct_declarator
	: IDENTIFIER {
		DBG("direct_declarator -> IDENTIFIER");
		$$ = getNode($1);
		std::string check = std::string($1);
		tdstring2=check;
		if (flag==1){
			tdstring = std::string($1);
		}
		else if (flag==2) {
			typedefTable.push_back(make_pair(check, tdstring));
			//flag = 0;
		}
		// Semantics
		$$->expType = 1; // Variable
		$$->type = type;
		$$->temp_name = string($1);
		$$->size = getSize(type);
		//3AC
		$$->place = $$->temp_name;
	}
	| CONSTANT IDENTIFIER {
		DBG("direct_declarator -> CONSTANT IDENTIFIER");
		yyerror("invalid identifier", "syntax error");
		$$ = getNode($2);
	}
	| '(' declarator ')' {
		DBG("direct_declarator -> '(' declarator ')'");
		$$ = $2;  // Just pass through the declarator node
		//3AC
		$$->place = $$->temp_name;
	}
	| direct_declarator '[' constant_expression ']' {
		DBG("direct_declarator -> direct_declarator '[' constant_expression ']'");
		Node* node = getNode("[ ]", mergeAttrs($3));
		$$ = getNode("direct_declarator[..]", mergeAttrs($1, node));

		// Semantics
		if ($1->expType == 1 || $1->expType == 2) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = $1->size * $3->intVal;
			array_decl = 1;
			//3AC
			$$->place = $$->temp_name;
		} else {
			semantic_error(("Function " + $1->temp_name + " cannot be used as an array").c_str(), "type error");
		}
	}
	| direct_declarator '[' ']' {
		DBG("direct_declarator -> direct_declarator '[' ']'");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, NULL, "[ ]", 0);
		$$ = getNode("direct_declarator[]", &attr);

		// Semantics
		if ($1->expType <= 2) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = 4;
			array_decl = 1;
			//3AC
			$$->place = $$->temp_name;
		}
		else{
			semantic_error(("Function " + $1->temp_name + " cannot be used as an array").c_str(), "type error");
		}
	}
	| direct_declarator '(' A parameter_type_list ')' NEXT_QUAD {
		DBG("direct_declarator -> direct_declarator '(' A parameter_type_list ')'");
		Node *node = getNode("( )", mergeAttrs($4));
		$$ = getNode("direct_declarator", mergeAttrs($1, node));

		noArgs = 0;
		// Semantics
		string baseName = $1->temp_name;//just function name -> don't change to funcName will cause issues as it is global
        	// If inside a class definition, use qualified name
		std::string mangledName;
        if (!className.empty()) {
            	mangledName=mangleFunctionName(baseName, funcArgs);
				mangledName="FUNC_" + std::to_string(className.size()) + className + "_" + mangledName.substr(5);
        		// Add implicit 'this' pointer as first parameter
        		string thisType = "CLASS_" + className + "*";
        		vector<string> newFuncArgs;

        		// Add 'this' as first parameter
        		newFuncArgs.push_back(thisType);

        		// Add the rest of the parameters
        		for (const auto& arg : funcArgs) {
        		    newFuncArgs.push_back(arg);
        		}

        		// Replace funcArgs with the new list including 'this'
        		funcArgs = newFuncArgs;

        		// Insert 'this' parameter into symbol table
        		insertSymbol(*curr_table, "this", thisType, 4, true, NULL);
        }
		else mangledName = mangleFunctionName(baseName, funcArgs);
		if($1->expType == 1) {
			$$->temp_name = $1->temp_name;
			$$->expType = 3;
			$$->type = $1->type;
			$$->size = getSize($$->type);
			vector<string> temp = getFuncArgs(mangledName);
			
			if(temp.size() == 1 && temp[0] == "#NO_FUNC"){
				insertFuncArg(mangledName, funcArgs);
				funcName = string(baseName);
				funcType = $1->type;
			}
			else{
				// Check if temp is correct
				if(temp == funcArgs){
					funcName = string(baseName);
					funcType = $1->type;
				}
				else{
					// This isn't an error anymore - it's overloading!
                	// Just create a new entry with different parameter types
                	insertFuncArg(mangledName, funcArgs);
                	funcName = string(baseName);
                	funcType = $1->type;
				}
			}
			//3 AC
			$$->place =$$->temp_name;
			backpatch($4->nextlist,$6);
			emit(mangledName + " start :", "", "", "", -2);
		}
		else{
			if($1->expType == 2){
				semantic_error( (baseName + "declared as array of function").c_str(), "type error");
			}
			else{
				semantic_error( (baseName + "declared as function of function").c_str(), "type error");
			}
		}
	}
	| direct_declarator '(' A identifier_list ')'{ //useless production
        DBG("direct_declarator -> direct_declarator '(' A identifier_list ')'");
		Node *node = getNode("( )", mergeAttrs($4));
		$$ = getNode("direct_declarator", mergeAttrs($1, node));

		// Semantics
		// ToDo : check if A is needed
		// ToDo : Check if func declaration exists and args match
		fn_decl = 1;
		$$->temp_name = $1->temp_name;
		$$->expType = 3;
		$$->type = $1->type;
		$$->size = getSize($$->type);
		funcType = $1->type;
		funcName = string($1->temp_name);
		vector<string> args = getFuncArgs($$->temp_name);
		if (args.size() == 1 && args[0] == "#NO_FUNC") {
			args.clear();
			for (int i = 0; i < idList.size(); i++) {
				DBG("Adding argument " + idList[i]);
				insertSymbol(*curr_table, idList[i], "int", 4, 1, NULL);
				args.push_back("int");
			}
			insertFuncArg($1->temp_name, args);
		}
		if (args.size() == idList.size()) {
			for (int i = 0; i < args.size(); i++) {
				if (args[i] == "...") {
					semantic_error(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
					break;
				}
				DBG("Adding argument " + idList[i]);
				insertSymbol(*curr_table, idList[i], args[i], getSize(args[i]), 1, NULL);
			}
			idList.clear();
			//3 AC
			$$->place =$$->temp_name;
			emit("FUNC_" + $$->temp_name + " start :", "", "", "", -2);
		} else {
			semantic_error(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
			idList.clear();
		}
	}
	| direct_declarator '(' A ')' {
		DBG("direct_declarator -> direct_declarator '(' A ')'");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, NULL, "( )", 0);
		$$ = getNode("direct_declarator", &attr);

		string baseName = $1->temp_name;
		std::string mangledName;

		if (!className.empty()) {
				mangledName=mangleFunctionName(baseName, funcArgs);
				mangledName="FUNC_" + std::to_string(className.size()) + className + "_" + mangledName.substr(5);
        // Add implicit 'this' pointer for class methods
        string thisType = "CLASS_" + className + "*";
        funcArgs.push_back(thisType);
        
        // Insert 'this' parameter into symbol table
        insertSymbol(*curr_table, "this", thisType, 8, true, NULL);
    	}
		else mangledName = mangleFunctionName(baseName, funcArgs);
		// Semantics
		if ($1->expType == 1) {
			$$->temp_name = $1->temp_name;
			$$->expType = 3;
			$$->type = $1->type;
			$$->size = getSize($$->type);

			vector<string> temp = getFuncArgs(mangledName);
			if(temp.size() == 1 && temp[0] == "#NO_FUNC"){
				insertFuncArg(mangledName, funcArgs);
				funcName = string($1->temp_name);
				funcType = $1->type;
			} else {
				semantic_error(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
			}
			//3 AC
			$$->place =$$->temp_name;
			emit(mangledName + " start :", "", "", "", -2);
		} else {
			if ($1->expType == 2) {
				semantic_error(($1->temp_name + " declared as array of function").c_str(), "type error");
			} else {
				semantic_error(($1->temp_name + " declared as function of function").c_str(), "type error");
			}
		}
	}
	;

A
	: %empty {
		DBG("A -> %empty");
		type = "";
		func_flag = 0;
		funcArgs.clear();
		createParamList();
		//3AC
		gotolablelist.clear();
		gotolabel.clear();
	}
	;

pointer
	: '*' {
		DBG("pointer -> '*'");
		$$ = getNode("*(Pointer)");
		$$->type = "*";
		currentDataType += "*";
	}
	| '*' type_qualifier_list {
		DBG("pointer -> '*' type_qualifier_list");
		$$ = getNode("*(Pointer)", mergeAttrs($2));
		$$->type = "*";
		currentDataType += "*";
	}
	| '*' pointer {
		DBG("pointer -> '*' pointer");
		$$ = getNode("*(Pointer)", mergeAttrs($2));
		$$->type = "*" + $2->type;
		currentDataType += "*";
	}
	| '*' type_qualifier_list pointer {
		DBG("pointer -> '*' type_qualifier_list pointer");
		$$ = getNode("*(Pointer)", mergeAttrs($2, $3));
		$$->type = "*" + $3->type;
		currentDataType += "*";
	}
	;

type_qualifier_list
	: type_qualifier {
		DBG("type_qualifier_list -> type_qualifier");
		$$ = $1;
	}
	| type_qualifier_list type_qualifier {
		DBG("type_qualifier_list -> type_qualifier_list type_qualifier");
		$$ = getNode("type_qualifier_list", mergeAttrs($1, $2));
	}
	;

parameter_type_list
	: parameter_list {
		DBG("parameter_type_list -> parameter_list");
		$$ = $1;
	}
	| parameter_list ',' ELLIPSIS {
		DBG("parameter_type_list -> parameter_list ',' ELLIPSIS");
		$$ = getNode("parameter_type_list", mergeAttrs($1, getNode($3)));
		// Semantics - add ellipsis to function argument list
		funcArgs.push_back("...");
		//3AC
		$$->nextlist = $1->nextlist;
	}
	;

parameter_list
	: parameter_declaration {
		DBG("parameter_list -> parameter_declaration");
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' NEXT_QUAD parameter_declaration {
		DBG("parameter_list -> parameter_list ',' parameter_declaration");
		$$ = getNode("parameter_list", mergeAttrs($1, $4));
		noArgs++;
		//3AC
		backpatch($1->nextlist, $3);
		$$->nextlist = $4->nextlist;
	}
	;

parameter_declaration
	: declaration_specifiers declarator {
		DBG("parameter_declaration -> declaration_specifiers declarator");
		$$ = getNode("parameter_declaration", mergeAttrs($1, $2));
		// Semantics
		type = "";
		if ($2->expType == 1 || $2->expType == 2) {
			if (currLookup($2->temp_name)) {
				semantic_error(("Redeclaration of Parameter " + $2->temp_name).c_str(), "scope error");
			} else {
				DBG("Inserting into symbol table: " + $2->temp_name);
				insertSymbol(*curr_table, $2->temp_name, $2->type, $2->size, true, NULL);
			}
			funcArgs.push_back($2->type);
		}
	}
	| declaration_specifiers abstract_declarator {
		DBG("parameter_declaration -> declaration_specifiers abstract_declarator");
		$$ = getNode("parameter_declaration", mergeAttrs($1, $2));
		// Semantics
		type = "";
	}
	| declaration_specifiers {
		DBG("parameter_declaration -> declaration_specifiers");
		$$ = $1;
		// Semantics
		funcArgs.push_back(type);
		type = "";
	}
	;

identifier_list
	: IDENTIFIER {
		DBG("identifier_list -> IDENTIFIER");
		$$ = getNode($1);
		idList.push_back($1); // Semantics
	}
	| identifier_list ',' IDENTIFIER {
		DBG("identifier_list -> identifier_list ',' IDENTIFIER");
		$$ = getNode("identifier_list", mergeAttrs($1, getNode($3)));
		idList.push_back($3); // Semantics
	}
	;

type_name
	: specifier_qualifier_list {
		DBG("type_name -> specifier_qualifier_list");
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator {
		DBG("type_name -> specifier_qualifier_list abstract_declarator");
		$$ = getNode("type_name", mergeAttrs($1, $2));
		$$->type=$1->type+$2->type;
	}
	;

abstract_declarator
	: pointer {
		DBG("abstract_declarator -> pointer");
		$$ = $1;
	}
	| direct_abstract_declarator {
		DBG("abstract_declarator -> direct_abstract_declarator");
		$$ = $1;
	}
	| pointer direct_abstract_declarator {
		DBG("abstract_declarator -> pointer direct_abstract_declarator");
		$$ = getNode("abstract_declarator", mergeAttrs($1, $2));
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		DBG("direct_abstract_declarator -> '(' abstract_declarator ')'");
		$$ = $2;
	}
	| '[' ']' {
		DBG("direct_abstract_declarator -> '[' ']'");
		$$ = getNode("[ ]");
		$$->type = type + "*"; // Semantics
		$$->size = 4; //Default size for pointer
	}
	| '[' constant_expression ']' {
		DBG("direct_abstract_declarator -> '[' constant_expression ']'");
		$$ = getNode("[ ]", mergeAttrs($2));
		$$->type = type + "*"; // Semantics
		$$->size = getSize(type) * $2->intVal;
	}
	| direct_abstract_declarator '[' ']' {
		DBG("direct_abstract_declarator -> direct_abstract_declarator '[' ']'");
		std::vector<Data> attr;
		insertAttr(attr,NULL,"[ ]",0);
		insertAttr(attr,$1,"",1);
		$$ = getNode("direct_abstract_declarator",&attr);

		$$->type = $1->type + "*"; // Semantics
		$$->size = 4; //Default size for pointer
	}
	| direct_abstract_declarator '[' constant_expression ']' {
		DBG("direct_abstract_declarator -> direct_abstract_declarator '[' constant_expression ']'");
		$$ = getNode("direct_abstract_declarator", mergeAttrs($1, getNode("[ ]", mergeAttrs($3))));
		$$->type = $1->type + "*"; // Semantics
		$$->size = $1->size * $3->intVal;
	}
	| '(' ')' {
		DBG("direct_abstract_declarator -> '(' ')'");
		$$ = getNode("( )");
		$$->type = "FUNC_" + type; // Semantics
		$$->size = 0;
	}
	| '(' parameter_type_list ')' {
		DBG("direct_abstract_declarator -> '(' parameter_type_list ')'");
		$$ = $2;
		$$->type = "FUNC_" + type; // Semantics
		$$->size = 0;
	}
	| direct_abstract_declarator '(' ')' {
		DBG("direct_abstract_declarator -> direct_abstract_declarator '(' ')'");
		std::vector<Data> attr;
		insertAttr(attr, NULL, "( )", 0);
		insertAttr(attr, $1, "", 1);
		$$ = getNode("direct_abstract_declarator",&attr);		

		$$->type = "FUNC_" + $1->type; // Semantics
		$$->size = 0;
	}
	| direct_abstract_declarator '(' parameter_type_list ')' {
		DBG("direct_abstract_declarator -> direct_abstract_declarator '(' parameter_type_list ')'");
		$$ = getNode("direct_abstract_declarator", mergeAttrs($1, getNode("( )", mergeAttrs($3))));
		$$->type = "FUNC_" + $1->type; // Semantics
		$$->size = 0;
	}
	;

initializer
	: assignment_expression {
		DBG("initializer -> assignment_expression");
		$$ = $1;
		//3AC
		list_values.push_back($1->place);
	}
	| '{' initializer_list '}' {
		DBG("initializer -> '{' initializer_list '}'");
		$$ = $2;
		 // Semantics
		$$->type = $2->type+"*"; // For checking array declaration -- {1,2,3} -> int*
		$$->isInit = 1;
	}
	| '{' initializer_list ',' '}' {
		DBG("initializer -> '{' initializer_list ',' '}'");
		$$ = $2;
		$$->isInit = 1;
		// Semantics
		$$->type = $2->type+"*"; // For checking array declaration  -- {1,2,3} -> int*
		//3AC
		$$->place = $2->place;
		$$->nextlist = $2->nextlist;
	}
	;

initializer_list
	: initializer {
		DBG("initializer_list -> initializer");
		$$ = $1;
	}
	| initializer_list ',' NEXT_QUAD initializer {
		DBG("initializer_list -> initializer_list ',' initializer");
		$$ = getNode("initializer_list", mergeAttrs($1, $4));
		$$->isInit = ($1->isInit && $4->isInit);
		// Semantics
		std::string temp = checkType($1->type, $4->type);
		if (temp.empty()) {
			semantic_error(("Incompatible types in initializer list: " + temp).c_str(), "type error");
		}
		$$->type = $1->type;
		//3AC
		backpatch($1->nextlist, $3);
		$$->nextlist = $4->nextlist;
	}
	;

statement
	: labeled_statement {
		DBG("statement -> labeled_statement");
		$$ = $1;
	}
	| compound_statement {
		DBG("statement -> compound_statement");
		$$ = $1;
	}
	| expression_statement {
		DBG("statement -> expression_statement");
		$$ = $1;
	}
	| selection_statement {
		DBG("statement -> selection_statement");
		$$ = $1;
	}
	| iteration_statement {
		DBG("statement -> iteration_statement");
		$$ = $1;
	}
	| jump_statement {
		DBG("statement -> jump_statement");
		$$ = $1;
	}
	;

labeled_statement
	: IDENTIFIER ':'  NEXT_QUAD statement {
		DBG("labeled_statement -> IDENTIFIER ':' statement");
		$$ = getNode("labeled_statement", mergeAttrs(getNode($1), $4));
		//3AC
		gotolabel[$1] = $3;

        $$->nextlist = $4->nextlist;
        $$->caselist = $4->caselist;
        $$->continuelist = $4->continuelist;
        $$->breaklist = $4->breaklist;
	}
	| CASE_CODE NEXT_QUAD statement {
		DBG("labeled_statement -> CASE constant_expression ':' statement");
		$$ = getNode("case", mergeAttrs($1, $3));

		//3AC
		backpatch($1->truelist, $2);
		extendList($3->nextlist, $1->falselist); //! ---
        $$->breaklist = $3->breaklist;
        $$->nextlist = $3->nextlist;
        $$->caselist = $1->caselist;
        $$->continuelist = $3->continuelist;
	}
	| DEFAULT_CODE ':' statement {
		DBG("labeled_statement -> DEFAULT ':' statement");
		std::vector<Data> attr;
        insertAttr(attr, NULL, "default", 0);
        insertAttr(attr, $3, "", 1);
        $$ = getNode("case", &attr);

		//3AC
		$$->breaklist = $3->breaklist;
        $$->nextlist = $3->nextlist;
        $$->continuelist = $3->continuelist;	
	}
	;

DEFAULT_CODE
	: DEFAULT {
		DBG("DEFAULT_CODE -> DEFAULT");
		$$ = getNode($1);

		int a = getCurrentSize();
		if(previousCaseList.size() > 0){
			int prevCaseLabel = previousCaseList.back();
			previousCaseList.pop_back();
			singlePatch(prevCaseLabel, a);
		}
	}
	;

CASE_CODE
	: CASE constant_expression ':' {
        $$ = $2;
		std::string t = getTempVariable($$->type);

		int a = getCurrentSize();
		if(previousCaseList.size() > 0){
			int prevCaseLabel = previousCaseList.back();
			previousCaseList.pop_back();
			emit("GOTO", "", "", "", a+4);
			a = getCurrentSize();
			singlePatch(prevCaseLabel, a);
		}

		emit("==", "", $2->place, t, -1); //for switch ('a'){} -> arg1 -> 'a' 
		int b = getCurrentSize();
		emit("GOTO", "IF", t, "", 0);
		int c = getCurrentSize();
		emit("GOTO", "", "", "", 0);
		$$->caselist.push_back(a);
		$$->truelist.push_back(b);
		previousCaseList.push_back(c);

		// $$->falselist.push_back(c);
	}
	;

compound_statement
	: '{' '}' {
		DBG("compound_statement -> '{' '}'");
		$$ = getNode("{ }");
	}
	| '{' CHANGE_TABLE statement_list '}' {
		DBG("compound_statement -> '{' CHANGE_TABLE statement_list '}'");
		$$ = $3;
		
		// Semantics - clean up block scope
		if(func_flag >= 2){ //>=2 essentially means we are in a block within a funcion
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName + str + ".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	| '{' CHANGE_TABLE declaration_list '}' {
		DBG("compound_statement -> '{' CHANGE_TABLE declaration_list '}'");
		$$ = $3;
		
		// Semantics - clean up block scope
		if(func_flag >= 2){//>=2 essentially means we are in a block within a funcion
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName + str + ".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	| '{' CHANGE_TABLE declaration_list NEXT_QUAD  statement_list '}'	{
        DBG("compound_statement -> '{' CHANGE_TABLE declaration_list statement_list '}'");
		$$ = getNode("compound_statement", mergeAttrs($3, $5));
		
		// Semantics - clean up block scope
		if(func_flag >= 2){//>=2 essentially means we are in a block within a funcion
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName + str + ".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}

		//3AC
		backpatch($3->nextlist, $4);
        $$->nextlist = $5->nextlist;
        $$->caselist = $5->caselist;
        $$->continuelist = $5->continuelist;
        $$->breaklist = $5->breaklist;
	}
	;

CHANGE_TABLE
	: %empty {
		DBG("CHANGE_TABLE -> %empty");
		if(func_flag){
			string str = "Block" + to_string(block_count);
			block_stack.push(block_count);
			block_count++;
			makeSymbolTable(str, "");
		}
		else {
			if (!className.empty() && funcName != "") {
                inMethodBody = true;  // We're entering a method body in a class
            }
		}
		func_flag++;
		$$ = strdup("");
	}
	;

declaration_list
	: declaration {
		DBG("declaration_list -> declaration");
		$$ = $1;
	}
	| declaration_list NEXT_QUAD declaration {
		DBG("declaration_list -> declaration_list declaration");
		$$ = getNode("declaration_list", mergeAttrs($1, $3));
		//3AC
		backpatch($1->nextlist, $2);
        $$->nextlist = $3->nextlist;
	}
	;

statement_list
	: statement {
		DBG("statement_list -> statement");
		$$ = $1;
	}
	| statement_list NEXT_QUAD statement {
		DBG("statement_list -> statement_list statement");
		$$ = getNode("statement_list", mergeAttrs($1, $3));

		//3AC
		backpatch($1->nextlist, $2);
		$$->nextlist = $3->nextlist;
		extendList($1->caselist, $3->caselist);
		$$->caselist = $1->caselist;
		extendList($1->continuelist, $3->continuelist);
		extendList($1->breaklist, $3->breaklist);
		$$->continuelist = $1->continuelist;
		$$->breaklist = $1->breaklist;
	}
	;

expression_statement
	: ';' {
		DBG("expression_statement -> ';'");
		$$ = getNode(";");
	}
	| expression ';' {
		DBG("expression_statement -> expression ';'");
		$$ = $1;
	}
	;

IF_CODE
    : IF {if_found = 1;} '(' expression ')' {
        if($4->truelist.empty() && $4->falselist.empty()) {
            int a = getCurrentSize();
			backpatch($4->nextlist, a);
            emit("GOTO","IF", $4->place, "",0);
            int b = getCurrentSize();
            emit("GOTO","", "", "",0);
            $4->truelist.push_back(a);
            $4->falselist.push_back(b);
        }
        $$ = $4;
		if_found = 0;
    }
	;

N
    : %empty {
        int a = getCurrentSize();
		$$ = new Node;
        emit("GOTO", "", "", "", 0);
        $$->nextlist.push_back(a);
    }
    ;

selection_statement
	: IF_CODE NEXT_QUAD statement {
		DBG("selection_statement -> IF '(' expression ')' statement");
		$$ = getNode("if", mergeAttrs($1, $3));
		//3AC
		backpatch($1->truelist, $2);
		extendList($3->nextlist, $1->falselist);
		$$->nextlist = $3->nextlist;
		$$->continuelist = $3->continuelist;
		$$->breaklist = $3->breaklist;
	}
	| IF_CODE NEXT_QUAD statement N ELSE NEXT_QUAD statement {
		DBG("selection_statement -> IF '(' expression ')' statement ELSE statement");
		$$ = getNode("if-else", mergeAttrs($1, $3, $7));
		//3AC
		backpatch($1->truelist, $2);
		backpatch($1->falselist, $6);

		extendList($3->nextlist, $4->nextlist);
		extendList($3->nextlist, $7->nextlist);
		$$->nextlist = $3->nextlist;

		extendList($3->breaklist, $7->breaklist);
		$$->breaklist = $3->breaklist;
		extendList($3->continuelist, $7->continuelist);
		$$->continuelist = $3->continuelist;
	}
	| SWITCH '(' expression ')' statement {
		DBG("selection_statement -> SWITCH '(' expression ')' statement");
		$$ = getNode("switch", mergeAttrs($3, $5));
		//3AC
		casepatch($5->caselist, $3->place);

		extendList($5->nextlist, $5->breaklist);
		$$->nextlist = $5->nextlist;
		$$->continuelist = $5->continuelist;

		if(previousCaseList.size()>0){
			$$->nextlist.push_back(previousCaseList.back());
		}
		previousCaseList.clear();
	}
	;


EXPR_CODE
    : {if_found = 1;} expression {
        if($2->truelist.empty() && $2->falselist.empty()) {
            int a = getCurrentSize();
			backpatch($2->nextlist, a);
            emit("GOTO","IF", $2->place, "",0);
            int b = getCurrentSize();
            emit("GOTO","", "", "",0);
            $2->truelist.push_back(a);
            $2->falselist.push_back(b);
        }
        $$ = $2;
		if_found = 0;
    }
    ;

EXPR_STMT_CODE
    : {if_found = 1;} expression_statement { 
		if($2->truelist.empty() && $2->falselist.empty()) {
            int a = getCurrentSize();
			backpatch($2->nextlist, a);
            emit("GOTO","IF", $2->place, "",0);
            int b = getCurrentSize();
            emit("GOTO","", "", "",0);
            $2->truelist.push_back(a);
            $2->falselist.push_back(b);
        }
        $$ = $2;
		if_found = 0;
	}
    ;


iteration_statement
    : WHILE '(' NEXT_QUAD EXPR_CODE ')' NEXT_QUAD statement {
		DBG("iteration_statement -> WHILE '(' expression ')' statement");
		$$ = getNode("while-loop", mergeAttrs($4, $7));

        backpatch($4->truelist, $6);
		extendList($7->nextlist, $7->continuelist);
        backpatch($7->nextlist, $3);
        
        $$->nextlist = $4->falselist;
		extendList($$->nextlist, $7->breaklist);
        emit("GOTO", "", "", "", $3);
	}
	| UNTIL '(' NEXT_QUAD EXPR_CODE ')' NEXT_QUAD statement { /*** Added UNTIL grammar ***/
		DBG("iteration_statement -> UNTIL '(' expression ')' statement");
		$$ = getNode("until-loop", mergeAttrs($4, $7));

		backpatch($4->falselist, $6);
		extendList($7->nextlist, $7->continuelist);
		backpatch($7->nextlist, $3);
		
		$$->nextlist = $4->truelist;
		extendList($$->nextlist, $7->breaklist);
		emit("GOTO", "", "", "", $3);
	}
	| DO NEXT_QUAD statement WHILE '(' NEXT_QUAD EXPR_CODE ')' ';' {
		DBG("iteration_statement -> DO statement WHILE '(' expression ')' ';'");
		$$ = getNode("do-while-loop", mergeAttrs($3, $7));

		backpatch($7->truelist, $2);
		extendList($3->nextlist, $3->continuelist);
		backpatch($3->nextlist, $6);

		$$->nextlist = $7->falselist;
		extendList($$->nextlist, $3->breaklist);
	}
	| FOR '(' expression_statement NEXT_QUAD EXPR_STMT_CODE ')' NEXT_QUAD statement {
		DBG("iteration_statement -> FOR '(' expression_statement expression_statement ')' statement");
		$$ = getNode("for-loop(w/o update stmt)", mergeAttrs($3, $5, $8));

		backpatch($3->nextlist, $4);
		backpatch($5->truelist, $7);

		$$->nextlist = $5->falselist;
		extendList($$->nextlist, $8->breaklist);

		extendList($8->nextlist, $8->continuelist);
		backpatch($8->nextlist, $4);

		emit("GOTO", "", "", "", $4);
	}
	| FOR '(' expression_statement NEXT_QUAD EXPR_STMT_CODE NEXT_QUAD expression N ')' NEXT_QUAD statement {
		DBG("iteration_statement -> FOR '(' expression_statement expression_statement expression ')' statement");
		$$ = getNode("for-loop", mergeAttrs($3, $5, $7, $11));

		backpatch($3->nextlist, $4);
		backpatch($5->truelist, $10);

		$$->nextlist = $5->falselist;
		extendList($$->nextlist, $11->breaklist);

		extendList($11->nextlist, $11->continuelist);
		backpatch($11->nextlist, $6);

		extendList($7->nextlist, $8->nextlist);
		backpatch($7->nextlist, $4);

		emit("GOTO", "", "", "", $6);
	}
	| WHILE '[' expression ']' statement {
		DBG("iteration_statement -> WHILE '[' expression ']' statement");
		yyerror("incorrect parentheses in while-loop.", "syntax error");
		$$ = getNode("Invalid While-loop", nullptr);
	}
	| UNTIL '[' expression ']' statement {
		DBG("iteration_statement -> UNTIL '[' expression ']' statement");
		yyerror("incorrect parentheses in until-loop.", "syntax error");
		$$ = getNode("Invalid Until-loop", nullptr);
	}
	| FOR '(' expression ',' expression ',' expression ')' statement {
		DBG("iteration_statement -> FOR '(' expression ',' expression ',' expression ')' statement");
		yyerror("comma used instead of semicolons.", "syntax error");
		$$ = getNode("Invalid for-loop", nullptr);
	}
	;

jump_statement
	: GOTO IDENTIFIER ';' {
		DBG("jump_statement -> GOTO IDENTIFIER ';'");
		$$ = getNode(std::string($1) + " : " + std::string($2));
		//3AC
		int a = getCurrentSize();
        emit("GOTO", "", "", "", 0);
        gotolablelist[$2].push_back(a);
	}
	| CONTINUE ';' {
		DBG("jump_statement -> CONTINUE ';'");
		$$ = getNode($1);
		//3AC
		int a = getCurrentSize();
        emit("GOTO", "", "", "", 0);
        $$->continuelist.push_back(a);
	}
	| BREAK ';' {
		DBG("jump_statement -> BREAK ';'");
		$$ = getNode($1);
		//3AC
		int a = getCurrentSize();
        emit("GOTO", "", "", "", 0);
        $$->breaklist.push_back(a);
	}
	| RETURN ';' {
		DBG("jump_statement -> RETURN ';'");
		$$ = getNode($1);
		//3AC
		emit("RETURN", "", "", "", -1);
	}
	| RETURN expression ';' {
		DBG("jump_statement -> RETURN expression ';'");
		$$ = getNode("jump_stmt", mergeAttrs(getNode($1), $2));
		//3AC
		backpatch($2->nextlist,getCurrentSize());
        emit("RETURN", $2->place, "", "", -1);
	}
	;

translation_unit
	: external_declaration {
		DBG("translation_unit -> external_declaration");
		$$ = $1;
	}
	| translation_unit NEXT_QUAD external_declaration {
		DBG("translation_unit -> translation_unit external_declaration");
		$$ = getNode("program", mergeAttrs($1, $3));
		//3AC
		backpatch($1->nextlist, $2);
		$$->nextlist = $3->nextlist;
		extendList($1->caselist, $3->caselist);
		$$->caselist = $1->caselist;
		extendList($1->continuelist, $3->continuelist);
		extendList($1->breaklist, $3->breaklist);
		$$->continuelist = $1->continuelist;
		$$->breaklist = $1->breaklist;
	}
	| error ';' {
		DBG("translation_unit -> error ';'");
		$$ = getNode("error-node", nullptr); yyerrok;
	}
	| error ',' {
		DBG("translation_unit -> error ','");
		$$ = getNode("error-node", nullptr); yyerrok;
	}
	| error {
		DBG("translation_unit -> error");
		$$ = getNode("error-node", nullptr); yyerrok;
	}
	;

external_declaration
	: function_definition {
		DBG("external_declaration -> function_definition");
		$$ = $1;
	}
	| declaration {
		DBG("external_declaration -> declaration");
		$$ = $1;
	}
	;

function_definition
	: declaration_specifiers declarator F declaration_list compound_statement {
		DBG("function_definition -> declaration_specifiers declarator F declaration_list compound_statement");
		$$ = getNode("function", mergeAttrs($1, $2, $4, $5));
		// Semantics
		// Extract and propagate function name and return type for classes
        $$->temp_name = $2->temp_name;  // Function name from declarator
        if ($2->type.find("*") != string::npos) {
            // Use the complete type from declarator which includes pointer
            $$->type = $2->type;
            funcType = $2->type;
        } else {
            // Otherwise it's a non-pointer type
            $$->type = funcType;
        }
        // Return type from declaration_specifiers
        $$->size = $2->size;            // Size if applicable

		type = "";
		string fName = string($3);
		if(fName!="("){//in case of error it gives this as fName
			printSymbolTable(curr_table, fName + ".csv");
			updSymbolTable(fName);
			inMethodBody = false;
			//3AC
			for(auto i: gotolablelist){
				backpatch(i.second, gotolabel[i.first]);
			}
        	emit(fName + " end", "", "", "", -1);
        	remainingBackpatch();
		}
	}
	| declaration_specifiers declarator F compound_statement {
		DBG("function_definition -> declaration_specifiers declarator F compound_statement");
		$$ = getNode("function (w/o decl_list)", mergeAttrs($1, $2, $4));
		// Semantics 
		// Extract and propagate function name and return type for classes
        $$->temp_name = $2->temp_name;  // Function name from declarator
		if ($2->type.find("*") != string::npos) {
            // Use the complete type from declarator which includes pointer
            $$->type = $2->type;
            funcType = $2->type;
        } else {
            // Otherwise it's a non-pointer type
            $$->type = funcType;
        }
        // Return type from declaration_specifiers
        $$->size = $2->size;            // Size if applicable

		type = "";
		string fName = string($3);
		if(fName!="("){//in case of error it gives this as fName  ->this check works perfectly for this case need to review for others
			printSymbolTable(curr_table, fName + ".csv");
			updSymbolTable(fName);
			inMethodBody = false;
			//3AC
			for(auto i: gotolablelist){
				backpatch(i.second, gotolabel[i.first]);
			}
        	emit(fName + " end", "", "", "", -1);
        	remainingBackpatch();
		}
		
	}
	| declarator F declaration_list compound_statement {
		DBG("function_definition -> declarator F declaration_list compound_statement");
		$$ = getNode("function (w/o decl_specifiers)", mergeAttrs($1, $3, $4));
		// Semantics
		type = "";
		string fName = string($2);
		if(fName!="("){//in case of error it gives this as fName
			printSymbolTable(curr_table, fName + ".csv");
			updSymbolTable(fName);
			inMethodBody = false;
			//3AC
			for(auto i: gotolablelist){
				backpatch(i.second, gotolabel[i.first]);
			}
        	emit(fName + " end", "", "", "", -1);
        	remainingBackpatch();
		}
	}
	| declarator F compound_statement {
		DBG("function_definition -> declarator F compound_statement");
		$$ = getNode("function (w/o specifiers and decl_list)", mergeAttrs($1, $3));
		// Semantics
		type = "";
		string fName = string($2);
		if(fName!="("){//in case of error it gives this as fName
			printSymbolTable(curr_table, fName + ".csv");
			updSymbolTable(fName);
			inMethodBody = false;
			//3AC
			for (auto &i : gotolablelist) {
				backpatch(i.second, gotolabel[i.first]);
        	}
        	emit(fName + " end", "", "", "", -1);
        	remainingBackpatch();
		}
	}
	;

F 
	: %empty {
        DBG("F -> %empty");
		std::string qualifiedFuncName = funcName;
		if (!className.empty()) {
			classMethodArgs = funcArgs;  // Cache arguments for class methods
		    // Check if funcName already has className prefix to avoid duplication
		    if (funcName.find(className + "_") == 0) {
				funcName=funcName.substr(className.size()+1);
		    } 
		    qualifiedFuncName=mangleFunctionName(funcName, std::vector<string>(funcArgs.begin() + 1, funcArgs.end()));//need to skip 'this'
			qualifiedFuncName="FUNC_" + std::to_string(className.size()) + className + "_" + qualifiedFuncName.substr(5);
		    
		}
		else qualifiedFuncName = mangleFunctionName(funcName, funcArgs);
		funcArgs.clear();
		if (gst.find(qualifiedFuncName) != gst.end()){
			removeFuncProto();//added for handling func redifinition
			semantic_error(("Redefinition of function " + funcName).c_str(), "scope error");
		}
		else{
			if (type.find("*") != string::npos) {//pointers prolly can handle struct ,classes return type here too
            	funcType = type;
        	}
			makeSymbolTable(qualifiedFuncName, funcType);
			$$ = strdup(qualifiedFuncName.c_str());
			block_count = 1;
			type = "";
		}
	}
	;

%%

void performParsing(const std::string &inputFile)
{
	if(debug_enabled)
		out = std::ofstream(outputDir + inputFile + "_debug_file.txt");
    beginAST(inputFile);
    yyparse();
    endAST();
}
// Add this function near the yyerror function (before the end of the file)

void semantic_error(const char* s, const std::string &errorType) {
    // Mark that an error occurred but DON'T call yyclearin
    has_error = true;
    std::ifstream file(inputFilename);
    std::string curr_line;
    int count = 1;
    std::string error_line(s);
    
    // Read through the file to find the line where the error occurred.
    while (std::getline(file, curr_line)) {
        if (count == line) {
            // Print error in a C/C++ style error message format.
            std::cerr << "\033[1;31merror: \033[0m" 
                      << errorType << "::" 
                      << line << ":" << (column - yyleng) 
                      << ": " << error_line << "\n\n";
            std::cerr << line << " | " << curr_line << "\n";
            std::cerr << std::string(column - yyleng + 4, ' ') << "^\n";
            break;
        }
        count++;
    }
    file.close();
    return;
}
void yyerror(const char* s, const std::string &errorType) {
    yyclearin;  // clear the input token stream (if applicable)
    has_error = true;
    std::ifstream file(inputFilename);
    std::string curr_line;
    int count = 1;
    std::string error_line(s);
    // Read through the file to find the line where the error occurred.
    while (std::getline(file, curr_line)) {
        if (count == line) {
            // Print error in a C/C++ style error message format.
            std::cerr << "\033[1;31merror: \033[0m" 
                      << errorType << "::" 
                      << line << ":" << (column - yyleng) 
                      << ": " << error_line << "\n\n";
            std::cerr << line << " | " << curr_line << "\n";
            std::cerr << std::string(column - yyleng + 4, ' ') << "^\n";
            break;
        }
        count++;
    }
    file.close();
    return;
}

// Add the warning function for issueing warnings
int warning(const char* s) { 
	std::ifstream file(inputFilename);  
	std::string curr_line;
	int count = 1;
	std::string heading("warning");
	
	while (std::getline(file, curr_line)) {
		if (count == line) {
			std::cerr << "\033[1;33mwarning: \033[0m" << heading << "::" << line << ":" << column - yyleng << ": " << s << "\n\n";
			std::cerr << line << " | " << curr_line << "\n";
			std::cerr << std::string(column - yyleng + 4, ' ') << "^\n";
			break;
		}
		count++;
	}
	file.close();
	return 0;
}
