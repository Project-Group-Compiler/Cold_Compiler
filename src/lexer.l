
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include "AST.hpp"
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
void reportError();
int check_type();

int column = 1;
int line = 1;
std::string current_token_type;
bool has_error = false;
%}

%option noyywrap
%%

"/*"([^*]|(\*+[^*/]))*\*+\/	{ count();}

"//"[^\n]*		{ count(); }

"auto"          { count(); yylval.str = strdup(yytext); current_token_type="AUTO"; return(AUTO); }
"break"         { count(); yylval.str = strdup(yytext); current_token_type="BREAK"; return(BREAK); }
"case"          { count(); yylval.str = strdup(yytext); current_token_type="CASE"; return(CASE); }
"char"          { count(); yylval.str = strdup(yytext); current_token_type="CHAR"; return(CHAR); }
"const"         { count(); yylval.str = strdup(yytext); current_token_type="CONST"; return(CONST); }
"continue"      { count(); yylval.str = strdup(yytext); current_token_type="CONTINUE"; return(CONTINUE); }
"default"       { count(); yylval.str = strdup(yytext); current_token_type="DEFAULT"; return(DEFAULT); }
"do"            { count(); yylval.str = strdup(yytext); current_token_type="DO"; return(DO); }
"double"        { count(); yylval.str = strdup(yytext); current_token_type="DOUBLE"; return(DOUBLE); }
"else"          { count(); yylval.str = strdup(yytext); current_token_type="ELSE"; return(ELSE); }
"enum"          { count(); yylval.str = strdup(yytext); current_token_type="ENUM"; return(ENUM); }
"extern"        { count(); yylval.str = strdup(yytext); current_token_type="EXTERN"; return(EXTERN); }
"float"         { count(); yylval.str = strdup(yytext); current_token_type="FLOAT"; return(FLOAT); }
"for"           { count(); yylval.str = strdup(yytext); current_token_type="FOR"; return(FOR); }
"goto"          { count(); yylval.str = strdup(yytext); current_token_type="GOTO"; return(GOTO); }
"if"            { count(); yylval.str = strdup(yytext); current_token_type="IF"; return(IF); }
"int"           { count(); yylval.str = strdup(yytext); current_token_type="INT"; return(INT); }
"long"          { count(); yylval.str = strdup(yytext); current_token_type="LONG"; return(LONG); }
"register"      { count(); yylval.str = strdup(yytext); current_token_type="REGISTER"; return(REGISTER); }
"return"        { count(); yylval.str = strdup(yytext); current_token_type="RETURN"; return(RETURN); }
"short"         { count(); yylval.str = strdup(yytext); current_token_type="SHORT"; return(SHORT); }
"signed"        { count(); yylval.str = strdup(yytext); current_token_type="SIGNED"; return(SIGNED); }
"sizeof"        { count(); yylval.str = strdup(yytext); current_token_type="SIZEOF"; return(SIZEOF); }
"static"        { count(); yylval.str = strdup(yytext); current_token_type="STATIC"; return(STATIC); }
"struct"        { count(); yylval.str = strdup(yytext); current_token_type="STRUCT"; return(STRUCT); }
"switch"        { count(); yylval.str = strdup(yytext); current_token_type="SWITCH"; return(SWITCH); }
"typedef"       { count(); yylval.str = strdup(yytext); current_token_type="TYPEDEF"; return(TYPEDEF); }
"union"         { count(); yylval.str = strdup(yytext); current_token_type="UNION"; return(UNION); }
"unsigned"      { count(); yylval.str = strdup(yytext); current_token_type="UNSIGNED"; return(UNSIGNED); }
"until"         { count(); yylval.str = strdup(yytext); current_token_type="UNTIL"; return(UNTIL); }
"void"          { count(); yylval.str = strdup(yytext); current_token_type="VOID"; return(VOID); }
"volatile"      { count(); yylval.str = strdup(yytext); current_token_type="VOLATILE"; return(VOLATILE); }
"while"         { count(); yylval.str = strdup(yytext); current_token_type="WHILE"; return(WHILE); }
"class"          { count(); yylval.str = strdup(yytext); current_token_type="CLASS"; return(CLASS); }
"public"         { count(); yylval.str = strdup(yytext); current_token_type="PUBLIC"; return(PUBLIC); }
"private"        { count(); yylval.str = strdup(yytext); current_token_type="PRIVATE"; return(PRIVATE); }
"protected"      { count(); yylval.str = strdup(yytext); current_token_type="PROTECTED"; return(PROTECTED); }


