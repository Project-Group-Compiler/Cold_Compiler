%{
#include <stdio.h>
#include <iostream>
#include "AST.h"
#include "typecheck.h"

extern char* yytext;
extern int column;
extern int line;
int yyerror(const char*);
int warning(const char*);
int yylex();
int only_lexer = 0;
FILE* dotfile;
FILE* lexer_file;
char* curr_file;

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


extern int yylex();
extern int yyrestart(FILE*);
extern FILE* yyin;
#define YYERROR_VERBOSE
%}

%union{
	char* str;
	treeNode* ptr;
	constants* num;
}

%token<str> IDENTIFIER STRING_LITERAL SIZEOF
%token<str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token<str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token<str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token<str> XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token<num> CONSTANT

%token<str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token<str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token<str> STRUCT UNION ENUM ELLIPSIS
%token<str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
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

%left ';'

%%


primary_expression
    : IDENTIFIER {
    	$$ = makeleaf($1);
		
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
	| CONSTANT {
		$$ = makeleaf($1->str);
		$$->type = $1->type;
		$$->intVal = $1->intVal;
		$$->realVal = $1->realVal;
		$$->expType = 4;
		$$->temp_name = $1->str;
	}
	| STRING_LITERAL {
		$$ = makeleaf($1);
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
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("postfix_expression", attr);

		//Semantics
		if($1->isInit && $3->isInit){
			$$->isInit = 1;
		}
		string temp = postfixExpression($1->type,1);
		if(!temp.empty()){	
			$$->type = temp;
		}
		else{
			yyerror(("Array " + $1->temp_name +  " Index out of bound").c_str());
		}
	}
	| postfix_expression '(' ')' {
		$$ = $1;

		//Semantics
		$$->isInit = 1;
		string temp = postfixExpression($1->type,2);
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
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("postfix_expression", attr);

		//Semantics
		$$->isInit = $3->isInit;
		string temp = postfixExpression($1->type,3);

		if(!temp.empty()){	
			$$->type = temp;
			if($1->expType ==3){
				vector<string> funcArgs = getFuncArgs($1->temp_name);

				for(int i=0;i<funcArgs.size();i++){
					if(funcArgs[i]=="...")break;
					if(currArgs.size()==i){
						
						yyerror(("Too few Arguments to Function " + $1->temp_name).c_str());
						break;
					}
					string msg = checkType(funcArgs[i],currArgs[i]);

					if(msg =="warning"){
						warning(("Incompatible conversion of " +  currArgs[i] + " to parameter of type " + funcArgs[i]).c_str());
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
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, makeleaf($3), "", 1);
		$$ = makenode("expression.id", attr);

		//Semantics
		string temp = string($3);
		int ret = lookupStruct($1->type,temp);
		if(ret == -1){
			//TODO
			yyerror(("Struct " + $1->node_name + " not defined").c_str());
		}
		else if (ret == 0){
			//TODO
			yyerror("Attribute of Struct not defined");
		}
		else{
			$$->type = StructAttrType($1->type,temp);
			$$->temp_name = $1->temp_name + "." + temp;
		}
	}
	| postfix_expression PTR_OP IDENTIFIER {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, makeleaf($3), "", 1);
		$$ = makenode($2, attr);

		//Semantics
		string temp = string($3);
		string temp1 = ($1->type);
		if(temp1.back() != '*'){
			yyerror(( $1->node_name + " is not a pointer, did you mean to use '.' ").c_str());
		}
		else temp1.pop_back();

		int ret = lookupStruct(temp1, temp);
		if(ret ==-1){
			//TODO
			yyerror("Struct not defined");
		}
		else if (ret == 0){
			//TODO
			yyerror("Attribute of Struct not defined");
		}
		else{
			$$->type = StructAttrType(temp1, temp);
			$$->temp_name = $1->temp_name + "->" + temp;
		}

	}
	| postfix_expression INC_OP {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = makenode($2, attr);

		//Semantics
		$$->isInit = $1->isInit;
		string temp = postfixExpression($1->type,6);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $1->intVal + 1;
		}
		else{
			yyerror("Increment not defined for this type");
		}

	}
	| postfix_expression DEC_OP {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = makenode($2, attr);

		//Semantics
		$$->isInit = $1->isInit;
		string temp = postfixExpression($1->type,7);
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

		//Semantic
		$$->isInit = $1->isInit;
		currArgs.push_back($1->type);
		$$->type = "void";

	}
	| argument_expression_list ',' assignment_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("argument_list", attr);

		//Semantic
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
		vector<data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = makenode($1,attr);

		//Semantic
		$$->isInit = $2->isInit;
		string temp = postfixExpression($2->type,6);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal +1;

		}
		else{
			//TODO
			yyerror("Increment not defined for this type");
		}
	}
	| DEC_OP unary_expression {
		vector<data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = makenode($1,attr);

		//Semantic
		$$->isInit = $2->isInit;
		string temp = postfixExpression($2->type,7);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal -1;

		}
		else{
			//TODO
			yyerror("Decrement not defined for this type");
		}
	}
	| unary_operator cast_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = makenode("unary_exp",attr);

		//Semantic
		$$->isInit = $2->isInit;
		string temp = unaryExp($1->node_name,$2->type);
		if(!temp.empty()){
			$$->type = temp;
			$$->intVal = $2->intVal;

		}
		else{
			//TODO
			yyerror("Type inconsistent with operator");
		}
	}
	| SIZEOF unary_expression {
		vector<data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = makenode($1,attr);

		//Semantic
		$$->type = "int";
		$$->isInit =1;
		$$->intVal = $2->size;
	}
	| SIZEOF '(' type_name ')' {
		vector<data> attr;
		insertAttr(attr, $3, "", 1);
		$$ = makenode($1,attr);

		//Semantic
		$$->type = "int";
		$$->isInit =1;
		$$->intVal = $3->size;
	}
	;

