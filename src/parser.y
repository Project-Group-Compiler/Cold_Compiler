%{
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "AST.hpp"

extern int yyleng;
extern int column;
extern int line;
extern std::string inputFilename;
extern bool has_error;
extern std::string current_token_lexeme; // Declare the external variable

int yyerror(const char*);
int yylex();
bool DEBUG = 0;
%}

%define parse.error detailed
%define parse.lac full

%union{
	char* str;
	int number;
	Node* ptr;
}

%token<str> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token<str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token<str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token<str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token<str> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token<str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token<str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token<str> STRUCT UNION ENUM ELLIPSIS

%token<str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token<str> UNTIL /*** Added UNTIL token ***/

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
%expect 1 

%%

primary_expression
    : IDENTIFIER {
  		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: primary_expression -> IDENTIFIER\n");
    	$$ = createASTNode($1);
    }
	| CONSTANT IDENTIFIER{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: primary_expression -> CONSTANT IDENTIFIER\n");
		yyerror("Invalid syntax");
		$$ = createASTNode($1);
	} 
	| CONSTANT CONSTANT{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: primary_expression -> CONSTANT IDENTIFIER\n");
		yyerror("Invalid syntax");
		$$ = createASTNode($1);
	}
	| CONSTANT 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: primary_expression -> CONSTANT\n");
		$$ = createASTNode($1);
	}
	| STRING_LITERAL {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: primary_expression -> STRING_LITERAL\n");
		$$ = createASTNode($1);
	}
	| '(' expression ')' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: primary_expression -> ( expression )\n");
		$$ = $2;
	}
	;

postfix_expression
	: primary_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> primary_expression\n");
		$$ = $1;
	}
	| postfix_expression '[' expression ']' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression [ expression ]\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("postfix_expression", &attr);
	}
	| postfix_expression '(' ')' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression ( )\n");
		$$ = $1;
	}
	| postfix_expression '(' argument_expression_list ')' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression ( argument_expression_list )\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("postfix_expression", &attr);
	}
	| postfix_expression '.' IDENTIFIER {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression . IDENTIFIER\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode("expression.id", &attr);
	}
	| postfix_expression PTR_OP IDENTIFIER {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression PTR_OP IDENTIFIER\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode($2, &attr);
	}
	| postfix_expression INC_OP {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression INC_OP\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| postfix_expression DEC_OP {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: postfix_expression -> postfix_expression DEC_OP\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

argument_expression_list
	: assignment_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: argument_expression_list -> assignment_expression\n");
		$$ = $1;
	}
	| argument_expression_list ',' assignment_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: argument_expression_list -> argument_expression_list , assignment_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("argument_list", &attr);
	}
	;

unary_expression
	: postfix_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_expression -> postfix_expression\n");
		$$ = $1;
	}
	| INC_OP unary_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_expression -> INC_OP unary_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
	}
	| DEC_OP unary_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_expression -> DEC_OP unary_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
	}
	| unary_operator cast_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_expression -> unary_operator cast_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("unary_exp", &attr);
	}
	| SIZEOF unary_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_expression -> SIZEOF unary_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
	}
	| SIZEOF '(' type_name ')' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_expression -> SIZEOF ( type_name )\n");
		std::vector<Data> attr;
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($1, &attr);
	}
	;

unary_operator
	: '&' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_operator -> &\n");
		$$ = createASTNode("&");
	}
	| '*' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_operator -> *\n");
		$$ = createASTNode("*");
	}
	| '+' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_operator -> +\n");
		$$ = createASTNode("+");
	}
	| '-' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_operator -> -\n");
		$$ = createASTNode("-");
	}
	| '~' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_operator -> ~\n");
		$$ = createASTNode("~");
	}
	| '!' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: unary_operator -> !\n");
		$$ = createASTNode("!");
	}
	;

cast_expression
	: unary_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: cast_expression -> unary_expression\n");
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: cast_expression -> ( type_name ) cast_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		insertAttr(attr, $4, "", 1);
		$$ = createASTNode("cast_expression", &attr);
	}
	;

