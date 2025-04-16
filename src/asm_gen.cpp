#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "x86_32_lib.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

template <typename T>
void __print__(const T &x)
{
    std::cerr << x;
}

template <typename T, typename... Args>
void __print__(const T &x, const Args &...rest)
{
    std::cerr << x << ", ";
    __print__(rest...);
}
#define _debug_(x...)                                             \
    std::cerr << "(Line " << __LINE__ << "): [" << #x << "] => "; \
    __print__(x);                                                 \
    std::cerr << std::endl;

std::ofstream asm_file;

std::map<std::string, int> string_literals;
bool inside_fn = false;

enum
{
    EAX = 0,
    EBX = 1,
    ECX = 2,
    EDX = 3,
    ESI = 4,
    EDI = 5,
    ESP = 6,
    EBP = 7
};

std::string reg_names[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp"};

int uRegCnt = 6; // usableRegCnt
std::vector<std::vector<operand>> regDesc(uRegCnt);

/*
curr handle
x = y op z
x = y

Input 3AC I:x = yopz
Output Returns registers to hold the value of x, y, and z
Assumption There is no global register allocation
*/
std::set<operand> seenOperand;

void updateSeenOperand(quad &instr){
    if(instr.arg1.entry){
        seenOperand.insert(instr.arg1);
    }
    if(instr.arg2.entry){
        seenOperand.insert(instr.arg2);
    }
    if(instr.result.entry){
        seenOperand.insert(instr.result);
    }
}

void printReg_addr_Desc(int currInstrLabel) {
    std::ofstream out;
    out.open("Descriptor.txt");
    out << "Instruction Label: " << currInstrLabel << "\n\n";

    out << "REGISTER DESCRIPTOR\n";
    out << std::left << std::setw(12) << "Register" << "Operands\n";
    out << std::string(40, '-') << "\n";

    std::map<std::string, std::vector<int>> ops;
    std::map<std::string, bool> opsinmem;
    for (int reg = 0; reg < uRegCnt; reg++) {
        out << std::left << std::setw(12) << reg_names[reg];
        for (auto& op : regDesc[reg]) {
            out << op.value << ' ';
            ops[op.value].push_back(reg);
            if(op.entry->addrDesc.inStack){
                opsinmem[op.value] = 1;
            }
        }
        out << '\n';
    }

    out << "\nADDRESS DESCRIPTOR\n";
    out << std::left << std::setw(20) << "Operand" << "Locations\n";
    out << std::string(40, '-') << "\n";

    for(auto& it:seenOperand){
        out << std::left << std::setw(20) << it.value;
        for(auto reg:it.entry->addrDesc.inRegs){
            out << reg_names[reg] << ' ';
        }
        if(it.entry->addrDesc.inStack){
            out << "| In Memory";
        }
        out << '\n';
    }
}
std::string getMem(operand &op)
{
    op.entry->addrDesc.inStack = 1;
    return "mem_"+op.value;
}

// Stop after removing one, since all elements are unique
template <typename T>
void eraseFromVector(std::vector<T> &vec, const T &value)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == value)
        {
            vec.erase(vec.begin() + i);
            break;
        }
    }
}

// Make sure to use op.isLive not op->entry.isLive
// Store only that op that islive and not present other than this reg
// Check: op.entry null ho skti hai kya at this point

void spillReg(int reg)
{
    for (auto &op : regDesc[reg])
    {
        eraseFromVector(op.entry->addrDesc.inRegs, reg);
        if (op.isLive)
        {
            // if (op.entry->isGlobal || op.entry->isStatic > 0)//check
            // {
            //     emit_instr(x86_lib::mov_mem_reg(op.value, reg_names[reg]));
            // }
            if (!(op.entry->addrDesc.inRegs.size() || op.entry->addrDesc.inStack || op.entry->addrDesc.inHeap || op.entry->isGlobal || op.entry->isStatic > 0))
            {
                std::string memAddr = getMem(op);
                op.entry->addrDesc.inStack = 1;//check
                emit_instr(x86_lib::mov_mem_reg(memAddr, reg_names[reg]));   
            }
        }
    }

    regDesc[reg].clear();
}