unary_operator
	: '&' {
		$$ = makeleaf("&");
	}
	| '*' {
		$$ = makeleaf("*");
	}
	| '+' {
		$$ = makeleaf("+");
	}
	| '-' {
		$$ = makeleaf("-");
	}
	| '~' {
		$$ = makeleaf("~");
	}
	| '!' {
		$$ = makeleaf("!");
	}
	;

cast_expression
	: unary_expression {
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		vector<data> attr;
		insertAttr(attr, $2, "", 1);
		insertAttr(attr, $4, "", 1);
		$$ = makenode("cast_expression" ,attr);

		//Semantic
		$$->type = $2->type;
		$$->isInit = $4->isInit;
	}
	;

multiplicative_expression
	: cast_expression {
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("*" ,attr);

		//Semantic
		$$->intVal = $1->intVal * $3->intVal; 

		//TODO for real
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = mulExp($1->type, $3->type, '*');

		if(!temp.empty()){
			if(temp == "int"){
				$$->type = "long long" ;
			}
			else if(temp == "float"){
				$$->type = "long double";
			}

		}
		else{
			//TODO
			yyerror("Incompatible type for * operator");
		}


	}
	| multiplicative_expression '/' cast_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("/" ,attr);

		//Semantic
		if($3->intVal!=0)$$->intVal = $1->intVal / $3->intVal;
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp =mulExp($1->type,$3->type,'/');
		if(!temp.empty()){
			if(temp == "int"){
				$$->type = "long long" ;
			}
			else if(temp == "float"){
				$$->type = "long double";
			}

		}
		else{
			//TODO
			yyerror("Incompatible type for / operator");
		}
	}
	| multiplicative_expression '%' cast_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("%" ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		if($3->intVal!=0)$$->intVal = $1->intVal % $3->intVal;
		string temp =mulExp($1->type,$3->type,'%');
		if(temp == "int"){
			$$->type = "long long" ;
		}
		else{
			//TODO
			yyerror("Incompatible type for % operator");
		}

	}
	;


additive_expression
	: multiplicative_expression {
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("+" ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		$$->intVal = $1->intVal + $3->intVal;
		string temp = addExp($1->type,$3->type,'+');
		if(!temp.empty()){
			if(temp == "int")$$->type = "long long";
			else if(temp == "real")$$->type = "long double";
			else $$->type =  temp;
		}
		else{
			//TODO
			yyerror("Incompatible type for + operator");
		}
	}
	| additive_expression '-' multiplicative_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("-" ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		$$->intVal = $1->intVal - $3->intVal;
		string temp = addExp($1->type,$3->type,'-');
		if(!temp.empty()){
			if(temp == "int")$$->type = "long long";
			else if(temp == "real")$$->type = "long double";
			else $$->type = temp;
		}
		else{
			//TODO
			yyerror("Incompatible type for - operator");
		}
	}
	;

shift_expression
	: additive_expression {
		$$ = $1;
	}
	| shift_expression LEFT_OP additive_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2 ,attr);

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
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2 ,attr);

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