multiplicative_expression
	: cast_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: multiplicative_expression -> cast_expression\n");
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: multiplicative_expression -> multiplicative_expression * cast_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("*", &attr);
	}
	| multiplicative_expression '/' cast_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: multiplicative_expression -> multiplicative_expression / cast_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("/", &attr);
	}
	| multiplicative_expression '%' cast_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: multiplicative_expression -> multiplicative_expression % cast_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("%", &attr);
	}
	;

additive_expression
	: multiplicative_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: additive_expression -> multiplicative_expression\n");
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: additive_expression -> additive_expression + multiplicative_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("+", &attr);
	}
	| additive_expression '-' multiplicative_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: additive_expression -> additive_expression - multiplicative_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("-", &attr);
	}
	;

shift_expression
	: additive_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: shift_expression -> additive_expression\n");
		$$ = $1;
	}
	| shift_expression LEFT_OP additive_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: shift_expression -> shift_expression LEFT_OP additive_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| shift_expression RIGHT_OP additive_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: shift_expression -> shift_expression RIGHT_OP additive_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	; 

relational_expression
	: inclusive_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: relational_expression -> inclusive_or_expression\n");
		$$ = $1;
	}
	| relational_expression '<' inclusive_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: relational_expression -> relational_expression < inclusive_or_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("<", &attr);
	}
	| relational_expression '>' inclusive_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: relational_expression -> relational_expression > inclusive_or_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode(">", &attr);
	}
	| relational_expression LE_OP inclusive_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: relational_expression -> relational_expression LE_OP inclusive_or_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| relational_expression GE_OP inclusive_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: relational_expression -> relational_expression GE_OP inclusive_or_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

equality_expression
	: relational_expression { 
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: equality_expression -> relational_expression\n");
		$$ = $1;
	}
	| equality_expression EQ_OP relational_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: equality_expression -> equality_expression EQ_OP relational_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| equality_expression NE_OP relational_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: equality_expression -> equality_expression NE_OP relational_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

and_expression
	: shift_expression {	
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: and_expression -> shift_expression\n");
		$$ = $1; 
	}
	| and_expression '&' shift_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: and_expression -> and_expression & shift_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("&", &attr);
	}
	;

exclusive_or_expression
	: and_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: exclusive_or_expression -> and_expression\n");
		$$ = $1;
	}
	| exclusive_or_expression '^' and_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: exclusive_or_expression -> exclusive_or_expression ^ and_expression\n");
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("^", &attr);
    }
	;

inclusive_or_expression
	: exclusive_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: inclusive_or_expression -> exclusive_or_expression\n");
		$$ = $1;
	}
	| inclusive_or_expression '|' exclusive_or_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: inclusive_or_expression -> inclusive_or_expression | exclusive_or_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("|", &attr);
	}
	;

logical_and_expression
	: equality_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: logical_and_expression -> equality_expression\n");
		$$ = $1;
	}
	| logical_and_expression AND_OP equality_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: logical_and_expression -> logical_and_expression AND_OP equality_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("&&", &attr);
	}
	;

logical_or_expression
	: logical_and_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: logical_or_expression -> logical_and_expression\n");
		$$ = $1;
	}
	| logical_or_expression OR_OP logical_and_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: logical_or_expression -> logical_or_expression OR_OP logical_and_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("||", &attr);
	}
	;

conditional_expression
	: logical_or_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: conditional_expression -> logical_or_expression\n");
		$$ = $1;
	}
	| logical_or_expression '?' expression ':' conditional_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: conditional_expression -> logical_or_expression ? expression : conditional_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		insertAttr(attr, $5, "", 1);
		$$ = createASTNode("ternary operator", &attr);
	}
	;

assignment_expression
	: conditional_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_expression -> conditional_expression\n");
		$$ = $1;
	}
	| unary_expression assignment_operator assignment_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_expression -> unary_expression assignment_operator assignment_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

assignment_operator
	: '='	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> =\n");
		$$ = strdup("=");
	}
	| MUL_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> MUL_ASSIGN\n");
		$$ = $1;
	}
	| DIV_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> DIV_ASSIGN\n");
		$$ = $1;
	}
	| MOD_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> MOD_ASSIGN\n");
		$$ = $1;
	}
	| ADD_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> ADD_ASSIGN\n");
		$$ = $1;
	}
	| SUB_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> SUB_ASSIGN\n");
		$$ = $1;
	}
	| LEFT_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> LEFT_ASSIGN\n");
		$$ = $1;
	}
	| RIGHT_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> RIGHT_ASSIGN\n");
		$$ = $1;
	}
	| AND_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> AND_ASSIGN\n");
		$$ = $1;
	}
	| XOR_ASSIGN		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> XOR_ASSIGN\n");
		$$ = $1;
	}
	| OR_ASSIGN			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: assignment_operator -> OR_ASSIGN\n");
		$$ = $1;
	}
	;

