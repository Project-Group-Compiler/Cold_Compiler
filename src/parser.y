%{
#include "AST.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstring>

typedef struct token_data
{
	int line;
	int column;
	std::string token_type;
	std::string lexeme;
} TOKEN;


extern char* yytext;
extern int column;
extern int line;
extern std::string current_token_type;

int yyerror(const char*);
int yylex();
int onlyLexer = 0;
FILE* dotfile;

extern int yyleng;
extern int yylex();
extern int yyrestart(FILE*);
extern FILE* yyin;
#define YYERROR_VERBOSE
%}

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
    	$$ = createLeaf($1);
    }
	| CONSTANT 	{
		$$ = createLeaf($1);
	}
	| STRING_LITERAL {
		$$ = createLeaf($1);
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
		$$ = createNode("postfix_expression", attr);
	}
	| postfix_expression '(' ')' {
		$$ = $1;
	}
	| postfix_expression '(' argument_expression_list ')' {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("postfix_expression", attr);
	}
	| postfix_expression '.' IDENTIFIER {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createLeaf($3), "", 1);
		$$ = createNode("expression.id", attr);
	}
	| postfix_expression PTR_OP IDENTIFIER {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createLeaf($3), "", 1);
		$$ = createNode($2, attr);
	}
	| postfix_expression INC_OP {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createNode($2, attr);
	}
	| postfix_expression DEC_OP {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createNode($2, attr);
	}
	;

argument_expression_list
	: assignment_expression {
		$$ = $1;
	}
	| argument_expression_list ',' assignment_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("argument_list", attr);
	}
	;

unary_expression
	: postfix_expression {
		$$ = $1;
	}
	| INC_OP unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createNode($1,attr);
	}
	| DEC_OP unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createNode($1,attr);
	}
	| unary_operator cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createNode("unary_exp",attr);
	}
	| SIZEOF unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createNode($1,attr);
	}
	| SIZEOF '(' type_name ')' {
		std::vector<Data> attr;
		insertAttr(attr, $3, "", 1);
		$$ = createNode($1,attr);
	}
	;

unary_operator
	: '&' {
		$$ = createLeaf("&");
	}
	| '*' {
		$$ = createLeaf("*");
	}
	| '+' {
		$$ = createLeaf("+");
	}
	| '-' {
		$$ = createLeaf("-");
	}
	| '~' {
		$$ = createLeaf("~");
	}
	| '!' {
		$$ = createLeaf("!");
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
		$$ = createNode("cast_expression" ,attr);
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
		$$ = createNode("*" ,attr);
	}
	| multiplicative_expression '/' cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("/" ,attr);
	}
	| multiplicative_expression '%' cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("%" ,attr);
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
		$$ = createNode("+" ,attr);
	}
	| additive_expression '-' multiplicative_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("-" ,attr);
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
		$$ = createNode($2 ,attr);
	}
	| shift_expression RIGHT_OP additive_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode($2 ,attr);
	}
	; 

relational_expression
	: shift_expression {
		 $$ = $1;
	}
	| relational_expression '<' shift_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("<" ,attr);
	}
	| relational_expression '>' shift_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode(">" ,attr);
	}
	| relational_expression LE_OP shift_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode($2 ,attr);
	}
	| relational_expression GE_OP shift_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode($2 ,attr);
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
		$$ = createNode($2 ,attr);
	}
	| equality_expression NE_OP relational_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode($2 ,attr);
	}
	;

and_expression
	: equality_expression {	
		$$ = $1; 
	}
	| and_expression '&' equality_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("&",attr);
	}
	;

exclusive_or_expression
	: and_expression {
		$$ = $1;
	}
	| exclusive_or_expression '^' and_expression 						{
																			std::vector<Data> attr;
																			insertAttr(attr, $1, "", 1);
																			insertAttr(attr, $3, "", 1);
																			$$ = createNode("^",attr);
																		}
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("|",attr);
	}
	;

logical_and_expression
	: inclusive_or_expression {$$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("&&",attr);
	}
	;

logical_or_expression
	: logical_and_expression{$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("||",attr);
	}
	;

conditional_expression
	: logical_or_expression {$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		insertAttr(attr, $5, "", 1);
		$$ = createNode("ternary operator",attr);
	}
	;

