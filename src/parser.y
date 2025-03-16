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
bool flag=0,flag2=0;;

extern char* yytext;
extern int column;
extern int line;
extern std::string inputFilename;
extern bool has_error;

//Semantics
string funcName = "";
string structName = "";
string funcType = "";
int block_count = 0;
stack<int> block_stack;
bool fn_decl = 0;
int func_flag = 0;

string type = "";
int Anon_StructCounter=0;
vector<string> funcArgs;
vector<string> idList;
vector<string> currArgs;

int yyerror(const char*);
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
%type<str> F G
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
    	$$ = createASTNode($1);
		
		// Semantics
		string temp = primaryExpression(string($1));
		if(temp == ""){
			yyerror(("Undeclared Identifier " + string($1)).c_str());
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
		}
    }
	| CONSTANT IDENTIFIER {
		yyerror("syntax error, invalid identifier");
		$$ = createASTNode($1->str);
	}
	| CONSTANT CONSTANT {
		yyerror("syntax error, invalid constant");
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
		std::string check=std::string($1);
		addToConstantTable(check,"String Literal");
		$$ = createASTNode($1);
		
		// Semantics for string literals
		$$->type = string("char*");
		$$->temp_name = string($1);
		$$->strVal = string($1);
	}
	| '(' expression ')' {
		$$ = $2;
	}
	;

postfix_expression
	: primary_expression {
		$$ = $1;
	}
	| postfix_expression '[' expression ']' {
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
			yyerror(("Array " + $1->temp_name +  " Index out of bound").c_str());
		}
	}
	| postfix_expression '(' ')' {
		$$ = $1;
		
		// Semantics
		$$->isInit = 1;
		string temp = postfixExpression($1->type, 2);
		if(!temp.empty()){	
			$$->type = temp;
			if($1->expType == 3){
				vector<string> funcArg = getFuncArgs($1->temp_name);
				if(!funcArg.empty()){
					yyerror(("Too few Arguments to Function " + $1->temp_name).c_str());
				}
			}
		}
		else{
			yyerror(("Function " + $1->temp_name + " not declared in this scope").c_str());
		}
		currArgs.clear(); 
	}
	| postfix_expression '(' argument_expression_list ')' {
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
						yyerror(("Too few Arguments to Function " + $1->temp_name).c_str());
						break;
					}
					string msg = checkType(funcArgs[i],currArgs[i]);
					
					if(msg =="warning"){
						yyerror(("Incompatible conversion of " +  currArgs[i] + " to parameter of type " + funcArgs[i]).c_str());
					}
					else if(msg.empty()){
						yyerror(("Incompatible Argument to the function " + $1->temp_name).c_str());
						break;
					}
					if(i==funcArgs.size()-1 && i<currArgs.size()-1){
						yyerror(("Too many Arguments to Function " + $1->temp_name).c_str());
						break;
					}
				}
			}
		}
		else{
			yyerror("Invalid function call");
		}
		currArgs.clear();
	}
	| postfix_expression '.' IDENTIFIER {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode("expression.id", &attr);
		
		// Semantics
		string temp = string($3);
		int ret = lookupStruct($1->type, temp);
		if(ret == -1){
			yyerror(("Struct " + $1->node_name + " not defined").c_str());
		}
		else if (ret == 0){
			yyerror("Attribute of Struct not defined");
		}
		else{
			$$->type = StructAttrType($1->type, temp);
			$$->temp_name = $1->temp_name + "." + temp;
		}
	}
	| postfix_expression PTR_OP IDENTIFIER {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		string temp = string($3);
		string temp1 = ($1->type);
		if(temp1.back() != '*'){
			yyerror(($1->node_name + " is not a pointer, did you mean to use '.' ").c_str());
		}
		else temp1.pop_back();
		
		int ret = lookupStruct(temp1, temp);
		if(ret == -1){
			yyerror("Struct not defined");
		}
		else if (ret == 0){
			yyerror("Attribute of Struct not defined");
		}
		else{
			$$->type = StructAttrType(temp1, temp);
			$$->temp_name = $1->temp_name + "->" + temp;
		}
	}
	| postfix_expression INC_OP {
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
			yyerror("Increment not defined for this type");
		}
	}
	| postfix_expression DEC_OP {
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
			yyerror("Decrement not defined for this type");
		}
	}
	;

