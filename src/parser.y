%{
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstring>
#include "AST.hpp"
#include "data_structures.hpp"
#include "typecheck.h"
#include "symbol_table.h"
extern int yyleng;
std::string currentDataType="";
std::string currentAccess = "";//for classes
int noArgs=0;
bool flag=0,flag2=0;

extern char* yytext;
extern int column;
extern int line;
extern std::string inputFilename;
extern bool has_error;

//Semantics
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
vector<string> idList;
vector<string> currArgs;

// Debug tracking
bool debug_enabled = 1; // Flag to enable or disable debugging
#define DEBUG_PARSER(rule) if (debug_enabled) printf("DEBUG: Processing rule '%s' at line %d\n", rule, line)

int yyerror(const char* s, const std::string &errorType = "syntax error");
int yylex();
int warning(const char*);
%}

%define parse.error detailed
%define parse.lac full

%union{
	char* str;
	Node* ptr;
	constants* num;
}

%token<str> IDENTIFIER STRING_LITERAL SIZEOF
%token<str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP INHERITANCE_OP
%token<str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token<str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token<str> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token<str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token<str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID FILE_MAN
%token<str> STRUCT UNION ENUM ELLIPSIS
%token<str> CLASS PUBLIC PRIVATE PROTECTED

%token<str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token<str> UNTIL /*** Added UNTIL token ***/
//Semantic
%token<num> CONSTANT
%type<str> F G G_C
%type<str> CHANGE_TABLE 

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
        DEBUG_PARSER("primary_expression -> IDENTIFIER");
    	$$ = createASTNode($1);
		
		// Semantics
		string temp = primaryExpression(string($1));
		if(temp == ""){
			yyerror(("Undeclared Identifier " + string($1)).c_str(), "scope error");
		}
		else{
			if(temp.substr(0, 5) == "FUNC_"){
				$$->expType = 3;
			}
			else if(temp.back() == '*'){
				$$->expType = 2; 
			}
			else $$->expType = 1;
			printf("DEBUG: Identifier '%s' type: '%s'\n", $1, temp.c_str());
			$$->type = temp;
			$$->isInit = lookup(string($1))->init;
			$$->size = getSize(temp);
			$$->temp_name = string($1); 
		}
    }
	| CONSTANT IDENTIFIER {
        DEBUG_PARSER("primary_expression -> CONSTANT IDENTIFIER");
		yyerror("invalid identifier", "syntax error");
		$$ = createASTNode($1->str);
	}
	| CONSTANT CONSTANT {
        DEBUG_PARSER("primary_expression -> CONSTANT CONSTANT");
		yyerror("invalid constant", "syntax error");
		$$ = createASTNode($1->str);
	}
	| CONSTANT {
		$$ = createASTNode($1->str);
		
		// Semantics for constants
		$$->type = $1->type;
		$$->intVal = $1->intVal;
		$$->realVal = $1->realVal;
		$$->expType = 4;
		$$->temp_name = $1->str;
	}
	| STRING_LITERAL {
        DEBUG_PARSER("primary_expression -> STRING_LITERAL");
		std::string check=std::string($1);
		addToConstantTable(check,"String Literal");
		$$ = createASTNode($1);
		
		// Semantics for string literals
		$$->type = string("char*");
		$$->temp_name = string($1);
		$$->strVal = string($1);
	}
	| '(' expression ')' {
        DEBUG_PARSER("primary_expression -> '(' expression ')'");
		$$ = $2;
	}
	;

postfix_expression
	: primary_expression {
        DEBUG_PARSER("postfix_expression -> primary_expression");
		$$ = $1;
	}
	| postfix_expression '[' expression ']' {
        DEBUG_PARSER("postfix_expression -> postfix_expression '[' expression ']'");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("postfix_expression", &attr);
		
		// Semantics
		if($1->isInit && $3->isInit){
			$$->isInit = 1;
		}
		string temp = postfixExpression($1->type, 1);
		if(!temp.empty()){	
			$$->type = temp;
		}
		else{
			yyerror(("Array " + $1->temp_name +  " Index out of bound").c_str(), "semantic error");
		}
	}
	| postfix_expression '(' ')' {
        DEBUG_PARSER("postfix_expression -> postfix_expression '(' ')'");
		$$ = $1;
		
		// Semantics
		$$->isInit = 1;
		string temp = postfixExpression($1->type, 2);
		if(!temp.empty()){	
			$$->type = temp;
			if($1->expType == 3){
				vector<string> funcArg = getFuncArgs($1->temp_name);
				if (!(funcArg.size()==1 && funcArg[0]=="#NO_FUNC")){
					yyerror(("Incorrect number of arguments to Function " + $1->temp_name).c_str(), "semantic error");
				}
			}
		}
		else{
			yyerror(("Function " + $1->temp_name + " not declared in this scope").c_str(), "scope error");
		}
		currArgs.clear(); 
	}
	| postfix_expression '(' argument_expression_list ')' {
        DEBUG_PARSER("postfix_expression -> postfix_expression '(' argument_expression_list ')'");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("postfix_expression", &attr);
		
		// Semantics
		$$->isInit = $3->isInit;
		string temp = postfixExpression($1->type, 3);
		
		if(!temp.empty()){	
			$$->type = temp;
			if($1->expType == 3){
				vector<string> funcArgs = getFuncArgs($1->temp_name);
				
				for(int i=0; i<funcArgs.size(); i++){
					if(funcArgs[i]=="...")break;
					if(currArgs.size()==i){
						yyerror(("Incorrect number of arguments to Function " + $1->temp_name).c_str(), "semantic error");
						break;
					}
					string msg = checkType(funcArgs[i],currArgs[i]);
					
					if(msg =="warning"){
						warning(("Incompatible conversion of " +  currArgs[i] + " to parameter of type " + funcArgs[i]).c_str());
					}
					else if(msg.empty()){
						yyerror(("Incompatible Argument to the function " + $1->temp_name).c_str(), "semantic error");
						break;
					}
					if(i==funcArgs.size()-1 && i<currArgs.size()-1){
						yyerror(("Too many Arguments to Function " + $1->temp_name).c_str(), "semantic error");
						break;
					}
				}
			}
		}
		else{
			yyerror("Invalid function call", "semantic error");
		}
		currArgs.clear();
	}
	| postfix_expression '.' IDENTIFIER {
    	DEBUG_PARSER("postfix_expression -> postfix_expression '.' IDENTIFIER");
    	std::vector<Data> attr;
    	insertAttr(attr, $1, "", 1);
    	insertAttr(attr, createASTNode($3), "", 1);
    	$$ = createASTNode("expression.id", &attr);
	
    	// Semantics - check if it's a class or struct
    	string temp = string($3);
    	string memberName = $1->temp_name;
    	string type = $1->type;
	
    	// First check if it's a class
    	if (type.substr(0, 6) == "CLASS_") {
    	    int ret = lookupClass(type, temp);
    	    if (ret == 1) {
    	        // Class member found
    	        $$->type = ClassAttrType(type, temp); 
    	        $$->temp_name = $1->temp_name + "." + temp;
    	    }
    	    else if (ret == 0) {
    	        yyerror(("Member '" + temp + "' not found in class").c_str(), "scope error");
    	    }
    	    else {
    	        yyerror(("Class '" + memberName + "' not defined").c_str(), "scope error");
    	    }
    	}
    	// If not a class, try struct
    	else {
    	    int ret = lookupStruct($1->type, temp);
    	    if (ret == -1) {
    	        yyerror(("Struct or class '" + memberName + "' not defined").c_str(), "scope error");
    	    }
    	    else if (ret == 0) {
    	        yyerror(("Member '" + temp + "' not found in struct").c_str(), "scope error");
    	    }
    	    else {
    	        $$->type = StructAttrType($1->type, temp);
    	        $$->temp_name = $1->temp_name + "." + temp;
    	    }
    	}
	}
	| postfix_expression '.' IDENTIFIER '(' ')'  {
	    DEBUG_PARSER("postfix_expression -> postfix_expression '.' IDENTIFIER '(' ')'");
	    std::vector<Data> attr;
	    insertAttr(attr, $1, "", 1);
	    insertAttr(attr, createASTNode($3), "", 1);
	    $$ = createASTNode("method_call", &attr);
	
	    // Semantics - check if it's a class method call
	    string classType = $1->type;
	    string methodName = string($3);
	
	    // First check if it's a class
	    if (classType.substr(0, 6) == "CLASS_") {
	        // Look up method in class
	        int ret = lookupClass(classType, methodName);
	        if (ret == 1) {
	            // Get method type (should be FUNC_returnType)
	            string methodType = ClassAttrType(classType, methodName);
	
	            // Check if it's a function
	            if (methodType.substr(0, 5) == "FUNC_") {
	                string returnType = methodType.substr(5); // Extract return type
	                $$->type = returnType;
	                $$->temp_name = $1->temp_name + "." + methodName;
	                $$->isInit = 1;
	
	                // Check arguments (none for this rule)
	                vector<string> methodArgs = getFuncArgs(classType.substr(6) + "_" + methodName);
	                if (methodArgs.size() > 0 && !(methodArgs.size()==1 && methodArgs[0]=="#NO_FUNC")) {
	                    yyerror(("Incorrect number of arguments to method " + methodName).c_str(), "semantic error");
	                }
	            } else {
	                yyerror(("Member '" + methodName + "' is not a method").c_str(), "semantic error");
	            }
	        } else if (ret == 0) {
	            yyerror(("Method '" + methodName + "' not found in class").c_str(), "scope error");
	        } else {
	            yyerror(("Class '" + $1->temp_name + "' not defined").c_str(), "scope error");
	        }
	    } else {
	        yyerror("Cannot call method on non-class type", "type error");
	    }
	    currArgs.clear();
	}
	| postfix_expression '.' IDENTIFIER '(' argument_expression_list ')' {
	    DEBUG_PARSER("postfix_expression -> postfix_expression '.' IDENTIFIER '(' argument_expression_list ')'");
	    std::vector<Data> attr;
	    insertAttr(attr, $1, "", 1);
	    insertAttr(attr, createASTNode($3), "", 1);
	    insertAttr(attr, $5, "", 1);
	    $$ = createASTNode("method_call_args", &attr);
	
	    // Semantics - check if it's a class method call with arguments
	    string classType = $1->type;
	    string methodName = string($3);
	
	    // First check if it's a class
	    if (classType.substr(0, 6) == "CLASS_") {
	        // Look up method in class
	        int ret = lookupClass(classType, methodName);
	        if (ret == 1) {
	            // Get method type (should be FUNC_returnType)
	            string methodType = ClassAttrType(classType, methodName);
	
	            // Check if it's a function
	            if (methodType.substr(0, 5) == "FUNC_") {
	                string returnType = methodType.substr(5); // Extract return type
	
	                // Check arguments against parameter types
	                vector<string> methodArgs = getFuncArgs(classType.substr(6) + "_" + methodName);//gives className_func ->className empty right now
	
	                for (int i = 0; i < methodArgs.size(); i++) {
	                    if (methodArgs[i] == "...") break;
	                    if (currArgs.size() == i) {
	                        yyerror(("Incorrect number of arguments to method " + methodName).c_str(), "semantic error");
	                        break;
	                    }
						if (i == methodArgs.size() - 1 && i < currArgs.size() - 1) {
	                        yyerror(("Incorrect number of arguments to method " + methodName).c_str(), "semantic error");
	                        break;
	                    }
	                    string msg = checkType(methodArgs[i], currArgs[i]);
	                    if (msg == "warning") {
	                        warning(("Incompatible conversion of " + currArgs[i] + 
	                                " to parameter of type " + methodArgs[i]).c_str());
	                    } else if (msg.empty()) {
	                        yyerror(("Incompatible argument to method " + methodName).c_str(), 
	                               "semantic error");
	                        break;
	                    }
	                }
	
	                $$->type = returnType;
	                $$->temp_name = $1->temp_name + "." + methodName;
	                $$->isInit = $5->isInit;
	            } else {
	                yyerror(("Member '" + methodName + "' is not a method").c_str(), "semantic error");
	            }
	        } else if (ret == 0) {
	            yyerror(("Method '" + methodName + "' not found in class").c_str(), "scope error");
	        } else {
	            yyerror(("Class '" + $1->temp_name + "' not defined").c_str(), "scope error");
	        }
	    } else {
	        yyerror("Cannot call method on non-class type", "type error");
	    }
	    currArgs.clear();
	}
	| postfix_expression PTR_OP IDENTIFIER {
        DEBUG_PARSER("postfix_expression -> postfix_expression PTR_OP IDENTIFIER");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		string temp = string($3);
		string temp1 = ($1->type);
		if(temp1.back() != '*'){
			yyerror(($1->node_name + " is not a pointer, did you mean to use '.' ").c_str(), "type error");
		}
		else temp1.pop_back();
		
		int ret = lookupStruct(temp1, temp);
		if(ret == -1){
			yyerror("Struct not defined", "scope error");
		}
		else if (ret == 0){
			yyerror("Attribute of Struct not defined", "scope error");
		}
		else{
			$$->type = StructAttrType(temp1, temp);
			$$->temp_name = $1->temp_name + "->" + temp;
		}
	}
	| postfix_expression INC_OP {
        DEBUG_PARSER("postfix_expression -> postfix_expression INC_OP");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		$$->isInit = $1->isInit;
		string temp = postfixExpression($1->type, 6);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $1->intVal + 1;
		}
		else{
			yyerror("Increment not defined for this type", "type error");
		}
	}
	| postfix_expression DEC_OP {
        DEBUG_PARSER("postfix_expression -> postfix_expression DEC_OP");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		$$->isInit = $1->isInit;
		string temp = postfixExpression($1->type, 7);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $1->intVal - 1;
		}
		else{
			yyerror("Decrement not defined for this type", "type error");
		}
	}
	;

