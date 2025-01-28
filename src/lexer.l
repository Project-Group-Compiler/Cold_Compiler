D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include "parser.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>

typedef struct token_data
{
	int line;
	int column;
	std::string token_type;
	std::string lexeme;
} TOKEN;

void count();
void comment();
int check_type();

int column = 1;
int line = 1;
std::string current_token_type;
%}

%option noyywrap
%%

"/*"([^*]|(\*+[^*/]))*\*+\/	{ count();}

"//"[^\n]*		{ count(); }

"auto"          { count(); current_token_type="AUTO"; return(AUTO); }
"break"         { count(); current_token_type="BREAK"; return(BREAK); }
"case"          { count(); current_token_type="CASE"; return(CASE); }
"char"          { count(); current_token_type="CHAR"; return(CHAR); }
"const"         { count(); current_token_type="CONST"; return(CONST); }
"continue"      { count(); current_token_type="CONTINUE"; return(CONTINUE); }
"default"       { count(); current_token_type="DEFAULT"; return(DEFAULT); }
"do"            { count(); current_token_type="DO"; return(DO); }
"double"        { count(); current_token_type="DOUBLE"; return(DOUBLE); }
"else"          { count(); current_token_type="ELSE"; return(ELSE); }
"enum"          { count(); current_token_type="ENUM"; return(ENUM); }
"extern"        { count(); current_token_type="EXTERN"; return(EXTERN); }
"float"         { count(); current_token_type="FLOAT"; return(FLOAT); }
"for"           { count(); current_token_type="FOR"; return(FOR); }
"goto"          { count(); current_token_type="GOTO"; return(GOTO); }
"if"            { count(); current_token_type="IF"; return(IF); }
"int"           { count(); current_token_type="INT"; return(INT); }
"long"          { count(); current_token_type="LONG"; return(LONG); }
"register"      { count(); current_token_type="REGISTER"; return(REGISTER); }
"return"        { count(); current_token_type="RETURN"; return(RETURN); }
"short"         { count(); current_token_type="SHORT"; return(SHORT); }
"signed"        { count(); current_token_type="SIGNED"; return(SIGNED); }
"sizeof"        { count(); current_token_type="SIZEOF"; return(SIZEOF); }
"static"        { count(); current_token_type="STATIC"; return(STATIC); }
"struct"        { count(); current_token_type="STRUCT"; return(STRUCT); }
"switch"        { count(); current_token_type="SWITCH"; return(SWITCH); }
"typedef"       { count(); current_token_type="TYPEDEF"; return(TYPEDEF); }
"union"         { count(); current_token_type="UNION"; return(UNION); }
"unsigned"      { count(); current_token_type="UNSIGNED"; return(UNSIGNED); }
"until"         { count(); current_token_type="UNTIL"; return(UNTIL); }
"void"          { count(); current_token_type="VOID"; return(VOID); }
"volatile"      { count(); current_token_type="VOLATILE"; return(VOLATILE); }
"while"         { count(); current_token_type="WHILE"; return(WHILE); }

{L}({L}|{D})*   { count(); current_token_type="IDENTIFIER"; return(check_type()); }

0[xX]{H}+{IS}?  { count(); current_token_type="CONSTANT"; return(CONSTANT); }
0{D}+{IS}?      { count(); current_token_type="CONSTANT"; return(CONSTANT); }
{D}+{IS}?       { count(); current_token_type="CONSTANT"; return(CONSTANT); }
L?'(\\.|[^\\'])+'  { count(); current_token_type="CONSTANT"; return(CONSTANT); }

{D}+{E}{FS}?    { count(); current_token_type="CONSTANT"; return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?  { count(); current_token_type="CONSTANT"; return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?  { count(); current_token_type="CONSTANT"; return(CONSTANT); }

L?\"(\\.|[^\\"])*\"  { count(); current_token_type="STRING_LITERAL"; return(STRING_LITERAL); }