argument_expression_list
	: assignment_expression {
		$$ = $1;
		
		// Semantic
		$$->isInit = $1->isInit;
		currArgs.push_back($1->type);
		$$->type = "void";
	}
	| argument_expression_list ',' assignment_expression {
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
		$$ = $1;
	}
	| INC_OP unary_expression {
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
			yyerror("Increment not defined for this type");
		}
	}
	| DEC_OP unary_expression {
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
			yyerror("Decrement not defined for this type");
		}
	}
	| unary_operator cast_expression {
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
			yyerror("Type inconsistent with operator");
		}
	}
	| SIZEOF unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
		
		// Semantic
		$$->type = "int";
		$$->isInit = 1;
		$$->intVal = $2->size;
	}
	| SIZEOF '(' type_name ')' {
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
		$$ = createASTNode("&");
	}
	| '*' {
		$$ = createASTNode("*");
	}
	| '+' {
		$$ = createASTNode("+");
	}
	| '-' {
		$$ = createASTNode("-");
	}
	| '~' {
		$$ = createASTNode("~");
	}
	| '!' {
		$$ = createASTNode("!");
	}
	;

cast_expression
	: unary_expression {
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
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
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
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
			yyerror("Incompatible type for * operator");
		}
	}
	| multiplicative_expression '/' cast_expression {
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
			yyerror("Incompatible type for / operator");
		}
	}
	| multiplicative_expression '%' cast_expression {
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
			yyerror("Incompatible type for % operator");
		}
	}
	;

additive_expression
	: multiplicative_expression {
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression {
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
			yyerror("Incompatible type for + operator");
		}
	}
	| additive_expression '-' multiplicative_expression {
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
			yyerror("Incompatible type for - operator");
		}
	}
	;

shift_expression
	: additive_expression {
		$$ = $1;
	}
	| shift_expression LEFT_OP additive_expression {
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
			yyerror("Invalid operands to binary <<");
		}
	}
	| shift_expression RIGHT_OP additive_expression {
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
			yyerror("Invalid operands to binary >>");
		}
	}
	; 

relational_expression   //POTENTIAL ISSUE
    : inclusive_or_expression {
          $$ = $1;
      }
    | relational_expression '<' inclusive_or_expression {
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
              yyerror("Invalid operands to binary <");
          }
      }
    | relational_expression '>' inclusive_or_expression {
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
              yyerror("Invalid operands to binary >");
          }
      }
    | relational_expression LE_OP inclusive_or_expression {
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
              yyerror("Invalid operands to binary <=");
          }
      }
    | relational_expression GE_OP inclusive_or_expression {
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
              yyerror("Invalid operands to binary >=");
          }
      }
    ;


equality_expression
	: relational_expression { 
		$$ = $1;
	}
	| equality_expression EQ_OP relational_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = eqExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				yyerror("Comparison between pointer and integer");
			}
			$$->type = "bool";
		}
		else{
			yyerror("Invalid operands to binary ==");
		}
	}
	| equality_expression NE_OP relational_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
		
		// Semantics
		if($1->isInit == 1 && $3->isInit == 1) $$->isInit = 1;
		string temp = eqExp($1->type, $3->type);
		if(!temp.empty()){
			if(temp == "ok"){
				yyerror("Comparison between pointer and integer");
			}
			$$->type = "bool";
		}
		else{
			yyerror("Invalid operands to binary !=");
		}
	}
	;

and_expression
	: shift_expression {	
		$$ = $1; 
	}
	| and_expression '&' shift_expression{
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
			yyerror("Invalid operands to binary &");
		}
	}
	;