int getBestReg()
{
    for (int reg = 0; reg < uRegCnt; reg++)
    {
        if (regDesc[reg].size() == 0)
            return reg;
    }

    int bestReg = 0;
    int minCnt = 100;
    for (int reg = 0; reg < uRegCnt; reg++)
    {
        int cnt = 0;
        for (auto &op : regDesc[reg])
        {
            if (op.isLive)
            {
                if (op.entry == NULL || !(op.entry->addrDesc.inRegs.size() > 1 || op.entry->addrDesc.inStack || op.entry->addrDesc.inHeap))
                    cnt++;
            }
        }
        if (minCnt > cnt)
        {
            minCnt = cnt;
            bestReg = reg;
        }
    }

    spillReg(bestReg);
    return bestReg;
}

// Check reference
// Warning : make sure that addrDesc and regDesc contain unique entry
int getReg(operand &op, bool willYouModify)
{
    if (!op.entry)
        return -1;
    auto &addrDesc = op.entry->addrDesc;

    if (willYouModify == 0)
    {
        if (addrDesc.inRegs.size())
            return addrDesc.inRegs[0];
        return -1;
    }

    if (addrDesc.inRegs.size())
    {
        int bestReg = addrDesc.inRegs[0];
        int minCnt = 100;
        for (auto reg : addrDesc.inRegs)
        {
            int cnt = 0;
            for (auto tempOp : regDesc[reg])
            {
                if (tempOp.isLive)
                {
                    if (tempOp.entry == NULL || !((tempOp.entry->addrDesc.inRegs.size() > 1) || tempOp.entry->addrDesc.inStack || tempOp.entry->addrDesc.inHeap))
                        cnt++;
                }
            }
            if (minCnt > cnt)
            {
                minCnt = cnt;
                bestReg = reg;
            }
        }

        spillReg(bestReg);
        return bestReg;
    }
    else
    {
        int bestReg = getBestReg();
        std::string memAddr = getMem(op);
        if(op.entry->isGlobal || op.entry->isStatic > 0)
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], op.value));//[lexeme]
        }
        else emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], memAddr));

        regDesc[bestReg].push_back(op);
        op.entry->addrDesc.inRegs.push_back(bestReg);

        // TODO: Handle __str_

        return bestReg;
    }
}
void updateRegDesc_assign(int reg, operand &op) 
{
    if(op.entry->isGlobal || op.entry->isStatic > 0)
    {
        emit_instr(x86_lib::mov_mem_reg(op.value,reg_names[reg]));
    }
    
    for (int i = 0; i < uRegCnt; i++)
    eraseFromVector(regDesc[i], op);
    regDesc[reg].push_back(op);    
    op.entry->addrDesc.inRegs.clear();
    op.entry->addrDesc.inRegs.push_back(reg);
    op.entry->addrDesc.inStack = 0;
    op.entry->addrDesc.inHeap = 0;
    
}
void updateRegDesc(int reg, operand &op)
{
    if(op.entry->isGlobal || op.entry->isStatic > 0)
    {
        emit_instr(x86_lib::mov_mem_reg(op.value,reg_names[reg]));
    }
    for (auto &top : regDesc[reg])
        eraseFromVector(top.entry->addrDesc.inRegs, reg);

    for (int i = 0; i < uRegCnt; i++)
        eraseFromVector(regDesc[i], op);


    regDesc[reg].clear();
    regDesc[reg].push_back(op);
    op.entry->addrDesc.inRegs.clear();
    op.entry->addrDesc.inRegs.push_back(reg);
    op.entry->addrDesc.inStack = 0;
    op.entry->addrDesc.inHeap = 0;
}

