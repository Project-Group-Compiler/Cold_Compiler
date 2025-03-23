#pragma once
#ifndef AST_HPP
#define AST_HPP
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include "data_structures.hpp"
typedef std::pair<std::string, SymbolTableEntry*> qid;

struct Node
{
    long long node_id; // unique id
    std::string node_name;
    long long intVal;
    long long realVal;
    char charVal;
    std::string strVal;

    int size;
	std::string type;
	int expType = 0;	// 1 -> Variable, 2 -> Array, 3-> Function, 4 -> Constant, 5 -> string  
    bool is_error = false;
    qid place;
    std::string temp_name;
    std::vector<int> truelist;
    std::vector<int> falselist;
    std::vector<int> breaklist;
    std::vector<int> continuelist;
    std::vector<int> nextlist;
    std::vector<int> caselist;
    
    int argCount = 0;
};

struct Data
{
    Node *node;
    std::string str;
    bool is_node;
};

void beginAST(const std::string &inputFile);
void endAST();
void insertAttr(std::vector<Data> &v, Node *node, const std::string &str, bool isNode);
std::vector<Data> *mergeAttrs(Node *node1, Node *node2=nullptr, Node *node3 = nullptr, Node *node4 = nullptr);
Node *getNode(const std::string &str, std::vector<Data> *v = nullptr);

#endif