exclusive_or_expression
	: and_expression {
		$$ = $1;
	}
	| exclusive_or_expression '^' and_expression{
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
			yyerror("Invalid operands to binary ^");
		}
    }
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression{
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
			yyerror("Invalid operands to binary |");
		}
	}
	;

logical_and_expression
	: equality_expression {$$ = $1;}
	| logical_and_expression AND_OP equality_expression{
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
	: logical_and_expression{$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression{
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
	: logical_or_expression {$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression{
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
			yyerror("Type mismatch in Conditional Expression");
		}
		if($1->isInit == 1 && $3->isInit == 1 && $5->isInit == 1) $$->isInit = 1;
	}
	;

assignment_expression
	: conditional_expression{$$ = $1;}
	| unary_expression assignment_operator assignment_expression{
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
				yyerror("Assignment with incompatible pointer type");
			}
		}
		else{
			yyerror("Incompatible types when assigning type");
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
	: assignment_expression{$$ = $1;}
	| expression ',' assignment_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("expression", &attr);
		//Semantic
		$$->type = string("void");
	}
	;

constant_expression
	: conditional_expression{$$ = $1;}
	;

declaration //POTENTIAL ISSUE
    : declaration_specifiers ';' { 
          $$ = $1; 
      }
    | declaration_specifiers init_declarator_list ';' {
          std::vector<Data> attr;
          insertAttr(attr, $1, "", 1);
          insertAttr(attr, $2, "", 1);
          $$ = createASTNode("declaration", &attr);
          type = "";
          // Check if this is a function declaration
          if($2->expType == 3) {
              // Remove the temporary function prototype from the symbol table.
              removeFuncProto();  // Assumes a function exists to clear the dummy function entry.
              
              // Retrieve any previously stored argument list for this function name.
              std::vector<std::string> prevArgs = getFuncArgs($2->temp_name);
              
              // If the function already has an entry in the FuncArgs map, validate the parameter types.
              if(!prevArgs.empty() && prevArgs[0] != "#NO_FUNC") {
                  if(prevArgs.size() != funcArgs.size()) {
                      yyerror(("Function " + std::string($2->temp_name) + 
                               " declared with a different number of arguments").c_str());
                  } else {
                      for (size_t i = 0; i < prevArgs.size(); ++i) {
                          if(prevArgs[i] != funcArgs[i]) {
                              yyerror(("Argument type mismatch in function " + 
                                       std::string($2->temp_name)).c_str());
                              break;
                          }
                      }
                  }
              } else {
                  // This is the first declaration: insert the function's argument list.
                  insertFuncArg($2->temp_name, funcArgs);
              }
              // Clear the global argument list for the next function declaration.
              funcArgs.clear();
          }
      }
    ;


declaration_specifiers
	: storage_class_specifier { $$ = $1; }
	| storage_class_specifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_specifier{ $$ = $1; }
	| type_specifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_qualifier{ $$ = $1; }
	| type_qualifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	;

init_declarator_list
	: init_declarator{$$ = $1;}
	| init_declarator_list ',' init_declarator{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("init_declarator_list", &attr);
	}
	;

init_declarator
	: declarator {
		$$ = $1;
		
		// Semantics
		if(currLookup($1->temp_name)){
			string errstr = $1->temp_name + " is already declared";
			yyerror(errstr.c_str());
		}
		else if($1->expType == 3){
			if(fn_decl){
				yyerror("A parameter list without types is only allowed in a function definition");
				fn_decl = 0;
			}
			removeFuncProto();
		}
		else{
			insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 0, NULL);
		}
	}
	| declarator '=' initializer{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
		
		// Semantics
		if(currLookup($1->temp_name)){
			string errstr = $1->temp_name + " is already declared";
			yyerror(errstr.c_str());
		}
		else{
			insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 1, NULL);
		}
	}
	;

storage_class_specifier
	: TYPEDEF	{ $$ = createASTNode($1); flag=1;}
	| EXTERN	{ $$ = createASTNode($1); currentDataType="extern "; flag2=1;}
	| STATIC	{ $$ = createASTNode($1); currentDataType="static "; flag2=1;}
	| AUTO		{ $$ = createASTNode($1); currentDataType="auto "; flag2=1;}
	| REGISTER	{ $$ = createASTNode($1); currentDataType="register "; flag2=1;}
	;