relational_expression
	: shift_expression {
		$$ = $1;
	}
	| relational_expression '<' shift_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("<" ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = relExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
			}
			else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
		}
		else{
			yyerror("Invalid operands to binary <");
		}


	}
	| relational_expression '>' shift_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode(">" ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = relExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
			}
			else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
		}
		else{
			yyerror("Invalid operands to binary >");
		}
	}
	| relational_expression LE_OP shift_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2 ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = relExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
			}
			else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
		}
		else{
			yyerror("Invalid operands to binary <=");
		}
	}
	| relational_expression GE_OP shift_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2 ,attr);

		//Semantic
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = relExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp == "bool"){
				$$->type = "bool";
			}
			else if(temp == "Bool"){
				$$->type = "bool";
				 warning("Comparison between pointer and integer");
			}
		}
		else{
			yyerror("Invalid operands to binary >=");
		}
	}
	;

equality_expression
	: relational_expression {
		$$ = $1;
	}
	| equality_expression EQ_OP relational_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2 ,attr);

		//Semantics
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = eqExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp =="ok"){
				warning("Comparison between pointer and integer");
			}
			$$->type = "bool";
			
		}
		else{
			yyerror("Invalid operands to binary ==");
		}
	}
	| equality_expression NE_OP relational_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2 ,attr);

		//Semantics
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = eqExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp =="ok"){
				warning("Comparison between pointer and integer");
			}
			$$->type = "bool";
			
		}
		else{
			yyerror("Invalid operands to binary !=");
		}
	}
	;