argument_expression_list
	: assignment_expression {
        DEBUG_PARSER("argument_expression_list -> assignment_expression");
		$$ = $1;
		
		// Semantic
		$$->isInit = $1->isInit;
		currArgs.push_back($1->type);
		$$->type = "void";
	}
	| argument_expression_list ',' assignment_expression {
        DEBUG_PARSER("argument_expression_list -> argument_expression_list ',' assignment_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("argument_list", &attr);
		
		// Semantic
		string temp = argExp($1->type, $3->type, 2);
		
		if($1->isInit && $3->isInit) $$->isInit=1;
		currArgs.push_back($3->type);
		$$->type = "void";
	}
	;

unary_expression
	: postfix_expression {
        DEBUG_PARSER("unary_expression -> postfix_expression");
		$$ = $1;
	}
	| INC_OP unary_expression {
        DEBUG_PARSER("unary_expression -> INC_OP unary_expression");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
		
		// Semantic
		$$->isInit = $2->isInit;
		string temp = postfixExpression($2->type, 6);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal + 1;
		}
		else{
			yyerror("Increment not defined for this type", "type error");
		}
	}
	| DEC_OP unary_expression {
        DEBUG_PARSER("unary_expression -> DEC_OP unary_expression");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
		
		// Semantic
		$$->isInit = $2->isInit;
		string temp = postfixExpression($2->type, 7);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal - 1;
		}
		else{
			yyerror("Decrement not defined for this type", "type error");
		}
	}
	| unary_operator cast_expression {
        DEBUG_PARSER("unary_expression -> unary_operator cast_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("unary_exp", &attr);
		
		// Semantic
		$$->isInit = $2->isInit;
		string temp = unaryExp($1->node_name, $2->type);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal;
		}
		else{
			yyerror("Type inconsistent with operator", "type error");
		}
	}
	| SIZEOF unary_expression {
        DEBUG_PARSER("unary_expression -> SIZEOF unary_expression");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
		
		// Semantic
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $2->size;
	}
	| SIZEOF '(' type_name ')' {
        DEBUG_PARSER("unary_expression -> SIZEOF '(' type_name ')'");
		std::vector<Data> attr;
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($1, &attr);
		
		// Semantic
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $3->size;
	}
	;

unary_operator
	: '&' {
		DEBUG_PARSER("unary_operator -> '&'");
		$$ = createASTNode("&");
	}
	| '*' {
		DEBUG_PARSER("unary_operator -> '*'");
		$$ = createASTNode("*");
	}
	| '+' {
		DEBUG_PARSER("unary_operator -> '+'");
		$$ = createASTNode("+");
	}
	| '-' {
		DEBUG_PARSER("unary_operator -> '-'");
		$$ = createASTNode("-");
	}
	| '~' {
		DEBUG_PARSER("unary_operator -> '~'");
		$$ = createASTNode("~");
	}
	| '!' {
		DEBUG_PARSER("unary_operator -> '!'");
		$$ = createASTNode("!");
	}
	;

cast_expression
	: unary_expression {
		DEBUG_PARSER("cast_expression -> unary_expression");
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		DEBUG_PARSER("cast_expression -> '(' type_name ')' cast_expression");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		insertAttr(attr, $4, "", 1);
		$$ = createASTNode("cast_expression", &attr);
		
		// Semantic
		$$->type = $2->type;
		$$->isInit = $4->isInit;
	}
	;