void emit_asm(const std::string &inputFile)
{
    asm_file = std::ofstream(outputDir + inputFile + ".asm");
    if (!asm_file)
    {
        print_error("cannot open " + outputDir + inputFile + ".asm");
        return;
    }

    if (tac_code.empty())
        return;

    compute_basic_blocks();
    // print_basic_blocks();
    // print_tac_code(inputFile);

    add_extern_funcs();
    emit_section(".text");
    emit_data("global main");

    for (auto &block : basic_blocks)
    {
        next_use_analysis(block);
        emit_instr("# Block begins");
        for (auto &instr : block)
        {
            updateSeenOperand(instr);
            const std::string curr_op = instr.op;
            if (curr_op.substr(0, 5) == "FUNC_")
            {
                if (curr_op.substr(curr_op.length() - 7) == "start :")
                {
                    inside_fn = true;
                    emit_fn_defn(instr);
                }
                else
                    inside_fn = false;
            }
            else if (curr_op.back() == ':')
                emit_label(curr_op.substr(0, curr_op.length() - 1));
            else if (curr_op == "=" || curr_op == "(f)=")
                emit_assign(instr);
            else if (curr_op == "+")
                emit_add(instr);
            else if (curr_op == "-")
                emit_sub(instr);
            else if(curr_op=="*")

            printReg_addr_Desc(instr.Label);
        }
        emit_instr("# spilling all registers");
        for(int reg=0;reg<uRegCnt;reg++){
            spillReg(reg);
        }
    }

    emit_section(".data");
    emit_data_section(); // add initialized data
    emit_section(".bss");
    emit_bss_section(); // add uninitialized data
}

void emit_fn_defn(quad &instr)
{
    if (instr.op.substr(5, 5) == "4main")
        emit_label("\nmain ");
    else
        emit_label("\n" + instr.op.substr(0, instr.op.length() - 7));

    // TODO : add function prologue
}

void emit_assign(quad &instr)
{
    if (!inside_fn) // global
        return;
    if(is_int_constant(instr.arg1.value))
    {  //can use mov_mem_imm for more optimal code
        if(instr.result.entry->isGlobal || instr.result.entry->isStatic > 0)
        {
            emit_instr(x86_lib::mov_mem_imm("dword",instr.result.value, instr.arg1.value));
        }
        else
        {
            std::string mem = getMem(instr.result);
            emit_instr(x86_lib::mov_mem_imm("dword",mem, instr.arg1.value));
        }
    }
    else{
        int reg2= getReg(instr.arg1, 1);
        updateRegDesc_assign(reg2, instr.result);
        

        // std::cout<<"here"<<std::endl;
        // int reg2= getReg(instr.arg1, 0);
        // if (reg2 != -1)
        // {
        //     std::cout<<"here2"<<std::endl;
        //     updateRegDesc_assign(reg2, instr.result);
        // }
        // else
        // {
        //     std::cout<<"here3"<<std::endl;
        //     std::string mem = getMem(instr.arg1);
        //     int reg1= getReg(instr.result, 1);
        //     emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], mem));
        //     updateRegDesc(reg1, instr.result);
        // }
    }

}

