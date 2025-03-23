#include "AST.hpp"
#include <fstream>

std::ofstream dotfile;
extern bool print_ast;
void print_error(const std::string &message);
extern std::string astDir;

long long NodeCounter = 0;
std::string padding = "0.1";

void print_in_dotfile(const std::string &str)
{
    dotfile << str;
}

void beginAST(const std::string &inputFile)
{
    if (print_ast)
    {
        dotfile = std::ofstream(astDir + inputFile + "_AST.dot");
        if (!dotfile)
        {
            print_error("cannot open " + astDir + inputFile + "_AST.dot");
            return;
        }
        print_in_dotfile("digraph AST {\n\trankdir=TB;\n\tordering=out;\n");
    }
}

void endAST()
{
    if (print_ast)
    {
        print_in_dotfile("}\n");
        dotfile.close();
    }
}

void insertAttr(std::vector<Data> &v, Node *node, const std::string &str, bool isNode)
{
    v.push_back(Data{node, str, isNode});
}

std::vector<Data> *mergeAttrs(Node *node1, Node *node2, Node *node3, Node *node4)
{
    auto *attrs = new std::vector<Data>();
    if (node1) insertAttr(*attrs, node1, "", true);
    if (node2) insertAttr(*attrs, node2, "", true);
    if (node3) insertAttr(*attrs, node3, "", true);
    if (node4) insertAttr(*attrs, node4, "", true);
    return attrs;
}

Node *getNode(const std::string &str, std::vector<Data> *v)
{
    Node *node = new Node;
    node->node_id = ++NodeCounter;
    node->node_name = str;

    if (!v)
    { // Leaf Node
        std::string format;
        for (char c : str)
        {
            format += c;
            if (c == '\\')
                format += c;
        }
        node->node_name = format;
        if (str[0] == '"')
        { // String literal
            std::string s = format.substr(1, format.size() - 2);
            if (print_ast)
                print_in_dotfile("\t" + std::to_string(node->node_id) + " [label=\"\\\"" + s + "\\\"\" shape=box style=filled fillcolor=\"#98D8EF\" pad=" + padding + "]\n");
        }
        else
        {
            if (print_ast)
                print_in_dotfile("\t" + std::to_string(node->node_id) + " [label=\"" + node->node_name + "\" shape=box style=filled fillcolor=\"#98D8EF\" pad=" + padding + "]\n");
        }
    }
    else
    { // Non-leaf Node
        std::vector<long long> op_id;
        for (auto &data : *v)
        {
            if (!data.is_node)
            {
                long long opid = ++NodeCounter;
                op_id.push_back(opid);
                if (!data.str.empty())
                {
                    if (print_ast)
                        print_in_dotfile("\t" + std::to_string(opid) + " [label=\"" + data.str + "\" shape=box style=filled fillcolor=\"#98D8EF\" pad=" + padding + "]\n");
                }
            }
        }

        if (print_ast)
            print_in_dotfile("\t" + std::to_string(node->node_id) + " [label=\"" + node->node_name + "\" shape=box style=filled fillcolor=\"#F2EFE7\" pad=" + padding + "]\n");

        int j = 0;
        for (auto &data : *v)
        {
            if (data.is_node && data.node)
            {
                if (print_ast)
                    print_in_dotfile("\t" + std::to_string(node->node_id) + " -> " + std::to_string(data.node->node_id) + " [color=\"#578FCA\" pad=" + padding + "]\n");
            }
            if (!data.is_node && !data.str.empty())
            {
                if (print_ast)
                    print_in_dotfile("\t" + std::to_string(node->node_id) + " -> " + std::to_string(op_id[j]) + " [color=\"#578FCA\" pad=" + padding + "]\n");
                j++;
            }
        }
    }

    return node;
}