multiplicative_expression
	: cast_expression {
		DEBUG_PARSER("multiplicative_expression -> cast_expression");
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
		DEBUG_PARSER("multiplicative_expression -> multiplicative_expression '*' cast_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("*", &attr);
		
		// Semantic
		$$->intVal = $1->intVal * $3->intVal;
		
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = mulExp($1->type, $3->type, '*');
		
		if(!temp.empty()){
			if(temp == "int"){
				$$->type = "long long";
			}
			else if(temp == "float"){
				$$->type = "long double";
			}
		}
		else{
			yyerror("Incompatible type for * operator", "type error");
		}
	}
	| multiplicative_expression '/' cast_expression {
		DEBUG_PARSER("multiplicative_expression -> multiplicative_expression '/' cast_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("/", &attr);
		
		// Semantic
		if($3->intVal != 0) $$->intVal = $1->intVal / $3->intVal;
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = mulExp($1->type, $3->type, '/');
		if(!temp.empty()){
			if(temp == "int"){
				$$->type = "long long";
			}
			else if(temp == "float"){
				$$->type = "long double";
			}
		}
		else{
			yyerror("Incompatible type for / operator", "type error");
		}
	}
	| multiplicative_expression '%' cast_expression {
		DEBUG_PARSER("multiplicative_expression -> multiplicative_expression '%' cast_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("%", &attr);
		
		// Semantic
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		if($3->intVal != 0) $$->intVal = $1->intVal % $3->intVal;
		string temp = mulExp($1->type, $3->type, '%');
		if(temp == "int"){
			$$->type = "long long";
		}
		else{
			yyerror("Incompatible type for % operator", "type error");
		}
	}
	;

additive_expression
	: multiplicative_expression {
		DEBUG_PARSER("additive_expression -> multiplicative_expression");
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression {
        DEBUG_PARSER("additive_expression -> additive_expression '+' multiplicative_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("+", &attr);
		
		// Semantic
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		$$->intVal = $1->intVal + $3->intVal;
		string temp = addExp($1->type, $3->type, '+');
		if(!temp.empty()){
			if(temp == "int") $$->type = "long long";
			else if(temp == "real") $$->type = "long double";
			else $$->type = temp;
		}
		else{
			yyerror("Incompatible type for + operator", "type error");
		}
	}
	| additive_expression '-' multiplicative_expression {
		DEBUG_PARSER("additive_expression -> additive_expression '-' multiplicative_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("-", &attr);
		
		// Semantic
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		$$->intVal = $1->intVal - $3->intVal;
		string temp = addExp($1->type, $3->type, '-');
		if(!temp.empty()){
			if(temp == "int") $$->type = "long long";
			else if(temp == "real") $$->type = "long double";
			else $$->type = temp;
		}
		else{
			yyerror("Incompatible type for - operator", "type error");
		}
	}
	;

shift_expression
	: additive_expression {
		DEBUG_PARSER("shift_expression -> additive_expression");
		$$ = $1;
	}
	| shift_expression LEFT_OP additive_expression {
		DEBUG_PARSER("shift_expression -> shift_expression LEFT_OP additive_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = shiftExp($1->type,$3->type);
		if(!temp.empty()){
			$$->type = $1->type;
		}
		else{
			yyerror("Invalid operands to binary <<", "type error");
		}
	}
	| shift_expression RIGHT_OP additive_expression {
		DEBUG_PARSER("shift_expression -> shift_expression RIGHT_OP additive_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = shiftExp($1->type,$3->type);
		if(!temp.empty()){
			$$->type = $1->type;
		}
		else{
			yyerror("Invalid operands to binary >>", "type error");
		}
	}
	; 

relational_expression   //POTENTIAL ISSUE
    : inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> inclusive_or_expression");
		$$ = $1;
	}
    | relational_expression '<' inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression '<' inclusive_or_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
          $$ = createASTNode("<", &attr);

          // Semantic actions:
          if($1->isInit == 1 && $3->isInit == 1)
              $$->isInit = 1;
          std::string temp = relExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
              } else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
          } else {
			yyerror("Invalid operands to binary <", "type error");
		}
	}
    | relational_expression '>' inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression '>' inclusive_or_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
          $$ = createASTNode(">", &attr);

          // Semantic actions:
          if($1->isInit == 1 && $3->isInit == 1)
              $$->isInit = 1;
          std::string temp = relExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
              } else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
          } else {
			yyerror("Invalid operands to binary >", "type error");
		}
	}
    | relational_expression LE_OP inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression LE_OP inclusive_or_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
          $$ = createASTNode($2, &attr);

          // Semantic actions:
          if($1->isInit == 1 && $3->isInit == 1)
              $$->isInit = 1;
          std::string temp = relExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
              } else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
          } else {
			yyerror("Invalid operands to binary <=", "type error");
		}
	}
    | relational_expression GE_OP inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression GE_OP inclusive_or_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
          $$ = createASTNode($2, &attr);

          // Semantic actions:
          if($1->isInit == 1 && $3->isInit == 1)
              $$->isInit = 1;
          std::string temp = relExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
              } else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
          } else {
			yyerror("Invalid operands to binary >=", "type error");
		}
	}
	;


equality_expression
	: relational_expression { 
        DEBUG_PARSER("equality_expression -> relational_expression");
		$$ = $1;
	}
	| equality_expression EQ_OP relational_expression {
        DEBUG_PARSER("equality_expression -> equality_expression EQ_OP relational_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = eqExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				yyerror("Comparison between pointer and integer", "type error");
			}
			$$->type = "bool";
		}
		else{
			yyerror("Invalid operands to binary ==", "type error");
		}
	}
	| equality_expression NE_OP relational_expression {
        DEBUG_PARSER("equality_expression -> equality_expression NE_OP relational_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = eqExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				yyerror("Comparison between pointer and integer", "type error");
			}
			$$->type = "bool";
		}
		else{
			yyerror("Invalid operands to binary !=", "type error");
		}
	}
	;

and_expression
	: shift_expression {	
        DEBUG_PARSER("and_expression -> shift_expression");
		$$ = $1;
	}
	| and_expression '&' shift_expression{
        DEBUG_PARSER("and_expression -> and_expression '&' shift_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("&", &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = bitExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				$$->type = "bool";
			}
			else $$->type = "long long";
		}
		else{
			yyerror("Invalid operands to binary &", "type error");
		}
	}
	;

exclusive_or_expression
	: and_expression {
        DEBUG_PARSER("exclusive_or_expression -> and_expression");
		$$ = $1;
	}
	| exclusive_or_expression '^' and_expression{
        DEBUG_PARSER("exclusive_or_expression -> exclusive_or_expression '^' and_expression");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("^", &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = bitExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				$$->type = "bool";
			}
			else $$->type = "long long";
		}
		else{
			yyerror("Invalid operands to binary ^", "type error");
		}
    }
	;

inclusive_or_expression
	: exclusive_or_expression {
        DEBUG_PARSER("inclusive_or_expression -> exclusive_or_expression");
        $$ = $1;
    }
	| inclusive_or_expression '|' exclusive_or_expression{
        DEBUG_PARSER("inclusive_or_expression -> inclusive_or_expression '|' exclusive_or_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("|", &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = bitExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				$$->type = "bool";
			}
			else $$->type = "long long";
		}
		else{
			yyerror("Invalid operands to binary |", "type error");
		}
	}
	;

