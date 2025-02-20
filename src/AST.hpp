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
Node *createASTNode(const std::string &str, std::vector<Data> *v = nullptr);

#endif