assignment_expression
	: conditional_expression{$$ = $1;}
	| unary_expression assignment_operator assignment_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode($2,attr);
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
		$$ = createNode("expression",attr);
	}
	;

constant_expression
	: conditional_expression{$$ = $1;}
	;

declaration
	: declaration_specifiers ';'{ $$ = $1; }
	| declaration_specifiers init_declarator_list ';'{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createNode("declaration",attr);
	}
	;

declaration_specifiers
	: storage_class_specifier { $$ = $1; }
	| storage_class_specifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createNode("declaration_specifiers",attr);
	}
	| type_specifier{ $$ = $1; }
	| type_specifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createNode("declaration_specifiers",attr);
	}
	| type_qualifier{ $$ = $1; }
	| type_qualifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createNode("declaration_specifiers",attr);
	}
	;

init_declarator_list
	: init_declarator{$$ = $1;}
	| init_declarator_list ',' init_declarator{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createNode("init_declarator_list",attr);
	}
	;

init_declarator
	: declarator {$$ = $1;}
	| declarator '=' initializer{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("=", v);
	}
	;

storage_class_specifier
	: TYPEDEF	{ $$ = createLeaf($1);}
	| EXTERN	{ $$ = createLeaf($1);}
	| STATIC	{ $$ = createLeaf($1);}
	| AUTO		{ $$ = createLeaf($1);}
	| REGISTER	{ $$ = createLeaf($1);}
	;

type_specifier
	: VOID			{$$ = createLeaf($1);}	
	| CHAR			{$$ = createLeaf($1);}	
	| SHORT			{$$ = createLeaf($1);}	
	| INT			{
		if (!$1) {
			yyerror("Parser error: NULL INT token received");
			YYABORT;
		}
		$$ = createLeaf($1);
	}
	| LONG			{$$ = createLeaf($1);}
	| FLOAT			{$$ = createLeaf($1);}
	| DOUBLE		{$$ = createLeaf($1);}
	| SIGNED		{$$ = createLeaf($1);}
	| UNSIGNED		{$$ = createLeaf($1);}
	| struct_or_union_specifier	{$$ = $1;}	
	| enum_specifier			{$$ = $1;}
	| TYPE_NAME		{$$ = createLeaf($1);}	
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
		std::vector<Data> v;
		insertAttr(v, createLeaf($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createNode($1, v);
	}
	| struct_or_union '{' struct_declaration_list '}'		{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createNode($1, v);
	}
	| struct_or_union IDENTIFIER 	{
		std::vector<Data> v;
		insertAttr(v, createLeaf($2), "", 1);
		$$ = createNode($1, v);
	}
	;

struct_or_union
	: STRUCT	{$$ = $1;}
	| UNION		{$$ = $1;}
	;

struct_declaration_list
	: struct_declaration	{ $$ = $1 ;}
	| struct_declaration_list struct_declaration 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("struct_declaration_list", v);
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("struct_declaration", v);
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("specifier_qualifier_list", v);
	}
	| type_specifier	{ $$ = $1; }
	| type_qualifier specifier_qualifier_list 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("specifier_qualifier_list", v);
	}
	| type_qualifier	{ $$ = $1; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator {
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("struct_declarator_list", v);
	}
	;

struct_declarator
	: declarator	{ $$ = $1; }
	| ':' constant_expression	{ $$ = $2; }
	| declarator ':' constant_expression	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode(":", v);
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createNode($1, v);
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
		std::vector<Data> v;
		insertAttr(v, createLeaf($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createNode($1, v);
	}
	| ENUM IDENTIFIER {
		std::vector<Data> v;
		insertAttr(v, createLeaf($2), "", 1);
		$$ = createNode($1, v);
	}
	;

enumerator_list
	: enumerator 	{ $$ = $1; }
	| enumerator_list ',' enumerator 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("enumerator_list", v);
	}
	;

enumerator
	: IDENTIFIER	{ $$ = createLeaf($1); }
	| IDENTIFIER '=' constant_expression 	{
		std::vector<Data> v;
		insertAttr(v, createLeaf($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("=", v);
	}
	;

type_qualifier
	: CONST		{ $$ = createLeaf($1); }
	| VOLATILE	{ $$ = createLeaf($1); }
	;


declarator
	: pointer direct_declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("declarator", v);
	}
	| direct_declarator {
		$$ = $1 ;
	}
	;


direct_declarator
	: IDENTIFIER {
		$$ = createLeaf($1);
	}
	| '(' declarator ')'  {
		$$ = $2 ;
	}
	| direct_declarator '[' constant_expression ']'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createNode("[ ]", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createNode("direct_declarator", v);
	}
	| direct_declarator '[' ']'{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		$$ = createNode("direct_declarator", v);
	}
	| direct_declarator '(' parameter_type_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createNode("( )", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createNode("direct_declarator", v);
	}
	| direct_declarator '(' identifier_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createNode("( )", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createNode("direct_declarator", v);
	}
	| direct_declarator '(' ')'{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "( )", 0);
		$$ = createNode("direct_declarator", v);
	}
	;

pointer
	: '*' {
		$$ = createLeaf("*(Pointer)");
	}
	| '*' type_qualifier_list{
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createNode("*(Pointer)",v);
	}
	| '*' pointer{
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createNode("*(Pointer)",v);
	}
	| '*' type_qualifier_list pointer{
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		insertAttr(v,$3,"",1);
		$$ = createNode("*(Pointer)",v);
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
		$$ = createNode("type_qualifier_list",v);
	}
	;


parameter_type_list
	: parameter_list {
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v, createLeaf($3), "", 1);
		$$ = createNode("parameter_type_list",v);
	}
	;