{L}({L}|{D})*   { count(); yylval.str = strdup(yytext); current_token_type="IDENTIFIER"; return(check_type()); }

0[xX]{H}+{IS}?  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }
0{D}+{IS}?      { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }
{D}+{IS}?       { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }
L?'(\\.|[^\\'])+'  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }

{D}+{E}{FS}?    { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; return(CONSTANT); }

L?\"(\\.|[^\\"])*\"  { count(); yylval.str = strdup(yytext); current_token_type="STRING_LITERAL"; return(STRING_LITERAL); }

"..."          { count(); yylval.str = strdup(yytext); current_token_type="ELLIPSIS"; return(ELLIPSIS); }
">>="          { count(); yylval.str = strdup(yytext); current_token_type="RIGHT_ASSIGN"; return(RIGHT_ASSIGN); }
"<<="          { count(); yylval.str = strdup(yytext); current_token_type="LEFT_ASSIGN"; return(LEFT_ASSIGN); }
"+="           { count(); yylval.str = strdup(yytext); current_token_type="ADD_ASSIGN"; return(ADD_ASSIGN); }
"-="           { count(); yylval.str = strdup(yytext); current_token_type="SUB_ASSIGN"; return(SUB_ASSIGN); }
"*="           { count(); yylval.str = strdup(yytext); current_token_type="MUL_ASSIGN"; return(MUL_ASSIGN); }
"/="           { count(); yylval.str = strdup(yytext); current_token_type="DIV_ASSIGN"; return(DIV_ASSIGN); }
"%="           { count(); yylval.str = strdup(yytext); current_token_type="MOD_ASSIGN"; return(MOD_ASSIGN); }
"&="           { count(); yylval.str = strdup(yytext); current_token_type="AND_ASSIGN"; return(AND_ASSIGN); }
"^="           { count(); yylval.str = strdup(yytext); current_token_type="XOR_ASSIGN"; return(XOR_ASSIGN); }
"|="           { count(); yylval.str = strdup(yytext); current_token_type="OR_ASSIGN"; return(OR_ASSIGN); }
">>"           { count(); yylval.str = strdup(yytext); current_token_type="RIGHT_OP"; return(RIGHT_OP); }
"<<"           { count(); yylval.str = strdup(yytext); current_token_type="LEFT_OP"; return(LEFT_OP); }
"++"           { count(); yylval.str = strdup(yytext); current_token_type="INC_OP"; return(INC_OP); }
"--"           { count(); yylval.str = strdup(yytext); current_token_type="DEC_OP"; return(DEC_OP); }
"->"           { count(); yylval.str = strdup(yytext); current_token_type="PTR_OP"; return(PTR_OP); }
"&&"           { count(); yylval.str = strdup(yytext); current_token_type="AND_OP"; return(AND_OP); }
"||"           { count(); yylval.str = strdup(yytext); current_token_type="OR_OP"; return(OR_OP); }
"<="           { count(); yylval.str = strdup(yytext); current_token_type="LE_OP"; return(LE_OP); }
">="           { count(); yylval.str = strdup(yytext); current_token_type="GE_OP"; return(GE_OP); }
"=="           { count(); yylval.str = strdup(yytext); current_token_type="EQ_OP"; return(EQ_OP); }
"!="           { count(); yylval.str = strdup(yytext); current_token_type="NE_OP"; return(NE_OP); }
";"            { count(); yylval.str = strdup(yytext); current_token_type=";"; return(';'); }
("{"|"<%")     { count(); yylval.str = strdup(yytext); current_token_type="{"; return('{'); }
("}"|"%>")     { count(); yylval.str = strdup(yytext); current_token_type="}"; return('}'); }
","            { count(); yylval.str = strdup(yytext); current_token_type=","; return(','); }
":"            { count(); yylval.str = strdup(yytext); current_token_type=":"; return(':'); }
"="            { count(); yylval.str = strdup(yytext); current_token_type="="; return('='); }
"("            { count(); yylval.str = strdup(yytext); current_token_type="("; return('('); }
")"            { count(); yylval.str = strdup(yytext); current_token_type=")"; return(')'); }
("["|"<:")     { count(); yylval.str = strdup(yytext); current_token_type="["; return('['); }
("]"|":>")     { count(); yylval.str = strdup(yytext); current_token_type="]"; return(']'); }
"."            { count(); yylval.str = strdup(yytext); current_token_type="."; return('.'); }
"&"            { count(); yylval.str = strdup(yytext); current_token_type="&"; return('&'); }
"!"            { count(); yylval.str = strdup(yytext); current_token_type="!"; return('!'); }
"~"            { count(); yylval.str = strdup(yytext); current_token_type="~"; return('~'); }
"-"            { count(); yylval.str = strdup(yytext); current_token_type="-"; return('-'); }
"+"            { count(); yylval.str = strdup(yytext); current_token_type="+"; return('+'); }
"*"            { count(); yylval.str = strdup(yytext); current_token_type="*"; return('*'); }
"/"            { count(); yylval.str = strdup(yytext); current_token_type="/"; return('/'); }
"%"            { count(); yylval.str = strdup(yytext); current_token_type="%"; return('%'); }
"<"            { count(); yylval.str = strdup(yytext); current_token_type="<"; return('<'); }
">"            { count(); yylval.str = strdup(yytext); current_token_type=">"; return('>'); }
"^"            { count(); yylval.str = strdup(yytext); current_token_type="^"; return('^'); }
"|"            { count(); yylval.str = strdup(yytext); current_token_type="|"; return('|'); }
"?"            { count(); yylval.str = strdup(yytext); current_token_type="?"; return('?'); }
"<-"   { count(); yylval.str = strdup(yytext); current_token_type = "INHERITANCE_OP"; return(INHERITANCE_OP); }

