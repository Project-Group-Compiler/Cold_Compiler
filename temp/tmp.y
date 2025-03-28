%{
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include "AST.hpp"
#include "tac.hpp"
#include "data_structures.hpp"

extern int yyleng;
std::string currentDataType="";

bool DEBUG = 0;

int noArgs=0;
bool flag=0,flag2=0;;

extern char* yytext;
extern int column;
extern int line;
extern std::string inputFilename;
extern bool has_error;

int yyerror(const char*);
int yylex();
int rValue = 0;
int if_found = 0; //TODO : Rename to inside a selection stmt/also in while 
int previous_if_found = 0; // TODO: May need later
std::vector<std::string> list_values;
std::map<std::string, std::vector<int>> gotolablelist;
std::map<std::string, int> gotolabel;
%}

%define parse.error detailed
%define parse.lac full

%union{
	char* str;
	int Int;
	Node* ptr;
}

%token<str> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
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

%start translation_unit

%type<Int> NEXT_QUAD WRITE_GOTO
%type<ptr> GOTO_COND CASE_CODE IF_CODE EXPR_CODE EXPR_STMT_CODE 
%type<ptr> N

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
        $$ = getNode($1);

        // 3AC
        SymbolTableEntry* entry = lookup($1);
		
        if(entry) {
			$$->type = entry->data_type;
			$$->temp_name = entry->token;
			$$->nextlist.clear();
            if(entry->data_type == "Procedure") {
                $$->place = "CALL " + std::string($1);
            } 
            else {
                $$->place = std::string($1);
            }
        } 
        else {
            $$->place = "UNKNOWN";
        }
    }
    | CONSTANT {
        $$ = getNode($1);

        // 3AC
		SymbolTableEntry* entry = lookup($1);
		if(entry) {
			$$->type = entry->data_type;
			$$->temp_name = entry->token;
			$$->nextlist.clear();
			$$->place = entry->token;
		}else {
            $$->place = "UNKNOWN";
        }
    }
    | STRING_LITERAL {
        $$ = getNode($1);

        addToConstantTable(std::string($1), "String Literal");
        // 3AC
        SymbolTableEntry* entry = lookup($1);
		if(entry) {
			$$->type = entry->data_type;
			$$->temp_name = entry->token;
			$$->nextlist.clear();
			$$->place = entry->token;
		}else {
            $$->place = "UNKNOWN";
        }
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
        $$ = getNode("postfix_expression", mergeAttrs($1, $3));

		// std::cerr << "\n" << line << " Postfix Expression 144: " << $1->type << ' ' << $3->type << std::endl;
		// std::cerr << "Postfix Expression 144: " << $1->temp_name << ' ' << $3->temp_name << std::endl;
		// std::cerr << "Postfix Expression 144: " << lookup($1->temp_name)->data_type << ' ' << lookup($3->temp_name)->data_type << std::endl;
        //3AC
		// arr[i] -> 

		//arr[][] -> arr[] -> arr as we apply [] operator
		$$->type = $1->type;
		$$->type.pop_back();
		$$->type.pop_back();

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
			$$->temp_name = $1->temp_name;
			emit("[ ]", $1->place, $3->place, q, -1);
		}

		$$->nextlist.clear();
		std::cerr << $$->place << '\n';
		std::cerr << $$->type << '\n';
    }
    | postfix_expression '(' ')' {
        $$ = $1;

        //3AC
		// Todo -> $$->type
		std::string q = getTempVariable($1->type);
		$$->place = q;
		$$->temp_name = $1->temp_name;
		$$->nextlist.clear();
		emit("CALL", $$->temp_name, "0", q, -1);
    }
    | postfix_expression '(' argument_expression_list ')' {
		$$ = getNode("postfix_expression", mergeAttrs($1, $3));

        //3AC
		// Todo -> $$->type
		std::string q = getTempVariable($1->type);
		$$->place = q;
		$$->temp_name = $1->temp_name;
		$$->nextlist.clear();
		emit("CALL", $$->temp_name, std::to_string($3->argCount), q, -1);
    }
    | postfix_expression '.' IDENTIFIER { //p.x
        $$ = getNode("expression.id", mergeAttrs($1, getNode($3)));

        //3AC
		// Todo -> $$->type
        std::string temp_var = getTempVariable($$->type);
        $$->place = temp_var;
		// $$->nextlist.clear();
        emit("member_access", $1->place, std::string($3), temp_var, -1);
    }
    | postfix_expression PTR_OP IDENTIFIER {
        $$ = getNode($2, mergeAttrs($1, getNode($3)));

        //3AC
		// Todo $$ -> type
		std::string temp_var = getTempVariable($$->type);
		emit("PTR_OP", $1->place, std::string($3), temp_var, -1);
		$$->place = temp_var;
    }
    | postfix_expression INC_OP {
        $$ = getNode($2, mergeAttrs($1, nullptr));

        //3AC
        std::string q = getTempVariable($1->type);
        $$->place = q;
		$$->type = $1->type;
		$$->nextlist.clear();
        emit("S++", $1->place, "", q, -1);
    }
    | postfix_expression DEC_OP {
        $$ = getNode($2, mergeAttrs($1, nullptr));

        //3AC
        std::string q = getTempVariable($1->type);
        $$->place = q;
		$$->type = $1->type;
		$$->nextlist.clear();
        emit("S--", $1->place, "", q, -1);
    }
