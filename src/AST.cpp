#include "AST.hpp"
#include <iostream>

extern FILE* dotfile;

long long NodeCounter = 0;

void beginAST() {
    std::fprintf(dotfile, "digraph AST {\n\trankdir=TB;\n\tordering=out;\n");
}

void endAST() {
    std::fprintf(dotfile, "}\n");
}

void insertAttr(std::vector<Data>& v, Node* node, const std::string& str, bool isNode) {
    v.push_back(Data{node, str, isNode});
}

Node* createASTNode(const std::string& str, std::vector<Data>* v) {
    Node* node = new Node;
    node->node_id = ++NodeCounter;
    node->node_name = str;

    if (!v) { // Leaf Node
        std::string format;
        for (char c : str) {
            format += c;
            if (c == '\\') format += c;
        }
        node->node_name = format;
        if (str[0] == '"') { // String literal
            std::string s = format.substr(1, format.size() - 2);
            std::fprintf(dotfile, "\t%lld [label=\"\\\"%s\\\"\" shape=box style=filled color=\"dodgerblue\" fillcolor=\"lightyellow\"]\n", 
                         node->node_id, s.c_str());
        } else {
            std::fprintf(dotfile, "\t%lld [label=\"%s\" shape=box style=filled color=\"dodgerblue\" fillcolor=\"lightyellow\"]\n", 
                         node->node_id, node->node_name.c_str());
        }
    } else { // Non-leaf Node
        std::vector<long long> op_id;
        for (auto& data : *v) {
            if (!data.is_node) {
                long long opid = ++NodeCounter;
                op_id.push_back(opid);
                if (!data.str.empty()) {
                    std::fprintf(dotfile, "\t%lld [label=\"%s\"]\n", opid, data.str.c_str());
                }
            }
        }
        
        std::fprintf(dotfile, "\t%lld [label=\"%s\"]\n", node->node_id, node->node_name.c_str());
        
        int j = 0;
        for (auto& data : *v) {
            if (data.is_node && data.node) {
                std::fprintf(dotfile, "\t%lld -> %lld\n", node->node_id, data.node->node_id);
            }
            if (!data.is_node && !data.str.empty()) {
                std::fprintf(dotfile, "\t%lld -> %lld\n", node->node_id, op_id[j]);
                j++;
            }
        }
    }

    return node;
}