parameter_list
	: parameter_declaration{
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("parameter_list",v);
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("parameter_declaration",v);
	}
	| declaration_specifiers abstract_declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("parameter_declaration",v);
	}
	| declaration_specifiers {
		$$ = $1;
	}
	;

identifier_list
	: IDENTIFIER {
		$$ =createLeaf($1);
	}
	| identifier_list ',' IDENTIFIER{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,createLeaf($3),"",1);
		$$ = createNode("identifier_list",v);
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
		$$ = createNode("type_name",v);
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
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createNode("abstract_declarator",v);
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		$$ = $2;
	}
	| '[' ']'{
		$$ = createLeaf("[ ]") ;
	}
	| '[' constant_expression ']' {
		$$ = $2;
	}
	| direct_abstract_declarator '[' ']' {
		std::vector<Data> v;
		insertAttr(v,NULL,"[ ]",0);
		insertAttr(v,$1,"",1);
		$$ = createNode("direct_abstract_declarator",v);
	}
	| direct_abstract_declarator '[' constant_expression ']'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, NULL, 1);
		Node* node = createNode("[ ]", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createNode("direct_abstract_declarator", v);
	}
	| '(' ')'{
		$$ = createLeaf("( )") ;
	}
	| '(' parameter_type_list ')'{
		$$ = $2 ;
	}
	| direct_abstract_declarator '(' ')'{
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, $1, "", 1);
		$$ = createNode("direct_abstract_declarator",v);
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createNode("( )", v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createNode("direct_abstract_declarator", v);
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
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("initializer_list", v);
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
		insertAttr(v, createLeaf($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("labeled_statement", v);
	}
	| CASE constant_expression ':' statement	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createNode("case", v);
	}
	| DEFAULT ':' statement	{
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, $3, "", 1);
		$$ = createNode("case", v);
	}
	;

compound_statement
	: '{' '}'	{$$ = createLeaf("{ }");}
	| '{' statement_list '}'	{$$ = $2;}
	| '{' declaration_list '}'	{$$ = $2;}
	| '{' declaration_list statement_list '}'	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("compound_statement", v);
	}
	;

declaration_list
	: declaration	{$$ = $1;}
	| declaration_list declaration	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("declaration_list", v);
	}
	;

statement_list
	: statement	{$$ = $1;}
	| statement_list statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("statement_list", v);
	}
	;

expression_statement
	: ';'	{$$ = createLeaf(";");}
	| expression ';'	{$$ = $1;}
	;

