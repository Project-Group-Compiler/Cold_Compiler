#include "AST.hpp"
#include <fstream>

std::ofstream dotfile;
extern bool print_ast;
void print_error(const std::string &message);
extern std::string outputDir;

long long NodeCounter = 0;
std::string padding = "0.1";

void beginAST(const std::string &inputFile)
{
    if(print_ast)
	{
		dotfile = std::ofstream(outputDir + inputFile + "_AST.dot");
		if (!dotfile)
		{
			print_error("cannot open " + outputDir + inputFile + "_AST.dot");
			return;
		}
        dotfile << "digraph AST {\n\trankdir=TB;\n\tordering=out;\n";
	}
}

void endAST()
{
    if (print_ast)
        dotfile << "}\n";
    	dotfile.close();
}

void insertAttr(std::vector<Data> &v, Node *node, const std::string &str, bool isNode)
{
    v.push_back(Data{node, str, isNode});
}

Node *createASTNode(const std::string &str, std::vector<Data> *v)
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
                dotfile << "\t" << node->node_id << " [label=\"\\\"" << s << "\\\"\" shape=box fillcolor=\"#98D8EF\" pad=" << padding << "]\n";
        }
        else
        {
            if (print_ast)
                dotfile << "\t" << node->node_id << " [label=\"" << node->node_name << "\" shape=box fillcolor=\"#98D8EF\" pad=" << padding << "]\n";
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
                        dotfile << "\t" << opid << " [label=\"" << data.str << "\" shape=box style=filled color=\"dodgerblue\" fillcolor=\"#9ACBD0\" pad=" << padding << "]\n";
                }
            }
        }

        if (print_ast)
            dotfile << "\t" << node->node_id << " [label=\"" << node->node_name << "\" shape=box style=filled color=\"#9ACBD0\" fillcolor=\"#F2EFE7\" pad=" << padding << "]\n";

        int j = 0;
        for (auto &data : *v)
        {
            if (data.is_node && data.node)
            {
                if (print_ast)
                    dotfile << "\t" << node->node_id << " -> " << data.node->node_id << " [color=\"#578FCA\" pad=" << padding << "]\n";
            }
            if (!data.is_node && !data.str.empty())
            {
                if (print_ast)
                    dotfile << "\t" << node->node_id << " -> " << op_id[j] << " [color=\"#578FCA\" pad=" << padding << "]\n";
                j++;
            }
        }
    }

    return node;
}