expression
	: assignment_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: expression -> assignment_expression\n");
		$$ = $1;
	}
	| expression ',' assignment_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: expression -> expression , assignment_expression\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("expression", &attr);
	}
	;

constant_expression
	: conditional_expression {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: constant_expression -> conditional_expression\n");
		$$ = $1;
	}
	;
declaration
	: declaration_specifiers ';'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration -> declaration_specifiers ;\n");
		$$ = $1;
	}
	| declaration_specifiers init_declarator_list ';'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration -> declaration_specifiers init_declarator_list ;\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration", &attr);
	} 
	;

declaration_specifiers
	: storage_class_specifier {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_specifiers -> storage_class_specifier\n");
		$$ = $1;
	}
	| storage_class_specifier declaration_specifiers{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_specifiers -> storage_class_specifier declaration_specifiers\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_specifier{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_specifiers -> type_specifier\n");
		$$ = $1;
	}
	| type_specifier declaration_specifiers{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_specifiers -> type_specifier declaration_specifiers\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_qualifier{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_specifiers -> type_qualifier\n");
		$$ = $1;
	}
	| type_qualifier declaration_specifiers{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_specifiers -> type_qualifier declaration_specifiers\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	;

init_declarator_list
	: init_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: init_declarator_list -> init_declarator\n");
		$$ = $1;
	}
	| init_declarator_list ',' init_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: init_declarator_list -> init_declarator_list , init_declarator\n");
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("init_declarator_list", &attr);
	}
	;

init_declarator
	: declarator {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: init_declarator -> declarator\n");
		$$ = $1;
	}
	| declarator '=' initializer{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: init_declarator -> declarator = initializer\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
	}
	;

storage_class_specifier
	: TYPEDEF	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: storage_class_specifier -> TYPEDEF\n");
		$$ = createASTNode($1);
	}
	| EXTERN	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: storage_class_specifier -> EXTERN\n");
		$$ = createASTNode($1);
	}
	| STATIC	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: storage_class_specifier -> STATIC\n");
		$$ = createASTNode($1);
	}
	| AUTO		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: storage_class_specifier -> AUTO\n");
		$$ = createASTNode($1);
	}
	| REGISTER	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: storage_class_specifier -> REGISTER\n");
		$$ = createASTNode($1);
	}
	;

type_specifier
	: VOID			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> VOID\n");
		$$ = createASTNode($1);
	}	
	| CHAR			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> CHAR\n");
		$$ = createASTNode($1);
	}	
	| SHORT			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> SHORT\n");
		$$ = createASTNode($1);
	}	
	| INT			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> INT\n");
		$$ = createASTNode($1);
	}
	| LONG			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> LONG\n");
		$$ = createASTNode($1);
	}
	| FLOAT			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> FLOAT\n");
		$$ = createASTNode($1);
	}
	| DOUBLE		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> DOUBLE\n");
		$$ = createASTNode($1);
	}
	| SIGNED		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> SIGNED\n");
		$$ = createASTNode($1);
	}
	| UNSIGNED		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> UNSIGNED\n");
		$$ = createASTNode($1);
	}
	| struct_or_union_specifier	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> struct_or_union_specifier\n");
		$$ = $1;
	}	
	| enum_specifier			{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> enum_specifier\n");
		$$ = $1;
	}
	| TYPE_NAME		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_specifier -> TYPE_NAME\n");
		$$ = createASTNode($1);
	}	
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_or_union_specifier -> struct_or_union IDENTIFIER { struct_declaration_list }\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
	}
	| struct_or_union '{' struct_declaration_list '}'		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_or_union_specifier -> struct_or_union { struct_declaration_list }\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createASTNode($1, &v);
	}
	| struct_or_union IDENTIFIER 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_or_union_specifier -> struct_or_union IDENTIFIER\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		$$ = createASTNode($1, &v);
	}
	;