type_specifier
	: VOID			{
		$$ = createASTNode($1); 
		currentDataType="void";
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| CHAR			{
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" char";flag2=0;}else{currentDataType="char";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| SHORT			{
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" short";flag2=0;}else{currentDataType="short";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| INT			{
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" int";flag2=0;}else{currentDataType="int";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| LONG			{
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" long";flag2=0;}else{currentDataType="long";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| FLOAT			{
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" float";flag2=0;}else{currentDataType="float";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| DOUBLE		{
		$$ = createASTNode($1); 
		if(flag2){currentDataType+=" double";flag2=0;}else{currentDataType="double";}
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| SIGNED		{
		$$ = createASTNode($1); 
		currentDataType="signed"; 
		flag2=1;
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| UNSIGNED		{
		$$ = createASTNode($1); 
		currentDataType="unsigned"; 
		flag2=1;
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| FILE_MAN      { 
		$$ = createASTNode($1); 
		currentDataType="file";
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| struct_or_union_specifier	{$$ = $1;}	
	| class_definition 			{$$ = $1;}
	| enum_specifier			{$$ = $1;}
	| TYPE_NAME		{
		$$ = createASTNode($1);
		
		// Semantics
		string temp = getType($1);
		type = temp;
	}	
	;

inheritance_specifier
	: access_specifier IDENTIFIER {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        /* Wrap IDENTIFIER into an AST node */
        insertAttr(attr, createASTNode($2), "", 1);
        $$ = createASTNode("inheritance_specifier", &attr);
    }
	;

inheritance_specifier_list
	: inheritance_specifier { $$ = $1; }
	| inheritance_specifier_list ',' inheritance_specifier{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("inheritance_specifier_list", &attr);
    }
	;

access_specifier 
	: PRIVATE { $$ = createASTNode($1); }
	| PUBLIC { $$ = createASTNode($1); }
	| PROTECTED { $$ = createASTNode($1); }
	;

class
	: CLASS { $$ = createASTNode($1); }
	;

class_definition_head 
	: class INHERITANCE_OP inheritance_specifier_list {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_definition_head", &attr);
        // Semantics: For inherited classes without an explicit name, mark as anonymous.
        $$->type = currentDataType; 
        $$->temp_name = "AnonymousClass";  
    }
	| class IDENTIFIER {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($2), "", 1);
        currentDataType = "Class " + std::string($2);
        $$ = createASTNode("class_definition_head", &attr);
        // Semantics: Save the class name for later symbol table insertion.
        $$->temp_name = std::string($2);
        $$->type = currentDataType;
    }
	| class IDENTIFIER INHERITANCE_OP inheritance_specifier_list {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($2), "", 1);
        insertAttr(attr, $4, "", 1);
        currentDataType = "Class " + std::string($2);
        $$ = createASTNode("class_definition_head", &attr);
        $$->temp_name = std::string($2);
        $$->type = currentDataType;
    }
	;

class_definition 
	: class_definition_head '{' class_internal_definition_list '}' {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_definition", &attr);
        // Semantics: Insert the class into the symbol table after ensuring no duplicate exists.
        if (currLookup($1->temp_name)) {
            yyerror(("Class " + std::string($1->temp_name) + " is already declared").c_str());
        } else {
            insertSymbol(*curr_table, $1->temp_name, $1->type, 0, 0, NULL);
        }
    }
	| class_definition_head { $$ = $1; }
	;

class_internal_definition_list
	: class_internal_definition { $$ = $1; }
	| class_internal_definition_list class_internal_definition {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("class_internal_definition_list", &attr);
    }
	; 

class_internal_definition	
	: access_specifier '{' class_member_list '}' ';' {
		currentAccess = $1->strVal;
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_internal_definition", &attr);
        // Semantics: Propagate the access specifier from $1 to every member in $3.
       
    }
	;

class_member_list
	: class_member { $$ = $1; }
	| class_member_list class_member {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("class_member_list", &attr);
    }
	;

class_member
	: function_definition { 
		 $1->strVal = currentAccess;
		 $$ = $1; }
	| declaration { 
		$1->strVal = currentAccess;
		$$ = $1; }
	;

struct_or_union_specifier
	: struct_or_union G S '{' struct_declaration_list '}'	{
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
		else {
			yyerror(("Struct " + string($2) + " is already defined").c_str());
		}
	}
	| struct_or_union S '{' struct_declaration_list '}'		{
		std::vector<Data> v;
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
		
		// Semantics
		Anon_StructCounter++;
		if(printStructTable("STRUCT_" + to_string(Anon_StructCounter)) == 1){
			if(type == "") type = "STRUCT_" + to_string(Anon_StructCounter);
			else type += " STRUCT_" + to_string(Anon_StructCounter);
		}
		else {
			yyerror("Struct is already defined");
		}
	}
	| struct_or_union IDENTIFIER 	{
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
		else {
			yyerror(("Struct " + string($2) + " is not defined").c_str());
		}
	}
	| struct_or_union IDENTIFIER '{' '}'	{
		yyerror("syntax error, struct must be non-empty");
		$$ = createASTNode("Invalid Struct", nullptr);
	}
	;

G 
	: IDENTIFIER {
		$$ = $1;
		structName = $1;
	}
	;

S 
	: %empty {
		createStructTable();
	}
	;

struct_or_union
	: STRUCT	{$$ = $1; currentDataType="struct";}
	| UNION		{$$ = $1; currentDataType="union";}
	;

struct_declaration_list
	: struct_declaration	{ $$ = $1 ;}
	| struct_declaration_list struct_declaration 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration_list", &v);
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
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
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_specifier	{ $$ = $1; }
	| type_qualifier specifier_qualifier_list 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_qualifier	{ $$ = $1; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator {
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("struct_declarator_list", &v);
	}
	;

struct_declarator
	: declarator	{ 
		$$ = $1; 
		
		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $1->size, 0) != 1){
			yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str());
		}
	}
	| ':' constant_expression	{ $$ = $2; }
	| declarator ':' constant_expression	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode(":", &v);
		
		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $3->intVal, 0) != 1){
			yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str());
		}
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createASTNode($1, &v);
		
		// TODO: Add enum semantics
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
	}
	| ENUM IDENTIFIER {
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		currentDataType="Enum ";
		std::string check=std::string($2);
		currentDataType+=check;
		$$ = createASTNode($1, &v);
	}
	;

enumerator_list
	: enumerator 	{ $$ = $1; }
	| enumerator_list ',' enumerator 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("enumerator_list", &v);
	}
	;

enumerator
	: IDENTIFIER	{ $$ = createASTNode($1); }
	| IDENTIFIER '=' constant_expression 	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
	}
	;

