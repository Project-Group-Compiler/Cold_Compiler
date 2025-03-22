#include "tac.hpp"
#include <map>

bool is_int_constant(const std::string &s);
void addgotoLabels();
void constant_folding();
void print_tac_code(const std::string &inputFile);
void run_optimisations();
void build_cfg();
void print_cfg();

void program1()
{
    tac_code.push_back(quad(0, qid("=", nullptr), qid("9", nullptr), qid("", nullptr), qid("a", nullptr), -1));
    tac_code.push_back(quad(1, qid("FUNC_main start :", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(2, qid("=", nullptr), qid("9", nullptr), qid("", nullptr), qid("x", nullptr), -1));
    tac_code.push_back(quad(3, qid("GOTO", nullptr), qid("IF", nullptr), qid("1", nullptr), qid("", nullptr), 4+1));
    tac_code.push_back(quad(4, qid("GOTO", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), 6+1));
    tac_code.push_back(quad(5, qid("++S", nullptr), qid("x", nullptr), qid("", nullptr), qid("#V0", nullptr), -1));
    tac_code.push_back(quad(6, qid("GOTO", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), 7+1));
    tac_code.push_back(quad(7, qid("--S", nullptr), qid("x", nullptr), qid("", nullptr), qid("#V0", nullptr), -1));
    tac_code.push_back(quad(8, qid("RETURN", nullptr), qid("0", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(9, qid("FUNC_main end :", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(10, qid("=", nullptr), qid("9", nullptr), qid("", nullptr), qid("b", nullptr), -1));
}

/*
for :
int target() {
    do {
    } while (0);

    return 99;
}
*/

void program2()
{
    tac_code.push_back(quad(0 + 11, qid("FUNC_target1 start :", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(1 + 11, qid("GOTO", nullptr), qid("IF", nullptr), qid("0", nullptr), qid("", nullptr), 1 + 11));
    tac_code.push_back(quad(2 + 11, qid("GOTO", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), 3 + 11));
    tac_code.push_back(quad(3 + 11, qid("RETURN", nullptr), qid("99", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(4 + 11, qid("FUNC_target1 end :", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
}

void program3()
{
    tac_code.push_back(quad(15, qid("=", nullptr), qid("9", nullptr), qid("", nullptr), qid("c", nullptr), -1));
    tac_code.push_back(quad(0 + 17, qid("FUNC_target2 start :", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(1 + 17, qid("=", nullptr), qid("9", nullptr), qid("", nullptr), qid("x", nullptr), -1));
    tac_code.push_back(quad(2 + 17, qid("GOTO", nullptr), qid("IF", nullptr), qid("0", nullptr), qid("", nullptr), 4 + 17));
    tac_code.push_back(quad(3 + 17, qid("GOTO", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), 6 + 17));
    tac_code.push_back(quad(4 + 17, qid("=", nullptr), qid("5", nullptr), qid("", nullptr), qid("x", nullptr), -1));
    tac_code.push_back(quad(5 + 17, qid("GOTO", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), 2 + 17));
    tac_code.push_back(quad(6 + 17, qid("RETURN", nullptr), qid("x", nullptr), qid("", nullptr), qid("", nullptr), -1));
    tac_code.push_back(quad(7 + 17, qid("FUNC_target2 end :", nullptr), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
}

int main()
{
    tac_code.clear();
    program1();
    program2();
    program3();
    print_tac_code("hello");
    run_optimisations();
}