struct_or_union
	: STRUCT	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_or_union -> STRUCT\n");
		$$ = $1;
	}
	| UNION		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_or_union -> UNION\n");
		$$ = $1;
	}
	;

struct_declaration_list
	: struct_declaration	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declaration_list -> struct_declaration\n");
		$$ = $1;
	}
	| struct_declaration_list struct_declaration 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declaration_list -> struct_declaration_list struct_declaration\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration_list", &v);
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declaration -> specifier_qualifier_list struct_declarator_list ;\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration", &v);
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: specifier_qualifier_list -> type_specifier specifier_qualifier_list\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_specifier	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: specifier_qualifier_list -> type_specifier\n");
		$$ = $1;
	}
	| type_qualifier specifier_qualifier_list 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: specifier_qualifier_list -> type_qualifier specifier_qualifier_list\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_qualifier	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: specifier_qualifier_list -> type_qualifier\n");
		$$ = $1;
	}
	;

struct_declarator_list
	: struct_declarator {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declarator_list -> struct_declarator\n");
		$$ = $1;
	}
	| struct_declarator_list ',' struct_declarator {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declarator_list -> struct_declarator_list , struct_declarator\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("struct_declarator_list", &v);
	}
	;

struct_declarator
	: declarator	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declarator -> declarator\n");
		$$ = $1;
	}
	| ':' constant_expression	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declarator -> : constant_expression\n");
		$$ = $2;
	}
	| declarator ':' constant_expression	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: struct_declarator -> declarator : constant_expression\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode(":", &v);
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enum_specifier -> ENUM { enumerator_list }\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createASTNode($1, &v);
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enum_specifier -> ENUM IDENTIFIER { enumerator_list }\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
	}
	| ENUM IDENTIFIER {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enum_specifier -> ENUM IDENTIFIER\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		$$ = createASTNode($1, &v);
	}
	;

enumerator_list
	: enumerator 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enumerator_list -> enumerator\n");
		$$ = $1;
	}
	| enumerator_list ',' enumerator 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enumerator_list -> enumerator_list , enumerator\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("enumerator_list", &v);
	}
	;

enumerator
	: IDENTIFIER	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enumerator -> IDENTIFIER\n");
		$$ = createASTNode($1);
	}
	| IDENTIFIER '=' constant_expression 	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: enumerator -> IDENTIFIER = constant_expression\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
	}
	;

type_qualifier
	: CONST		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_qualifier -> CONST\n");
		$$ = createASTNode($1);
	}
	| VOLATILE	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_qualifier -> VOLATILE\n");
		$$ = createASTNode($1);
	}
	;

declarator
	: pointer direct_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declarator -> pointer direct_declarator\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declarator", &v);
	}
	| direct_declarator {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declarator -> direct_declarator\n");
		$$ = $1 ;
	}
	;

direct_declarator
	: IDENTIFIER {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> IDENTIFIER\n");
		$$ = createASTNode($1);
	}
	| '(' declarator ')'  {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> ( declarator )\n");
		$$ = $2 ;
	}
	| direct_declarator '[' constant_expression ']'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> direct_declarator [ constant_expression ]\n");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '[' ']'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> direct_declarator [ ]\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' parameter_type_list ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> direct_declarator ( parameter_type_list )\n");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' identifier_list ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> direct_declarator ( identifier_list )\n");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_declarator -> direct_declarator ( )\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "( )", 0);
		$$ = createASTNode("direct_declarator", &v);
	}
	;

pointer
	: '*' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: pointer -> *\n");
		$$ = createASTNode("*(Pointer)");
	}
	| '*' type_qualifier_list{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: pointer -> * type_qualifier_list\n");
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
	}
	| '*' pointer{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: pointer -> * pointer\n");
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
	}
	| '*' type_qualifier_list pointer{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: pointer -> * type_qualifier_list pointer\n");
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		insertAttr(v,$3,"",1);
		$$ = createASTNode("*(Pointer)",&v);
	}
	;

type_qualifier_list
	: type_qualifier {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_qualifier_list -> type_qualifier\n");
		$$ = $1 ;
	}
	| type_qualifier_list type_qualifier{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_qualifier_list -> type_qualifier_list type_qualifier\n");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_qualifier_list",&v);
	}
	;

