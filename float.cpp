int main(){
    int a = 5;
    float b = 3.14;
    char c = a;
    void* x = &a;
    int* y= (int*) x;
    b = (float) c;
    printf("%d\n", a);
}


void compute_basic_blocks()
{
    basic_blocks.clear();
    std::vector<quad> block;
    for (auto &instr : tac_code)
    {
        const std::string &curr_op = instr.op;
        if(curr_op.substr(0, 5) == "FUNC_" && curr_op.substr(curr_op.length() - 7) ==  "start :"){
            if (!block.empty())
            {
                basic_blocks.push_back(block);
                block.clear();
            }
            block.push_back(instr);
            basic_blocks.push_back(block);
            block.clear();
        }
        else if (curr_op.back() == ':')
        {
            if (!block.empty())
            {
                basic_blocks.push_back(block);
                block.clear();
            }
            block.push_back(instr);
        }
        else if ((curr_op == "GOTO") || curr_op == "RETURN" )
        {
            block.push_back(instr);
            basic_blocks.push_back(block);
            block.clear();
        }
        else if ((curr_op.substr(0, 5) == "FUNC_" && curr_op.substr(curr_op.length() - 3) == "end")){
            if (!block.empty())
            {
                basic_blocks.push_back(block);
                block.clear();
            }
            block.push_back(instr);
            basic_blocks.push_back(block);
            block.clear();
        }
        else
            block.push_back(instr);
    }
    if (!block.empty())
        basic_blocks.push_back(block);
}