;


argument_expression_list
    : assignment_expression {
        $$ = $1;

        //3AC
        $$->argCount = 1;
		$$->nextlist.clear();
        emit("param", $$->place, "", "", -1);
    }
    | argument_expression_list ',' assignment_expression {
        $$ = getNode("argument_list", mergeAttrs($1, $3));

        //3AC
		$$->nextlist.clear();
        $$->argCount = $1->argCount + 1;
        int Label = emit("param", $3->place, "", "", -1);
		backpatch($1->nextlist, Label);
    }
;

unary_expression
    : postfix_expression {
        $$ = $1;
    }
    | INC_OP unary_expression {
        $$ = getNode($1, mergeAttrs($2, nullptr));

        //3AC
        std::string q = getTempVariable($2->type);
        $$->place = q;
		$$->type = $2->type;
        $$->nextlist.clear();
        emit("++P", $2->place, "", q, -1);
    }
    | DEC_OP unary_expression {
        $$ = getNode($1, mergeAttrs($2, nullptr));

        //3AC
        std::string q = getTempVariable($2->type);
        $$->place = q;
		$$->type = $2->type;
        $$->nextlist.clear();
        emit("--P", $2->place, "", q, -1);
    }
    | unary_operator cast_expression { //TODO: l value .. Here
        $$ = getNode("unary_exp", mergeAttrs($1, $2));
		if(DEBUG) {
			std::cerr << '\n' << line << '\n';
			std::cerr << "Unary Expression 260: "<< $2->type << ' ' << $1->place << ' ' << $2->place << ' ' << rValue << std::endl;
		}
        //3AC
		if($1->place == "unary*"){
			// Reduce one level of pointer indirection for $$->type
			if($2->type.back() == '*') {
				$$->type = $2->type.substr(0, $2->type.size() - 1);
			} else {
				yyerror("syntax error, Invalid dereference of non-pointer type");
				$$->type = "ERROR";
			}
		} else if($1->place == "unary&") {
			// Add one level of pointer indirection for $$->type
			$$->type = $2->type + "*";
		}else{
			$$->type = $2->type;
		}

		if(rValue == 0 && $2->type == "int*"){ // (*ptr) = 10 -> ptr store 10 
			$$->temp_name = $2->temp_name;
			$$->place = "*" + $2->place;
			$$->nextlist.clear();
		}else{
			std::string q = getTempVariable($2->type);
			$$->temp_name = $2->temp_name;
			$$->place = q;
			$$->nextlist.clear();
			emit($1->place, $2->place, "", q, -1);
		}
    }
    | SIZEOF unary_expression {
        $$ = getNode($1, mergeAttrs($2, nullptr));

        //3AC
        std::string q = getTempVariable("int");
		$$->type = "int";
        $$->place = q;
        $$->nextlist.clear();
        emit("SIZEOF", $2->place, "", q, -1);
    }
    | SIZEOF '(' type_name ')' {
        $$ = getNode($1, mergeAttrs($3, nullptr));

        //3AC
        std::string q = getTempVariable("int");
        $$->place = q;
		$$->type = "int";
        $$->nextlist.clear();

        emit("SIZEOF", $3->place, "", q, -1);
    }
;

unary_operator
    : '&' {
        $$ = getNode("&");

        //3AC
        $$->place = "unary&";
    }
    | '*' {
        $$ = getNode("*");

        //3AC
        $$->place = "unary*";
    }
    | '+' {
        $$ = getNode("+");

        //3AC
        $$->place = "unary+";
    }
    | '-' {
        $$ = getNode("-");

        //3AC
        $$->place = "unary-";
    }
    | '~' {
        $$ = getNode("~");

        //3AC
        $$->place = "~";
    }
    | '!' {
        $$ = getNode("!");

        //3AC
        $$->place = "!";
    }
;

cast_expression
    : unary_expression {
        $$ = $1;
    }
    | '(' type_name ')' cast_expression {
        $$ = getNode("cast_expression", mergeAttrs($2, $4));

        //3AC
        std::string q = getTempVariable($2->type);
        $$->place = q;
		$$->type = $2->type;
		$4->nextlist.clear();
		//TODO: Try to do CAST_typename
		std::cerr << "369: CAST " << $4->place << " " << $2->place << ' ' << $2->type << std::endl;
        emit("CAST", $4->place, "", q, -1);
    }
;