type_qualifier
	: CONST		{ 
		$$ = createASTNode($1); 
		currentDataType="const "; 
		flag2=1;
	}
	| VOLATILE	{ 
		$$ = createASTNode($1); 
		currentDataType="volatile "; 
		flag2=1;
	}
	;


declarator
	: pointer direct_declarator{
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
		$$ = $1 ;
	}
	;


direct_declarator
	: IDENTIFIER {
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
		yyerror("syntax error, invalid identifier");
		$$ = createASTNode("Invalid Identifier");
	}
	| '(' declarator ')'  {
		$$ = $2 ;
	}
	| direct_declarator '[' constant_expression ']'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		updateLastSymbolEntry();
	}
	| direct_declarator '[' ']'{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		updateLastSymbolEntry();
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' parameter_type_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		updateFuncSymbolEntry(noArgs);
		noArgs=0;
	}
	| direct_declarator '(' identifier_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' ')'{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "( )", 0);
		$$ = createASTNode("direct_declarator", &v);
		updateFuncSymbolEntry(0);
	}
	;

pointer
	: '*' {
		currentDataType+="*";
		$$ = createASTNode("*(Pointer)");
		$$->type = "*";
	}
	| '*' type_qualifier_list{
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
		$$->type = "*";
	}
	| '*' pointer{
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
		$$->type = "*" + $2->type;
	}
	| '*' type_qualifier_list pointer{
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
		$$ = $1 ;
	}
	| type_qualifier_list type_qualifier{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_qualifier_list",&v);
	}
	;