parameter_type_list
	: parameter_list {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_type_list -> parameter_list\n");
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_type_list -> parameter_list , ELLIPSIS\n");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v, createASTNode($3), "", 1);
		$$ = createASTNode("parameter_type_list",&v);
	}
	;

parameter_list
	: parameter_declaration{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_list -> parameter_declaration\n");
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_list -> parameter_list , parameter_declaration\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("parameter_list",&v);
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_declaration -> declaration_specifiers declarator\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
	}
	| declaration_specifiers abstract_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_declaration -> declaration_specifiers abstract_declarator\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
	}
	| declaration_specifiers {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: parameter_declaration -> declaration_specifiers\n");
		$$ = $1;
	}
	;

identifier_list
	: IDENTIFIER {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: identifier_list -> IDENTIFIER\n");
		$$ =createASTNode($1);
	}
	| identifier_list ',' IDENTIFIER{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: identifier_list -> identifier_list , IDENTIFIER\n");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,createASTNode($3),"",1);
		$$ = createASTNode("identifier_list",&v);
	}
	;

type_name
	: specifier_qualifier_list{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_name -> specifier_qualifier_list\n");
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: type_name -> specifier_qualifier_list abstract_declarator\n");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_name",&v);
	}
	;

abstract_declarator
	: pointer {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: abstract_declarator -> pointer\n");
		$$ = $1;
	}
	| direct_abstract_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: abstract_declarator -> direct_abstract_declarator\n");
		$$ = $1;
	}
	| pointer direct_abstract_declarator{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: abstract_declarator -> pointer direct_abstract_declarator\n");
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("abstract_declarator",&v);
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> ( abstract_declarator )\n");
		$$ = $2;
	}
	| '[' ']'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> [ ]\n");
		$$ = createASTNode("[ ]") ;
	}
	| '[' constant_expression ']' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> [ constant_expression ]\n");
		$$ = $2;
	}
	| direct_abstract_declarator '[' ']' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> direct_abstract_declarator [ ]\n");
		std::vector<Data> v;
		insertAttr(v,NULL,"[ ]",0);
		insertAttr(v,$1,"",1);
		$$ = createASTNode("direct_abstract_declarator",&v);
	}
	| direct_abstract_declarator '[' constant_expression ']'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> direct_abstract_declarator [ constant_expression ]\n");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, NULL, 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
	}
	| '(' ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> ( )\n");
		$$ = createASTNode("( )") ;
	}
	| '(' parameter_type_list ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> ( parameter_type_list )\n");
		$$ = $2 ;
	}
	| direct_abstract_declarator '(' ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> direct_abstract_declarator ( )\n");
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, $1, "", 1);
		$$ = createASTNode("direct_abstract_declarator",&v);
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: direct_abstract_declarator -> direct_abstract_declarator ( parameter_type_list )\n");
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
	}
	;

initializer
	: assignment_expression{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: initializer -> assignment_expression\n");
		$$ = $1 ;
	}
	| '{' initializer_list '}' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: initializer -> { initializer_list }\n");
		$$ = $2 ;
	}
	| '{' initializer_list ',' '}'{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: initializer -> { initializer_list , }\n");
		$$ = $2;
	}
	;

initializer_list
	: initializer	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: initializer_list -> initializer\n");
		$$ = $1;
	}
	| initializer_list ',' initializer	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: initializer_list -> initializer_list , initializer\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("initializer_list", &v);
	}
	;

statement
	: labeled_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> labeled_statement\n");
		$$ = $1;
	}
	| compound_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> compound_statement\n");
		$$ = $1;
	}
	| expression_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> expression_statement\n");
		$$ = $1;
	}
	| selection_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> selection_statement\n");
		$$ = $1;
	}
	| iteration_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> iteration_statement\n");
		$$ = $1;
	}
	| jump_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> jump_statement\n");
		$$ = $1;
	}
	| error ';' {
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement -> error ;\n");
		$$ = new Node; yyclearin; yyerrok;
	}
	;

labeled_statement
	: IDENTIFIER ':' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: labeled_statement -> IDENTIFIER : statement\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("labeled_statement", &v);
	}
	| CASE constant_expression ':' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: labeled_statement -> CASE constant_expression : statement\n");
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("case", &v);
	}
	| DEFAULT ':' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: labeled_statement -> DEFAULT : statement\n");
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("case", &v);
	}
	;