multiplicative_expression
    : cast_expression {
        $$ = $1;
    }
    | multiplicative_expression '*' cast_expression {
        $$ = getNode("*", mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable($1->type); //TODO not always int
		$$->type = $1->type; //TODO not always int
        $$->place = q;
        $$->nextlist.clear();
        emit("*", $1->place, $3->place, q, -1);
    }
    | multiplicative_expression '/' cast_expression {
        $$ = getNode("/", mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable("int"); //TODO not always int
		$$->type = $1->type; //TODO not always int
        $$->place = q;
        $$->nextlist.clear();
        emit("/", $1->place, $3->place, q, -1);
    }
    | multiplicative_expression '%' cast_expression {
        $$ = getNode("%", mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable($1->type); //TODO not always int
		$$->type = $1->type; //TODO not always int
        $$->place = q;
        $$->nextlist.clear();
        emit("%", $1->place, $3->place, q, -1);
    }
;


additive_expression
    : multiplicative_expression {
        $$ = $1;
    }
    | additive_expression '+' multiplicative_expression {
        $$ = getNode("+", mergeAttrs($1, $3));

        //3AC
		if(DEBUG) {
			std::cerr<<"Additive Expression 414: "<<$1->type << ' ' << $1->place << ' ' << $3->type <<std::endl;
		}

		std::string q = getTempVariable($1->type);//TODO not always int
		$$->type = $1->type;
		$$->place = q;
		if(($1->type).back() == '*' && (($3->type == "int") || ($3->type == "Integer Constant"))){  //int** + ...
			std::string q2 = getTempVariable($3->type);
			emit("*", $3->place, getSizeOfType($1->type.substr(0, $1->type.size()-1)), q2, -1);
			emit("ptr+", $1->place, q2, q, -1);
		}else{
			emit("+", $1->place, $3->place, q, -1);
		}
        $$->nextlist.clear();
    }
    | additive_expression '-' multiplicative_expression {
        $$ = getNode("-", mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable($1->type);//TODO not always int
		$$->type = $1->type; // TODO not always int
        $$->place = q;
        $$->nextlist.clear();

        emit("-", $1->place, $3->place, q, -1);
    }
;


shift_expression
    : additive_expression {
        $$ = $1;
    }
    | shift_expression LEFT_OP additive_expression {
        $$ = getNode($2, mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable("int");//TODO not always int
		$$->type = "int"; 
        $$->place = q;
        $$->nextlist.clear();

        emit("<<", $1->place, $3->place, q, -1);
    }
    | shift_expression RIGHT_OP additive_expression {
        $$ = getNode($2, mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable("int");//TODO not always int
		$$->type = "int"; 
        $$->place = q;
        $$->nextlist.clear();

        emit(">>", $1->place, $3->place, q, -1);
    }
;

relational_expression
    : shift_expression {
        $$ = $1; 
    }
    | relational_expression '<' shift_expression {
        $$ = getNode("<", mergeAttrs($1, $3));
		// TODO : Can do constant folding if both $1 and $2 are constant
        // 3AC 
        std::string q = getTempVariable("int");
        emit("<", $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | relational_expression '>' shift_expression {
        $$ = getNode(">", mergeAttrs($1, $3));

        // 3AC
        std::string q = getTempVariable("int");
        emit(">", $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | relational_expression LE_OP shift_expression {
        $$ = getNode("<=", mergeAttrs($1, $3));

        // 3AC
        std::string q = getTempVariable("int");
        emit("<=", $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | relational_expression GE_OP shift_expression {
        $$ = getNode(">=", mergeAttrs($1, $3));

        // 3AC
        std::string q = getTempVariable("int");
        emit(">=", $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    ;


equality_expression
    : relational_expression { 
        $$ = $1;
    }
    | equality_expression EQ_OP relational_expression {
        $$ = getNode($2, mergeAttrs($1, $3));

        //3AC
		//TODO : No need to emit if $1 and $3 are constant. Directly store value

        std::string q = getTempVariable("int");
        emit("==", $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | equality_expression NE_OP relational_expression {
        $$ = getNode($2, mergeAttrs($1, $3));

        //3AC
        std::string q = getTempVariable("int");
        emit("!=", $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
;

and_expression
    : equality_expression {
        $$ = $1;
    }
    | and_expression '&' equality_expression {
        $$ = getNode("&", mergeAttrs($1, $3));

        // 3AC
		//TODO : No need to emit if $1 and $3 are constant
        std::string q = getTempVariable("int");
        emit("&", $1->place, $3->place, q, -1);
        $$->place = q;
    }
    ;



exclusive_or_expression
    : and_expression {
        $$ = $1;
    }
    | exclusive_or_expression '^' and_expression {
        $$ = getNode("^", mergeAttrs($1, $3));

        // -3AC ---
        std::string temp = getTempVariable("int");  
        emit("^", $1->place, $3->place, temp, -1);
        $$->place = temp;

        $$->nextlist.clear();
    }
;


inclusive_or_expression
    : exclusive_or_expression { 
        $$ = $1; 
    }
    | inclusive_or_expression '|' exclusive_or_expression {
        $$ = getNode("|", mergeAttrs($1, $3));

        // -3AC ---
        std::string temp = getTempVariable("int"); 
        emit("|", $1->place, $3->place, temp, -1);
        $$->place = temp;

        $$->nextlist.clear();
    }
;
//TODO : Define token also
logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression {
		$$ = getNode("&&", mergeAttrs($1, $3));

		// 3AC
		std::string q = getTempVariable("int");
		emit("&&", $1->place, $3->place, q, -1);
		$$->place = q;
		$$->nextlist.clear();
	}
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression {
		$$ = getNode("||", mergeAttrs($1, $3));

		// 3AC
		std::string q = getTempVariable("int");
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
	: logical_or_expression { $$ = $1; }
	| GOTO_COND NEXT_QUAD expression WRITE_GOTO ':' NEXT_QUAD conditional_expression {
		$$ = getNode("ternary operator", mergeAttrs($1, $3, $7));

		// 3AC
		std::string temp1 = getTempVariable("int");

		backpatch($1->truelist, $2);
		backpatch($1->falselist, $6);
		backpatch($3->nextlist, $4-1);
		backpatch($3->truelist, $4-1);
		backpatch($3->falselist, $4-1);

		setArg1($4-1,$3->place);
		setResult($4-1,temp1);

		backpatch($7->nextlist, getCurrentSize());
		backpatch($7->falselist, getCurrentSize());
		backpatch($7->truelist, getCurrentSize());

		emit("=", $7->place, "", temp1, -1);
		$$->nextlist.push_back($4);
		$$->place = temp1;
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
    : conditional_expression { $$ = $1; }
    | unary_expression assignment_operator { if_found = 0; rValue = 1;} assignment_expression {
        $$ = getNode($2, mergeAttrs($1, $4));

        // 3AC
		$$->type = $1->type;
		if(DEBUG) {
			std::cerr<<"Assignment Expression 656: "<<$1->type << ' ' << $1->place <<std::endl;
		}
		int num = assign_exp($2, $$->type, $1->type, $4->type, $1->place, $4->place);
        $$->place = $1->place;
        backpatch($4->nextlist, num);
		rValue = 0;
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
	| expression ',' NEXT_QUAD assignment_expression {
		$$ = getNode("expression", mergeAttrs($1, $4));

		backpatch($1->nextlist, $3);
        backpatch($1->truelist, $3);
        backpatch($1->falselist, $3);
        $$->nextlist = $4->nextlist;
        $$->place = $4->place;
	}
	;

constant_expression
	: conditional_expression{$$ = $1;}
	;

declaration
	: declaration_specifiers ';'{ $$ = $1; }
	| declaration_specifiers init_declarator_list ';'{
		$$ = getNode("declaration", mergeAttrs($1, $2));

		// 3AC
		$$->nextlist = $2->nextlist;
	}
	;

declaration_specifiers
	: storage_class_specifier { $$ = $1; }
	| storage_class_specifier declaration_specifiers{
		$$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
	}
	| type_specifier{ $$ = $1; }
	| type_specifier declaration_specifiers{
		$$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
	}
	| type_qualifier{ $$ = $1; }
	| type_qualifier declaration_specifiers{
		$$ = getNode("declaration_specifiers", mergeAttrs($1, $2));
	}
	;

init_declarator_list
    : init_declarator { $$ = $1; }
    | init_declarator_list ',' NEXT_QUAD init_declarator {
		$$ = getNode("init_declarator_list", mergeAttrs($1, $4));

        // 3AC
        backpatch($1->nextlist, $3);
        $$->nextlist = $4->nextlist;
    }
    ;

init_declarator
    : declarator {
        $$ = $1;
        $$->place = $1->temp_name;
		if(DEBUG) {
			std::cerr<<"Init Declarator 737: "<<$1->temp_name<<std::endl;
		}
    }
    | declarator '=' {rValue = 1;} NEXT_QUAD initializer {
		$$ = getNode("=", mergeAttrs($1, $5));

        // 3AC
		//TODO: Handle other things like arrays...etc .(void case also)
		// for(auto i:list_values){
		// 	std::cerr << "Init Declarator 808: " << i << std::endl;
		// }
		if(DEBUG){
		}
		// std::cerr<<"Init Declarator 746: "<<$1->temp_name<< ' ' << $1->place << ' ' << $1->type <<std::endl;
		std::string type = lookup($1->temp_name)->data_type;
		if(type.find("int[]") != std::string::npos){
			for(int i = 0; i<list_values.size();i++){
				emit("CopyToOffset", list_values[i], std::to_string(i*4), $1->temp_name, -1);
			}
		}else{
			assign_exp("=", $1->type,$1->type, $5->type, $1->place, $5->place);
		}

		$$->place = $1->temp_name;
        $$->nextlist = $5->nextlist;
        backpatch($1->nextlist, $4);

		rValue = 0;
		list_values.clear();
    }
    ;


storage_class_specifier
	: TYPEDEF	{ $$ = getNode($1); flag=1;}
	| EXTERN	{ $$ = getNode($1); currentDataType="extern "; flag2=1;}
	| STATIC	{ $$ = getNode($1); currentDataType="static "; flag2=1;}
	| AUTO		{ $$ = getNode($1); currentDataType="auto "; flag2=1;}
	| REGISTER	{ $$ = getNode($1); currentDataType="register "; flag2=1;}
	;

type_specifier
	: VOID			{$$ = getNode($1); currentDataType="void";}	
	| CHAR			{$$ = getNode($1); if(flag2){currentDataType+=" char";flag2=0;}else{currentDataType="char";}; }	
	| SHORT			{$$ = getNode($1); if(flag2){currentDataType+=" short";flag2=0;}else{currentDataType="short";}; }	
	| INT			{$$ = getNode($1); if(flag2){currentDataType+=" int";flag2=0;}else{currentDataType="int";} }
	| LONG			{$$ = getNode($1); if(flag2){currentDataType+=" long";flag2=0;}else{currentDataType="long";}; }
	| FLOAT			{$$ = getNode($1); if(flag2){currentDataType+=" float";flag2=0;}else{currentDataType="float";}; }
	| DOUBLE		{$$ = getNode($1); if(flag2){currentDataType+=" double";flag2=0;}else{currentDataType="double";}; }
	| SIGNED		{$$ = getNode($1); currentDataType="signed"; flag2=1; }
	| UNSIGNED		{$$ = getNode($1); currentDataType="unsigned"; flag2=1; }
	| FILE_MAN          { $$ = getNode($1); currentDataType="file";  }
	| struct_or_union_specifier	{$$ = $1;}	
	| class_definition 			{$$ = $1;}
	| enum_specifier			{$$ = $1;}
	| TYPE_NAME		{$$ = getNode($1);}	
	;

inheritance_specifier
	: access_specifier IDENTIFIER {
		$$ = getNode("inheritance_specifier", mergeAttrs($1, getNode($2)));
    }
	;

inheritance_specifier_list
	: inheritance_specifier { $$ = $1; }
	| inheritance_specifier_list ',' inheritance_specifier{
		$$ = getNode("inheritance_specifier_list", mergeAttrs($1, $3));
    }
	;

access_specifier 
	: PRIVATE { $$ = getNode($1); }
	| PUBLIC { $$ = getNode($1); }
	| PROTECTED { $$ = getNode($1); }
	;

class
	: CLASS { $$ = getNode($1); }
	;

class_definition_head 
	: class INHERITANCE_OP inheritance_specifier_list{
		$$ = getNode("class_definition_head", mergeAttrs($1, $3));
    }
	| class IDENTIFIER {
		$$ = getNode("class_definition_head", mergeAttrs($1, getNode($2)));

		currentDataType="Class ";
		std::string check=std::string($2);
		currentDataType+=check;
    }
	| class IDENTIFIER  INHERITANCE_OP inheritance_specifier_list{
		$$ = getNode("class_definition_head", mergeAttrs($1, getNode($2), $4));
    }
	;

class_definition 
	: class_definition_head '{' class_internal_definition_list '}'{
		$$ = getNode("class_definition", mergeAttrs($1, $3));
    }
	| class_definition_head { $$ = $1; }
	;

class_internal_definition_list
	: class_internal_definition { $$ = $1; }
	| class_internal_definition_list class_internal_definition {
		$$ = getNode("class_internal_definition_list", mergeAttrs($1, $2));
    }
	; 

class_internal_definition	
	: access_specifier '{' class_member_list '}' ';'{
		$$ = getNode("class_internal_definition", mergeAttrs($1, $3));
    }
	;

class_member_list
	: class_member{ $$ = $1; }
	| class_member_list class_member{
		$$ = getNode("class_member_list", mergeAttrs($1, $2));
    }
	;

class_member
	: function_definition { $$ = $1; }
	| declaration { $$ = $1; }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
		$$ = getNode($1, mergeAttrs(getNode($2), $4));
		std::string check=std::string($2); //Use ?
	}
	| struct_or_union '{' struct_declaration_list '}'		{
		$$ = getNode($1, mergeAttrs($3));
	}
	| struct_or_union IDENTIFIER 	{
		$$ = getNode($1, mergeAttrs(getNode($2)));
		currentDataType += " " + std::string($2);
	}
	| struct_or_union IDENTIFIER '{' '}'	{
		yyerror("syntax error, struct must be non-empty");
		$$ = getNode("Invalid Struct",nullptr);
	}
	;

struct_or_union
	: STRUCT	{$$ = $1; currentDataType="struct";}
	| UNION		{$$ = $1; currentDataType="union";}
	;

struct_declaration_list
	: struct_declaration { 
		$$ = getNode("struct_declaration_list", mergeAttrs($1)); 
	}
	| struct_declaration_list struct_declaration {
		$$ = getNode("struct_declaration_list", mergeAttrs($1, $2));
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
		$$ = getNode("struct_declaration", mergeAttrs($1, $2));
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
		$$ = getNode("specifier_qualifier_list", mergeAttrs($1, $2));
	}
	| type_specifier	{ $$ = $1; }
	| type_qualifier specifier_qualifier_list 	{
		$$ = getNode("specifier_qualifier_list", mergeAttrs($1, $2));
	}
	| type_qualifier	{ $$ = $1; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator {
		$$ = getNode("struct_declarator_list", mergeAttrs($1, $3));
	}
	;

struct_declarator
	: declarator	{ $$ = $1; }
	| ':' constant_expression	{ $$ = $2; }
	| declarator ':' constant_expression	{
		$$ = getNode(":", mergeAttrs($1, $3));
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}' {
		$$ = getNode($1, mergeAttrs($3));
	}
	| ENUM IDENTIFIER '{' enumerator_list '}' {
		$$ = getNode($1, mergeAttrs(getNode($2), $4));
	}
	| ENUM IDENTIFIER {
		$$ = getNode($1, mergeAttrs(getNode($2)));
		currentDataType = "Enum " + std::string($2);
	}
	;

enumerator_list
	: enumerator {
		$$ = $1;
	}
	| enumerator_list ',' enumerator {
		$$ = getNode("enumerator_list", mergeAttrs($1, $3));
	}
	;

enumerator
	: IDENTIFIER {
		$$ = getNode($1);
	}
	| IDENTIFIER '=' constant_expression {
		$$ = getNode("=", mergeAttrs(getNode($1), $3));
	}
	;

type_qualifier
	: CONST {
		$$ = getNode($1);
		currentDataType = "const ";
		flag2 = 1;
	}
	| VOLATILE {
		$$ = getNode($1);
		currentDataType = "volatile ";
		flag2 = 1;
	}
	;

declarator
	: pointer direct_declarator {
		$$ = getNode("declarator", mergeAttrs($1, $2));
		$$->temp_name = $2->temp_name;
		$$->place = $$->temp_name;
		if(DEBUG) {
			std::cerr<<"Declarator 963: "<<$2->temp_name<<std::endl;
		}
	}
	| direct_declarator {
		$$ = $1;
		$$->place = $$->temp_name;
	}
	;


direct_declarator
	: IDENTIFIER {
		$$ = getNode($1);
		std::string check=std::string($1);
		if(flag){
			typedefTable.push_back(make_pair(check,currentDataType));
			flag=0;
		}
		else{
		std::string check=std::string($1);
		addToSymbolTable(check,currentDataType);
		}

		//3AC
		$$->temp_name = $1;
		$$->place = $$->temp_name;
	}
	| CONSTANT IDENTIFIER {
		yyerror("syntax error, invalid identifier");
		$$ = getNode("Invalid Identifier");
	}
	| '(' declarator ')'  {
		$$ = $2 ;

		//3AC
		$$->place = $$->temp_name;
		
	}
	| direct_declarator '[' constant_expression ']'{
		Node* node = getNode("[ ]", mergeAttrs($3));
		$$ = getNode("direct_declarator[..]", mergeAttrs($1, node));

		updateLastSymbolEntry();

		// if(DEBUG){
			std::cerr << "Direct Declarator 1052: " << $1->temp_name << ' ' << lookup($1->temp_name)->data_type << ' ' << $3->place << std::endl;
		// }
		//3AC
		$$->temp_name = $1->temp_name;
		$$->place = $$->temp_name;

	}
	| direct_declarator '[' ']'{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, NULL, "[ ]", 0);
		$$ = getNode("direct_declarator[]", &attr);

		updateLastSymbolEntry();
		//3AC
		$$->temp_name = $1->temp_name;
		$$->place = $$->temp_name;
	}
	| direct_declarator '(' A parameter_type_list ')' NEXT_QUAD {
		Node *node = getNode("( )", mergeAttrs($4));
		$$ = getNode("direct_declarator", mergeAttrs($1, node));

		updateFuncSymbolEntry(noArgs);
		noArgs=0;
		//3 AC
		$$->temp_name = $1->temp_name;
        $$->place =$$->temp_name;
		backpatch($4->nextlist,$6);
        emit("FUNC_" + $$->temp_name + " start :", "", "", "", -2);

	}
	| direct_declarator '(' A identifier_list ')'{//!
		Node *node = getNode("( )", mergeAttrs($4));
		$$ = getNode("direct_declarator", mergeAttrs($1, node));

		//3 AC
		$$->temp_name = $1->temp_name;
        $$->place =$$->temp_name;
        emit("FUNC_" + $$->temp_name + " start :", "", "", "", -2);

	}
	| direct_declarator '(' A ')'{//!
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, NULL, "( )", 0);
		$$ = getNode("direct_declarator", &attr);
		updateFuncSymbolEntry(0);

		//3 AC
		$$->temp_name = $1->temp_name;
        $$->place =$$->temp_name;
        emit("FUNC_" + $$->temp_name + " start :", "", "", "", -2);

	}
	;

A
	: %empty	{
		gotolablelist.clear();
		gotolabel.clear();
	}

pointer
	: '*' {
		currentDataType+="*";
		$$ = getNode("*(Pointer)");
	}
	| '*' type_qualifier_list{
		currentDataType+="*";
		$$ = getNode("*(Pointer)", mergeAttrs($2));
	}
	| '*' pointer{
		currentDataType+="*";
		$$ = getNode("*(Pointer)", mergeAttrs($2));
	}
	| '*' type_qualifier_list pointer{
		currentDataType+="*";
		$$ = getNode("*(Pointer)", mergeAttrs($2, $3));
	}
	;

type_qualifier_list
	: type_qualifier {
		$$ = $1 ;
	}
	| type_qualifier_list type_qualifier{
		$$ = getNode("type_qualifier_list", mergeAttrs($1, $2));
	}
	;


parameter_type_list
	: parameter_list {
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
		$$ = getNode("parameter_type_list", mergeAttrs($1, getNode($3)));
		$$->nextlist = $1->nextlist;
	}
	;

parameter_list
	: parameter_declaration{
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' NEXT_QUAD parameter_declaration{
		noArgs++;
		$$ = getNode("parameter_list", mergeAttrs($1, $4));
		backpatch($1->nextlist, $3);
		$$->nextlist = $4->nextlist;
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		$$ = getNode("parameter_declaration", mergeAttrs($1, $2));
	}
	| declaration_specifiers abstract_declarator{
		$$ = getNode("parameter_declaration", mergeAttrs($1, $2));
	}
	| declaration_specifiers {
		$$ = $1;
	}
	;

identifier_list
	: IDENTIFIER {
		$$ = getNode($1);
	}
	| identifier_list ',' IDENTIFIER{
		$$ = getNode("identifier_list", mergeAttrs($1, getNode($3)));
	}
	;

type_name
	: specifier_qualifier_list{
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator{
		$$ = getNode("type_name", mergeAttrs($1, $2));
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
		$$ = getNode("abstract_declarator", mergeAttrs($1, $2));
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		$$ = $2;
	}
	| '[' ']'{
		$$ = getNode("[ ]") ;
	}
	| '[' constant_expression ']' {
		$$ = $2;
	}
	| direct_abstract_declarator '[' ']' {//!
		std::vector<Data> attr;
		insertAttr(attr,NULL,"[ ]",0);
		insertAttr(attr,$1,"",1);
		$$ = getNode("direct_abstract_declarator",&attr);
	}
	| direct_abstract_declarator '[' constant_expression ']'{
		$$ = getNode("direct_abstract_declarator", mergeAttrs($1, getNode("[ ]", mergeAttrs($3))));
	}
	| '(' ')'{
		$$ = getNode("( )") ;
	}
	| '(' parameter_type_list ')'{
		$$ = $2 ;
	}
	| direct_abstract_declarator '(' ')'{//!
		std::vector<Data> attr;
		insertAttr(attr, NULL, "( )", 0);
		insertAttr(attr, $1, "", 1);
		$$ = getNode("direct_abstract_declarator",&attr);		
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		$$ = getNode("direct_abstract_declarator", mergeAttrs($1, getNode("( )", mergeAttrs($3))));
	}
	;

initializer
	: assignment_expression{
		$$ = $1 ;
		list_values.push_back($1->place);
	}
	| '{' initializer_list '}' {
		$$ = $2 ;
	}
	| '{' initializer_list ',' '}'{
		$$ = $2;

		$$->place = $2->place;
		$$->nextlist = $2->nextlist;
	}
	;


initializer_list
	: initializer	{
		$$ = $1;
	}
	| initializer_list ',' NEXT_QUAD initializer	{
		$$ = getNode("initializer_list", mergeAttrs($1, $4));

		backpatch($1->nextlist, $3);
		$$->nextlist = $4->nextlist;
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
    : IDENTIFIER ':' NEXT_QUAD statement {
        $$ = getNode("labeled_statement", mergeAttrs(getNode($1), $4));

        gotolabel[$1] = $3;

        $$->nextlist = $4->nextlist;
        $$->caselist = $4->caselist;
        $$->continuelist = $4->continuelist;
        $$->breaklist = $4->breaklist;
    }
    | CASE_CODE NEXT_QUAD statement {
        $$ = getNode("case", mergeAttrs($1, $3));

        backpatch($1->truelist, $2);
		extendList($3->nextlist, $1->falselist);
        $$->breaklist = $3->breaklist;
        $$->nextlist = $3->nextlist;
        $$->caselist = $1->caselist;
        $$->continuelist = $3->continuelist;
    }
    | DEFAULT ':' statement {//!
		std::vector<Data> attr;
        insertAttr(attr, NULL, "default", 0);
        insertAttr(attr, $3, "", 1);
        $$ = getNode("case", &attr);

        $$->breaklist = $3->breaklist;
        $$->nextlist = $3->nextlist;
        $$->continuelist = $3->continuelist;
    }
    ;

CASE_CODE
	: CASE constant_expression ':' {
        $$ = $2;
		std::string t = getTempVariable($$->type);
		int a = getCurrentSize();
		emit("==", "", $2->place, t, -1);
		int b = getCurrentSize();
		emit("GOTO", "IF", t, "", 0);
		int c = getCurrentSize();
		emit("GOTO", "", "", "", 0);
		$$->caselist.push_back(a);
		$$->truelist.push_back(b);
		$$->falselist.push_back(c);
	}
	;


compound_statement
	: '{' '}' {
        $$ = getNode("{ }");
    }
	| '{' statement_list '}' {
        $$ = $2;
    }
	| '{' declaration_list '}' {
        $$ = $2;
    }
	| '{' declaration_list NEXT_QUAD statement_list '}' {
        $$ = getNode("compound_statement", mergeAttrs($2, $4));

		//TODO : Testing 
		backpatch($2->nextlist, $3);
        $$->nextlist = $4->nextlist;
        $$->caselist = $4->caselist;
        $$->continuelist = $4->continuelist;
        $$->breaklist = $4->breaklist;
	}
	;

declaration_list
	: declaration {
        $$ = $1;
    }
	| declaration_list NEXT_QUAD declaration {
        $$ = getNode("declaration_list", mergeAttrs($1, $3));

		backpatch($1->nextlist, $2);
        $$->nextlist = $3->nextlist;
	}
	;

statement_list
	: statement {
		$$ = $1;
	}
	| statement_list NEXT_QUAD statement {
		$$ = getNode("statement_list", mergeAttrs($1, $3));

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
	: ';'	{$$ = getNode(";");}
	| expression ';'	{$$ = $1;}
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
		$$ = getNode("if", mergeAttrs($1, $3));

		backpatch($1->truelist, $2);
		extendList($3->nextlist, $1->falselist);

		$$->nextlist = $3->nextlist;
		$$->continuelist = $3->continuelist;
		$$->breaklist = $3->breaklist;
	}
	| IF_CODE NEXT_QUAD statement N ELSE NEXT_QUAD statement {
		$$ = getNode("if-else", mergeAttrs($1, $3, $7));

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
		$$ = getNode("switch", mergeAttrs($3, $5));

		casepatch($5->caselist, $3->place);

		extendList($5->nextlist, $5->breaklist);
		$$->nextlist = $5->nextlist;
		$$->continuelist = $5->continuelist;
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
        $$ = getNode("while-loop", mergeAttrs($4, $7));

        backpatch($4->truelist, $6);
		extendList($7->nextlist, $7->continuelist);
        backpatch($7->nextlist, $3);
        
        $$->nextlist = $4->falselist;
		extendList($$->nextlist, $7->breaklist);
        emit("GOTO", "", "", "", $3);
    }
	| UNTIL '(' NEXT_QUAD EXPR_CODE ')' NEXT_QUAD statement { /*** Added UNTIL grammar ***/
		$$ = getNode("until-loop", mergeAttrs($4, $7));

		backpatch($4->falselist, $6);
		extendList($7->nextlist, $7->continuelist);
		backpatch($7->nextlist, $3);
		
		$$->nextlist = $4->truelist;
		extendList($$->nextlist, $7->breaklist);
		emit("GOTO", "", "", "", $3);
	}
	| DO NEXT_QUAD statement WHILE '(' NEXT_QUAD EXPR_CODE ')' ';' {
		$$ = getNode("do-while-loop", mergeAttrs($3, $7));

		backpatch($7->truelist, $2);
		extendList($3->nextlist, $3->continuelist);
		backpatch($3->nextlist, $6);

		$$->nextlist = $7->falselist;
		extendList($$->nextlist, $3->breaklist);
	}
	| FOR '(' expression_statement NEXT_QUAD EXPR_STMT_CODE ')' NEXT_QUAD statement {
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
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
		$$ = getNode(std::string($1) + " : " + std::string($2));

		int a = getCurrentSize();
        emit("GOTO", "", "", "", 0);
        gotolablelist[$2].push_back(a);
	}
	| CONTINUE ';'	{
		$$ = getNode($1);

		int a = getCurrentSize();
        emit("GOTO", "", "", "", 0);
        $$->continuelist.push_back(a);
	}
	| BREAK ';'		{
		$$ = getNode($1);

		int a = getCurrentSize();
        emit("GOTO", "", "", "", 0);
        $$->breaklist.push_back(a);
	}
	| RETURN ';'	{
		$$ = getNode($1);

		emit("RETURN", "", "", "", -1);
	}
	| RETURN expression ';'	{
		$$ = getNode("jump_stmt", mergeAttrs(getNode($1), $2));

		backpatch($2->nextlist,getCurrentSize());
        emit("RETURN", $2->place, "", "", -1);
	}
	;

translation_unit
	: external_declaration	{
		$$ = $1;
	}
	| translation_unit NEXT_QUAD external_declaration	{
		$$ = getNode("program", mergeAttrs($1, $3));

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
    : declaration_specifiers declarator declaration_list compound_statement {
		$$ = getNode("function", mergeAttrs($1, $2, $3, $4));

        std::string fName = $2->temp_name;  // Moved from F

		for(auto i: gotolablelist){
			backpatch(i.second, gotolabel[i.first]);
		}

        emit("FUNC_" + fName + " end", "", "", "", -1);
        remainingBackpatch();
    }

    | declaration_specifiers declarator compound_statement {
		$$ = getNode("function (w/o decl_list)", mergeAttrs($1, $2, $3));

        std::string fName = $2->temp_name;

		for(auto i: gotolablelist){
			backpatch(i.second, gotolabel[i.first]);
		}

        emit("FUNC_" + fName + " end", "", "", "", -1);
        remainingBackpatch();
    }

    | declarator declaration_list compound_statement {
		$$ = getNode("function (w/o decl_specifiers)", mergeAttrs($1, $2, $3));
        
		std::string fName = $1->temp_name; 

		for(auto i: gotolablelist){
			backpatch(i.second, gotolabel[i.first]);
		}

        emit("FUNC_" + fName + " end", "", "", "", -1);
        remainingBackpatch();
    }

    | declarator compound_statement {
		$$ = getNode("function (w/o specifiers and decl_list)", mergeAttrs($1, $2));

        for (auto &i : gotolablelist) {
			backpatch(i.second, gotolabel[i.first]);
        }

        std::string fName = $1->temp_name;
        emit("FUNC_" + fName + " end", "", "", "", -1);
        remainingBackpatch();
    };
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