selection_statement
	: IF '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createNode("if", v);
	}
	| IF '(' expression ')' statement ELSE statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createNode("if-else", v);
	}
	| SWITCH '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createNode("switch", v);
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createNode("while-loop", v);
	}
	| DO statement WHILE '(' expression ')' ';'	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createNode("do-while-loop", v);
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $6, "", 1);
		$$ = createNode("for-loop(w/o update stmt)", v);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createNode("for-loop", v);
	}
    | UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createNode("until-loop", v);
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
		std::string s;
		s = (std::string)$1 + " : " + (std::string)$2;
        $$ = createLeaf(s);
	}
	| CONTINUE ';'	{$$ = createLeaf($1);}
	| BREAK ';'		{$$ = createLeaf($1);}
	| RETURN ';'	{$$ = createLeaf($1);}
	| RETURN expression ';'	{
		std::vector<Data> v;
		insertAttr(v, createLeaf($1), "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("jump_stmt", v);
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
		$$ = createNode("program", v);
	}
	;

external_declaration
	: function_definition	{ $$ = $1;}
	| declaration	{ $$ = $1;}
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createNode("function", v);
	}
	| declaration_specifiers declarator compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createNode("function (w/o decl_list)", v);
	}
	| declarator declaration_list compound_statement	{
		std::vector<Data> v;
                insertAttr(v, $1, "", 1);
                insertAttr(v, $2, "", 1);
                insertAttr(v, $3, "", 1);
                $$ = createNode("function (w/o decl_specifiers)", v);
	}
	| declarator compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createNode("function (w/o specifiers and decl_list)", v);
	}
	;

%%

std::string lexerOutputFile;
std::string currentFilename; 

void print_error(const std::string& message) {	
    std::cerr << "\033[1;31mError: \033[0m" << message << "\n";
}

void print_options() {
	//Later
}

void performLexicalAnalysis(const std::string& filename) {
    std::ofstream out(lexerOutputFile);
    if (!out) {
        print_error("Cannot open output file " + lexerOutputFile);
        return;
    }

    std::unordered_map<std::string, TOKEN> symbol_table;
    std::vector<TOKEN> tokens;
    
    int token_id;
    while ((token_id = yylex()) != 0) {
        TOKEN token;
        token.token_type = current_token_type;
        token.lexeme = std::string(yytext);
        token.line = line;
        token.column = column - yyleng;

        if (symbol_table.find(token.lexeme) == symbol_table.end()) {
            symbol_table[token.lexeme] = token;
        }
        tokens.push_back(token);
    }

    out << "Lexical Analysis for " << filename << "\n\n";
    out << std::left 
        << std::setw(20) << "Token" 
        << std::setw(40) << "Lexeme" 
        << std::setw(10) << "Line" 
        << std::setw(10) << "Column" << "\n"
        << std::string(80, '-') << "\n";

    for (auto& token : tokens) {
        out << std::left 
            << std::setw(20) << token.token_type
            << std::setw(40) << token.lexeme
            << std::setw(10) << token.line
            << std::setw(10) << token.column << "\n";
    }

    out.close();
}

void performParsing() {
    dotfile = fopen("src/AST.dot", "w");
    if (!dotfile) {
        print_error("Cannot open AST.dot for writing.");
        return;
    }

    beginAST();
    yyparse();
    endAST();

    fclose(dotfile);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_error("No input files provided.");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l") { // Lexer-only mode
            if (i + 1 >= argc) {
                print_error("Missing filename for -l option.");
                return -1;
            }
            onlyLexer = true;
            lexerOutputFile = argv[++i];
            continue;
        }

		currentFilename = argv[i];
        yyin = fopen(argv[i], "r");
        if (!yyin) {
            print_error("Cannot open file " + arg);
            continue;
        }

        line = 1;
        column = 0;
        yyrestart(yyin);

        if (onlyLexer) {
            performLexicalAnalysis(arg);
        } else {
            performParsing();
        }

        fclose(yyin);
    }

    return 0;
}

// Error handling function
int yyerror(const char* s) {
    std::cerr << "\033[1;31mError:\033[0m " << s << " at line " << line 
              << ", column " << column + 1 - (yytext ? strlen(yytext) : 0) 
              << std::endl;

    if (currentFilename.empty()) {
        print_error("Filename not set.");
        return -1;
    }

    std::ifstream file(currentFilename);  
    if (!file) {
        print_error("Cannot open source file: " + currentFilename);
        return -1;
    }

    std::string curr_line;
    int count = 1;
    
    while (std::getline(file, curr_line)) {
        if (count == line) {
            std::cerr << "\n" << line << " | " << curr_line << "\n";
            std::cerr << "    " << std::string(column - yyleng + 1, ' ') << "^\n";
            std::cerr << "\033[1;31mError: \033[0m" << s << "\n\n";
            break;
        }
        count++;
    }

    file.close();
    return -1;
}