[ \t\v\n\f]	   { count(); }

.			   { count(); reportError();}

%%

void reportError() {
    has_error = true;
	std::cerr<<"Error: Unknown token "<<yytext<<" at line "<<line<<" column "<<column - yyleng<<"\n";
}

void count()
{
	for (int i = 0; yytext[i] != '\0'; i++)
	{
		if (yytext[i] == '\n')
			column = 1, line++;
		else if (yytext[i] == '\t') // tab size : 4
			column += 4 - (column % 4);
		else
			column++;
	}
}


int check_type()
{
	return(IDENTIFIER);
}

// int main(int argc, char *argv[])
// {
// 	if(argc != 3)
// 	{
// 		std::cerr << "Usage : ./lexer <input_file> <output_file>\n";
// 		return 1;
// 	}
// 	yyin = fopen(argv[1], "r");
// 	if(!yyin)
// 	{
// 		std::cerr << "Error : Can't open input file "<<argv[1]<<"\n";
// 		return 1;
// 	}
// 	std::ofstream out(argv[2]);
// 	if(!out)
// 	{
// 		std::cerr << "Error : Can't open output file "<<argv[2]<<"\n";
// 		return 1;
// 	}
// 	std::unordered_map<std::string, TOKEN> symbol_table;
// 	std::vector<TOKEN> tokens;
// 	int token_id;
// 	while((token_id = yylex()) != 0)
// 	{
// 		TOKEN token;
// 		token.token_type = current_token_type;
// 		token.lexeme = std::string(yytext);
// 		token.line = line;
// 		token.column = column - yyleng;
// 		if(symbol_table.find(token.lexeme) == symbol_table.end())
// 			symbol_table[token.lexeme] = token;	
// 		tokens.push_back(token);
// 	}
// 	if (has_error)
// 	{
// 		out << "Lexical errors printed on stdout.\n";
// 		return 1;
// 	}
// 	out << "Lexical Analysis for " << argv[1] << "\n\n";
// 	out << std::left 
//     	<< std::setw(20) << "Token" 
//     	<< std::setw(40) << "Lexeme" 
//     	<< std::setw(10) << "Line" 
//     	<< std::setw(10) << "Column" << "\n"
//     	<< std::string(80, '-') << "\n";
// 	for(auto &token : tokens)
// 	{
// 		out << std::left 
// 			<< std::setw(20) << token.token_type
// 			<< std::setw(40) << token.lexeme
// 			<< std::setw(10) << token.line
// 			<< std::setw(10) << token.column << "\n";
// 	}
// 	return 0;
// }