#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

struct Node
{
    long long node_id; // unique id
    std::string node_name;
    long long intVal;
    long long realVal;
    char charVal;
    std::string strVal;
    //semantics addition
    std::string type;
	int expType;	// 1 -> Variable, 2 -> Array, 3-> Function, 4 -> Constant, 5 -> string  			 	
	int isInit;
    std::string temp_name;
    long long int size;
};

struct Data
{
    Node *node;
    std::string str;
    bool is_node;
};

struct constants{
	std::string str;
	std::string type;
	long long int intVal;
	long double realVal;
	std::string strVal;
};

void beginAST(const std::string &inputFile);
void endAST();
void insertAttr(std::vector<Data> &v, Node *node, const std::string &str, bool isNode);
Node *createASTNode(const std::string &str, std::vector<Data> *v = nullptr);

#endif // AST_HPP