void emit_sub(quad &instr)
{
    // x = y - z
    int reg1 = getReg(instr.arg1, 1);
    if(is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::sub_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if(instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::sub_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else{
        int reg2 = getReg(instr.arg2, 0);
        if (reg2 != -1)
        {
            emit_instr(x86_lib::sub(reg_names[reg1], reg_names[reg2]));
        }
        else
        {
            std::string mem = getMem(instr.arg2);
            emit_instr(x86_lib::sub_reg_mem(reg_names[reg1], mem));
        }
    }
    updateRegDesc(reg1, instr.result);
}
void emit_add(quad &instr)
{
    // x = y + z
    int reg1 = getReg(instr.arg1, 1);
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::add_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if(instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::add_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else{
        int reg2 = getReg(instr.arg2, 0);
        // _debug_(stringify(instr));
        // _debug_(reg1,reg2);
        if (reg2 != -1)
        { // emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], memAddr));
            emit_instr(x86_lib::add(reg_names[reg1], reg_names[reg2]));
        }
        else
        {
            std::string mem = getMem(instr.arg2);
            emit_instr(x86_lib::add_reg_mem(reg_names[reg1], mem));
        }
    }
    updateRegDesc(reg1, instr.result);
}

void get_string_literals()
{
    int str_cnt = 0;
    for (auto &instr : tac_code)
    {
        if (instr.arg1.value.length() > 0 && instr.arg1.value[0] == '\"')
        {
            if (string_literals.find(instr.arg1.value) == string_literals.end())
                string_literals[instr.arg1.value] = str_cnt++;
            instr.arg1.value = "__str_" + std::to_string(string_literals[instr.arg1.value]);
        }
        if (instr.arg2.value.length() > 0 && instr.arg2.value[0] == '\"')
        {
            if (string_literals.find(instr.arg2.value) == string_literals.end())
                string_literals[instr.arg2.value] = str_cnt++;
            instr.arg2.value = "__str_" + std::to_string(string_literals[instr.arg2.value]);
        }
        if (instr.result.value.length() > 0 && instr.result.value[0] == '\"')
        {
            if (string_literals.find(instr.result.value) == string_literals.end())
                string_literals[instr.result.value] = str_cnt++;
            instr.result.value = "__str_" + std::to_string(string_literals[instr.result.value]);
        }
    }
}

void global_init_pass()
{
    bool in_fn = false;
    // replace static variables with modified names
    for (auto &instr : tac_code)
    {
        const std::string curr_op = instr.op;
        if (curr_op.substr(0, 5) == "FUNC_")
        {
            if (curr_op.substr(curr_op.length() - 7) == "start :")
                in_fn = true;
            else
                in_fn = false;
        }
        else
        {
            if (in_fn)
            {
                if (instr.result.entry && instr.result.entry->isStatic > 0)
                {
                    instr.result.value = "_s_" + instr.result.value;
                    if (!instr.result.entry->isGlobal)
                        instr.result.value += "_" + std::to_string(instr.result.entry->isStatic);
                }
            }
            if (instr.arg1.entry && instr.arg1.entry->isStatic > 0)
            {
                instr.arg1.value = "_s_" + instr.arg1.value;
                if (!instr.arg1.entry->isGlobal)
                    instr.arg1.value += "_" + std::to_string(instr.arg1.entry->isStatic);
            }
            if (instr.arg2.entry && instr.arg2.entry->isStatic > 0)
            {
                instr.arg2.value = "_s_" + instr.arg2.value;
                if (!instr.arg2.entry->isGlobal)
                    instr.arg2.value += "_" + std::to_string(instr.arg2.entry->isStatic);
            }
        }
    }
    in_fn = false;
    for (const auto &instr : tac_code)
    {
        const std::string curr_op = instr.op;
        if (curr_op.substr(0, 5) == "FUNC_")
        {
            if (curr_op.substr(curr_op.length() - 7) == "start :")
                in_fn = true;
            else
                in_fn = false;
        }
        else if (curr_op == "=" || curr_op == "(f)=")
        {
            if (!in_fn)
                global_init[instr.result.value] = instr.arg1.value;
        }
    }

    // Replace enums with their corresponding values
    for (auto &instr : tac_code)
    {
        if (instr.arg1.entry && instr.arg1.entry->isEnum)
            instr.arg1.value = global_init[instr.arg1.value];
        if (instr.arg2.entry && instr.arg2.entry->isEnum)
            instr.arg2.value = global_init[instr.arg2.value];
    }
}

void update_ir()
{
    addgotoLabels();
    get_string_literals();
    global_init_pass();
}

void emit_data_section()
{
    for (auto &[name, entry] : gst)
    {
        if (entry->isGlobal && !entry->isStatic && !entry->isEnum && entry->init)
        {
            if (entry->type == "char")
                emit_data(name + ": db " + global_init[name]);
            else if (entry->type == "int" || entry->type == "float")
                emit_data(name + ": dd " + global_init[name]);
            else if (entry->type == "char*")
            {
                if (entry->isArray)
                {
                    // Initialized array pending as of now
                }
                else
                {
                    std::string init_val = global_init[name];
                    emit_data(name + ": dd " + init_val);
                }
            }
            else if (entry->type.back() == '*')
            {
                if (entry->isArray)
                {
                    // Initialized array pending as of now
                }
                else
                {
                    std::string init_val = global_init[name];
                    emit_data(name + ": dd " + init_val);
                }
            }
            // Initialized structs, unions and classes pending as of now
        }
    }

    for (const auto &op : static_vars)
    {
        auto entry = op.entry;
        std::string act_name = "_s_" + op.value;
        if (!entry->isGlobal)
            act_name += "_" + std::to_string(entry->isStatic);

        if (entry->init && !entry->isEnum)
        {
            if (entry->type == "char")
                emit_data(act_name + ": db " + global_init[act_name]);
            else if (entry->type == "int" || entry->type == "float")
                emit_data(act_name + ": dd " + global_init[act_name]);
            else if (entry->type == "char*")
            {
                if (entry->isArray)
                {
                    // Initialized array pending as of now
                }
                else
                {
                    std::string init_val = global_init[act_name];
                    emit_data(act_name + ": dd " + init_val);
                }
            }
            else if (entry->type.back() == '*')
            {
                if (entry->isArray)
                {
                    // Initialized array pending as of now
                }
                else
                {
                    std::string init_val = global_init[act_name];
                    emit_data(act_name + ": dd " + init_val);
                }
            }
            // Initialized structs, unions and classes pending as of now
        }
    }
    for (auto i = 0; i < string_literals.size(); i++)
    {
        for (const auto &[str, cnt] : string_literals)
        {
            if (cnt == i)
                emit_data("__str_" + std::to_string(cnt) + ": db " + str + ", 0");
        }
    }
}

void emit_bss_section()
{
    for (auto &[name, entry] : gst)
    {
        if (entry->isGlobal && !entry->isStatic && !entry->isEnum && !entry->init)
        {
            if (entry->type == "char")
                emit_data(name + ": resb 1");
            else if (entry->type == "int" || entry->type == "float")
                emit_data(name + ": resd 1");
            else if (entry->type == "char*")
            {
                if (entry->isArray)
                    emit_data(name + ": resb " + std::to_string(entry->size));
                else
                    emit_data(name + ": resd 1");
            }
            else if (entry->type.back() == '*')
            {
                if (entry->isArray)
                    emit_data(name + ": resd " + std::to_string(entry->size / 4));
                else
                    emit_data(name + ": resd 1");
            }
            else if (entry->type.substr(0, 6) == "UNION_")
            {
                if (entry->size == 1)
                    emit_data(name + ": resb 1");
                else
                    emit_data(name + ": resd " + std::to_string(entry->size / 4));
            }
            else if (entry->type.substr(0, 7) == "STRUCT_" || entry->type.substr(0, 6) == "CLASS_")
                emit_data(name + ": resd " + std::to_string(entry->size / 4));
        }
    }

    for (const auto &op : static_vars)
    {
        auto entry = op.entry;
        std::string act_name = "_s_" + op.value;
        if (!entry->isGlobal)
            act_name += "_" + std::to_string(entry->isStatic);

        if (!entry->init && !entry->isEnum)
        {
            if (entry->type == "char")
                emit_data(act_name + ": resb 1");
            else if (entry->type == "int" || entry->type == "float")
                emit_data(act_name + ": resd 1");
            else if (entry->type == "char*")
            {
                if (entry->isArray)
                    emit_data(act_name + ": resb " + std::to_string(entry->size));
                else
                    emit_data(act_name + ": resd 1");
            }
            else if (entry->type.back() == '*')
            {
                if (entry->isArray)
                    emit_data(act_name + ": resd " + std::to_string(entry->size / 4));
                else
                    emit_data(act_name + ": resd 1");
            }
            else if (entry->type.substr(0, 6) == "UNION_")
            {
                if (entry->size == 1)
                    emit_data(act_name + ": resb 1");
                else
                    emit_data(act_name + ": resd " + std::to_string(entry->size / 4));
            }
            else if (entry->type.substr(0, 7) == "STRUCT_" || entry->type.substr(0, 6) == "CLASS_")
                emit_data(act_name + ": resd " + std::to_string(entry->size / 4));
        }
    }
}

void next_use_analysis(std::vector<quad> &block)
{
    // Assume symbol table shows all non-temporary and temporaries variables in B as live on exit
    for (int j = 0; j < block.size(); j++)
    {
        quad &I = block[j];
        if (I.arg1.entry != NULL)
        {
            I.arg1.entry->isLive = 1;
            I.arg1.entry->nextUse = -1;
        }

        if (I.arg2.entry != NULL)
        {
            I.arg2.entry->isLive = 1;
            I.arg2.entry->nextUse = -1;
        }

        if (I.result.entry != NULL)
        {
            I.result.entry->isLive = 1;
            I.result.entry->nextUse = -1;
        }
    }

    for (int j = (int)block.size() - 1; j >= 0; j--)
    {
        quad &I = block[j];
        if (I.arg1.entry != NULL)
        {
            I.arg1.isLive = I.arg1.entry->isLive;
            I.arg1.nextUse = I.arg1.entry->nextUse;
        }

        if (I.arg2.entry != NULL)
        {
            I.arg2.isLive = I.arg2.entry->isLive;
            I.arg2.nextUse = I.arg2.entry->nextUse;
        }

        if (I.result.entry != NULL)
        {
            I.result.isLive = I.result.entry->isLive;
            I.result.nextUse = I.result.entry->nextUse;
        }

        if (I.arg1.entry != NULL)
        {
            I.arg1.entry->isLive = 1;
            I.arg1.entry->nextUse = I.Label;
        }

        if (I.arg2.entry != NULL)
        {
            I.arg2.entry->isLive = 1;
            I.arg2.entry->nextUse = I.Label;
        }

        if (I.result.entry != NULL)
        {
            I.result.entry->isLive = 0;
            I.result.entry->nextUse = -1;
        }
    }
}

void print_next_use()
{
    std::cout << "NEXT USE INFO\n";
    int block_no = 0;
    for (auto &block : basic_blocks)
    {
        std::cout << "Block " << block_no++ << ":\n";
        for (auto &instr : block)
        {
            std::cout << stringify(instr) << std::endl;
            if (instr.result.entry)
                std::cout << '\t' << instr.result.value << ": " << instr.result.isLive << ' ' << instr.result.nextUse << '\n';
            if (instr.arg1.entry)
                std::cout << '\t' << instr.arg1.value << ": " << instr.arg1.isLive << ' ' << instr.arg1.nextUse << '\n';
            if (instr.arg2.entry)
                std::cout << '\t' << instr.arg2.value << ": " << instr.arg2.isLive << ' ' << instr.arg2.nextUse << '\n';
            // std::cout << '(' << (instr.arg1.entry != NULL) << ' ' << instr.arg1.isLive << ' ' << instr.arg1.nextUse << ')';
            // std::cout << '(' << (instr.arg2.entry != NULL) << ' ' << instr.arg2.isLive << ' ' << instr.arg2.nextUse << ')';
            // std::cout << '(' << (instr.result.entry != NULL) << ' ' << instr.result.isLive << ' ' << instr.result.nextUse << ")\n";
        }
        std::cout << "\n";
    }
}