parameter_type_list
	: parameter_list {
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
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
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		noArgs++;
		$$ = createASTNode("parameter_list",&v);
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
		
		// Semantics
		type = "";
		if($2->expType == 1 || $2->expType == 2) {
			if(currLookup($2->temp_name)) {
				yyerror(("Redeclaration of Parameter " + $2->temp_name).c_str());
			}
			else {
				insertSymbol(*curr_table, $2->temp_name, $2->type, $2->size, true, NULL);
			}
			funcArgs.push_back($2->type);
		}
	}
	| declaration_specifiers abstract_declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
		
		// Semantics
		type = "";
	}
	| declaration_specifiers {
		$$ = $1;
		
		// Semantics
		funcArgs.push_back(type);
		type = "";
	}
	;

identifier_list
	: IDENTIFIER {
		$$ = createASTNode($1);
		
		// Semantics - add identifier to list for old-style function declarations
		idList.push_back($1);
	}
	| identifier_list ',' IDENTIFIER{
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
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_name",&v);
	}
	;

abstract_declarator
	: pointer {
		$$ = $1;
	}
	| direct_abstract_declarator{
		$$ = $1;
	}
	| pointer direct_abstract_declarator{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("abstract_declarator",&v);
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		$$ = $2;
	}
	| '[' ']'{
		$$ = createASTNode("[ ]");
		
		// Semantics
		$$->type = type + "*"; // Array type
		$$->size = 8; // Default size for a pointer
	}
	| '[' constant_expression ']' {
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("[ ]", &v);
		
		// Semantics
		$$->type = type + "*"; 
		$$->size = getSize(type) * $2->intVal;
	}
	| direct_abstract_declarator '[' ']' {
		std::vector<Data> v;
		insertAttr(v, NULL, "[ ]", 0);
		insertAttr(v, $1, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
		
		// Semantics
		$$->type = $1->type + "*";
		$$->size = 8;
	}
	| direct_abstract_declarator '[' constant_expression ']'{
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
		$$ = createASTNode("( )");
		
		// Semantics - function type with no parameters
		$$->type = "FUNC_" + type;
		$$->size = 0;
	}
	| '(' parameter_type_list ')'{
		$$ = $2;
		
		// Semantics - function type with parameters
		$$->type = "FUNC_" + type;
		$$->size = 0;
	}
	| direct_abstract_declarator '(' ')'{
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, $1, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
		
		// Semantics
		$$->type = "FUNC_" + $1->type;
		$$->size = 0;
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
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
		$$ = $1;
	}
	| '{' initializer_list '}' {
		$$ = $2;
		
		// Semantics for array initialization
		$$->isInit = 1;
	}
	| '{' initializer_list ',' '}'{
		$$ = $2;
		
		// Semantics
		$$->isInit = 1;
	}
	;


initializer_list
	: initializer	{
		$$ = $1;
	}
	| initializer_list ',' initializer	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("initializer_list", &v);
		
		// Semantics
		$$->isInit = ($1->isInit && $3->isInit);
	}
	;

statement
	: labeled_statement	{$$ = $1;}
	| compound_statement	{$$ = $1;}
	| expression_statement	{$$ = $1;}
	| selection_statement	{$$ = $1;}
	| iteration_statement	{$$ = $1;}
	| jump_statement	{$$ = $1;}
	;

labeled_statement
	: IDENTIFIER ':' statement	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("labeled_statement", &v);
	}
	| CASE constant_expression ':' statement	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("case", &v);
	}
	| DEFAULT ':' statement	{
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("case", &v);
	}
	;