logical_and_expression
	: equality_expression {
        DEBUG_PARSER("logical_and_expression -> equality_expression");
		$$ = $1;
	}
	| logical_and_expression AND_OP equality_expression{
        DEBUG_PARSER("logical_and_expression -> logical_and_expression AND_OP equality_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("&&", &attr);

		// Semantics
		$$->type = string("bool");
		$$->isInit = (($1->isInit) & ($3->isInit));   
		$$->intVal = $1->intVal && $3->intVal;
	}
	;

logical_or_expression
	: logical_and_expression {
        DEBUG_PARSER("logical_or_expression -> logical_and_expression");
        $$ = $1;
    }
	| logical_or_expression OR_OP logical_and_expression{
        DEBUG_PARSER("logical_or_expression -> logical_or_expression OR_OP logical_and_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("||", &attr);
		
		// Semantics
		$$->type = string("bool");
		$$->isInit = (($1->isInit) & ($3->isInit));
		$$->intVal = $1->intVal || $3->intVal;
	}
	;

conditional_expression
	: logical_or_expression {
        DEBUG_PARSER("conditional_expression -> logical_or_expression");
        $$ = $1;
    }
	| logical_or_expression '?' expression ':' conditional_expression{
        DEBUG_PARSER("conditional_expression -> logical_or_expression '?' expression ':' conditional_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		insertAttr(attr, $5, "", 1);
		$$ = createASTNode("ternary operator", &attr);
		
		// Semantics
		string temp = condExp($3->type, $5->type);
		if(!temp.empty()){
			$$->type = "int";
		}
		else {
			yyerror("Type mismatch in Conditional Expression", "type error");
		}
		if($1->isInit == 1 && $3->isInit == 1 && $5->isInit == 1) $$->isInit = 1;
	}
	;

assignment_expression
	: conditional_expression {
        DEBUG_PARSER("assignment_expression -> conditional_expression");
        $$ = $1;
    }
	| unary_expression assignment_operator assignment_expression{
        DEBUG_PARSER("assignment_expression -> unary_expression assignment_operator assignment_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		string temp = assignExp($1->type, $3->type, string($2));
		if(!temp.empty()){
			if(temp == "ok"){
				$$->type = $1->type;
			}
			else if(temp == "warning"){
				$$->type = $1->type;
				warning("Assignment with incompatible pointer type");
			}
		}
		else{
			yyerror("Incompatible types when assigning type", "type error");
		}
		if($1->expType == 3 && $3->isInit){
			updInit($1->temp_name);
		}
	}
	;

assignment_operator
	: '='	{
		$$ = strdup("=");
	}
	| MUL_ASSIGN		{$$ = $1;}
	| DIV_ASSIGN		{$$ = $1;}
	| MOD_ASSIGN		{$$ = $1;}
	| ADD_ASSIGN		{$$ = $1;}
	| SUB_ASSIGN		{$$ = $1;}
	| LEFT_ASSIGN		{$$ = $1;}
	| RIGHT_ASSIGN		{$$ = $1;}
	| AND_ASSIGN		{$$ = $1;}
	| XOR_ASSIGN		{$$ = $1;}
	| OR_ASSIGN			{$$ = $1;}
	;

expression
	: assignment_expression {
        DEBUG_PARSER("expression -> assignment_expression");
        $$ = $1;
    }
	| expression ',' assignment_expression{
        DEBUG_PARSER("expression -> expression ',' assignment_expression");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("expression", &attr);
		//Semantic
		$$->type = string("void");
	}
	;

constant_expression
	: conditional_expression {
        DEBUG_PARSER("constant_expression -> conditional_expression");
        $$ = $1;
    }
	;

declaration //POTENTIAL ISSUE
    : declaration_specifiers ';' { 
        DEBUG_PARSER("declaration -> declaration_specifiers ';'");
          $$ = $1; 
      }
    | declaration_specifiers init_declarator_list ';' {
        DEBUG_PARSER("declaration -> declaration_specifiers init_declarator_list ';'");
          std::vector<Data> attr;
          insertAttr(attr, $1, "", 1);
          insertAttr(attr, $2, "", 1);
          $$ = createASTNode("declaration", &attr);

			//changes made for classes
		  $$->temp_name = $2->temp_name;
        $$->type = $2->type;
        $$->size = $2->size;
        
        type = "";

          // Check if this is a function declaration
          if($2->expType == 3) {
          //    // Remove the temporary function prototype from the symbol table.
          //    removeFuncProto();  // Assumes a function exists to clear the dummy function entry.
          //    
          //    // Retrieve any previously stored argument list for this function name.
          //    std::vector<std::string> prevArgs = getFuncArgs($2->temp_name);
          //    
          //    // If the function already has an entry in the FuncArgs map, validate the parameter types.
          //    if(!prevArgs.empty() && prevArgs[0] != "#NO_FUNC") {
          //        if(prevArgs.size() != funcArgs.size()) {
          //            yyerror(("Function " + std::string($2->temp_name) + 
          //                     " declared with a different number of arguments").c_str(), "semantic error");
          //        } else {
          //            for (size_t i = 0; i < prevArgs.size(); ++i) {
          //                if(prevArgs[i] != funcArgs[i]) {
          //                    yyerror(("Argument type mismatch in function " + 
          //                             std::string($2->temp_name)).c_str(), "semantic error");
          //                    break;
          //                }
          //            }
          //        }
          //    } else {
          //        // This is the first declaration: insert the function's argument list.
          //        insertFuncArg($2->temp_name, funcArgs);
          //    }
          //    // Clear the global argument list for the next function declaration.
          //    funcArgs.clear();
          }
      }
    ;


declaration_specifiers
	: storage_class_specifier {
        DEBUG_PARSER("declaration_specifiers -> storage_class_specifier");
        $$ = $1;
    }
	| storage_class_specifier declaration_specifiers{
        DEBUG_PARSER("declaration_specifiers -> storage_class_specifier declaration_specifiers");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_specifier {
        DEBUG_PARSER("declaration_specifiers -> type_specifier");
        $$ = $1;
    }
	| type_specifier declaration_specifiers{
        DEBUG_PARSER("declaration_specifiers -> type_specifier declaration_specifiers");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_qualifier {
        DEBUG_PARSER("declaration_specifiers -> type_qualifier");
        $$ = $1;
    }
	| type_qualifier declaration_specifiers{
        DEBUG_PARSER("declaration_specifiers -> type_qualifier declaration_specifiers");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	;

init_declarator_list
	: init_declarator {
        DEBUG_PARSER("init_declarator_list -> init_declarator");
        $$ = $1;
    }
	| init_declarator_list ',' init_declarator{
        DEBUG_PARSER("init_declarator_list -> init_declarator_list ',' init_declarator");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("init_declarator_list", &attr);
	}
	;

init_declarator
	: declarator {
        DEBUG_PARSER("init_declarator -> declarator");
		$$ = $1;
		
		// Semantics
		if(currLookup($1->temp_name)){
			string errstr = $1->temp_name + " is already declared";
			yyerror(errstr.c_str(), "scope error");
		}
		else if($1->expType == 3){
			if(fn_decl){
				yyerror("A parameter list without types is only allowed in a function definition", "syntax error");
				fn_decl = 0;
			}
			removeFuncProto();
		}
		else{
			insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 0, NULL);
		}
	}
	| declarator '=' initializer{
        DEBUG_PARSER("init_declarator -> declarator '=' initializer");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
		
		// Semantics
		if(currLookup($1->temp_name)){
			string errstr = $1->temp_name + " is already declared";
			yyerror(errstr.c_str(), "scope error");
		}
		else{
			insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 1, NULL);
		}
	}
	;

storage_class_specifier
	: TYPEDEF	{ 
        DEBUG_PARSER("storage_class_specifier -> TYPEDEF");
        $$ = createASTNode($1); flag=1;
    }
	| EXTERN	{ 
        DEBUG_PARSER("storage_class_specifier -> EXTERN");
        $$ = createASTNode($1); currentDataType="extern "; flag2=1;
    }
	| STATIC	{ 
        DEBUG_PARSER("storage_class_specifier -> STATIC");
        $$ = createASTNode($1); currentDataType="static "; flag2=1;
    }
	| AUTO		{ 
        DEBUG_PARSER("storage_class_specifier -> AUTO");
        $$ = createASTNode($1); currentDataType="auto "; flag2=1;
    }
	| REGISTER	{ 
        DEBUG_PARSER("storage_class_specifier -> REGISTER");
        $$ = createASTNode($1); currentDataType="register "; flag2=1;
    }
	;

type_specifier
	: VOID			{
        DEBUG_PARSER("type_specifier -> VOID");
		$$ = createASTNode($1); 
		currentDataType="void";
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| CHAR			{
        DEBUG_PARSER("type_specifier -> CHAR");
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" char";flag2=0;}else{currentDataType="char";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| SHORT			{
        DEBUG_PARSER("type_specifier -> SHORT");
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" short";flag2=0;}else{currentDataType="short";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| INT			{
        DEBUG_PARSER("type_specifier -> INT");
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" int";flag2=0;}else{currentDataType="int";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| LONG			{
        DEBUG_PARSER("type_specifier -> LONG");
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" long";flag2=0;}else{currentDataType="long";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| FLOAT			{
        DEBUG_PARSER("type_specifier -> FLOAT");
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" float";flag2=0;}else{currentDataType="float";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| DOUBLE		{
        DEBUG_PARSER("type_specifier -> DOUBLE");
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" double";flag2=0;}else{currentDataType="double";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| SIGNED		{
        DEBUG_PARSER("type_specifier -> SIGNED");
		$$ = createASTNode($1); 
		currentDataType="signed"; 
		flag2=1;
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| UNSIGNED		{
        DEBUG_PARSER("type_specifier -> UNSIGNED");
		$$ = createASTNode($1); 
		currentDataType="unsigned"; 
		flag2=1;
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| FILE_MAN      { 
        DEBUG_PARSER("type_specifier -> FILE_MAN");
		$$ = createASTNode($1); 
		currentDataType="file";
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| struct_or_union_specifier {
        DEBUG_PARSER("type_specifier -> struct_or_union_specifier");
        $$ = $1;
    }	
	| class_definition {
        DEBUG_PARSER("type_specifier -> class_definition");
        $$ = $1;
    }
	| enum_specifier {
        DEBUG_PARSER("type_specifier -> enum_specifier");
        $$ = $1;
    }
	| TYPE_NAME		{
        DEBUG_PARSER("type_specifier -> TYPE_NAME");
		$$ = createASTNode($1);
		
		// Semantics
		string temp = getType($1);
		type = temp;
	}	
	;

inheritance_specifier
	: access_specifier IDENTIFIER {
        DEBUG_PARSER("inheritance_specifier -> access_specifier IDENTIFIER");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        /* Wrap IDENTIFIER into an AST node */
        insertAttr(attr, createASTNode($2), "", 1);
        $$ = createASTNode("inheritance_specifier", &attr);
    }
	;

inheritance_specifier_list
	: inheritance_specifier {
        DEBUG_PARSER("inheritance_specifier_list -> inheritance_specifier");
        $$ = $1;
    }
	| inheritance_specifier_list ',' inheritance_specifier{
        DEBUG_PARSER("inheritance_specifier_list -> inheritance_specifier_list ',' inheritance_specifier");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("inheritance_specifier_list", &attr);
    }
	;

access_specifier 
	: PRIVATE {
        DEBUG_PARSER("access_specifier -> PRIVATE");
        $$ = createASTNode($1);
		currentAccess="private";
    }
	| PUBLIC {
        DEBUG_PARSER("access_specifier -> PUBLIC");
        $$ = createASTNode($1);
		currentAccess="public";
    }
	| PROTECTED {
        DEBUG_PARSER("access_specifier -> PROTECTED");
        $$ = createASTNode($1);
		currentAccess="public";
    }
	;

class
	: CLASS {
        DEBUG_PARSER("class -> CLASS");
        $$ = createASTNode($1);
    }
	;

class_definition_head 
	: class S_C INHERITANCE_OP inheritance_specifier_list {
        DEBUG_PARSER("class_definition_head -> class INHERITANCE_OP inheritance_specifier_list");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $4, "", 1);
        $$ = createASTNode("class_definition_head", &attr);
         // Semantics: For inherited classes without an explicit name, mark as anonymous.
        $$->type = currentDataType;
        Anon_ClassCounter++; 
		$$->temp_name = to_string(Anon_ClassCounter);  
    }
	| class G_C S_C {
        DEBUG_PARSER("class_definition_head -> class IDENTIFIER");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($2), "", 1);
        currentDataType = "Class " + std::string($2);
        $$ = createASTNode("class_definition_head", &attr);
		$$->temp_name = std::string($2); 
        // Semantics: Save the class name for later symbol table insertion.
    }
	| class G_C S_C INHERITANCE_OP inheritance_specifier_list {
        DEBUG_PARSER("class_definition_head -> class IDENTIFIER INHERITANCE_OP inheritance_specifier_list");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($2), "", 1);
        insertAttr(attr, $5, "", 1);
        currentDataType = "Class " + std::string($2);
        $$ = createASTNode("class_definition_head", &attr);
		$$->temp_name = std::string($2); 
        // Semantics: Save the class name for later symbol table insertion.
        // Process inheritance
        // TODO: Extract parent classes from $4 and add inheritance relationships
    }
	;

class_definition 
	: class_definition_head '{' class_internal_definition_list '}' {
        DEBUG_PARSER("class_definition -> class_definition_head '{' class_internal_definition_list '}'");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_definition", &attr);
		$$->temp_name = $1->temp_name;
		// Semantics
		if(printClassTable("CLASS_" + $1->temp_name) == 1){
			if(type == "") type = "CLASS_" + $1->temp_name;
			else type += " CLASS_" + $1->temp_name;
		}
		else{
			yyerror(("Class " + $1->temp_name + " is already defined").c_str(), "scope error");
		}
		className = "";
    }
	| class IDENTIFIER {
        DEBUG_PARSER("class_definition -> class_definition_head");
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
			yyerror(("Class " + string($2) + " is not defined").c_str(), "scope error");
		}
    }
	;

class_internal_definition_list
	: class_internal_definition {
        DEBUG_PARSER("class_internal_definition_list -> class_internal_definition");
        $$ = $1;
    }
	| class_internal_definition_list class_internal_definition {
        DEBUG_PARSER("class_internal_definition_list -> class_internal_definition_list class_internal_definition");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("class_internal_definition_list", &attr);
    }
	; 

class_internal_definition	
	: access_specifier '{' class_member_list '}' ';' {
        DEBUG_PARSER("class_internal_definition -> access_specifier '{' class_member_list '}' ';'");
		currentAccess = $1->strVal;
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_internal_definition", &attr);
        // Semantics: Propagate the access specifier from $1 to every member in $3.
       
    }
	;

class_member_list
	: class_member {
        DEBUG_PARSER("class_member_list -> class_member");
        $$ = $1;
    }
	| class_member_list class_member {
        DEBUG_PARSER("class_member_list -> class_member_list class_member");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("class_member_list", &attr);
    }
	;

class_member
	: function_definition { 
        DEBUG_PARSER("class_member -> function_definition");
		 $1->strVal = currentAccess;
		 // Add function as a class member with proper access specifier
		 printf("DEBUG: Function member name=%s, type=%s\n", $1->temp_name.c_str(), $1->type.c_str());

        insertClassAttr($1->temp_name, "FUNC_"+$1->type, $1->size, 0,currentAccess);
		 $$ = $1; 
	}
	| declaration { 
        DEBUG_PARSER("class_member -> declaration");
		$1->strVal = currentAccess;
		// Add declaration as a class member with proper access specifier
		printf("DEBUG: Variable member name=%s, type=%s\n", $1->temp_name.c_str(), $1->type.c_str());
        insertClassAttr($1->temp_name, $1->type, $1->size, 0,currentAccess);
		$$ = $1; 
	}
	;

G_C 
	: IDENTIFIER {
        DEBUG_PARSER("G_C -> IDENTIFIER");
		$$ = $1;
		className = $1;
	}
	;

S_C 
	: %empty {
        DEBUG_PARSER("S_C -> %empty");
		createClassTable();
	}
	;
struct_or_union_specifier
	: struct_or_union G S '{' struct_declaration_list '}'	{
        DEBUG_PARSER("struct_or_union_specifier -> struct_or_union G S '{' struct_declaration_list '}'");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $5, "", 1);
		std::string check=std::string($2);
		$$ = createASTNode($1, &v);
		
		// Semantics
		if(printStructTable("STRUCT_" + string($2)) == 1){
			if(type == "") type = "STRUCT_" + string($2);
			else type += " STRUCT_" + string($2);
		}
		else{
			yyerror(("Struct " + string($2) + " is already defined").c_str(), "scope error");
		}
	}
	| struct_or_union S '{' struct_declaration_list '}'		{
        DEBUG_PARSER("struct_or_union_specifier -> struct_or_union S '{' struct_declaration_list '}'");
		std::vector<Data> v;
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
		
		// Semantics
		Anon_StructCounter++;
		if(printStructTable("STRUCT_" + to_string(Anon_StructCounter)) == 1){
			if(type == "") type = "STRUCT_" + to_string(Anon_StructCounter);
			else type += " STRUCT_" + to_string(Anon_StructCounter);
		}
		else{
			yyerror("Struct is already defined", "scope error");
		}
	}
	| struct_or_union IDENTIFIER 	{
        DEBUG_PARSER("struct_or_union_specifier -> struct_or_union IDENTIFIER");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		std::string check=std::string($2);
		currentDataType+=" ";
		currentDataType+=check;
		$$ = createASTNode($1, &v);
		
		// Semantics
		if(findStruct("STRUCT_" + string($2)) == 1){
			if(type == "") type = "STRUCT_" + string($2);
			else type += " STRUCT_" + string($2);
		}
		else if(structName == string($2)){
			// We are inside a struct
			type = "#INSIDE";
		}
		else{
			yyerror(("Struct " + string($2) + " is not defined").c_str(), "scope error");
		}
	}
	;

G 
	: IDENTIFIER {
        DEBUG_PARSER("G -> IDENTIFIER");
		$$ = $1;
		structName = $1;
	}
	;

S 
	: %empty {
        DEBUG_PARSER("S -> %empty");
		createStructTable();
	}
	;

struct_or_union
	: STRUCT {
        DEBUG_PARSER("struct_or_union -> STRUCT");
        $$ = $1; currentDataType="struct";
    }
	| UNION {
        DEBUG_PARSER("struct_or_union -> UNION");
        $$ = $1; currentDataType="union";
    }
	;

struct_declaration_list
	: struct_declaration {
        DEBUG_PARSER("struct_declaration_list -> struct_declaration");
        $$ = $1;
    }
	| struct_declaration_list struct_declaration 	{
        DEBUG_PARSER("struct_declaration_list -> struct_declaration_list struct_declaration");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration_list", &v);
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
        DEBUG_PARSER("struct_declaration -> specifier_qualifier_list struct_declarator_list ';'");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration", &v);
		
		// Semantics
		type = "";
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
        DEBUG_PARSER("specifier_qualifier_list -> type_specifier specifier_qualifier_list");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_specifier {
        DEBUG_PARSER("specifier_qualifier_list -> type_specifier");
        $$ = $1;
    }
	| type_qualifier specifier_qualifier_list 	{
        DEBUG_PARSER("specifier_qualifier_list -> type_qualifier specifier_qualifier_list");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_qualifier {
        DEBUG_PARSER("specifier_qualifier_list -> type_qualifier");
        $$ = $1;
    }
	;

struct_declarator_list
	: struct_declarator {
        DEBUG_PARSER("struct_declarator_list -> struct_declarator");
        $$ = $1;
    }
	| struct_declarator_list ',' struct_declarator {
        DEBUG_PARSER("struct_declarator_list -> struct_declarator_list ',' struct_declarator");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("struct_declarator_list", &v);
	}
	;

struct_declarator
	: declarator	{ 
        DEBUG_PARSER("struct_declarator -> declarator");
		$$ = $1; 
		
		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $1->size, 0) != 1){
			yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str(), "scope error");
		}
	}
	| ':' constant_expression {
        DEBUG_PARSER("struct_declarator -> ':' constant_expression");
        $$ = $2;
    }
	| declarator ':' constant_expression	{
        DEBUG_PARSER("struct_declarator -> declarator ':' constant_expression");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode(":", &v);
		
		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $3->intVal, 0) != 1){
			yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str(), "scope error");
		}
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
        DEBUG_PARSER("enum_specifier -> ENUM '{' enumerator_list '}'");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createASTNode($1, &v);
		
		// TODO: Add enum semantics
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
        DEBUG_PARSER("enum_specifier -> ENUM IDENTIFIER '{' enumerator_list '}'");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
	}
	| ENUM IDENTIFIER {
        DEBUG_PARSER("enum_specifier -> ENUM IDENTIFIER");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		currentDataType="Enum ";
		std::string check=std::string($2);
		currentDataType+=check;
		$$ = createASTNode($1, &v);
	}
	;

enumerator_list
	: enumerator {
        DEBUG_PARSER("enumerator_list -> enumerator");
        $$ = $1;
    }
	| enumerator_list ',' enumerator 	{
        DEBUG_PARSER("enumerator_list -> enumerator_list ',' enumerator");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("enumerator_list", &v);
	}
	;

enumerator
	: IDENTIFIER {
        DEBUG_PARSER("enumerator -> IDENTIFIER");
        $$ = createASTNode($1);
    }
	| IDENTIFIER '=' constant_expression 	{
        DEBUG_PARSER("enumerator -> IDENTIFIER '=' constant_expression");
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
	}
	;

type_qualifier
	: CONST		{ 
        DEBUG_PARSER("type_qualifier -> CONST");
		$$ = createASTNode($1); 
		currentDataType="const "; 
		flag2=1;
	}
	| VOLATILE	{ 
        DEBUG_PARSER("type_qualifier -> VOLATILE");
		$$ = createASTNode($1); 
		currentDataType="volatile "; 
		flag2=1;
	}
	;


declarator
	: pointer direct_declarator{
        DEBUG_PARSER("declarator -> pointer direct_declarator");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declarator", &v);
		
		//Semantics
		if(type == "#INSIDE"){
			$$->type = "STRUCT_" + structName + $1->type;
			$$->temp_name = $2->temp_name;
			$$->size = 8;
			$$->expType = 2;
		}
		else{
			$$->type = $2->type + $1->type;
			$$->temp_name = $2->temp_name;
			$$->size = 8;
			$$->expType = 2;
		}
	}
	| direct_declarator {
        DEBUG_PARSER("declarator -> direct_declarator");
        $$ = $1 ;
    }
	;


direct_declarator
	: IDENTIFIER {
        DEBUG_PARSER("direct_declarator -> IDENTIFIER");
		$$ = createASTNode($1);
		std::string check=std::string($1);
		if(flag){
			typedefTable.push_back(make_pair(check,currentDataType));
			flag=0;
		}
		else{
		std::string check=std::string($1);
		addToSymbolTable(check,currentDataType);
		}
		
		// Semantics
		$$->expType = 1; // Variable
		$$->type = type;
		$$->temp_name = string($1);
		$$->size = getSize(type);
	}
	| CONSTANT IDENTIFIER {
        DEBUG_PARSER("direct_declarator -> CONSTANT IDENTIFIER");
		yyerror("invalid identifier", "syntax error");
		$$ = createASTNode("Invalid Identifier");
	}
	| '(' declarator ')'  {
        DEBUG_PARSER("direct_declarator -> '(' declarator ')'");
        $$ = $2 ;
    }
	| direct_declarator '[' constant_expression ']'{
        DEBUG_PARSER("direct_declarator -> direct_declarator '[' constant_expression ']'");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		updateLastSymbolEntry();

		// Semantics
		if($1->expType == 1 || $1->expType == 2) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = $1->size * $3->intVal;
		}
		else{
			yyerror(("Function " + $1->temp_name + " cannot be used as an array").c_str(), "type error");
		}
	}
	| direct_declarator '[' ']'{
        DEBUG_PARSER("direct_declarator -> direct_declarator '[' ']'");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		updateLastSymbolEntry();
		$$ = createASTNode("direct_declarator", &v);

		// Semantics
		if($1->expType <=2 ) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = 8;
		}
		else{
			yyerror(("Function " + $1->temp_name + " cannot be used as an array").c_str(), "type error");
		}
	}
	| direct_declarator '(' A parameter_type_list ')'{
        DEBUG_PARSER("direct_declarator -> direct_declarator '(' A parameter_type_list ')'");
		std::vector<Data> v, v2;
		insertAttr(v2, $4, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		updateFuncSymbolEntry(noArgs);
		noArgs=0;

		// Semantics
		string argMapKey = $1->temp_name;
        
        	// If inside a class definition, use qualified name
        if (!className.empty()) {
            	argMapKey = className + "_" + argMapKey;
        }
		if($1->expType == 1) {
			$$->temp_name = $1->temp_name;
			$$->expType = 3;
			$$->type = $1->type;
			$$->size = getSize($$->type);
			
			vector<string> temp = getFuncArgs(argMapKey);
			if(temp.size() == 1 && temp[0] == "#NO_FUNC"){
				insertFuncArg(argMapKey, funcArgs);
				funcArgs.clear();
				funcName = string(argMapKey);
				funcType = $1->type;
			}
			else{
				// Check if temp is correct
				if(temp == funcArgs){
					funcArgs.clear();
					funcName = string(argMapKey);
					funcType = $1->type;
				}
				else{
					yyerror(("Conflicting types for " + argMapKey).c_str(), "type error");
				}
			}
		}
		else{
			if($1->expType == 2){
				yyerror( (argMapKey + "declared as array of function").c_str(), "type error");
			}
			else{
				yyerror( (argMapKey + "declared as function of function").c_str(), "type error");
			}
		}
	}
	| direct_declarator '(' A identifier_list ')'{
        DEBUG_PARSER("direct_declarator -> direct_declarator '(' A identifier_list ')'");
		std::vector<Data> v, v2;
		insertAttr(v2, $4, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);

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
		if(args.size() == 1 && args[0] == "#NO_FUNC"){
			args.clear();
			for(int i = 0; i < idList.size(); i++){
				insertSymbol(*curr_table, idList[i], "int", 4, 1, NULL);
				args.push_back("int");
			}
			insertFuncArg($1->temp_name, args);
		}

		if(args.size() == idList.size()) {
			for(int i = 0; i < args.size(); i++) {
				if(args[i] == "..."){
					yyerror(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
					break;
				}
				insertSymbol(*curr_table, idList[i], args[i], getSize(args[i]), 1, NULL);
			}
			idList.clear();
		}
		else{
			yyerror(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
			idList.clear();
		}
	}
	| direct_declarator '(' A ')'{
        DEBUG_PARSER("direct_declarator -> direct_declarator '(' A ')'");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "( )", 0);
		$$ = createASTNode("direct_declarator", &v);
		updateFuncSymbolEntry(0);

		// Semantics
		if($1->expType == 1) {
			$$->temp_name = $1->temp_name;
			$$->expType = 3;
			$$->type = $1->type;
			$$->size = getSize($$->type);

			vector<string> temp = getFuncArgs($1->temp_name);
			if(temp.size() == 1 && temp[0] == "#NO_FUNC"){
				insertFuncArg($$->temp_name, funcArgs);
				funcArgs.clear();
				funcName = string($1->temp_name);
				funcType = $1->type;
			}
			else{
				yyerror(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
			}
		}
		else{
			if($1->expType == 2){
				yyerror( ($1->temp_name + "declared as array of function").c_str(), "type error");
			}
			else{
				yyerror( ($1->temp_name + "declared as function of function").c_str(), "type error");
			}
		}
	}
	;

A 
	: %empty	{
        DEBUG_PARSER("A -> %empty");
		type ="";
		func_flag = 0;
		funcArgs.clear();
		createParamList();
	}

pointer
	: '*' {
        DEBUG_PARSER("pointer -> '*'");
		currentDataType+="*";
		$$ = createASTNode("*(Pointer)");
		$$->type = "*";
	}
	| '*' type_qualifier_list{
        DEBUG_PARSER("pointer -> '*' type_qualifier_list");
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
		$$->type = "*";
	}
	| '*' pointer{
        DEBUG_PARSER("pointer -> '*' pointer");
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
		$$->type = "*" + $2->type;
	}
	| '*' type_qualifier_list pointer{
        DEBUG_PARSER("pointer -> '*' type_qualifier_list pointer");
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		insertAttr(v,$3,"",1);
		$$ = createASTNode("*(Pointer)",&v);
		$$->type = "*" + $3->type;
	}
	;

type_qualifier_list
	: type_qualifier {
        DEBUG_PARSER("type_qualifier_list -> type_qualifier");
        $$ = $1 ;
    }
	| type_qualifier_list type_qualifier{
        DEBUG_PARSER("type_qualifier_list -> type_qualifier_list type_qualifier");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_qualifier_list",&v);
	}
	;


parameter_type_list
	: parameter_list {
        DEBUG_PARSER("parameter_type_list -> parameter_list");
        $$ = $1 ;
    }
	| parameter_list ',' ELLIPSIS{
        DEBUG_PARSER("parameter_type_list -> parameter_list ',' ELLIPSIS");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v, createASTNode($3), "", 1);
		$$ = createASTNode("parameter_type_list",&v);
		
		// Semantics - add ellipsis to function argument list
		funcArgs.push_back("...");
	}
	;

parameter_list
	: parameter_declaration{
        DEBUG_PARSER("parameter_list -> parameter_declaration");
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
        DEBUG_PARSER("parameter_list -> parameter_list ',' parameter_declaration");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		noArgs++;
		$$ = createASTNode("parameter_list",&v);
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
        DEBUG_PARSER("parameter_declaration -> declaration_specifiers declarator");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
		
		// Semantics
		type = "";
		if($2->expType == 1 || $2->expType == 2) {
			if(currLookup($2->temp_name)) {
				yyerror(("Redeclaration of Parameter " + $2->temp_name).c_str(), "scope error");
			}
			else {
				insertSymbol(*curr_table, $2->temp_name, $2->type, $2->size, true, NULL);
			}
			funcArgs.push_back($2->type);
		}
	}
	| declaration_specifiers abstract_declarator{
        DEBUG_PARSER("parameter_declaration -> declaration_specifiers abstract_declarator");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
		
		// Semantics
		type = "";
	}
	| declaration_specifiers {
        DEBUG_PARSER("parameter_declaration -> declaration_specifiers");
        $$ = $1;
		
		// Semantics
		funcArgs.push_back(type);
		type = "";
	}
	;

identifier_list
	: IDENTIFIER {
        DEBUG_PARSER("identifier_list -> IDENTIFIER");
		$$ = createASTNode($1);
		
		// Semantics - add identifier to list for old-style function declarations
		idList.push_back($1);
	}
	| identifier_list ',' IDENTIFIER{
        DEBUG_PARSER("identifier_list -> identifier_list ',' IDENTIFIER");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,createASTNode($3),"",1);
		$$ = createASTNode("identifier_list",&v);
		
		// Semantics
		idList.push_back($3);
	}
	;

type_name
	: specifier_qualifier_list{
        DEBUG_PARSER("type_name -> specifier_qualifier_list");
        $$ = $1;
    }
	| specifier_qualifier_list abstract_declarator{
        DEBUG_PARSER("type_name -> specifier_qualifier_list abstract_declarator");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_name",&v);
	}
	;

abstract_declarator
	: pointer {
        DEBUG_PARSER("abstract_declarator -> pointer");
        $$ = $1;
    }
	| direct_abstract_declarator{
        DEBUG_PARSER("abstract_declarator -> direct_abstract_declarator");
        $$ = $1;
    }
	| pointer direct_abstract_declarator{
        DEBUG_PARSER("abstract_declarator -> pointer direct_abstract_declarator");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("abstract_declarator",&v);
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
        DEBUG_PARSER("direct_abstract_declarator -> '(' abstract_declarator ')'");
        $$ = $2;
    }
	| '[' ']'{
        DEBUG_PARSER("direct_abstract_declarator -> '[' ']'");
		$$ = createASTNode("[ ]");
		
		// Semantics
		$$->type = type + "*"; // Array type
		$$->size = 8; // Default size for a pointer
	}
	| '[' constant_expression ']' {
        DEBUG_PARSER("direct_abstract_declarator -> '[' constant_expression ']'");
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("[ ]", &v);
		
		// Semantics
		$$->type = type + "*"; 
		$$->size = getSize(type) * $2->intVal;
	}
	| direct_abstract_declarator '[' ']' {
        DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '[' ']'");
		std::vector<Data> v;
		insertAttr(v, NULL, "[ ]", 0);
		insertAttr(v, $1, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
		
		// Semantics
		$$->type = $1->type + "*";
		$$->size = 8;
	}
	| direct_abstract_declarator '[' constant_expression ']'{
        DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '[' constant_expression ']'");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, NULL, 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
		
		// Semantics
		$$->type = $1->type + "*";
		$$->size = $1->size * $3->intVal;
	}
	| '(' ')'{
        DEBUG_PARSER("direct_abstract_declarator -> '(' ')'");
		$$ = createASTNode("( )");
		
		// Semantics - function type with no parameters
		$$->type = "FUNC_" + type;
		$$->size = 0;
	}
	| '(' parameter_type_list ')'{
        DEBUG_PARSER("direct_abstract_declarator -> '(' parameter_type_list ')'");
        $$ = $2;
		
		// Semantics - function type with parameters
		$$->type = "FUNC_" + type;
		$$->size = 0;
	}
	| direct_abstract_declarator '(' ')'{
        DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '(' ')'");
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, $1, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
		
			// Semantics
		$$->type = "FUNC_" + $1->type;
		$$->size = 0;
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
        DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '(' parameter_type_list ')'");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
		
		// Semantics
		$$->type = "FUNC_" + $1->type;
		$$->size = 0;
	}
	;

initializer
	: assignment_expression{
        DEBUG_PARSER("initializer -> assignment_expression");
        $$ = $1;
    }
	| '{' initializer_list '}' {
        DEBUG_PARSER("initializer -> '{' initializer_list '}'");
        $$ = $2;
		
		// Semantics for array initialization
		$$->isInit = 1;
	}
	| '{' initializer_list ',' '}'{
        DEBUG_PARSER("initializer -> '{' initializer_list ',' '}'");
        $$ = $2;
		
		// Semantics
		$$->isInit = 1;
	}
	;


initializer_list
	: initializer	{
        DEBUG_PARSER("initializer_list -> initializer");
        $$ = $1;
    }
	| initializer_list ',' initializer	{
        DEBUG_PARSER("initializer_list -> initializer_list ',' initializer");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("initializer_list", &v);
		
		// Semantics
		$$->isInit = ($1->isInit && $3->isInit);
	}
	;

statement
	: labeled_statement {
        DEBUG_PARSER("statement -> labeled_statement");
        $$ = $1;
    }
	| compound_statement {
        DEBUG_PARSER("statement -> compound_statement");
        $$ = $1;
    }
	| expression_statement {
        DEBUG_PARSER("statement -> expression_statement");
        $$ = $1;
    }
	| selection_statement {
        DEBUG_PARSER("statement -> selection_statement");
        $$ = $1;
    }
	| iteration_statement {
        DEBUG_PARSER("statement -> iteration_statement");
        $$ = $1;
    }
	| jump_statement {
        DEBUG_PARSER("statement -> jump_statement");
        $$ = $1;
    }
	;

labeled_statement
	: IDENTIFIER ':' statement	{
        DEBUG_PARSER("labeled_statement -> IDENTIFIER ':' statement");
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("labeled_statement", &v);
	}
	| CASE constant_expression ':' statement	{
        DEBUG_PARSER("labeled_statement -> CASE constant_expression ':' statement");
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("case", &v);
	}
	| DEFAULT ':' statement	{
        DEBUG_PARSER("labeled_statement -> DEFAULT ':' statement");
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("case", &v);
	}
	;

compound_statement
	: '{' '}'	{
        DEBUG_PARSER("compound_statement -> '{' '}'");
		$$ = createASTNode("{ }");
	}
	| '{' CHANGE_TABLE statement_list '}'	{
        DEBUG_PARSER("compound_statement -> '{' CHANGE_TABLE statement_list '}'");
		$$ = $3;
		
		// Semantics - clean up block scope
		if(func_flag >= 2){
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName + str + ".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	| '{' CHANGE_TABLE declaration_list '}'	{
        DEBUG_PARSER("compound_statement -> '{' CHANGE_TABLE declaration_list '}'");
		$$ = $3;
		
		// Semantics - clean up block scope
		if(func_flag >= 2){
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName + str + ".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	| '{' CHANGE_TABLE declaration_list statement_list '}'	{
        DEBUG_PARSER("compound_statement -> '{' CHANGE_TABLE declaration_list statement_list '}'");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("compound_statement", &v);
		
		// Semantics - clean up block scope
		if(func_flag >= 2){
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName + str + ".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	;

CHANGE_TABLE
	: %empty {
        DEBUG_PARSER("CHANGE_TABLE -> %empty");
		if(func_flag){
			string str = "Block" + to_string(block_count);
			block_stack.push(block_count);
			block_count++;
			func_flag++;
			makeSymbolTable(str, "");
		}
		else func_flag++;
		
		$$ = strdup("");
	}
	;

declaration_list
	: declaration {
        DEBUG_PARSER("declaration_list -> declaration");
        $$ = $1;
    }
	| declaration_list declaration	{
        DEBUG_PARSER("declaration_list -> declaration_list declaration");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declaration_list", &v);
	}
	;

statement_list
	: statement {
        DEBUG_PARSER("statement_list -> statement");
        $$ = $1;
    }
	| statement_list statement	{
        DEBUG_PARSER("statement_list -> statement_list statement");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("statement_list", &v);
	}
	;

expression_statement
	: ';' {
        DEBUG_PARSER("expression_statement -> ';'");
        $$ = createASTNode(";");
    }
	| expression ';' {
        DEBUG_PARSER("expression_statement -> expression ';'");
        $$ = $1;
    }
	;

selection_statement
	: IF '(' expression ')' statement	{
        DEBUG_PARSER("selection_statement -> IF '(' expression ')' statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("if", &v);
	}
	| IF '(' expression ')' statement ELSE statement	{
        DEBUG_PARSER("selection_statement -> IF '(' expression ')' statement ELSE statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("if-else", &v);
	}
	| SWITCH '(' expression ')' statement	{
        DEBUG_PARSER("selection_statement -> SWITCH '(' expression ')' statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("switch", &v);
	}
	| IF '(' ')' statement {
        DEBUG_PARSER("selection_statement -> IF '(' ')' statement");
        yyerror("missing condition in 'if' statement.", "syntax error");
        $$ = createASTNode("error-node");
    }
	| SWITCH '(' ')' statement {
        DEBUG_PARSER("selection_statement -> SWITCH '(' ')' statement");
        yyerror("missing condition in 'switch' statement.", "syntax error");
        $$ = createASTNode("error-node");
    }
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
        DEBUG_PARSER("iteration_statement -> WHILE '(' expression ')' statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("while-loop", &v);
	}
	| DO statement WHILE '(' expression ')' ';'	{
        DEBUG_PARSER("iteration_statement -> DO statement WHILE '(' expression ')' ';'");
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("do-while-loop", &v);
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
        DEBUG_PARSER("iteration_statement -> FOR '(' expression_statement expression_statement ')' statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $6, "", 1);
		$$ = createASTNode("for-loop(w/o update stmt)", &v);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
        DEBUG_PARSER("iteration_statement -> FOR '(' expression_statement expression_statement expression ')' statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("for-loop", &v);
	}
    | UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
        DEBUG_PARSER("iteration_statement -> UNTIL '(' expression ')' statement");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("until-loop", &v);
	}
	| WHILE '[' expression ']' statement {
        DEBUG_PARSER("iteration_statement -> WHILE '[' expression ']' statement");
        yyerror("incorrect parentheses in while-loop.", "syntax error");
		$$ = createASTNode("Invalid While-loop", nullptr);
    }
    | UNTIL '[' expression ']' statement {
        DEBUG_PARSER("iteration_statement -> UNTIL '[' expression ']' statement");
        yyerror("incorrect parentheses in until-loop.", "syntax error");
		$$ = createASTNode("Invalid Until-loop", nullptr);
    }
    | FOR '(' expression ',' expression ',' expression ')' statement {
        DEBUG_PARSER("iteration_statement -> FOR '(' expression ',' expression ',' expression ')' statement");
        yyerror("comma used instead of semicolons.", "syntax error");
        $$ = createASTNode("Invalid for-loop", nullptr);
    }
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
        DEBUG_PARSER("jump_statement -> GOTO IDENTIFIER ';'");
		std::string s;
		s = (std::string)$1 + " : " + (std::string)$2;
        $$ = createASTNode(s);
	}
	| CONTINUE ';' {
        DEBUG_PARSER("jump_statement -> CONTINUE ';'");
        $$ = createASTNode($1);
    }
	| BREAK ';' {
        DEBUG_PARSER("jump_statement -> BREAK ';'");
        $$ = createASTNode($1);
    }
	| RETURN ';' {
        DEBUG_PARSER("jump_statement -> RETURN ';'");
        $$ = createASTNode($1);
    }
	| RETURN expression ';'	{
        DEBUG_PARSER("jump_statement -> RETURN expression ';'");
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("jump_stmt", &v);
	}
	;

translation_unit
	: external_declaration	{
        DEBUG_PARSER("translation_unit -> external_declaration");
		$$ = $1;
	}
	| translation_unit external_declaration	{
        DEBUG_PARSER("translation_unit -> translation_unit external_declaration");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("program", &v);
	}
	| error ';' {
        DEBUG_PARSER("translation_unit -> error ';'");
		$$ = new Node; yyerrok;
	}
	| error ','{
        DEBUG_PARSER("translation_unit -> error ','");
		$$ = new Node; yyerrok;
	}
	| error{
        DEBUG_PARSER("translation_unit -> error");
		$$ = new Node; yyerrok;
	}
	;

external_declaration
	: function_definition {
        DEBUG_PARSER("external_declaration -> function_definition");
        $$ = $1;
    }
	| declaration {
        DEBUG_PARSER("external_declaration -> declaration");
        $$ = $1;
    }
	;

function_definition
	: declaration_specifiers declarator F declaration_list compound_statement	{
        DEBUG_PARSER("function_definition -> declaration_specifiers declarator F declaration_list compound_statement");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("function", &v);

		// Semantics
		// Extract and propagate function name and return type for classes
        $$->temp_name = $2->temp_name;  // Function name from declarator
        $$->type = funcType;            // Return type from declaration_specifiers
        $$->size = $2->size;            // Size if applicable

		type = "";
		string fName = string($3);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declaration_specifiers declarator F compound_statement	{
        DEBUG_PARSER("function_definition -> declaration_specifiers declarator F compound_statement");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("function (w/o decl_list)", &v);

		// Semantics 
		// Extract and propagate function name and return type for classes
        $$->temp_name = $2->temp_name;  // Function name from declarator
        $$->type = funcType;            // Return type from declaration_specifiers
        $$->size = $2->size;            // Size if applicable

		type = "";
		string fName = string($3);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declarator F declaration_list compound_statement	{//this rule can be constructor of classess OOPS
        DEBUG_PARSER("function_definition -> declarator F declaration_list compound_statement");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("function (w/o decl_specifiers)", &v);

		// Semantics
		type = "";
		string fName = string($2);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declarator F compound_statement	{//this rule can be constructor of classess OOPS
        DEBUG_PARSER("function_definition -> declarator F compound_statement");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("function (w/o specifiers and decl_list)", &v);

		// Semantics
		type = "";
		string fName = string($2);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	;

F 
	: %empty {
        DEBUG_PARSER("F -> %empty");
		std::string qualifiedFuncName = funcName;
        if (!className.empty()) {
            qualifiedFuncName = className + "_" + funcName;
        }
		if (gst.find(qualifiedFuncName) != gst.end()){
			yyerror(("Redefinition of function " + qualifiedFuncName).c_str(), "scope error");
		}
		else{
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
	onlyLexin = false;
    beginAST(inputFile);
    yyparse();
    endAST();
}

int yyerror(const char* s, const std::string &errorType) {
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
    return -1;
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
