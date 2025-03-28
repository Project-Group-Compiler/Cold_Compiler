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
    	$$ = getNode(std::string($1));
		
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

			//3AC_1


		}
    }
	/* | CONSTANT IDENTIFIER {
		DEBUG_PARSER("primary_expression -> CONSTANT IDENTIFIER");
		yyerror("invalid identifier", "syntax error");
		$$ = getNode($1);
	}
	| CONSTANT CONSTANT {
		DEBUG_PARSER("primary_expression -> CONSTANT CONSTANT");
		yyerror("invalid constant", "syntax error");
		$$ = getNode($1);
	} */
	| CONSTANT {
		std::string tp = std::string($1->str);
		$$ = getNode(tp);
		
		// Semantics for constants
		$$->type = $1->type;
		$$->intVal = $1->intVal;
		$$->realVal = $1->realVal;
		$$->expType = 4;
		$$->temp_name = $1->str;

		//3AC_2


	}
	| STRING_LITERAL {
        DEBUG_PARSER("primary_expression -> STRING_LITERAL");
		std::string check=std::string($1);
		addToConstantTable(check,"String Literal");
		$$ = getNode($1);
		
		// Semantics for string literals
		$$->type = string("char*");
		$$->temp_name = string($1);
		$$->strVal = string($1);

		//3AC_3
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
		$$ = getNode("postfix_expression", mergeAttrs($1, $3));
		
		// Semantics
		if($1->isInit && $3->isInit){
			$$->isInit = 1;
		}
		string temp = postfixExpression($1->type, 1);

		//3AC_4

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
				if(!funcArg.empty()){
					yyerror(("Too few Arguments to Function " + $1->temp_name).c_str(), "semantic error");
				}

				//3AC_5

			}
		}
		else{
			yyerror(("Function " + $1->temp_name + " not declared in this scope").c_str(), "scope error");
		}
		currArgs.clear(); 
	}
	| postfix_expression '(' argument_expression_list ')' {
        DEBUG_PARSER("postfix_expression -> postfix_expression '(' argument_expression_list ')'");
		$$ = getNode("postfix_expression", mergeAttrs($1, $3));
		
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
						yyerror(("Too few Arguments to Function " + $1->temp_name).c_str(), "semantic error");
						break;
					}
					string msg = checkType(funcArgs[i],currArgs[i]);
					std::cout << funcArgs[i] << "     " << currArgs[i] << "     " << msg << '\n';
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
				//3AC_6
			}
		}
		else{
			yyerror("Invalid function call", "semantic error");
		}
		currArgs.clear();
	}
	| postfix_expression '.' IDENTIFIER {
    	DEBUG_PARSER("postfix_expression -> postfix_expression '.' IDENTIFIER");
    	$$ = getNode("expression.id", mergeAttrs($1, getNode($3)));
	
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
	| postfix_expression PTR_OP IDENTIFIER {
        DEBUG_PARSER("postfix_expression -> postfix_expression PTR_OP IDENTIFIER");
		$$ = getNode($2, mergeAttrs($1, getNode($3)));
		
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
		$$ = getNode($2, mergeAttrs($1, nullptr));
		
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
		$$ = getNode($2, mergeAttrs($1, nullptr));
		
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
		
		// Semantics
		$$->isInit = $1->isInit;
		currArgs.push_back($1->type);
		$$->type = "void";
	}
	| argument_expression_list ',' assignment_expression {
        DEBUG_PARSER("argument_expression_list -> argument_expression_list ',' assignment_expression");
		$$ = getNode("argument_list", mergeAttrs($1, $3));
		
		// Semantics
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
		$$ = getNode($1, mergeAttrs($2, nullptr));
		
		// Semantics
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
		$$ = getNode($1, mergeAttrs($2, nullptr));

		
		// Semantics
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
		$$ = getNode("unary_exp", mergeAttrs($1, $2));
		
		// Semantics
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
		$$ = getNode($1, mergeAttrs($2, nullptr));
		
		// Semantics
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $2->size;
	}
	| SIZEOF '(' type_name ')' {
        DEBUG_PARSER("unary_expression -> SIZEOF '(' type_name ')'");
		$$ = getNode($1, mergeAttrs($3, nullptr));
		
		// Semantics
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $3->size;
	}
	;

unary_operator
	: '&' {
		DEBUG_PARSER("unary_operator -> '&'");
		$$ = getNode("&");
	}
	| '*' {
		DEBUG_PARSER("unary_operator -> '*'");
		$$ = getNode("*");
	}
	| '+' {
		DEBUG_PARSER("unary_operator -> '+'");
		$$ = getNode("+");
	}
	| '-' {
		DEBUG_PARSER("unary_operator -> '-'");
		$$ = getNode("-");
	}
	| '~' {
		DEBUG_PARSER("unary_operator -> '~'");
		$$ = getNode("~");
	}
	| '!' {
		DEBUG_PARSER("unary_operator -> '!'");
		$$ = getNode("!");
	}
	;

cast_expression
	: unary_expression {
		DEBUG_PARSER("cast_expression -> unary_expression");
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		DEBUG_PARSER("cast_expression -> '(' type_name ')' cast_expression");
		$$ = getNode("cast_expression", mergeAttrs($2, $4));
		
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
		$$ = getNode("*", mergeAttrs($1, $3));
		
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
		$$ = getNode("/", mergeAttrs($1, $3));
		
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
		$$ = getNode("%", mergeAttrs($1, $3));
		
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
        $$ = getNode("+", mergeAttrs($1, $3));
        
        // Semantic
        if($1->isInit && $3->isInit) $$->isInit = 1;
        $$->intVal = $1->intVal + $3->intVal;
        string temp = addExp($1->type, $3->type, '+');
        if(!temp.empty()){
            $$->type = (temp == "int") ? "long long" : (temp == "real") ? "long double" : temp;
        } else {
            yyerror("Incompatible type for + operator", "type error");
        }
    }
    | additive_expression '-' multiplicative_expression {
        DEBUG_PARSER("additive_expression -> additive_expression '-' multiplicative_expression");
        $$ = getNode("-", mergeAttrs($1, $3));
        
        // Semantic
        if($1->isInit && $3->isInit) $$->isInit = 1;
        $$->intVal = $1->intVal - $3->intVal;
        string temp = addExp($1->type, $3->type, '-');
        if(!temp.empty()){
            $$->type = (temp == "int") ? "long long" : (temp == "real") ? "long double" : temp;
        } else {
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
        $$ = getNode($2, mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = shiftExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = $1->type;
        } else {
            yyerror("Invalid operands to binary <<", "type error");
        }
    }
    | shift_expression RIGHT_OP additive_expression {
        DEBUG_PARSER("shift_expression -> shift_expression RIGHT_OP additive_expression");
        $$ = getNode($2, mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = shiftExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = $1->type;
        } else {
            yyerror("Invalid operands to binary >>", "type error");
        }
    }
    ;

relational_expression
    : inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> inclusive_or_expression");
        $$ = $1;
    }
    | relational_expression '<' inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression '<' inclusive_or_expression");
        $$ = getNode("<", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "Bool") warning("Comparison between pointer and integer");
        } else {
            yyerror("Invalid operands to binary <", "type error");
        }
    }
    | relational_expression '>' inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression '>' inclusive_or_expression");
        $$ = getNode(">", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "Bool") warning("Comparison between pointer and integer");
        } else {
            yyerror("Invalid operands to binary >", "type error");
        }
    }
    | relational_expression LE_OP inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression LE_OP inclusive_or_expression");
        $$ = getNode("<=", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "Bool") warning("Comparison between pointer and integer");
        } else {
            yyerror("Invalid operands to binary <=", "type error");
        }
    }
    | relational_expression GE_OP inclusive_or_expression {
        DEBUG_PARSER("relational_expression -> relational_expression GE_OP inclusive_or_expression");
        $$ = getNode(">=", mergeAttrs($1, $3));
        // Semantic
        if ($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = relExp($1->type, $3->type);
        if (!temp.empty()) {
            $$->type = "bool";
            if (temp == "Bool") warning("Comparison between pointer and integer");
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
        $$ = getNode($2, mergeAttrs($1, $3));
        
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
        $$ = getNode($2, mergeAttrs($1, $3));
        
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
    | and_expression '&' shift_expression {
        DEBUG_PARSER("and_expression -> and_expression '&' shift_expression");
        $$ = getNode("&", mergeAttrs($1, $3));
        
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
    | exclusive_or_expression '^' and_expression {
        DEBUG_PARSER("exclusive_or_expression -> exclusive_or_expression '^' and_expression");
        $$ = getNode("^", mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = bitExp($1->type, $3->type);
        if(!temp.empty()){
            $$->type = (temp == "ok") ? "bool" : "long long";
        } else {
            yyerror("Invalid operands to binary ^", "type error");
        }
    }
    ;

inclusive_or_expression
    : exclusive_or_expression {
        DEBUG_PARSER("inclusive_or_expression -> exclusive_or_expression");
        $$ = $1;
    }
    | inclusive_or_expression '|' exclusive_or_expression {
        DEBUG_PARSER("inclusive_or_expression -> inclusive_or_expression '|' exclusive_or_expression");
        $$ = getNode("|", mergeAttrs($1, $3));
        
        // Semantics
        if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
        string temp = bitExp($1->type, $3->type);
        if(!temp.empty()){
            $$->type = (temp == "ok") ? "bool" : "long long";
        } else {
            yyerror("Invalid operands to binary |", "type error");
        }
    }
    ;

logical_and_expression
    : equality_expression {
        DEBUG_PARSER("logical_and_expression -> equality_expression");
        $$ = $1;
    }
    | logical_and_expression AND_OP equality_expression {
        DEBUG_PARSER("logical_and_expression -> logical_and_expression AND_OP equality_expression");
        $$ = getNode("&&", mergeAttrs($1, $3));

        // Semantics
        $$->type = "bool";
        $$->isInit = ($1->isInit & $3->isInit);   
        $$->intVal = $1->intVal && $3->intVal;
    }
    ;

logical_or_expression
    : logical_and_expression {
        DEBUG_PARSER("logical_or_expression -> logical_and_expression");
        $$ = $1;
    }
    | logical_or_expression OR_OP logical_and_expression {
        DEBUG_PARSER("logical_or_expression -> logical_or_expression OR_OP logical_and_expression");
        $$ = getNode("||", mergeAttrs($1, $3));
        
        // Semantics
        $$->type = "bool";
        $$->isInit = $1->isInit & $3->isInit;
        $$->intVal = $1->intVal || $3->intVal;
    }
    ;

conditional_expression
    : logical_or_expression {
        DEBUG_PARSER("conditional_expression -> logical_or_expression");
        $$ = $1;
    }
    | logical_or_expression '?' expression ':' conditional_expression {
        DEBUG_PARSER("conditional_expression -> logical_or_expression '?' expression ':' conditional_expression");
        $$ = getNode("ternary operator", mergeAttrs($1, $3, $5));
        
        // Semantics
        string temp = condExp($3->type, $5->type);
        if (!temp.empty()) {
            $$->type = "int";
        } else {
            yyerror("Type mismatch in Conditional Expression", "type error");
        }
        $$->isInit = $1->isInit & $3->isInit & $5->isInit;
    }
    ;

assignment_expression
    : conditional_expression {
        DEBUG_PARSER("assignment_expression -> conditional_expression");
        $$ = $1;
    }
    | unary_expression assignment_operator assignment_expression {
        DEBUG_PARSER("assignment_expression -> unary_expression assignment_operator assignment_expression");
        $$ = getNode($2, mergeAttrs($1, $3));
        
        // Semantics
        string temp = assignExp($1->type, $3->type, string($2));
        if (!temp.empty()) {
            if (temp == "ok") {
                $$->type = $1->type;
            } else if (temp == "warning") {
                $$->type = $1->type;
                warning("Assignment with incompatible pointer type");
            }
        } else {
            yyerror("Incompatible types when assigning type", "type error");
        }
        if ($1->expType == 3 && $3->isInit) {
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
            DEBUG_PARSER("expression -> assignment_expression");
            $$ = $1;
        }
        | expression ',' assignment_expression {
            DEBUG_PARSER("expression -> expression ',' assignment_expression");
            $$ = getNode("expression", mergeAttrs($1, $3));
            $$->type = "void"; // Semantic
        }
        ;

    constant_expression
        : conditional_expression {
            DEBUG_PARSER("constant_expression -> conditional_expression");
            $$ = $1;
        }
        ;

    declaration
        : declaration_specifiers ';' {
            DEBUG_PARSER("declaration -> declaration_specifiers ';'");
            $$ = $1;
        }
        | declaration_specifiers init_declarator_list ';' {
            DEBUG_PARSER("declaration -> declaration_specifiers init_declarator_list ';'");
            $$ = getNode("declaration", mergeAttrs($1, $2));
            $$->temp_name = $2->temp_name;
            $$->type = $2->type;
            $$->size = $2->size;
            type = ""; // Reset type
        }
        ;

    declaration_specifiers
        : storage_class_specifier {
            DEBUG_PARSER("declaration_specifiers -> storage_class_specifier");
            $$ = $1;
        }
        | storage_class_specifier declaration_specifiers {
            DEBUG_PARSER("declaration_specifiers -> storage_class_specifier declaration_specifiers");
            $$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
        }
        | type_specifier {
            DEBUG_PARSER("declaration_specifiers -> type_specifier");
            $$ = $1;
        }
        | type_specifier declaration_specifiers {
            DEBUG_PARSER("declaration_specifiers -> type_specifier declaration_specifiers");
            $$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
        }
        | type_qualifier {
            DEBUG_PARSER("declaration_specifiers -> type_qualifier");
            $$ = $1;
        }
        | type_qualifier declaration_specifiers {
            DEBUG_PARSER("declaration_specifiers -> type_qualifier declaration_specifiers");
            $$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
        }
        ;

    init_declarator_list
        : init_declarator {
            DEBUG_PARSER("init_declarator_list -> init_declarator");
            $$ = $1;
        }
        | init_declarator_list ',' init_declarator {
            DEBUG_PARSER("init_declarator_list -> init_declarator_list ',' init_declarator");
            $$ = getNode("init_declarator_list", mergeAttrs($1, $3));
        }
        ;
        init_declarator
            : declarator {
                DEBUG_PARSER("init_declarator -> declarator");
				//$$=getNode($1);
				//
                $$ = $1;  // Just pass the node directly
                
                // Semantics
                if(currLookup($1->temp_name)){
                    yyerror(($1->temp_name + " is already declared").c_str(), "scope error");
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
            | declarator '=' initializer {
                DEBUG_PARSER("init_declarator -> declarator '=' initializer");
                $$ = getNode("=", mergeAttrs($1, $3));
                
                // Semantics
                if(currLookup($1->temp_name)){
                    yyerror(($1->temp_name + " is already declared").c_str(), "scope error");
                }
                else{
                    insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 1, NULL);
                }
            }
            ;

        storage_class_specifier
            : TYPEDEF {
                DEBUG_PARSER("storage_class_specifier -> TYPEDEF");
                $$ = getNode($1);
                flag = 1;
            }
            | EXTERN {
                DEBUG_PARSER("storage_class_specifier -> EXTERN");
                $$ = getNode($1);
                currentDataType = "extern ";
                flag2 = 1;
            }
            | STATIC {
                DEBUG_PARSER("storage_class_specifier -> STATIC");
                $$ = getNode($1);
                currentDataType = "static ";
                flag2 = 1;
            }
            | AUTO {
                DEBUG_PARSER("storage_class_specifier -> AUTO");
                $$ = getNode($1);
                currentDataType = "auto ";
                flag2 = 1;
            }
            | REGISTER {
                DEBUG_PARSER("storage_class_specifier -> REGISTER");
                $$ = getNode($1);
                currentDataType = "register ";
                flag2 = 1;
            }
            ;

        type_specifier
            : VOID {
                DEBUG_PARSER("type_specifier -> VOID");
                $$ = getNode($1);
                currentDataType = "void";
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }	
            | CHAR {
                DEBUG_PARSER("type_specifier -> CHAR");
                $$ = getNode($1);
                currentDataType = flag2 ? currentDataType + " char" : "char";
                flag2 = 0;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }	
            | SHORT {
                DEBUG_PARSER("type_specifier -> SHORT");
                $$ = getNode($1);
                currentDataType = flag2 ? currentDataType + " short" : "short";
                flag2 = 0;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }	
            | INT {
                DEBUG_PARSER("type_specifier -> INT");
                $$ = getNode($1);
                currentDataType = flag2 ? currentDataType + " int" : "int";
                flag2 = 0;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }
            | LONG {
                DEBUG_PARSER("type_specifier -> LONG");
                $$ = getNode($1);
                currentDataType = flag2 ? currentDataType + " long" : "long";
                flag2 = 0;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }
            | FLOAT {
                DEBUG_PARSER("type_specifier -> FLOAT");
                $$ = getNode($1);
                currentDataType = flag2 ? currentDataType + " float" : "float";
                flag2 = 0;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }
            | DOUBLE {
                DEBUG_PARSER("type_specifier -> DOUBLE");
                $$ = getNode($1);
                currentDataType = flag2 ? currentDataType + " double" : "double";
                flag2 = 0;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }
            | SIGNED {
                DEBUG_PARSER("type_specifier -> SIGNED");
                $$ = getNode($1);
                currentDataType = "signed";
                flag2 = 1;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }
            | UNSIGNED {
                DEBUG_PARSER("type_specifier -> UNSIGNED");
                $$ = getNode($1);
                currentDataType = "unsigned";
                flag2 = 1;
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
            }
            | FILE_MAN {
                DEBUG_PARSER("type_specifier -> FILE_MAN");
                $$ = getNode($1);
                currentDataType = "file";
                if(type.empty()) type = $1;
                else type += " " + std::string($1);
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
            | TYPE_NAME {
                DEBUG_PARSER("type_specifier -> TYPE_NAME");
                $$ = getNode($1);
                type = getType($1);
            }	
            ;
            inheritance_specifier
                : access_specifier IDENTIFIER {
                    DEBUG_PARSER("inheritance_specifier -> access_specifier IDENTIFIER");
                    $$ = getNode("inheritance_specifier", mergeAttrs($1, getNode($2)));
                }
                ;

            inheritance_specifier_list
                : inheritance_specifier {
                    DEBUG_PARSER("inheritance_specifier_list -> inheritance_specifier");
                    $$ = $1;
                }
                | inheritance_specifier_list ',' inheritance_specifier {
                    DEBUG_PARSER("inheritance_specifier_list -> inheritance_specifier_list ',' inheritance_specifier");
                    $$ = getNode("inheritance_specifier_list", mergeAttrs($1, $3));
                }
                ;

            access_specifier 
                : PRIVATE {
                    DEBUG_PARSER("access_specifier -> PRIVATE");
                    $$ = getNode($1);
                }
                | PUBLIC {
                    DEBUG_PARSER("access_specifier -> PUBLIC");
                    $$ = getNode($1);
                }
                | PROTECTED {
                    DEBUG_PARSER("access_specifier -> PROTECTED");
                    $$ = getNode($1);
                }
                ;

            class
                : CLASS {
                    DEBUG_PARSER("class -> CLASS");
                    $$ = getNode($1);
                }
                ;

            class_definition_head 
                : class S_C INHERITANCE_OP inheritance_specifier_list {
                    DEBUG_PARSER("class_definition_head -> class INHERITANCE_OP inheritance_specifier_list");
                    $$ = getNode("class_definition_head", mergeAttrs($1, $4));
                    $$->type = currentDataType;
                    $$->temp_name = to_string(++Anon_ClassCounter);
                }
                | class G_C S_C {
                    DEBUG_PARSER("class_definition_head -> class IDENTIFIER");
                    $$ = getNode("class_definition_head", mergeAttrs($1, getNode($2)));
                    currentDataType = "Class " + std::string($2);
                    $$->temp_name = std::string($2);
                }
                | class G_C S_C INHERITANCE_OP inheritance_specifier_list {
                    DEBUG_PARSER("class_definition_head -> class IDENTIFIER INHERITANCE_OP inheritance_specifier_list");
                    $$ = getNode("class_definition_head", mergeAttrs($1, getNode($2), $5));
                    currentDataType = "Class " + std::string($2);
                    $$->temp_name = std::string($2);
                }
                ;

            class_definition 
                : class_definition_head '{' class_internal_definition_list '}' {
                    DEBUG_PARSER("class_definition -> class_definition_head '{' class_internal_definition_list '}'");
                    $$ = getNode("class_definition", mergeAttrs($1, $3));
                    $$->temp_name = $1->temp_name;
                    if (printClassTable("CLASS_" + $1->temp_name) == 1) {
                        type = type.empty() ? "CLASS_" + $1->temp_name : type + " CLASS_" + $1->temp_name;
                    } else {
                        yyerror(("Class " + $1->temp_name + " is already defined").c_str(), "scope error");
                    }
                }
                | class IDENTIFIER {
                    DEBUG_PARSER("class_definition -> class IDENTIFIER");
                    $$ = $1;
                    
                    if (findClass("CLASS_" + string($2)) == 1) {
                        type = type.empty() ? "CLASS_" + string($2) : type + " CLASS_" + string($2);
                    } else if (className == string($2)) {
                        type = "#INSIDE";
                    } else {
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
                    $$ = getNode("class_internal_definition_list", mergeAttrs($1, $2));
                }
                ; 

            class_internal_definition	
                : access_specifier '{' class_member_list '}' ';' {
                    DEBUG_PARSER("class_internal_definition -> access_specifier '{' class_member_list '}' ';'");
                    $$ = getNode("class_internal_definition", mergeAttrs($1, $3));
                    currentAccess = $1->strVal;
                }
                ;

class_member_list
    : class_member {
        DEBUG_PARSER("class_member_list -> class_member");
        $$ = $1;
    }
    | class_member_list class_member {
        DEBUG_PARSER("class_member_list -> class_member_list class_member");
        $$ = getNode("class_member_list", mergeAttrs($1, $2));
    }
    ;

class_member
    : function_definition { 
        DEBUG_PARSER("class_member -> function_definition");
        $1->strVal = currentAccess;
        printf("DEBUG: Function member name=%s, type=%s\n", $1->temp_name.c_str(), $1->type.c_str());
        insertClassAttr($1->temp_name, "FUNC_" + $1->type, $1->size, 0);
    }
    | declaration { 
        DEBUG_PARSER("class_member -> declaration");
        $1->strVal = currentAccess;
        printf("DEBUG: Variable member name=%s, type=%s\n", $1->temp_name.c_str(), $1->type.c_str());
        insertClassAttr($1->temp_name, $1->type, $1->size, 0);
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
    : struct_or_union G S '{' struct_declaration_list '}' {
        DEBUG_PARSER("struct_or_union_specifier -> struct_or_union G S '{' struct_declaration_list '}'");
        $$ = getNode($1, mergeAttrs(getNode($2), $5));
        if (printStructTable("STRUCT_" + string($2)) == 1) {
            type = type.empty() ? "STRUCT_" + string($2) : type + " STRUCT_" + string($2);
        } else {
            yyerror(("Struct " + string($2) + " is already defined").c_str(), "scope error");
        }
    }
    | struct_or_union S '{' struct_declaration_list '}' {
        DEBUG_PARSER("struct_or_union_specifier -> struct_or_union S '{' struct_declaration_list '}'");
        $$ = getNode($1, mergeAttrs($4, nullptr));
        Anon_StructCounter++;
        if (printStructTable("STRUCT_" + to_string(Anon_StructCounter)) == 1) {
            type = type.empty() ? "STRUCT_" + to_string(Anon_StructCounter) : type + " STRUCT_" + to_string(Anon_StructCounter);
        } else {
            yyerror("Struct is already defined", "scope error");
        }
    }
    | struct_or_union IDENTIFIER {
        DEBUG_PARSER("struct_or_union_specifier -> struct_or_union IDENTIFIER");
        $$ = getNode($1, mergeAttrs(getNode($2), nullptr));
        currentDataType += " " + string($2);
        if (findStruct("STRUCT_" + string($2)) == 1) {
            type = type.empty() ? "STRUCT_" + string($2) : type + " STRUCT_" + string($2);
        } else if (structName == string($2)) {
            type = "#INSIDE";
        } else {
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
            $$ = getNode("struct_declaration_list", mergeAttrs($1));
        }
        | struct_declaration_list struct_declaration {
            DEBUG_PARSER("struct_declaration_list -> struct_declaration_list struct_declaration");
            $$ = getNode("struct_declaration_list", mergeAttrs($1, $2));
        }
        ;

    struct_declaration
        : specifier_qualifier_list struct_declarator_list ';' {
            DEBUG_PARSER("struct_declaration -> specifier_qualifier_list struct_declarator_list ';'");
            $$ = getNode("struct_declaration", mergeAttrs($1, $2));
            
            // Semantics
            type = "";
        }
        ;

    specifier_qualifier_list
        : type_specifier specifier_qualifier_list {
            DEBUG_PARSER("specifier_qualifier_list -> type_specifier specifier_qualifier_list");
            $$ = getNode("specifier_qualifier_list", mergeAttrs($1, $2));
        }
        | type_specifier {
            DEBUG_PARSER("specifier_qualifier_list -> type_specifier");
            $$ = $1;
        }
        | type_qualifier specifier_qualifier_list {
            DEBUG_PARSER("specifier_qualifier_list -> type_qualifier specifier_qualifier_list");
            $$ = getNode("specifier_qualifier_list", mergeAttrs($1, $2));
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
            $$ = getNode("struct_declarator_list", mergeAttrs($1, $3));
        }
        ;

    struct_declarator
        : declarator {
            DEBUG_PARSER("struct_declarator -> declarator");
            $$ = $1;
            
            // Semantics
            if (insertStructAttr($1->temp_name, $1->type, $1->size, 0) != 1) {
                yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str(), "scope error");
            }
        }
        | ':' constant_expression {
            DEBUG_PARSER("struct_declarator -> ':' constant_expression");
            $$ = $2;
        }
        | declarator ':' constant_expression {
            DEBUG_PARSER("struct_declarator -> declarator ':' constant_expression");
            $$ = getNode(":", mergeAttrs($1, $3));
            
            // Semantics
            if (insertStructAttr($1->temp_name, $1->type, $3->intVal, 0) != 1) {
                yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str(), "scope error");
            }
        }
        ;
// haha

enum_specifier
	: ENUM '{' enumerator_list '}' {
		DEBUG_PARSER("enum_specifier -> ENUM '{' enumerator_list '}'");
		$$ = getNode($1, mergeAttrs($3));
		// TODO: Add enum semantics
	}
	| ENUM IDENTIFIER '{' enumerator_list '}' {
		DEBUG_PARSER("enum_specifier -> ENUM IDENTIFIER '{' enumerator_list '}'");
		$$ = getNode($1, mergeAttrs(getNode($2), $4));
	}
	| ENUM IDENTIFIER {
		DEBUG_PARSER("enum_specifier -> ENUM IDENTIFIER");
		$$ = getNode($1, mergeAttrs(getNode($2)));
		currentDataType = "Enum " + std::string($2);
	}
	;

enumerator_list
	: enumerator {
		DEBUG_PARSER("enumerator_list -> enumerator");
		$$ = $1;
	}
	| enumerator_list ',' enumerator {
		DEBUG_PARSER("enumerator_list -> enumerator_list ',' enumerator");
		$$ = getNode("enumerator_list", mergeAttrs($1, $3));
	}
	;

enumerator
	: IDENTIFIER {
		DEBUG_PARSER("enumerator -> IDENTIFIER");
		$$ = getNode($1);
	}
	| IDENTIFIER '=' constant_expression {
		DEBUG_PARSER("enumerator -> IDENTIFIER '=' constant_expression");
		$$ = getNode("=", mergeAttrs(getNode($1), $3));
	}
	;

type_qualifier
	: CONST {
		DEBUG_PARSER("type_qualifier -> CONST");
		$$ = getNode($1);
		currentDataType = "const ";
		flag2 = 1;
	}
	| VOLATILE {
		DEBUG_PARSER("type_qualifier -> VOLATILE");
		$$ = getNode($1);
		currentDataType = "volatile ";
		flag2 = 1;
	}
	;

declarator
	: pointer direct_declarator {
		DEBUG_PARSER("declarator -> pointer direct_declarator");
		$$ = getNode("declarator", mergeAttrs($1, $2));
		// Semantics
		if (type == "#INSIDE") {
			$$->type = "STRUCT_" + structName + $1->type;
			$$->temp_name = $2->temp_name;
			$$->size = 8;
			$$->expType = 2;
		} else {
			$$->type = $2->type + $1->type;
			$$->temp_name = $2->temp_name;
			$$->size = 8;
			$$->expType = 2;
		}
	}
	| direct_declarator {
		DEBUG_PARSER("declarator -> direct_declarator");
		$$ = $1;
	}
	;

direct_declarator
	: IDENTIFIER {
		DEBUG_PARSER("direct_declarator -> IDENTIFIER");
		$$ = getNode($1);
		std::string check = std::string($1);
		if (flag) {
			typedefTable.push_back(make_pair(check, currentDataType));
			flag = 0;
		} else {
			addToSymbolTable(check, currentDataType);
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
		$$ = getNode("Invalid Identifier");
	}
	| '(' declarator ')' {
		DEBUG_PARSER("direct_declarator -> '(' declarator ')'");
		$$ = $2;  // Just pass through the declarator node
	}
	| direct_declarator '[' constant_expression ']' {
		DEBUG_PARSER("direct_declarator -> direct_declarator '[' constant_expression ']'");
		Node* node = getNode("[ ]", mergeAttrs($3));
		$$ = getNode("direct_declarator[..]", mergeAttrs($1, node));

		updateLastSymbolEntry();
		// Semantics
		if ($1->expType == 1 || $1->expType == 2) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = $1->size * $3->intVal;
		} else {
			yyerror(("Function " + $1->temp_name + " cannot be used as an array").c_str(), "type error");
		}
	}
	| direct_declarator '[' ']' {
		DEBUG_PARSER("direct_declarator -> direct_declarator '[' ']'");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, NULL, "[ ]", 0);
		$$ = getNode("direct_declarator[]", &attr);

		updateLastSymbolEntry();
		// Semantics
		if ($1->expType <= 2) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = 8;
		} else {
			yyerror(("Function " + $1->temp_name + " cannot be used as an array").c_str(), "type error");
		}
	}
	| direct_declarator '(' A parameter_type_list ')' {
		DEBUG_PARSER("direct_declarator -> direct_declarator '(' A parameter_type_list ')'");
		Node *node = getNode("( )", mergeAttrs($4));
		$$ = getNode("direct_declarator", mergeAttrs($1, node));

		updateFuncSymbolEntry(noArgs);
		noArgs = 0;
		// Semantics
		string argMapKey = $1->temp_name;
		if (!className.empty()) {
			argMapKey = className + "_" + argMapKey;
		}
		if ($1->expType == 1) {
			$$->temp_name = $1->temp_name;
			$$->expType = 3;
			$$->type = $1->type;
			$$->size = getSize($$->type);
			vector<string> temp = getFuncArgs(argMapKey);
			if (temp.size() == 1 && temp[0] == "#NO_FUNC") {
				insertFuncArg(argMapKey, funcArgs);
				funcArgs.clear();
				funcName = string(argMapKey);
				funcType = $1->type;
			} else {
				if (temp == funcArgs) {
					funcArgs.clear();
					funcName = string(argMapKey);
					funcType = $1->type;
				} else {
					yyerror(("Conflicting types for " + argMapKey).c_str(), "type error");
				}
			}
		} else {
			if ($1->expType == 2) {
				yyerror((argMapKey + " declared as array of function").c_str(), "type error");
			} else {
				yyerror((argMapKey + " declared as function of function").c_str(), "type error");
			}
		}
	}
	| direct_declarator '(' A identifier_list ')' {
		DEBUG_PARSER("direct_declarator -> direct_declarator '(' A identifier_list ')'");
		Node *node = getNode("( )", mergeAttrs($4));
		$$ = getNode("direct_declarator", mergeAttrs($1, node));

		// Semantics
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
				insertSymbol(*curr_table, idList[i], "int", 4, 1, NULL);
				args.push_back("int");
			}
			insertFuncArg($1->temp_name, args);
		}
		if (args.size() == idList.size()) {
			for (int i = 0; i < args.size(); i++) {
				if (args[i] == "...") {
					yyerror(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
					break;
				}
				insertSymbol(*curr_table, idList[i], args[i], getSize(args[i]), 1, NULL);
			}
			idList.clear();
		} else {
			yyerror(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
			idList.clear();
		}
	}
	| direct_declarator '(' A ')' {
		DEBUG_PARSER("direct_declarator -> direct_declarator '(' A ')'");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, NULL, "( )", 0);
		$$ = getNode("direct_declarator", &attr);
		updateFuncSymbolEntry(0);
		// Semantics
		if ($1->expType == 1) {
			$$->temp_name = $1->temp_name;
			$$->expType = 3;
			$$->type = $1->type;
			$$->size = getSize($$->type);
			vector<string> temp = getFuncArgs($1->temp_name);
			if (temp.size() == 1 && temp[0] == "#NO_FUNC") {
				insertFuncArg($$->temp_name, funcArgs);
				funcArgs.clear();
				funcName = string($1->temp_name);
				funcType = $1->type;
			} else {
				yyerror(("Conflicting types for function " + $1->temp_name).c_str(), "type error");
			}
		} else {
			if ($1->expType == 2) {
				yyerror(($1->temp_name + " declared as array of function").c_str(), "type error");
			} else {
				yyerror(($1->temp_name + " declared as function of function").c_str(), "type error");
			}
		}
	}
	;

A
	: %empty {
		DEBUG_PARSER("A -> %empty");
		type = "";
		func_flag = 0;
		funcArgs.clear();
		createParamList();
	}
	;

pointer
	: '*' {
		DEBUG_PARSER("pointer -> '*'");
		$$ = getNode("*(Pointer)");
		$$->type = "*";
		currentDataType += "*";
	}
	| '*' type_qualifier_list {
		DEBUG_PARSER("pointer -> '*' type_qualifier_list");
		$$ = getNode("*(Pointer)", mergeAttrs($2));
		$$->type = "*";
		currentDataType += "*";
	}
	| '*' pointer {
		DEBUG_PARSER("pointer -> '*' pointer");
		$$ = getNode("*(Pointer)", mergeAttrs($2));
		$$->type = "*" + $2->type;
		currentDataType += "*";
	}
	| '*' type_qualifier_list pointer {
		DEBUG_PARSER("pointer -> '*' type_qualifier_list pointer");
		$$ = getNode("*(Pointer)", mergeAttrs($2, $3));
		$$->type = "*" + $3->type;
		currentDataType += "*";
	}
	;

type_qualifier_list
	: type_qualifier {
		DEBUG_PARSER("type_qualifier_list -> type_qualifier");
		$$ = $1;
	}
	| type_qualifier_list type_qualifier {
		DEBUG_PARSER("type_qualifier_list -> type_qualifier_list type_qualifier");
		$$ = getNode("type_qualifier_list", mergeAttrs($1, $2));
	}
	;

parameter_type_list
	: parameter_list {
		DEBUG_PARSER("parameter_type_list -> parameter_list");
		$$ = $1;
	}
	| parameter_list ',' ELLIPSIS {
		DEBUG_PARSER("parameter_type_list -> parameter_list ',' ELLIPSIS");
		$$ = getNode("parameter_type_list", mergeAttrs($1, getNode($3)));
		// Semantics - add ellipsis to function argument list
		funcArgs.push_back("...");
	}
	;

parameter_list
	: parameter_declaration {
		DEBUG_PARSER("parameter_list -> parameter_declaration");
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration {
		DEBUG_PARSER("parameter_list -> parameter_list ',' parameter_declaration");
		$$ = getNode("parameter_list", mergeAttrs($1, $3));
		noArgs++;
	}
	;

parameter_declaration
	: declaration_specifiers declarator {
		DEBUG_PARSER("parameter_declaration -> declaration_specifiers declarator");
		$$ = getNode("parameter_declaration", mergeAttrs($1, $2));
		// Semantics
		type = "";
		if ($2->expType == 1 || $2->expType == 2) {
			if (currLookup($2->temp_name)) {
				yyerror(("Redeclaration of Parameter " + $2->temp_name).c_str(), "scope error");
			} else {
				insertSymbol(*curr_table, $2->temp_name, $2->type, $2->size, true, NULL);
			}
			funcArgs.push_back($2->type);
		}
	}
	| declaration_specifiers abstract_declarator {
		DEBUG_PARSER("parameter_declaration -> declaration_specifiers abstract_declarator");
		$$ = getNode("parameter_declaration", mergeAttrs($1, $2));
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
//!
identifier_list
	: IDENTIFIER {
		DEBUG_PARSER("identifier_list -> IDENTIFIER");
		$$ = getNode($1);
		idList.push_back($1); // Semantics
	}
	| identifier_list ',' IDENTIFIER {
		DEBUG_PARSER("identifier_list -> identifier_list ',' IDENTIFIER");
		$$ = getNode("identifier_list", mergeAttrs($1, getNode($3)));
		idList.push_back($3); // Semantics
	}
	;

type_name
	: specifier_qualifier_list {
		DEBUG_PARSER("type_name -> specifier_qualifier_list");
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator {
		DEBUG_PARSER("type_name -> specifier_qualifier_list abstract_declarator");
		$$ = getNode("type_name", mergeAttrs($1, $2));
	}
	;

abstract_declarator
	: pointer {
		DEBUG_PARSER("abstract_declarator -> pointer");
		$$ = $1;
	}
	| direct_abstract_declarator {
		DEBUG_PARSER("abstract_declarator -> direct_abstract_declarator");
		$$ = $1;
	}
	| pointer direct_abstract_declarator {
		DEBUG_PARSER("abstract_declarator -> pointer direct_abstract_declarator");
		$$ = getNode("abstract_declarator", mergeAttrs($1, $2));
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		DEBUG_PARSER("direct_abstract_declarator -> '(' abstract_declarator ')'");
		$$ = $2;
	}
	| '[' ']' {
		DEBUG_PARSER("direct_abstract_declarator -> '[' ']'");
		$$ = getNode("[ ]");
		$$->type = type + "*"; // Semantics
		$$->size = 8;
	}
	| '[' constant_expression ']' {
		DEBUG_PARSER("direct_abstract_declarator -> '[' constant_expression ']'");
		$$ = getNode("[ ]", mergeAttrs($2));
		$$->type = type + "*"; // Semantics
		$$->size = getSize(type) * $2->intVal;
	}
	| direct_abstract_declarator '[' ']' {
		DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '[' ']'");
		std::vector<Data> attr;
		insertAttr(attr,NULL,"[ ]",0);
		insertAttr(attr,$1,"",1);
		$$ = getNode("direct_abstract_declarator",&attr);

		$$->type = $1->type + "*"; // Semantics
		$$->size = 8;
	}
	| direct_abstract_declarator '[' constant_expression ']' {
		DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '[' constant_expression ']'");
		$$ = getNode("direct_abstract_declarator", mergeAttrs($1, getNode("[ ]", mergeAttrs($3))));
		$$->type = $1->type + "*"; // Semantics
		$$->size = $1->size * $3->intVal;
	}
	| '(' ')' {
		DEBUG_PARSER("direct_abstract_declarator -> '(' ')'");
		$$ = getNode("( )");
		$$->type = "FUNC_" + type; // Semantics
		$$->size = 0;
	}
	| '(' parameter_type_list ')' {
		DEBUG_PARSER("direct_abstract_declarator -> '(' parameter_type_list ')'");
		$$ = $2;
		$$->type = "FUNC_" + type; // Semantics
		$$->size = 0;
	}
	| direct_abstract_declarator '(' ')' {
		DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '(' ')'");
		std::vector<Data> attr;
		insertAttr(attr, NULL, "( )", 0);
		insertAttr(attr, $1, "", 1);
		$$ = getNode("direct_abstract_declarator",&attr);		

		$$->type = "FUNC_" + $1->type; // Semantics
		$$->size = 0;
	}
	| direct_abstract_declarator '(' parameter_type_list ')' {
		DEBUG_PARSER("direct_abstract_declarator -> direct_abstract_declarator '(' parameter_type_list ')'");
		$$ = getNode("direct_abstract_declarator", mergeAttrs($1, getNode("( )", mergeAttrs($3))));
		$$->type = "FUNC_" + $1->type; // Semantics
		$$->size = 0;
	}
	;

initializer
	: assignment_expression {
		DEBUG_PARSER("initializer -> assignment_expression");
		$$ = $1;
	}
	| '{' initializer_list '}' {
		DEBUG_PARSER("initializer -> '{' initializer_list '}'");
		$$ = $2;
		$$->isInit = 1; // Semantics
	}
	| '{' initializer_list ',' '}' {
		DEBUG_PARSER("initializer -> '{' initializer_list ',' '}'");
		$$ = $2;
		$$->isInit = 1; // Semantics
	}
	;

initializer_list
	: initializer {
		DEBUG_PARSER("initializer_list -> initializer");
		$$ = $1;
	}
	| initializer_list ',' initializer {
		DEBUG_PARSER("initializer_list -> initializer_list ',' initializer");
		$$ = getNode("initializer_list", mergeAttrs($1, $3));
		$$->isInit = ($1->isInit && $3->isInit); // Semantics
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
	: IDENTIFIER ':' statement {
		DEBUG_PARSER("labeled_statement -> IDENTIFIER ':' statement");
		$$ = getNode("labeled_statement", mergeAttrs(getNode($1), $3));
	}
	| CASE constant_expression ':' statement {
		DEBUG_PARSER("labeled_statement -> CASE constant_expression ':' statement");
		$$ = getNode("case", mergeAttrs($2, $4));
	}
	| DEFAULT ':' statement {
		DEBUG_PARSER("labeled_statement -> DEFAULT ':' statement");
		std::vector<Data> attr;
        insertAttr(attr, NULL, "default", 0);
        insertAttr(attr, $3, "", 1);
        $$ = getNode("case", &attr);		
	}
	;

compound_statement
	: '{' '}' {
		DEBUG_PARSER("compound_statement -> '{' '}'");
		$$ = getNode("{ }");
	}
	| '{' CHANGE_TABLE statement_list '}' {
		DEBUG_PARSER("compound_statement -> '{' CHANGE_TABLE statement_list '}'");
		$$ = $3;
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
	| '{' CHANGE_TABLE declaration_list '}' {
		DEBUG_PARSER("compound_statement -> '{' CHANGE_TABLE declaration_list '}'");
		$$ = $3;
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
	| '{' CHANGE_TABLE declaration_list statement_list '}' {
		DEBUG_PARSER("compound_statement -> '{' CHANGE_TABLE declaration_list statement_list '}'");
		$$ = getNode("compound_statement", mergeAttrs($3, $4));
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
	| declaration_list declaration {
		DEBUG_PARSER("declaration_list -> declaration_list declaration");
		$$ = getNode("declaration_list", mergeAttrs($1, $2));
	}
	;

statement_list
	: statement {
		DEBUG_PARSER("statement_list -> statement");
		$$ = $1;
	}
	| statement_list statement {
		DEBUG_PARSER("statement_list -> statement_list statement");
		$$ = getNode("statement_list", mergeAttrs($1, $2));
	}
	;

expression_statement
	: ';' {
		DEBUG_PARSER("expression_statement -> ';'");
		$$ = getNode(";");
	}
	| expression ';' {
		DEBUG_PARSER("expression_statement -> expression ';'");
		$$ = $1;
	}
	;

selection_statement
	: IF '(' expression ')' statement {
		DEBUG_PARSER("selection_statement -> IF '(' expression ')' statement");
		$$ = getNode("if", mergeAttrs($3, $5));
	}
	| IF '(' expression ')' statement ELSE statement {
		DEBUG_PARSER("selection_statement -> IF '(' expression ')' statement ELSE statement");
		$$ = getNode("if-else", mergeAttrs($3, $5, $7));
	}
	| SWITCH '(' expression ')' statement {
		DEBUG_PARSER("selection_statement -> SWITCH '(' expression ')' statement");
		$$ = getNode("switch", mergeAttrs($3, $5));
	}
	| IF '(' ')' statement {
		DEBUG_PARSER("selection_statement -> IF '(' ')' statement");
		yyerror("missing condition in 'if' statement.", "syntax error");
		$$ = getNode("error-node", nullptr);
	}
	| SWITCH '(' ')' statement {
		DEBUG_PARSER("selection_statement -> SWITCH '(' ')' statement");
		yyerror("missing condition in 'switch' statement.", "syntax error");
		$$ = getNode("error-node", nullptr);
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement {
		DEBUG_PARSER("iteration_statement -> WHILE '(' expression ')' statement");
		$$ = getNode("while-loop", mergeAttrs($3, $5));
	}
	| DO statement WHILE '(' expression ')' ';' {
		DEBUG_PARSER("iteration_statement -> DO statement WHILE '(' expression ')' ';'");
		$$ = getNode("do-while-loop", mergeAttrs($2, $5));
	}
	| FOR '(' expression_statement expression_statement ')' statement {
		DEBUG_PARSER("iteration_statement -> FOR '(' expression_statement expression_statement ')' statement");
		$$ = getNode("for-loop(w/o update stmt)", mergeAttrs($3, $4, $6));
	}
	| FOR '(' expression_statement expression_statement expression ')' statement {
		DEBUG_PARSER("iteration_statement -> FOR '(' expression_statement expression_statement expression ')' statement");
		$$ = getNode("for-loop", mergeAttrs($3, $4, $5, $7));
	}
	| UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
		DEBUG_PARSER("iteration_statement -> UNTIL '(' expression ')' statement");
		$$ = getNode("until-loop", mergeAttrs($3, $5));
	}
	| WHILE '[' expression ']' statement {
		DEBUG_PARSER("iteration_statement -> WHILE '[' expression ']' statement");
		yyerror("incorrect parentheses in while-loop.", "syntax error");
		$$ = getNode("Invalid While-loop", nullptr);
	}
	| UNTIL '[' expression ']' statement {
		DEBUG_PARSER("iteration_statement -> UNTIL '[' expression ']' statement");
		yyerror("incorrect parentheses in until-loop.", "syntax error");
		$$ = getNode("Invalid Until-loop", nullptr);
	}
	| FOR '(' expression ',' expression ',' expression ')' statement {
		DEBUG_PARSER("iteration_statement -> FOR '(' expression ',' expression ',' expression ')' statement");
		yyerror("comma used instead of semicolons.", "syntax error");
		$$ = getNode("Invalid for-loop", nullptr);
	}
	;

jump_statement
	: GOTO IDENTIFIER ';' {
		DEBUG_PARSER("jump_statement -> GOTO IDENTIFIER ';'");
		$$ = getNode(std::string($1) + " : " + std::string($2));
	}
	| CONTINUE ';' {
		DEBUG_PARSER("jump_statement -> CONTINUE ';'");
		$$ = getNode($1);
	}
	| BREAK ';' {
		DEBUG_PARSER("jump_statement -> BREAK ';'");
		$$ = getNode($1);
	}
	| RETURN ';' {
		DEBUG_PARSER("jump_statement -> RETURN ';'");
		$$ = getNode($1);
	}
	| RETURN expression ';' {
		DEBUG_PARSER("jump_statement -> RETURN expression ';'");
		$$ = getNode("jump_stmt", mergeAttrs(getNode($1), $2));
	}
	;

translation_unit
	: external_declaration {
		DEBUG_PARSER("translation_unit -> external_declaration");
		$$ = $1;
	}
	| translation_unit external_declaration {
		DEBUG_PARSER("translation_unit -> translation_unit external_declaration");
		$$ = getNode("program", mergeAttrs($1, $2));
	}
	| error ';' {
		DEBUG_PARSER("translation_unit -> error ';'");
		$$ = getNode("error-node", nullptr); yyerrok;
	}
	| error ',' {
		DEBUG_PARSER("translation_unit -> error ','");
		$$ = getNode("error-node", nullptr); yyerrok;
	}
	| error {
		DEBUG_PARSER("translation_unit -> error");
		$$ = getNode("error-node", nullptr); yyerrok;
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
	: declaration_specifiers declarator F declaration_list compound_statement {
		DEBUG_PARSER("function_definition -> declaration_specifiers declarator F declaration_list compound_statement");
		$$ = getNode("function", mergeAttrs($1, $2, $4, $5));
		// Semantics
		// Extract and propagate function name and return type
        $$->temp_name = $2->temp_name;  // Function name from declarator
        $$->type = funcType;            // Return type from declaration_specifiers
        $$->size = $2->size;            // Size if applicable

		type = "";
		string fName = string($3);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declaration_specifiers declarator F compound_statement {
		DEBUG_PARSER("function_definition -> declaration_specifiers declarator F compound_statement");
		$$ = getNode("function (w/o decl_list)", mergeAttrs($1, $2, $4));
		// Semantics 
		// Extract and propagate function name and return type
        $$->temp_name = $2->temp_name;  // Function name from declarator
        $$->type = funcType;            // Return type from declaration_specifiers
        $$->size = $2->size;            // Size if applicable

		type = "";
		string fName = string($3);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declarator F declaration_list compound_statement {
		DEBUG_PARSER("function_definition -> declarator F declaration_list compound_statement");
		$$ = getNode("function (w/o decl_specifiers)", mergeAttrs($1, $3, $4));
		// Semantics
		type = "";
		string fName = string($2);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declarator F compound_statement {
		DEBUG_PARSER("function_definition -> declarator F compound_statement");
		$$ = getNode("function (w/o specifiers and decl_list)", mergeAttrs($1, $3));
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
		if (!className.empty() && funcName.find(className + "_") != 0) {
			qualifiedFuncName = className + "_" + funcName;
		}
		if (gst.find(qualifiedFuncName) != gst.end()) {
			yyerror(("Redefinition of function " + qualifiedFuncName).c_str(), "scope error");
		} else {
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