compound_statement
	: '{' '}'	{
		$$ = createASTNode("{ }");
	}
	| '{' CHANGE_TABLE statement_list '}'	{
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
	: declaration	{$$ = $1;}
	| declaration_list declaration	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declaration_list", &v);
	}
	;

statement_list
	: statement	{$$ = $1;}
	| statement_list statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("statement_list", &v);
	}
	;

expression_statement
	: ';'	{$$ = createASTNode(";");}
	| expression ';'	{$$ = $1;}
	;

selection_statement
	: IF '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("if", &v);
	}
	| IF '(' expression ')' statement ELSE statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("if-else", &v);
	}
	| SWITCH '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("switch", &v);
	}
	| IF '(' ')' statement {
        yyerror("syntax error, missing condition in 'if' statement.");
        $$ = createASTNode("error-node");
    }
	| SWITCH '(' ')' statement {
        yyerror("syntax error, missing condition in 'switch' statement.");
        $$ = createASTNode("error-node");
    }
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("while-loop", &v);
	}
	| DO statement WHILE '(' expression ')' ';'	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("do-while-loop", &v);
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $6, "", 1);
		$$ = createASTNode("for-loop(w/o update stmt)", &v);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("for-loop", &v);
	}
    | UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("until-loop", &v);
	}
	| WHILE '[' expression ']' statement {
        yyerror("syntax error, incorrect parentheses in while-loop.");
		$$ = createASTNode("Invalid While-loop", nullptr);
    }
    | UNTIL '[' expression ']' statement {
        yyerror("syntax error, incorrect parentheses in until-loop.");
		$$ = createASTNode("Invalid Until-loop", nullptr);
    }
    | FOR '(' expression ',' expression ',' expression ')' statement {
        yyerror("syntax error, comma used instead of semicolons.");
        $$ = createASTNode("Invalid for-loop", nullptr);
    }
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
		std::string s;
		s = (std::string)$1 + " : " + (std::string)$2;
        $$ = createASTNode(s);
	}
	| CONTINUE ';'	{$$ = createASTNode($1);}
	| BREAK ';'		{$$ = createASTNode($1);}
	| RETURN ';'	{$$ = createASTNode($1);}
	| RETURN expression ';'	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("jump_stmt", &v);
	}
	;

translation_unit
	: external_declaration	{
		$$ = $1;
	}
	| translation_unit external_declaration	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("program", &v);
	}
	| error ';' {
		$$ = new Node; yyerrok;
	}
	| error ','{
		$$ = new Node; yyerrok;
	}
	| error{
		$$ = new Node; yyerrok;
	}
	;

external_declaration
	: function_definition	{ $$ = $1;}
	| declaration	{ $$ = $1;}
	;

function_definition
	: declaration_specifiers declarator F declaration_list compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("function", &v);

		// Semantics
		type = "";
		string fName = string($3);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declaration_specifiers declarator F compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("function (w/o decl_list)", &v);

		// Semantics 
		type = "";
		string fName = string($3);
		printSymbolTable(curr_table, fName + ".csv");
		updSymbolTable(fName);
	}
	| declarator F declaration_list compound_statement	{
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
	| declarator F compound_statement	{
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
		if (gst.find(funcName) != gst.end()){
			yyerror(("Redefinition of function " + funcName).c_str());
		}
		else{
			makeSymbolTable(funcName, funcType);
			$$ = strdup(funcName.c_str());
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

int yyerror(const char* s) 
{
	yyclearin;
	has_error = true;
    std::ifstream file(inputFilename);  
    std::string curr_line;
    int count = 1;
	std::string heading("syntax error");
	std::string error_line(s);
	int pos = error_line.find_first_of(heading);
	if (pos != std::string::npos)
		error_line.erase(pos, heading.length() + 2);

    while (std::getline(file, curr_line)) 
	{
        if (count == line) {
            std::cerr << "\033[1;31merror: \033[0m" << heading << "::" << line << ":" << column - yyleng << ": " << error_line << "\n\n";
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