and_expression
	: equality_expression		{$$ = $1;}
	| and_expression '&' equality_expression {
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("&",attr);
		
		//Semantics
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = bitExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp =="ok"){
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
	: and_expression													{$$ = $1;}
	| exclusive_or_expression '^' and_expression 	{
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("^",attr);

		//Semantics
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = bitExp($1->type,$3->type);
		if(!temp.empty()){
			if(temp =="ok"){
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
	: exclusive_or_expression											{$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression	{
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("|",attr);
	

	//Semantics
		if($1->isInit ==1 && $3->isInit ==1) $$->isInit = 1;
		string temp = bitExp($1->type,$3->type);

		if(!temp.empty()){
			if(temp =="ok"){
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
	: inclusive_or_expression	{$$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression	{
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("&&",attr);

		// Semantics
		$$->type = string("bool");
		$$->isInit = (($1->isInit) & ($3->isInit));   
		$$->intVal = $1->intVal && $3->intVal;
	}
	;


logical_or_expression
	: logical_and_expression	{$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression	{
			vector<data> attr;
			insertAttr(attr, $1, "", 1);
			insertAttr(attr, $3, "", 1);
			$$ = makenode("||",attr);

			// Semantics
			$$->type = string("bool");
			$$->isInit = (($1->isInit) & ($3->isInit));   
			$$->intVal = $1->intVal || $3->intVal;
		}
	;

conditional_expression
	: logical_or_expression		{$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression	{
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		insertAttr(attr, $5, "", 1);
		$$ = makenode("ternary operator",attr);

		// Semantics
		string temp = condExp($3->type, $5->type);
		if(!temp.empty()){
			$$->type = "int";
		}
		else {
			yyerror("Type mismatch in Conditional Expression");
		}
		if($1->isInit==1 && $3->isInit==1 && $5->isInit==1) $$->isInit=1;
	}
	;


assignment_expression
	: conditional_expression	{$$ = $1;}
	| unary_expression assignment_operator assignment_expression 	{
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode($2,attr);

		//Semantics
		string temp = assignExp($1->type,$3->type,string($2));
		if(!temp.empty()){
			if(temp =="ok"){
				$$->type = $1->type;
			}
			else if(temp == "warning"){
				$$->type = $1->type;
				warning("Assignment with incompatible pointer type");
			} 
			
		}
		else{
			//TODO
			yyerror("Incompatible types when assigning type");
		}
		if($1->expType == 3 && $3->isInit){
			updInit($1->temp_name);
		}
	}
	;


assignment_operator
	: '='				{$$ = "=";}
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
	: assignment_expression				{ $$ = $1; }
	| expression ',' assignment_expression		{
		vector<data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = makenode("expression",attr);

		$$->type = string("void");
	}
	;


constant_expression
	: conditional_expression							{$$ = $1;}
	;

declaration
	: declaration_specifiers ';'						{$$ = $1;  type = "";}
	| declaration_specifiers init_declarator_list ';'	{
															vector<data> attr;
															insertAttr(attr, $1, "", 1);
															insertAttr(attr, $2, "", 1);
															$$ = makenode("declaration",attr);

															type = "";
															if($2->expType == 3){
																// Clear the Symbol table of Function;
																// But which function? We need func_name?
																// $2->temp_name
																// if func is already in the FuncArgs Map => Check argument types
																// If argument types dont match, return error!

															}
															
														}
	;


declaration_specifiers
	: storage_class_specifier							{ $$ = $1; }
	| storage_class_specifier declaration_specifiers	{
															vector<data> attr;
															insertAttr(attr, $1, "", 1);
															insertAttr(attr, $2, "", 1);
															$$ = makenode("declaration_specifiers",attr);
														}
	| type_specifier									{ $$ = $1; }
	| type_specifier declaration_specifiers				{
															vector<data> attr;
															insertAttr(attr, $1, "", 1);
															insertAttr(attr, $2, "", 1);
															$$ = makenode("declaration_specifiers",attr);
														}
	| type_qualifier									{ $$ = $1; }
	| type_qualifier declaration_specifiers				{
															vector<data> attr;
															insertAttr(attr, $1, "", 1);
															insertAttr(attr, $2, "", 1);
															$$ = makenode("declaration_specifiers",attr);
														}
	;

init_declarator_list
	: init_declarator									{$$ = $1;}
	| init_declarator_list ',' init_declarator			{
															vector<data> attr;
															insertAttr(attr, $1, "", 1);
															insertAttr(attr, $3, "", 1);
															$$ = makenode("init_declarator_list",attr);
														}
	;

init_declarator
	: declarator	{
		$$ = $1;

		// Semantics
		if( currLookup($1->temp_name) ){
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
	| declarator '=' initializer	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("=", v);
	
		// Semantics
		if( currLookup($1->temp_name) ){
			string errstr = $1->temp_name + " is already declared";
			yyerror(errstr.c_str());
		}
		else{
			insertSymbol(*curr_table, $1->temp_name, $1->type, $1->size, 1, NULL);
		}
	}
	;

storage_class_specifier
	: TYPEDEF	{
		$$ = makeleaf($1);
	}
	| EXTERN	{
		$$ = makeleaf($1);
	}
	| STATIC	{
		$$ = makeleaf($1);
	}
	| AUTO	{
		$$ = makeleaf($1);
	}
	| REGISTER	{
		$$ = makeleaf($1);
	}
	;

type_specifier
	: VOID		{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| CHAR		{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}	
	| SHORT		{
		$$ = makeleaf($1);
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);	}	
	| INT			{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| LONG			{
		$$ = makeleaf($1);
		
		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| FLOAT			{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| DOUBLE		{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| SIGNED		{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| UNSIGNED		{
		$$ = makeleaf($1);

		// Semantics
		if(type == "") type = string($1);
		else type += " " + string($1);
	}
	| struct_or_union_specifier	{
		$$ = $1;
	}	
	| enum_specifier	{
		$$ = $1;
		// TODO
	}
	| TYPE_NAME		{
		$$ = makeleaf($1);
		string temp = getType($1);
		type = temp;
	}	
	;

struct_or_union_specifier
	: struct_or_union G S '{' struct_declaration_list '}'	{
		vector<data> v;
		insertAttr(v, makeleaf($2), "", 1);
		insertAttr(v, $5, "", 1);
		$$ = makenode($1, v);

		// Semantics
		if(printStructTable("STRUCT_" + string($2)) == 1){
			if(type == "")type = "STRUCT_" + string($2);
			else type += " STRUCT_" + string($2);
		}
		else {
			yyerror(("Struct " + string($2) + " is already defined").c_str());
		}
		
	}
	| struct_or_union S '{' struct_declaration_list '}'		{
		vector<data> v;
		insertAttr(v, $4, "", 1);
		$$ = makenode($1, v);

		// Semantics
		Anon_StructCounter++;
		if(printStructTable("STRUCT_" + to_string(Anon_StructCounter))  == 1){
			if(type == "")type = "STRUCT_" + to_string(Anon_StructCounter);
			else type += " STRUCT_" + to_string(Anon_StructCounter);
		}
		else {
			// Wont come here
			yyerror(("Struct is already defined"));
		}
	}
	| struct_or_union IDENTIFIER {
		vector<data> v;
		insertAttr(v, makeleaf($2), "", 1);
		$$ = makenode($1, v);

		// Semantics
		// ToDo : Global Lookup
		if(findStruct("STRUCT_" + string($2)) == 1){
			if(type == "")type = "STRUCT_" + string($2);
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
	;

G 	
	: IDENTIFIER 	{
		$$ = $1;
		structName = $1;
	}


S 
	: %empty {
		createStructTable();
		
	}

struct_or_union
	: STRUCT	{$$ = $1;}
	| UNION		{$$ = $1;}
	;

struct_declaration_list
	: struct_declaration	{ $$ = $1 ;}
	| struct_declaration_list struct_declaration 	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("struct_declaration_list", v);
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("struct_declaration", v);

		type = "";
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("specifier_qualifier_list", v);
	}
	| type_specifier	{ $$ = $1; }
	| type_qualifier specifier_qualifier_list 	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("specifier_qualifier_list", v);
	}
	| type_qualifier	{ $$ = $1; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator {
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("struct_declarator_list", v);
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
	| ':' constant_expression	{ 
		$$ = $2; 
		// ????
	}
	| declarator ':' constant_expression	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode(":", v);

		// Semantics
		if (insertStructAttr($1->temp_name, $1->type, $3->intVal, 0) != 1){
			yyerror(("The Attribute " + string($1->temp_name) + " is already declared in the same struct").c_str());
		}
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
		// TODO
		vector<data> v;
		insertAttr(v, $3, "", 1);
		$$ = makenode($1, v);
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
		vector<data> v;
		insertAttr(v, makeleaf($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = makenode($1, v);
	}
	| ENUM IDENTIFIER {
		vector<data> v;
		insertAttr(v, makeleaf($2), "", 1);
		$$ = makenode($1, v);
	}
	;

enumerator_list
	: enumerator 	{ $$ = $1; }
	| enumerator_list ',' enumerator 	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("enumerator_list", v);
	}
	;

enumerator
	: IDENTIFIER	{ $$ = makeleaf($1); }
	| IDENTIFIER '=' constant_expression 	{
		vector<data> v;
		insertAttr(v, makeleaf($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("=", v);
	}
	;

type_qualifier
	: CONST		{ $$ = makeleaf($1); }
	| VOLATILE	{ $$ = makeleaf($1); }
	;


declarator
	: pointer direct_declarator{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("declarator", v);

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
		$$ = makeleaf($1);

		// Semantics
		$$->expType = 1; // Variable
		$$->type = type;
		$$->temp_name = string($1);
		$$->size = getSize(type);
	}
	| '(' declarator ')'  {
		$$ = $2 ;
	}
	| direct_declarator '[' constant_expression ']'{
		vector<data> v, v2;
		insertAttr(v2, $3, "", 1);
		treeNode* node = makenode("[ ]", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = makenode("direct_declarator", v);

		// Semantics
		if($1->expType == 1 || $1->expType == 2) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = $1->size * $3->intVal;
		}
		else {
			yyerror(("Function " + $1->temp_name + " cannot be used as an array").c_str());
		}

	}
	| direct_declarator '[' ']'{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		$$ = makenode("direct_declarator", v);

		// Semantics
		if($1->expType <=2 ) {
			$$->expType = 2;
			$$->type = $1->type + "*";
			$$->temp_name = $1->temp_name;
			$$->size = 8;
		}
		else {
			yyerror(("Function " + $1->temp_name + " cannot be used as an array").c_str());
		}
	}
	| direct_declarator '(' A parameter_type_list ')'{
		vector<data> v, v2;
		insertAttr(v2, $4, "", 1);
		treeNode* node = makenode("( )", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = makenode("direct_declarator", v);

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
				// Check if temp is correct
				if(temp == funcArgs){
					funcArgs.clear();
					funcName = string($1->temp_name);
					funcType = $1->type;
				}
				else {
					yyerror(("Conflicting types for " + $1->temp_name).c_str());
				}
			}
		}
		else {
			if($1->expType == 2){
				yyerror( ($1->temp_name + "declared as array of function").c_str());
			}
			else{
				yyerror( ($1->temp_name + "declared as function of function").c_str());
			}
		}
	}
	| direct_declarator '(' A identifier_list ')'{
		// Function should be already declared and used here.

		vector<data> v, v2;
		insertAttr(v2, $4, "", 1);
		treeNode* node = makenode("( )", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = makenode("direct_declarator", v);

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
					yyerror(("Conflicting types for function " + $1->temp_name).c_str());
					break;
				}
				insertSymbol(*curr_table, idList[i], args[i], getSize(args[i]), 1, NULL);
			}
			idList.clear();
		}
		else {
			yyerror(("Conflicting types for function " + $1->temp_name).c_str());
			idList.clear();
		}
	}
	| direct_declarator '(' A ')'{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "( )", 0);
		$$ = makenode("direct_declarator", v);

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
				yyerror(("Conflicting types for function " + $1->temp_name).c_str());
			}
		}
		else {
			if($1->expType == 2){
				yyerror( ($1->temp_name + "declared as array of function").c_str());
			}
			else{
				yyerror( ($1->temp_name + "declared as function of function").c_str());
			}
		}
	}
	;

A
	: %empty	{
		type ="";
		func_flag = 0;
		funcArgs.clear();
		createParamList();
	}

pointer
	: '*' {
		$$ = makeleaf("*(Pointer)");
		$$->type = "*";
	}
	| '*' type_qualifier_list{
		vector<data> v;
		insertAttr(v,$2,"",1);
		$$ = makenode("*(Pointer)",v);

		$$->type = "*";
	}
	| '*' pointer{
		vector<data> v;
		insertAttr(v,$2,"",1);
		$$ = makenode("*(Pointer)",v);

		$$->type = "*" + $2->type;
	}
	| '*' type_qualifier_list pointer{
		vector<data> v;
		insertAttr(v,$2,"",1);
		insertAttr(v,$3,"",1);
		$$ = makenode("*(Pointer)",v);

		$$->type = "*" + $3->type;
	}
	;

type_qualifier_list
	: type_qualifier {
		$$ = $1 ;
	}
	| type_qualifier_list type_qualifier{
		vector<data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = makenode("type_qualifier_list",v);
	}
	;


parameter_type_list
	: parameter_list {
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
		vector<data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v, makeleaf($3), "", 1);
		$$ = makenode("parameter_type_list",v);

		// Semantics
		funcArgs.push_back("...");
	}
	;

parameter_list
	: parameter_declaration{
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("parameter_list",v);
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("parameter_declaration",v);

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
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("parameter_declaration",v);

		type = "";
	}
	| declaration_specifiers {
		$$ = $1;
		funcArgs.push_back(type);
		type = "";
	}
	;

identifier_list
	: IDENTIFIER {			// Give id types acc to func args
		$$ =makeleaf($1);

		// Semantics
		idList.push_back($1);
	}
	| identifier_list ',' IDENTIFIER {
		vector<data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,makeleaf($3),"",1);
		$$ = makenode("identifier_list",v);

		// Semantics
		idList.push_back($3);

	}
	;


type_name
	: specifier_qualifier_list{
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator{
		vector<data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = makenode("type_name",v);
	}
	;

abstract_declarator
	: pointer {
		$$ =$1;
	}
	| direct_abstract_declarator{
		$$ = $1;
	}
	| pointer direct_abstract_declarator{
		vector<data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = makenode("abstract_declarator",v);
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		$$ = $2;
	}
	| '[' ']'{
		$$ = makeleaf("[ ]") ;
	}
	| '[' constant_expression ']' {
		$$ = $2;
	}
	| direct_abstract_declarator '[' ']' {
		vector<data> v;
		insertAttr(v,NULL,"[ ]",0);
		insertAttr(v,$1,"",1);
		$$ = makenode("direct_abstract_declarator",v);
	}
	| direct_abstract_declarator '[' constant_expression ']'{
		vector<data> v, v2;
		insertAttr(v2, $3, NULL, 1);
		treeNode* node = makenode("[ ]", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = makenode("direct_abstract_declarator", v);
	}
	| '(' ')'{
		$$ = makeleaf("( )") ;
	}
	| '(' parameter_type_list ')'{
		$$ = $2 ;
	}
	| direct_abstract_declarator '(' ')'{
		vector<data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, $1, "", 1);
		$$ = makenode("direct_abstract_declarator",v);
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		vector<data> v, v2;
		insertAttr(v2, $3, "", 1);
		treeNode* node = makenode("( )", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = makenode("direct_abstract_declarator", v);
	}
	;

initializer
	: assignment_expression{
		$$ = $1 ;
	}
	| '{' initializer_list '}' {
		$$ = $2 ;
	}
	| '{' initializer_list ',' '}'{
		$$ = $2;
	}
	;


initializer_list
	: initializer	{
		$$ = $1;
	}
	| initializer_list ',' initializer	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("initializer_list", v);
	}
	;

statement
	: labeled_statement		{$$ = $1;}
	| compound_statement	{$$ = $1;}
	| expression_statement	{$$ = $1;}
	| selection_statement	{$$ = $1;}
	| iteration_statement	{$$ = $1;}
	| jump_statement		{$$ = $1;}
	;

labeled_statement
	: IDENTIFIER ':' statement	{
		vector<data> v;
		insertAttr(v, makeleaf($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("labeled_statement", v);
	}
	| CASE constant_expression ':' statement	{
		vector<data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = makenode("case", v);
	}
	| DEFAULT ':' statement	{
		vector<data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, $3, "", 1);
		$$ = makenode("case", v);
	}
	;

compound_statement
	: '{' '}'	{$$ = makeleaf("{ }");}
	| '{' CHANGE_TABLE statement_list '}'	{
		$$ = $3;

		if(func_flag>=2){
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName+str+".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	| '{' CHANGE_TABLE declaration_list '}'	{
		$$ = $3;

		if(func_flag>=2){
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName+str+".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	| '{' CHANGE_TABLE declaration_list statement_list '}'	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = makenode("compound_statement", v);
		
		if(func_flag>=2){
			int bc = block_stack.top();
			block_stack.pop();
			string str = "Block" + to_string(bc);
			string name = funcName+str+".csv";
			printSymbolTable(curr_table, name);
			updSymbolTable(str);
			func_flag--;
		}
	}
	;

CHANGE_TABLE
	: %empty {
		if(func_flag){
			string str = "Block" +to_string(block_count);
			block_stack.push(block_count);
			block_count++;
			func_flag++;
			makeSymbolTable(str, "");
		}
		else func_flag++;
	}
	;

declaration_list
	: declaration	{$$ = $1;}
	| declaration_list declaration	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("declaration_list", v);
	}
	;

statement_list
	: statement	{$$ = $1;}
	| statement_list statement	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("statement_list", v);
	}
	;

expression_statement
	: ';'	{$$ = makeleaf(";");}
	| expression ';'	{$$ = $1;}
	;

selection_statement
	: IF '(' expression ')' statement	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = makenode("if", v);
	}
	| IF '(' expression ')' statement ELSE statement	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = makenode("if-else", v);
	}
	| SWITCH '(' expression ')' statement	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = makenode("switch", v);
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = makenode("while-loop", v);
	}
	| DO statement WHILE '(' expression ')' ';'	{
		vector<data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = makenode("do-while-loop", v);
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $6, "", 1);
		$$ = makenode("for-loop(w/o update stmt)", v);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
		vector<data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = makenode("for-loop", v);
	}
	;

		 
jump_statement
	: GOTO IDENTIFIER ';'	{
		string s;
		s = (string)$1 + " : " + (string)$2;
        $$ = makeleaf(s);
	}
	| CONTINUE ';'	{$$ = makeleaf($1);}
	| BREAK ';'		{$$ = makeleaf($1);}
	| RETURN ';'	{
		$$ = makeleaf($1);
		
	}
	| RETURN expression ';'	{
		vector<data> v;
		insertAttr(v, makeleaf($1), "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("jump_stmt", v);
	}
	;

translation_unit 
	: external_declaration	{
		$$ = $1;
	}
	| translation_unit external_declaration	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = makenode("program", v);
	}
	;

external_declaration
	: function_definition	{ $$ = $1; }
	| declaration			{ $$ = $1; }
	;


function_definition
	: declaration_specifiers declarator F declaration_list compound_statement	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = makenode("function", v);

		// Semantics
		type = "";
		string fName = string($3);
		printSymbolTable(curr_table ,fName + ".csv");
		updSymbolTable(fName);
	}

	| declaration_specifiers declarator F compound_statement 	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = makenode("function (w/o decl_list)", v);

		// Semantics 
		type = "";
		string fName = string($3);
		printSymbolTable(curr_table ,fName + ".csv");
		updSymbolTable(fName);

	}
	| declarator F declaration_list compound_statement	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("function (w/o decl_specifiers)", v);

		// Semantics
		type = "";
		string fName = string($2);
		printSymbolTable(curr_table ,fName + ".csv");
		updSymbolTable(fName);

	}
	| declarator F compound_statement	{
		vector<data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = makenode("function (w/o specifiers and decl_list)", v);

		// Semantics
		type = "";
		string fName = string($2);
		printSymbolTable(curr_table ,fName + ".csv");
		updSymbolTable(fName);
	}
	;


F 
	: %empty 		{
		
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

%%

void print_error(){
	cout<<"\033[1;31mError: \033[0m";
}

void print_warning(){
	cout<<"\033[1;36mWarning: \033[0m";
}

void print_options(){
	// To be constructed later
	cout<<"Usage: parser [OPTIONS] file...\n\n";
	cout<<"Options:\n";
	cout<<"\t--help\t\t\tDisplay available options\n";
	cout<<"\t-l <file>\t\tonly runs the lexer and dumps output in <file>\n";
	cout<<"\t-o <file>\t\tdump the dot script generated in <file>\n";
	cout<<"\n\n";
}

void no_file_present(){
	print_error();
	cout<<"no input files\nCompilation terminated\n";
}



int main(int argc, char* argv[]){
	
	char* file_name = "graph.dot", *lexer_file_name;
	int file_present = 0;

	if(argc <= 1){
		no_file_present();
		return -1;
	}
	
	for(int i = 1; i<argc; i++){
		if(!strcmp(argv[i], "--help")){
			print_options();
			return 0;
		}
	}

	for(int i = 1; i<argc; i++){
		if(!strcmp(argv[i], "-o")){
			if(i+1 >= argc || argv[i+1][0] == '-'){
				print_error();
				cout<<"missing filename after \'-o\'\nCompilation terminated\n";
				return -1;
			}
			else{
				file_name = argv[i+1];
				i++;
			}
		}
		else if(!strcmp(argv[i], "-l")){
			if(i+1 >= argc || argv[i+1][0] == '-'){
				print_error();
				cout<<"missing filename after \'-l\'\nCompilation terminated\n";
				return -1;
			}
			else{
				only_lexer = 1;
				lexer_file_name = argv[i+1];
				i++;
			}
		}
		else if(argv[i][0] != '-') file_present++;
	}

	if(!file_present){
		no_file_present();
		return -1;
	}

	if( only_lexer ){
		lexer_file = fopen(lexer_file_name, "w");

		if(lexer_file == NULL){
			print_error();
			cout<<"cannot open file "<<lexer_file_name<<"\nCompilation terminated\n";
			return -1;
		}

		for(int i = 1; i<argc; i++){
			if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "-l")){
				i++;
				continue;
			}

			yyin = fopen(argv[i], "r");
			
			// File open failed, proceeding to next file(s), if exist
			if(yyin == NULL){
				print_error();
				cout<<"cannot open file "<<argv[i]<<"\n\n";
				continue;
			}
			line = 1, column = 0;
			yyrestart(yyin);
			
			fprintf(lexer_file, "----------Lexer Output for file %s----------\n", argv[i]);
			fprintf(lexer_file, "TOKEN\t\t\tLEXEME\t\t\tLINE#\tCOLUMN#\n");
			while( yylex() > 0 ){

			}
			fprintf(lexer_file, "------------------------END of Output---------------------\n\n");
		}

		return 0;

	}

	dotfile = fopen(file_name, "w");
	
	if(dotfile == NULL){
		print_error();
		cout<<"cannot open the dot file "<<file_name<<"\nCompilation terminated\n";
		return -1;
	}
	symTable_init();
	beginAST();
	
	for(int i = 1; i<argc; i++){
		if(!strcmp(argv[i], "-o")){
			i++;
			continue;
		}

		yyin = fopen(argv[i], "r");
		
		// File open failed, proceeding to next file(s), if exist
		if(yyin == NULL){
			print_error();
			cout<<"cannot open file "<<argv[i]<<"\n\n";
			continue;
		}

		line = 1, column = 0;
		curr_file = argv[i];
		
		yyrestart(yyin);
		yyparse();
		
	}

	endAST();

	printSymbolTable(&gst, "#Global_Symbol_Table#.csv");
	return 0;
}

int yyerror(const char* s) { 
	FILE *dupfile = fopen(curr_file, "r");
	int count = 1;

	char currline[256]; /* or other suitable maximum line size */
	while (fgets(currline, sizeof(currline), dupfile) != NULL) {
		if (count == line){
			cout<<curr_file<<":"<<line<<":"<<column+1-strlen(yytext)<<":: "<<currline;
			print_error();
			cout<<s<<"\n\n";
			cout<<"\033[1;34m Compilation terminated...exiting\033[0m"<<endl;
			exit(0);
			// return -1;
		}
		else{
			count++;
		}
	}

	fclose(dupfile);
	return 1;
}

int warning(const char* s) { 
	FILE *dupfile = fopen(curr_file, "r");
	int count = 1;

	char currline[256]; /* or other suitable maximum line size */
	while (fgets(currline, sizeof(currline), dupfile) != NULL) {
		if (count == line){
			cout<<curr_file<<":"<<line<<":"<<column+1-strlen(yytext)<<":: "<<currline;
			print_warning();
			cout<<s<<"\n\n";
			return -1;
		}
		else{
			count++;
		}
	}

	fclose(dupfile);
	return 1;
}