compound_statement
	: '{' '}'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: compound_statement -> { }\n");
		$$ = createASTNode("{ }");
	}
	| '{' statement_list '}'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: compound_statement -> { statement_list }\n");
		$$ = $2;
	}
	| '{' declaration_list '}'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: compound_statement -> { declaration_list }\n");
		$$ = $2;
	}
	| '{' declaration_list statement_list '}'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: compound_statement -> { declaration_list statement_list }\n");
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("compound_statement", &v);
	}
	;

declaration_list
	: declaration	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_list -> declaration\n");
		$$ = $1;
	}
	| declaration_list declaration	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: declaration_list -> declaration_list declaration\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declaration_list", &v);
	}
	;

statement_list
	: statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement_list -> statement\n");
		$$ = $1;
	}
	| statement_list statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: statement_list -> statement_list statement\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("statement_list", &v);
	}
	;

expression_statement
	: ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: expression_statement -> ;\n");
		$$ = createASTNode(";");
	}
	| expression ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: expression_statement -> expression ;\n");
		$$ = $1;
	}
	;

selection_statement
	: IF '(' expression ')' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: selection_statement -> IF ( expression ) statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("if", &v);
	}
	| IF '(' expression ')' statement ELSE statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: selection_statement -> IF ( expression ) statement ELSE statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("if-else", &v);
	}
	| SWITCH '(' expression ')' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: selection_statement -> SWITCH ( expression ) statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("switch", &v);
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: iteration_statement -> WHILE ( expression ) statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("while-loop", &v);
	}
	| DO statement WHILE '(' expression ')' ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: iteration_statement -> DO statement WHILE ( expression ) ;\n");
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("do-while-loop", &v);
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: iteration_statement -> FOR ( expression_statement expression_statement ) statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $6, "", 1);
		$$ = createASTNode("for-loop(w/o update stmt)", &v);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: iteration_statement -> FOR ( expression_statement expression_statement expression ) statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("for-loop", &v);
	}
    | UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: iteration_statement -> UNTIL ( expression ) statement\n");
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("until-loop", &v);
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: jump_statement -> GOTO IDENTIFIER ;\n");
		std::string s;
		s = (std::string)$1 + " : " + (std::string)$2;
        $$ = createASTNode(s);
	}
	| CONTINUE ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: jump_statement -> CONTINUE ;\n");
		$$ = createASTNode($1);
	}
	| BREAK ';'		{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: jump_statement -> BREAK ;\n");
		$$ = createASTNode($1);
	}
	| RETURN ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: jump_statement -> RETURN ;\n");
		$$ = createASTNode($1);
	}
	| RETURN expression ';'	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: jump_statement -> RETURN expression ;\n");
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("jump_stmt", &v);
	}
	;

translation_unit
	: external_declaration	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: translation_unit -> external_declaration\n");
		$$ = $1;
	}
	| translation_unit external_declaration	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: translation_unit -> translation_unit external_declaration\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("program", &v);
	}
	;

external_declaration
	: function_definition	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: external_declaration -> function_definition\n");
		$$ = $1;
	}
	| declaration	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: external_declaration -> declaration\n");
		$$ = $1;
	}
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: function_definition -> declaration_specifiers declarator declaration_list compound_statement\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("function", &v);
	}
	| declaration_specifiers declarator compound_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: function_definition -> declaration_specifiers declarator compound_statement\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("function (w/o decl_list)", &v);
	}
	| declarator declaration_list compound_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: function_definition -> declarator declaration_list compound_statement\n");
		std::vector<Data> v;
                insertAttr(v, $1, "", 1);
                insertAttr(v, $2, "", 1);
                insertAttr(v, $3, "", 1);
                $$ = createASTNode("function (w/o decl_specifiers)", &v);
	}
	| declarator compound_statement	{
		if(DEBUG) std::cout<<line << ' ' << current_token_lexeme << " | " << ("Rule: function_definition -> declarator compound_statement\n");
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("function (w/o specifiers and decl_list)", &v);
	}
	;

%%

void performParsing(const std::string &inputFile)
{
    beginAST(inputFile);
    yyparse();
    endAST();
}

// Error handling function
int yyerror(const char* s) 
{
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