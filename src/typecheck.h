#include "symbol_table.h"
#include <bits/stdc++.h>
using namespace std;

string primaryExpression(string id);
string constantEx(int numType );
string postfixExpression(string type_name, int rule_num);
string checkType(string a, string b);
string argExp(string a, string b, int  rule_num);
string unaryExp(string op, string type);
string mulExp(string a, string b, char op);
string addExp(string a, string b, char op);
string shiftExp(string a, string b);
string relExp(string a, string b);
string eqExp(string a, string b);
string bitExp(string a, string b);
string assignExp(string a, string b, string op);
string condExp(string a,string b);
int checkInt (string type1);
bool isFloat (string type);