"..."           { count(); current_token_type="ELLIPSIS"; return(ELLIPSIS); }
">>="          { count(); current_token_type="RIGHT_ASSIGN"; return(RIGHT_ASSIGN); }
"<<="          { count(); current_token_type="LEFT_ASSIGN"; return(LEFT_ASSIGN); }
"+="           { count(); current_token_type="ADD_ASSIGN"; return(ADD_ASSIGN); }
"-="           { count(); current_token_type="SUB_ASSIGN"; return(SUB_ASSIGN); }
"*="           { count(); current_token_type="MUL_ASSIGN"; return(MUL_ASSIGN); }
"/="           { count(); current_token_type="DIV_ASSIGN"; return(DIV_ASSIGN); }
"%="           { count(); current_token_type="MOD_ASSIGN"; return(MOD_ASSIGN); }
"&="           { count(); current_token_type="AND_ASSIGN"; return(AND_ASSIGN); }
"^="           { count(); current_token_type="XOR_ASSIGN"; return(XOR_ASSIGN); }
"|="           { count(); current_token_type="OR_ASSIGN"; return(OR_ASSIGN); }
">>"           { count(); current_token_type="RIGHT_OP"; return(RIGHT_OP); }
"<<"           { count(); current_token_type="LEFT_OP"; return(LEFT_OP); }
"++"           { count(); current_token_type="INC_OP"; return(INC_OP); }
"--"           { count(); current_token_type="DEC_OP"; return(DEC_OP); }
"->"           { count(); current_token_type="PTR_OP"; return(PTR_OP); }
"&&"           { count(); current_token_type="AND_OP"; return(AND_OP); }
"||"           { count(); current_token_type="OR_OP"; return(OR_OP); }
"<="           { count(); current_token_type="LE_OP"; return(LE_OP); }
">="           { count(); current_token_type="GE_OP"; return(GE_OP); }
"=="           { count(); current_token_type="EQ_OP"; return(EQ_OP); }
"!="           { count(); current_token_type="NE_OP"; return(NE_OP); }
";"            { count(); current_token_type=";"; return(';'); }
("{"|"<%")     { count(); current_token_type="{"; return('{'); }
("}"|"%>")     { count(); current_token_type="}"; return('}'); }
","            { count(); current_token_type=","; return(','); }
":"            { count(); current_token_type=":"; return(':'); }
"="            { count(); current_token_type="="; return('='); }
"("            { count(); current_token_type="("; return('('); }
")"            { count(); current_token_type=")"; return(')'); }
("["|"<:")     { count(); current_token_type="["; return('['); }
("]"|":>")     { count(); current_token_type="]"; return(']'); }
"."            { count(); current_token_type="."; return('.'); }
"&"            { count(); current_token_type="&"; return('&'); }
"!"            { count(); current_token_type="!"; return('!'); }
"~"            { count(); current_token_type="~"; return('~'); }
"-"            { count(); current_token_type="-"; return('-'); }
"+"            { count(); current_token_type="+"; return('+'); }
"*"            { count(); current_token_type="*"; return('*'); }
"/"            { count(); current_token_type="/"; return('/'); }
"%"            { count(); current_token_type="%"; return('%'); }
"<"            { count(); current_token_type="<"; return('<'); }
">"            { count(); current_token_type=">"; return('>'); }
"^"            { count(); current_token_type="^"; return('^'); }
"|"            { count(); current_token_type="|"; return('|'); }
"?"            { count(); current_token_type="?"; return('?'); }

[ \t\v\n\f]		{ count(); }
.			{ count(); std::cerr<<"Error: Unknown token "<<yytext<<" at line "<<line<<" column "<<column<<"\n";}

%%

void count()
{
	for (int i = 0; yytext[i] != '\0'; i++)
	{
		if (yytext[i] == '\n')
			column = 1, line++;
		else if (yytext[i] == '\t')
			column += 4 - (column % 4);
		else
			column++;
	}
}


int check_type()
{
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return(TYPE_NAME);
*
*	return(IDENTIFIER);
*/

/*
*	it actually will only return IDENTIFIER
*/

	return(IDENTIFIER);
}

int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		yyin = fopen(argv[1], "r");
		if(!yyin)
		{
			std::cout << "Error : Can't open file "<<argv[1]<<"\n";
			return 1;
		}
	}
	else
	{
		std::cout << "Error : Specify one file to be lexed." << std::endl;
		return 1;
	}
	std::unordered_map<std::string, TOKEN> symbol_table;
	std::vector<TOKEN> tokens;
	int token_id;
	while((token_id = yylex()) != 0)
	{
		TOKEN token;
		token.token_type = current_token_type;
		token.lexeme = std::string(yytext);
		token.line = line;
		token.column = column - token.lexeme.length();
		symbol_table[token.lexeme] = token;	
		tokens.push_back(token);
	}
	std::ofstream out("output.txt");
	out << "Lexical Analysis for " << argv[1] << "\n\n";
	out << std::left 
    	<< std::setw(20) << "Token" 
    	<< std::setw(30) << "Lexeme" 
    	<< std::setw(10) << "Line" 
    	<< std::setw(10) << "Column" << "\n"
    	<< std::string(70, '-') << "\n";
	for(auto token : tokens)
	{
		out << std::left 
			<< std::setw(20) << token.token_type
			<< std::setw(30) << token.lexeme
			<< std::setw(10) << token.line
			<< std::setw(10) << token.column << "\n";
	}
	return 0;
}