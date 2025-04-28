/*
Rules to follow
-> Don't use arg1, arg2 or result directly without reg or mem unless it is constant
-> also update value through update reg_desciptors
-> Check bss section for int y = *x, *x = 5 ..
-> add check for type & in arg1 and arg2
-> If you are changin the value of a reg then call update reg descr
// to check if var in stack : condition is : op.entry->addrDesc.inStack==1
// inStack = 0 (for all local and global at start of program)
// inStack = 1 (correct value of local var available in stack)
// inStack = 2 (value in stack is old, need to generate a mov instr)
*/
#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "types.hpp"
#include "x86_32_lib.hpp"

void print_error(const std::string &message);
/* Variable declaration */
extern std::string outputDir;
std::ofstream asm_file;

std::map<std::string, int> string_literals;
std::map<std::string, int> float_constants;
std::map<std::string, std::vector<std::string>> global_array_init;
bool block_regs_spilled = false;
bool inside_fn = false;
bool main_no_ret = false;
bool fn_prologue_emitted = false;
bool fn_epilogue_emitted = false;
int params_size = 0;
int labelCnt = 0;
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
std::ofstream des_out("Descriptor.txt");
std::set<operand> seenOperand;
std::map<std::string, operand> mem_operand;

/* Optimization Related */
std::vector<x86_instr> asm_instr;

void updateSeenOperand(quad &instr)
{
    if (instr.arg1.entry)
    {
        seenOperand.insert(instr.arg1);
    }
    if (instr.arg2.entry)
    {
        seenOperand.insert(instr.arg2);
    }
    if (instr.result.entry)
    {
        seenOperand.insert(instr.result);
    }
}

void printReg_addr_Desc(int currInstrLabel)
{
    des_out << "Instruction Label: " << currInstrLabel << "\n\n";

    des_out << "REGISTER DESCRIPTOR\n";
    des_out << std::left << std::setw(12) << "Register" << "Operands\n";
    des_out << std::string(40, '-') << "\n";

    std::map<std::string, std::vector<int>> ops;
    std::map<std::string, bool> opsinmem;
    for (int reg = 0; reg < uRegCnt; reg++)
    {
        des_out << std::left << std::setw(12) << reg_names[reg];
        for (auto &op : regDesc[reg])
        {
            des_out << op.value << ' ';
            ops[op.value].push_back(reg);
            if (op.entry && op.entry->addrDesc.inStack == 1)
            {
                opsinmem[op.value] = 1;
            }
        }
        des_out << '\n';
    }

    des_out << "\nADDRESS DESCRIPTOR\n";
    des_out << std::left << std::setw(20) << "Operand" << "Locations\n";
    des_out << std::string(40, '-') << "\n";

    for (auto &it : seenOperand)
    {
        des_out << std::left << std::setw(20) << it.value;
        if (it.entry)
        {
            for (auto reg : it.entry->addrDesc.inRegs)
            {
                des_out << reg_names[reg] << ' ';
            }
            if (it.entry->addrDesc.inStack == 1)
            {
                des_out << "| In Memory";
            }
        }
        des_out << '\n';
    }
}
std::string getMem(operand &op)
{
    if (op.entry)
    {
        std::string memAddr;
        int offset = op.entry->offset;
        // emit_instr(";" + op.value + " = " + std::to_string(offset));
        if (offset < 0)
            memAddr = reg_names[EBP] + "+" + std::to_string(-offset);
        else
        {
            // if(op.entry->isArray) offset += op.entry->size;
            memAddr = reg_names[EBP] + "-" + std::to_string(offset + op.entry->size);
        }
        if (op.entry->addrDesc.inStack == 2)
        {
            if (op.entry->addrDesc.inRegs.size())
                emit_instr(x86_lib::mov_mem_reg(memAddr, reg_names[op.entry->addrDesc.inRegs[0]]));
            else
            {
                std::cerr << op.value << " : Operand not found anywhere (getMem)\n";
                exit(1);
            }
        }
        op.entry->addrDesc.inStack = 1;
        return memAddr;
    }
    std::cerr << op.value << " : Operand entry is null in getMem\n";
    exit(1);
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
        if (op.entry)
        {
            // if (op.isLive) //Stuck -> eg. val = t0, where we want to //remedy if val is not live then don't execute this instruction
            // {
            // if (op.entry->isGlobal || op.entry->isStatic > 0)//check
            // {
            //     emit_instr(x86_lib::mov_mem_reg(op.value, reg_names[reg]));
            // }
            if (!(op.entry->addrDesc.inRegs.size() > 1 || op.entry->addrDesc.inStack == 1 || op.entry->addrDesc.inHeap || op.entry->isGlobal || op.entry->isStatic > 0))
            {
                std::string memAddr = getMem(op);
                if (op.entry) // Extra check after getMem call
                    op.entry->addrDesc.inStack = 1;
            }
            // }
            eraseFromVector(op.entry->addrDesc.inRegs, reg);
        }
    }

    regDesc[reg].clear();
}

void spillAllReg()
{
    if (print_comments)
        emit_comment("(spilling all reg)");
    for (int reg = 0; reg < uRegCnt; reg++)
    {
        spillReg(reg);
    }
}

int getBestReg(std::vector<int> resReg)
{
    std::vector<int> isResReg(uRegCnt);
    for (auto it : resReg)
    {
        isResReg[it] = 1;
    }

    for (int reg = 0; reg < uRegCnt; reg++)
    {
        if (isResReg[reg])
            continue;
        if (regDesc[reg].size() == 0)
            return reg;
    }

    int bestReg = -1;
    int minCnt = 100;
    for (int reg = 0; reg < uRegCnt; reg++)
    {
        if (isResReg[reg])
            continue;
        int cnt = 0;
        for (auto &op : regDesc[reg])
        {
            if (op.isLive)
            {
                if (op.entry == NULL || !(op.entry->addrDesc.inRegs.size() > 1 || op.entry->addrDesc.inStack == 1 || op.entry->addrDesc.inHeap))
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
int getReg(operand &op, bool willYouModify, std::vector<int> resReg)
{
    // resReg->don't give this reg
    // for now if resReg is not empty pass willYouModify as 1
    std::vector<int> isResReg(uRegCnt);
    for (auto it : resReg)
    {
        isResReg[it] = 1;
    }

    if (op.value.size() >= 6 && op.value.substr(0, 6) == "__str_")
    {
        int bestReg = getBestReg(resReg);
        emit_instr(x86_lib::mov(reg_names[bestReg], op.value));
        return bestReg;
    }

    if (is_int_constant(op.value))
    {
        int bestReg = getBestReg(resReg);
        emit_instr(x86_lib::mov_reg_imm(reg_names[bestReg], op.value));
        return bestReg;
    }

    if (!op.entry)
        return -1;
    auto &addrDesc = op.entry->addrDesc;

    if (willYouModify == 0)
    {
        for (auto reg : addrDesc.inRegs)
        {
            if (isResReg[reg])
                continue;
            return reg;
        }

        int bestReg = getBestReg(resReg);
        std::string memAddr = getMem(op);
        // emit_instr(";" + op.value + "_" + memAddr);

        if (op.entry && (op.entry->isGlobal || op.entry->isStatic > 0))
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], op.value)); //[lexeme]
        }
        else
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], memAddr));
        }
        regDesc[bestReg].push_back(op);
        if (op.entry)
        {
            op.entry->addrDesc.inRegs.push_back(bestReg);
        }
        return bestReg;
    }

    if (addrDesc.inRegs.size())
    {
        int bestReg = -1;
        int minCnt = 100;
        for (auto reg : addrDesc.inRegs)
        {
            if (isResReg[reg])
                continue;
            int cnt = 0;
            for (auto tempOp : regDesc[reg])
            {
                if (tempOp.isLive)
                {
                    if (tempOp.entry == NULL || !((tempOp.entry->addrDesc.inRegs.size() > 1) || tempOp.entry->addrDesc.inStack == 1 || tempOp.entry->addrDesc.inHeap))
                        cnt++;
                }
            }
            if (minCnt > cnt)
            {
                minCnt = cnt;
                bestReg = reg;
            }
        }
        if (bestReg != -1)
        {
            spillReg(bestReg);
            return bestReg;
        }
    }

    int bestReg = getBestReg(resReg);
    std::string memAddr = getMem(op);
    // emit_instr(";" + op.value + "_" + memAddr);
    if (op.entry && (op.entry->isGlobal || op.entry->isStatic > 0))
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], op.value)); //[lexeme]
    }
    else
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], memAddr));
    }
    // TODO: Handle __str_
    return bestReg;
}

void setParticularReg(int reg, operand &op)
{
    if (is_int_constant(op.value))
    {
        spillReg(reg);
        emit_instr(x86_lib::mov_reg_imm(reg_names[reg], op.value));
        return;
    }
    if (op.value.size() >= 6 && op.value.substr(0, 6) == "__str_")
    {
        spillReg(reg);
        emit_instr(x86_lib::mov(reg_names[reg], op.value));
        return;
    }

    bool isPresent = 0;
    for (auto treg : op.entry->addrDesc.inRegs)
    {
        if (treg == reg)
        {
            isPresent = 1;
            break;
        }
    }
    spillReg(reg);
    if (!isPresent)
    {
        if (op.entry && (op.entry->isGlobal || op.entry->isStatic > 0))
        {
            if (op.entry->type == "char")
                emit_instr(x86_lib::movzx_reg_mem(reg_names[reg], "byte", op.value)); //[lexeme]
            else
                emit_instr(x86_lib::mov_reg_mem(reg_names[reg], op.value)); //[lexeme]
        }
        else
        {
            std::string memAddr = getMem(op);
            if (op.entry->type == "char")
                emit_instr(x86_lib::movzx_reg_mem(reg_names[reg], "byte", memAddr));
            else
                emit_instr(x86_lib::mov_reg_mem(reg_names[reg], memAddr));
        }
    }
    // regDesc[reg].push_back(op); //No need
    // if (op.entry)
    // {
    //     op.entry->addrDesc.inRegs.push_back(reg);
    // }
}

void updateRegDesc_assign(int reg, operand &op)
{
    if (op.entry)
    {
        if (op.entry->isGlobal || op.entry->isStatic > 0)
        {
            emit_instr(x86_lib::mov_mem_reg(op.value, reg_names[reg]));
        }
        for (int i = 0; i < uRegCnt; i++)
        {
            eraseFromVector(regDesc[i], op);
        }
        regDesc[reg].push_back(op);
        op.entry->addrDesc.inRegs.clear();
        op.entry->addrDesc.inRegs.push_back(reg);
        op.entry->addrDesc.inStack = 2;
        op.entry->addrDesc.inHeap = 0;
    }
}
// It should be called for lhs => Contact if you are using it for other asso
void updateRegDesc(int reg, operand &op)
{
    if (op.entry)
    {
        if (op.entry->isGlobal || op.entry->isStatic > 0)
        {
            emit_instr(x86_lib::mov_mem_reg(op.value, reg_names[reg]));
        }
        for (auto &top : regDesc[reg])
        {
            if (top.entry)
            {
                eraseFromVector(top.entry->addrDesc.inRegs, reg);
            }
        }

        for (int i = 0; i < uRegCnt; i++)
            eraseFromVector(regDesc[i], op);

        regDesc[reg].clear();
        regDesc[reg].push_back(op);
        op.entry->addrDesc.inRegs.clear();
        op.entry->addrDesc.inRegs.push_back(reg);
        op.entry->addrDesc.inStack = 2;
        op.entry->addrDesc.inHeap = 0;
    }
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
    fixgotoLabels();
    compute_basic_blocks();

    print_tac_code(inputFile, true); // only for debugging
    add_extern_funcs();
    emit_section(".text");
    emit_data("global main");

    for (auto &block : basic_blocks)
    {
        next_use_analysis(block);
        if (print_comments)
            emit_comment("Block begins");
        for (auto &instr : block)
        {
            if (print_comments)
                emit_comment(stringify(instr, true));
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
                {
                    if (!block_regs_spilled)
                    {
                        spillAllReg();
                        block_regs_spilled = true;
                    }
                    if (!fn_epilogue_emitted)
                    {
                        if (main_no_ret)
                        {
                            emit_instr(x86_lib::xor_op(reg_names[EAX], reg_names[EAX]));
                            main_no_ret = false;
                        }
                        emit_fn_epilogue();
                    }
                    inside_fn = false;
                    main_no_ret = false;
                }
            }
            else if (curr_op.back() == ':')
                emit_label(curr_op.substr(0, curr_op.length() - 2));
            else if (curr_op == "param")
                emit_param(instr);
            else if (curr_op == "CALL")
                emit_fn_call(instr);
            else if (curr_op == "RETURN")
                emit_return(instr);
            else if (curr_op == "=")
                emit_assign(instr);
            else if (curr_op == "(f)=")
                emit_fassign(instr);
            else if (curr_op == "(c)=")
                emit_cassign(instr);
            else if (curr_op == "+")
                emit_add(instr);
            else if (curr_op == "(f)+")
                emit_fadd(instr);
            else if (curr_op == "-")
                emit_sub(instr);
            else if (curr_op == "(f)-")
                emit_fsub(instr);
            else if (curr_op == "*")
                emit_mul(instr);
            else if (curr_op == "(f)*")
                emit_fmul(instr);
            else if (curr_op == "/")
                emit_div(instr);
            else if (curr_op == "(f)/")
                emit_fdiv(instr);
            else if (curr_op == "%")
                emit_mod(instr);
            else if (curr_op == "&")
                emit_and(instr);
            else if (curr_op == "|")
                emit_or(instr);
            else if (curr_op == "^")
                emit_xor(instr);
            else if (curr_op == "unary+")
                emit_unary_plus(instr);
            else if (curr_op == "unary-")
                emit_unary_minus(instr);
            else if (curr_op == "unary&")
                emit_unary_and(instr);
            else if (curr_op == "unary*")
                emit_unary_star(instr);
            else if (curr_op == "~")
                emit_not(instr);
            else if (curr_op == ">>")
                emit_right_shift(instr);
            else if (curr_op == "<<")
                emit_left_shift(instr);
            else if (curr_op == "==" || curr_op == "!=" || curr_op == ">" || curr_op == "<" || curr_op == "<=" || curr_op == ">=")
                emit_cmp(instr);
            else if (curr_op == "(f)<" || curr_op == "(f)<=" || curr_op == "(f)>" || curr_op == "(f)>=" || curr_op == "(f)==" || curr_op == "(f)!=")
                emit_fcmp(instr);
            else if (curr_op == "&&")
                emit_logical_and(instr);
            else if (curr_op == "||")
                emit_logical_or(instr);
            else if (curr_op == "!")
                emit_logical_not(instr);
            else if (curr_op == "intToFloat")
                emit_intToFloat(instr);
            else if (curr_op == "floatToInt")
                emit_floatToInt(instr);
            else if (curr_op == "intToChar")
                emit_intToChar(instr);
            else if (curr_op == "charToInt")
                emit_charToInt(instr);
            else if (curr_op == "GOTO")
            {
                if (!block_regs_spilled)
                {
                    spillAllReg();
                    block_regs_spilled = true;
                }
                emit_goto(instr);
            }
            else if (curr_op == "ptr+")
                emit_logical_ptr_add(instr);
            else if (curr_op == "ptr-")
                emit_logical_ptr_sub(instr);
            else if (curr_op == "CopyToOffset")
                emit_copy_to_offset(instr);
            else if (curr_op.substr(0, 3) == "va_")
                emit_va_instr(instr);

            printReg_addr_Desc(instr.Label);
        }
        if (!block_regs_spilled)
            spillAllReg();

        block_regs_spilled = false; // reset for next block
    }

    emit_section(".data");
    emit_data_section(); // add initialized data
    emit_section(".bss");
    emit_bss_section(); // add uninitialized data
}

void emit_copy_to_offset(quad &instr)
{
    if (!inside_fn)
        return;
    if (instr.result.entry && instr.result.entry->isStatic > 0)
        return;
    // char* ch[5] = {}
    spillAllReg();
    if(instr.result.entry && instr.result.entry->type.substr(0,4) == "char"){
        // int reg1 = getReg(instr.arg1, 1, {}); // reg1 -> value
        setParticularReg(EDX,instr.arg1);
        if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
        {
            emit_instr(x86_lib::movzx_reg_mem(reg_names[EDX], "byte", reg_names[EDX]));
        }
        int reg2 = getReg(instr.arg2, 0, {EDX}); //->reg2 -> offset
        int reg3 = getReg(instr.result, 1, {EDX, reg2});
        std::string mem = getMem(instr.result); // arr
        emit_instr(x86_lib::lea(reg_names[reg3], mem));
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg3] + "+" + reg_names[reg2], "dl"));
    }else if(instr.result.entry && instr.result.entry->type.substr(0,5) == "float" && instr.arg1.value.substr(0,4) == "__f_"){
        int reg2 = getReg(instr.arg2, 1,{}); //->reg2 -> offset
        int reg3 = getReg(instr.result, 1, {reg2});
        std::string mem = getMem(instr.result); // arr
        emit_instr(x86_lib::lea(reg_names[reg3], mem));
        // emit_instr(x86_lib::add(reg_names[reg3], reg_names[reg2]));
        //move arg1 in address of reg3
        int reg1 = getReg(instr.arg2,1,{reg2,reg3});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1],instr.arg1.value));
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg3] + "+" + reg_names[reg2], reg_names[reg1]));
    }else{
        int reg1 = getReg(instr.arg1, 1, {}); // reg1 -> value
        if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        }
        int reg2 = getReg(instr.arg2, 0, {reg1}); //->reg2 -> offset
        int reg3 = getReg(instr.result, 1, {reg1, reg2});
        std::string mem = getMem(instr.result); // arr
        emit_instr(x86_lib::lea(reg_names[reg3], mem));
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg3] + "+" + reg_names[reg2], reg_names[reg1]));
    }
}

void emit_logical_ptr_add(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    // emit_instr(";" + stringify(instr));
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::add_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::add_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::add(reg_names[reg1], reg_names[reg2]));
    }
    // emit_instr("; khatam");
    updateRegDesc(reg1, instr.result);
}

void emit_logical_ptr_sub(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::sub_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::sub_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::sub(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_unary_star(quad &instr)
{
    // _debug_(stringify(instr));
    if (instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&')
    {
        // to = * ptr => to is a reference
        // to = *arr
        // emit_instr(x86_lib::mov(reg_names[reg1], reg_names[reg1]));
        if (instr.arg1.entry && instr.arg1.entry->isArray)
        {
            std::string mem = getMem(instr.arg1);
            int reg1 = getReg(instr.result, 1, {});
            emit_instr(x86_lib::lea(reg_names[reg1], mem));
            updateRegDesc(reg1, instr.result);
        }
        else
        {
            int reg1 = getReg(instr.arg1, 1, {});
            updateRegDesc(reg1, instr.result);
        }
    }
    else
    {
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        updateRegDesc(reg1, instr.result);
    }
}

void emit_unary_and(quad &instr)
{
    // _debug_(stringify(instr));
    // ptr = &t1(&)
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        int reg1 = getReg(instr.arg1, 1, {});
        // emit_instr(x86_lib::mov(reg_names[reg1], reg_names[reg1]));
        updateRegDesc(reg1, instr.result);
    }
    else
    {
        std::string mem = getMem(instr.arg1);
        int reg1 = getReg(instr.result, 1, {});
        emit_instr(x86_lib::lea(reg_names[reg1], mem));
        updateRegDesc(reg1, instr.result);
    }
}

void emit_fn_defn(quad &instr)
{
    if (instr.op.substr(5, 5) == "4main")
    {
        emit_label("\nmain");
        main_no_ret = true;
    }
    else
        emit_label("\n" + instr.op.substr(0, instr.op.length() - 8));

    emit_instr(x86_lib::push(reg_names[EBP]));
    emit_instr(x86_lib::mov(reg_names[EBP], reg_names[ESP]));
    auto pos = instr.op.find(' ');
    std::string func_name = instr.op.substr(0, pos);
    int func_size = 0;
    auto entry = lookup(func_name);
    if (!entry)
    {
        std::string class_name = func_name.substr(5);
        int start_pos = 0;
        while (isdigit(class_name[start_pos]))
            start_pos++;
        int class_name_len = std::stoi(class_name.substr(0, start_pos));
        class_name = class_name.substr(start_pos, class_name_len);
        class_name = "CLASS_" + class_name;
        entry = lookupClassEntry(class_name, func_name);
    }

    if (entry)
    {
        func_size = entry->size;
        // if (func_size % 4 != 0)
        //     func_size += 4 - (func_size % 4);
        emit_instr(x86_lib::sub(reg_names[ESP], std::to_string(func_size)));
    }
    else
    {
        print_error("Function not found in symbol table: " + func_name);
        exit(1);
    }

    emit_instr(x86_lib::push(reg_names[EBX]));
    emit_instr(x86_lib::push(reg_names[ESI]));
    emit_instr(x86_lib::push(reg_names[EDI]));

    fn_epilogue_emitted = false;
}

void emit_param(quad &instr)
{
    if (!fn_prologue_emitted)
    {
        emit_instr(x86_lib::push(reg_names[EAX]));
        emit_instr(x86_lib::push(reg_names[ECX]));
        emit_instr(x86_lib::push(reg_names[EDX]));
        fn_prologue_emitted = true;
    }

    // TODO: char constant and string constant pending

    if (is_num_constant(instr.arg1.value))
    {
        emit_instr(x86_lib::push_imm(instr.arg1.value));
        params_size += 4;
        return;
    }
    
    if (instr.arg1.value.substr(0, 6) == "__str_"){
        emit_instr(x86_lib::push(instr.arg1.value));
        params_size += 4;
        return;
    }

    if(instr.arg1.entry && instr.arg1.entry->isArray){
        std::string mem = getMem(instr.arg1);
        int reg1 = getReg(instr.arg1, 1, {});
        emit_instr(x86_lib::lea(reg_names[reg1], mem));
        emit_instr(x86_lib::push(reg_names[reg1]));
        params_size += 4;
        return;
    }

    if (instr.arg1.entry)
    {   
        if (instr.arg1.entry->isGlobal || instr.arg1.entry->isStatic > 0 || instr.arg1.value.substr(0, 4) == "__f_") // TODO;handle & wala case
        {
            if ((instr.arg1.entry->type == "float" || instr.arg1.entry->type == "float&") && instr.arg2.value == "lea")
            {
                std::cout << instr.arg1.entry->type << " " << instr.arg1.value << std::endl;
                emit_fparam(instr.arg1);
            }
            else if (instr.arg1.entry->type == "char" || instr.arg1.entry->type == "char&")
            {
                setParticularReg(EBX, instr.arg1);
                // emit_instr(x86_lib::movzx_reg_mem(reg_names[EBX], "byte", instr.arg1.value));
                if(instr.arg1.entry->type == "char&"){
                    emit_instr(x86_lib::mov_reg_mem(reg_names[EBX], reg_names[EBX]));
                }
                emit_instr(x86_lib::push(reg_names[EBX]));
            }
            else
            {
                emit_instr(x86_lib::push_mem("dword", instr.arg1.value));
            }
        }
        else if (instr.arg1.entry->addrDesc.inRegs.size())
        {
            if ((instr.arg1.entry->type == "float" || instr.arg1.entry->type == "float&") && instr.arg2.value == "lea")
            {
                std::cout << instr.arg1.entry->type << " haaaaaaa " << instr.arg1.value << std::endl;
                emit_fparam(instr.arg1);
            }
            else if (instr.arg1.entry->type == "char" || instr.arg1.entry->type == "char&")
            {
                setParticularReg(EBX, instr.arg1);
                // emit_instr(x86_lib::movzx_reg_mem(reg_names[EBX], "byte", instr.arg1.value));
                if(instr.arg1.entry->type == "char&"){
                    emit_instr(x86_lib::mov_reg_mem(reg_names[EBX], reg_names[EBX]));
                }
                emit_instr(x86_lib::push(reg_names[EBX]));
            }
            else if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
            {
                std::cout << instr.arg1.entry->type << " 111 " << instr.arg1.value << std::endl;
                int reg1 = getReg(instr.arg1, 1, {}); // add of x
                emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
                emit_instr(x86_lib::push(reg_names[reg1]));
            }
            else
            {
                std::cout << instr.arg1.entry->type << " 2222 " << instr.arg1.value << std::endl;

                emit_instr(x86_lib::push(reg_names[instr.arg1.entry->addrDesc.inRegs[0]]));
            }
        }

        else if (instr.arg1.entry->addrDesc.inStack == 1 || instr.arg1.entry->addrDesc.inStack == 0) // initially at start of program inStack == 0 for local vars as well as args
        {
            if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type == "int&")
            {                                         // TODO:check once for float and char
                int reg1 = getReg(instr.arg1, 1, {}); // add of x
                emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
                emit_instr(x86_lib::push(reg_names[reg1]));
            }
            else
            {
                if ((instr.arg1.entry->type == "float" || instr.arg1.entry->type == "float&") && instr.arg2.value == "lea")
                {
                    std::cout << instr.arg1.entry->type << " " << instr.arg1.value << std::endl;
                    emit_fparam(instr.arg1);
                }
                else if (instr.arg1.entry->type == "char" || instr.arg1.entry->type == "char&")
                {
                    setParticularReg(EBX, instr.arg1);
                    std::cout << instr.arg1.entry->type << " charrrrrrrrrrrrrrrrrrr mem    " << instr.arg1.value << std::endl;
                    // emit_instr(x86_lib::movzx_reg_mem(reg_names[EBX], "byte", instr.arg1.value));
                    if(instr.arg1.entry->type == "char&"){
                        emit_instr(x86_lib::mov_reg_mem(reg_names[EBX], reg_names[EBX]));
                    }
                    emit_instr(x86_lib::push(reg_names[EBX]));
                }
                else
                {
                    emit_instr(x86_lib::push_mem("dword", getMem(instr.arg1))); // TODO : Check
                }
            }
        }
        else
        {
            std::cerr << instr.arg1.value << " : Param not found anywhere\n";
            exit(1);
        }
        int sz = instr.arg1.entry->size;
        // if (sz % 4 != 0)
        //     sz += 4 - (sz % 4);

        params_size += sz;
        if (instr.arg1.entry->type == "float" && instr.arg2.value == "lea")
        {
            params_size += 4;
        }
        if (instr.arg1.entry->type == "char")
        {
            params_size += 3;
        }
    }
}

void emit_fparam(operand op)
{
    emit_instr(x86_lib::lea(reg_names[ESP], reg_names[ESP] + "-" + std::to_string(8)));
    emit_fload(op);
    emit_instr(x86_lib::fstp_mem("qword", reg_names[ESP]));
}

void emit_fn_call(quad &instr)
{
    if (!fn_prologue_emitted)
    {
        emit_instr(x86_lib::push(reg_names[EAX]));
        emit_instr(x86_lib::push(reg_names[ECX]));
        emit_instr(x86_lib::push(reg_names[EDX]));
        fn_prologue_emitted = true;
    }

    emit_instr(x86_lib::call(instr.arg1.value));
    if (params_size > 0)
        emit_instr(x86_lib::add(reg_names[ESP], std::to_string(params_size)));
    params_size = 0;

    if (instr.result.value != "")
    {
        if (instr.result.entry)
        {
            std::string mem = getMem(instr.result);
            if (instr.result.entry->type == "float")
            {
                emit_fstore(instr.result);
            }
            else
                emit_instr(x86_lib::mov_mem_reg(mem, reg_names[EAX]));
        }
        else
        {
            std::cerr << "Error: Operand entry is null in emit_fn_call\n";
            exit(1);
        }
    }

    emit_instr(x86_lib::pop(reg_names[EDX]));
    emit_instr(x86_lib::pop(reg_names[ECX]));
    emit_instr(x86_lib::pop(reg_names[EAX]));

    fn_prologue_emitted = false; // reset for next function
}

void emit_fn_epilogue()
{
    emit_instr(x86_lib::pop(reg_names[EDI]));
    emit_instr(x86_lib::pop(reg_names[ESI]));
    emit_instr(x86_lib::pop(reg_names[EBX]));
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());
    fn_epilogue_emitted = true;
}

void emit_return(quad &instr)
{ // TODO:handle & for float and char...
    if (!block_regs_spilled)
    {
        spillAllReg();
        block_regs_spilled = true;
    }
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        int reg1 = getReg(instr.arg1, 1, {EAX}); // add of x
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        spillReg(EAX);
        emit_instr(x86_lib::mov(reg_names[EAX], reg_names[reg1]));
        // updateRegDesc(EAX,instr.arg1); //TODO check once
    }
    else
    {
        if (instr.arg1.entry && instr.arg1.entry->type == "float")
        {
            emit_fload(instr.arg1);
        }
        else
        {
            spillAllReg();
            setParticularReg(EAX, instr.arg1);
        }
    }

    emit_fn_epilogue();

    if (main_no_ret)
        main_no_ret = false;
}

void emit_goto(quad &instr)
{
    if (instr.arg1.value == "IF")
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        { // nhi need hai
            reg2 = getReg(instr.arg2, 1, {});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {});
        }
        emit_instr(x86_lib::cmp_reg_imm(reg_names[reg2], "0"));
        emit_instr(x86_lib::jne("L" + std::to_string(instr.gotoLabel)));
    }
    else
    {
        emit_instr(x86_lib::jmp("L" + std::to_string(instr.gotoLabel)));
    }
}
/* Logical Operator */
void emit_logical_and(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }

    int reg2;
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
    }
    else
    {
        reg2 = getReg(instr.arg2, 0, {reg1});
    }
    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);

    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg1], "0"));
    emit_instr(x86_lib::je(label1));
    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg2], "0"));
    emit_instr(x86_lib::je(label1));
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "1"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "0"));
    emit_label(label2);

    updateRegDesc(reg1, instr.result);
}

void emit_logical_or(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    int reg2 = getReg(instr.arg2, 1, {reg1});
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
    }
    else
    {
        reg2 = getReg(instr.arg2, 0, {reg1});
    }

    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);

    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg1], "0"));
    emit_instr(x86_lib::jne(label1));
    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg2], "0"));
    emit_instr(x86_lib::jne(label1));
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "0"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "1"));
    emit_label(label2);

    updateRegDesc(reg1, instr.result);
}

void emit_logical_not(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);

    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg1], "0"));
    emit_instr(x86_lib::jne(label1));
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "1"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "0"));
    emit_label(label2);

    updateRegDesc(reg1, instr.result);
}

void emit_cmp(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    int reg2;
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
    }
    else
    {
        reg2 = getReg(instr.arg2, 0, {reg1});
    }
    emit_instr(x86_lib::cmp(reg_names[reg1], reg_names[reg2]));
    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);

    if (instr.op == "==")
    {
        emit_instr(x86_lib::je(label1));
    }
    else if (instr.op == "!=")
    {
        emit_instr(x86_lib::jne(label1));
    }
    else if (instr.op == "<")
    {
        emit_instr(x86_lib::jl(label1));
    }
    else if (instr.op == ">")
    {
        emit_instr(x86_lib::jg(label1));
    }
    else if (instr.op == "<=")
    {
        emit_instr(x86_lib::jle(label1));
    }
    else if (instr.op == ">=")
    {
        emit_instr(x86_lib::jge(label1));
    }
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "0"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "1"));
    emit_label(label2);
    updateRegDesc(reg1, instr.result);
}

void emit_fcmp(quad &instr)
{
    emit_fload(instr.arg2);

    emit_fload(instr.arg1);

    emit_instr(x86_lib::fcomip());

    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);
    // emit_fstore(instr.arg2);
    int reg = getReg(instr.result, 1, {});

    if (instr.op == "(f)==")
    {
        emit_instr(x86_lib::je(label1));
    }
    else if (instr.op == "(f)!=")
    {
        emit_instr(x86_lib::jne(label1));
    }
    else if (instr.op == "(f)<")
    {
        emit_instr(x86_lib::jb(label1));
    }
    else if (instr.op == "(f)>")
    {
        emit_instr(x86_lib::ja(label1));
    }
    else if (instr.op == "(f)<=")
    {
        emit_instr(x86_lib::jbe(label1));
    }
    else if (instr.op == "(f)>=")
    {
        emit_instr(x86_lib::jae(label1));
    }


    emit_instr(x86_lib::mov_reg_imm(reg_names[reg], "0"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg], "1"));
    emit_label(label2);
    updateRegDesc(reg, instr.result);
}

/* Bitwise Operator */
void emit_right_shift(quad &instr) // TODO: Check
{                                  // check correct value of cl
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        int reg2 = getReg(instr.arg2, 1, {ECX}); // add of x
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        spillReg(ECX);
        emit_instr(x86_lib::mov(reg_names[ECX], reg_names[reg2]));
    }
    else
    {
        setParticularReg(ECX, instr.arg2);
    }
    int reg1 = getReg(instr.arg1, 1, {ECX});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    emit_instr(x86_lib::shr(reg_names[reg1], "cl"));
    updateRegDesc(reg1, instr.result);
}

void emit_left_shift(quad &instr)
{
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        int reg2 = getReg(instr.arg2, 1, {ECX}); // add of x
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        spillReg(ECX);
        emit_instr(x86_lib::mov(reg_names[ECX], reg_names[reg2]));
    }
    else
    {
        setParticularReg(ECX, instr.arg2);
    }
    int reg1 = getReg(instr.arg1, 1, {ECX});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    emit_instr(x86_lib::shl(reg_names[reg1], "cl"));
    updateRegDesc(reg1, instr.result);
}

void emit_or(quad &instr)
{
    // x = y | z
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::or_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::or_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::or_op(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_xor(quad &instr)
{
    // x = y ^ z
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::xor_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::xor_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::xor_op(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}
void emit_and(quad &instr)
{
    // x = y & z
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::and_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::and_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::and_op(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

/* Arithmetic */
void emit_unary_plus(quad &instr)
{
    emit_assign(instr); // a = +b;
}

void emit_unary_minus(quad &instr)
{ // a = -b;
    int reg = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg], reg_names[reg]));
    }
    emit_instr(x86_lib::neg(reg_names[reg]));
    updateRegDesc(reg, instr.result);
}

void emit_not(quad &instr)
{
    int reg = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg], reg_names[reg]));
    }
    emit_instr(x86_lib::not_op(reg_names[reg]));
    updateRegDesc(reg, instr.result);
}

void emit_div(quad &instr)
{ // c = a/b
    // mov arg1 to eax
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        int reg1 = getReg(instr.arg1, 1, {EAX}); // add of x
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        spillReg(EAX);
        emit_instr(x86_lib::mov(reg_names[EAX], reg_names[reg1]));
    }
    else
    {
        setParticularReg(EAX, instr.arg1);
    }
    spillReg(EDX);
    emit_instr(x86_lib::xor_op(reg_names[EDX], reg_names[EDX]));

    int reg2 = getReg(instr.arg2, 1, {EAX, EDX});
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
    }
    emit_instr(x86_lib::idiv(reg_names[reg2]));
    updateRegDesc(EAX, instr.result);
}

void emit_fdiv(quad &instr)
{
    emit_fload(instr.arg1);
    emit_fload(instr.arg2);
    emit_instr(x86_lib::fdivp());
    emit_fstore(instr.result);
}

void emit_mod(quad &instr)
{ // c = a/b
    // mov arg1 to eax
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        int reg1 = getReg(instr.arg1, 1, {EAX}); // add of x
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        spillReg(EAX);
        emit_instr(x86_lib::mov(reg_names[EAX], reg_names[reg1]));
    }
    else
    {
        setParticularReg(EAX, instr.arg1);
    }
    spillReg(EDX);
    emit_instr(x86_lib::xor_op(reg_names[EDX], reg_names[EDX]));

    int reg2 = getReg(instr.arg2, 1, {EAX, EDX});
    if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
    }
    emit_instr(x86_lib::idiv(reg_names[reg2]));
    updateRegDesc(EDX, instr.result);
}

void emit_mul(quad &instr)
{
    // x = y * z
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::imul_three(reg_names[reg1], reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::imul_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2 = getReg(instr.arg2, 1, {reg1});
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::imul(reg_names[reg1], reg_names[reg2]));
    }
    // _debug_(instr.result.value);
    updateRegDesc(reg1, instr.result);
    // _debug_(instr.result.value);
}

void emit_fmul(quad &instr)
{
    emit_fload(instr.arg1);
    emit_fload(instr.arg2);
    emit_instr(x86_lib::fmulp());
    emit_fstore(instr.result);
}

void emit_sub(quad &instr)
{
    if(instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&')
    {
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {});
        if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        }
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::sub(reg_names[reg1], reg_names[reg2]));
        int reg3 = getReg(instr.result,1,{reg1,reg2});
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg3], reg_names[reg1]));
        return;
    }
    // x = y - z
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }
    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::sub_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::sub_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::sub(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_fsub(quad &instr)
{
    emit_fload(instr.arg1);
    emit_fload(instr.arg2);
    emit_instr(x86_lib::fsubp());
    emit_fstore(instr.result);
}

void emit_add(quad &instr)
{
    if(instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&')
    {
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {});
        if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        }
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::add(reg_names[reg1], reg_names[reg2]));
        int reg3 = getReg(instr.result,1,{reg1,reg2});
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg3], reg_names[reg1]));
        return;
    }
    // x = y + z
    int reg1 = getReg(instr.arg1, 1, {});
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
    }

    if (is_int_constant(instr.arg2.value))
    {
        emit_instr(x86_lib::add_reg_imm(reg_names[reg1], instr.arg2.value));
    }
    else if (instr.arg2.entry && (instr.arg2.entry->isGlobal || instr.arg2.entry->isStatic > 0))
    {
        emit_instr(x86_lib::add_reg_mem(reg_names[reg1], instr.arg2.value));
    }
    else
    {
        int reg2;
        if (instr.arg2.entry && instr.arg2.entry->type.size() && instr.arg2.entry->type.back() == '&')
        {
            reg2 = getReg(instr.arg2, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
        }
        else
        {
            reg2 = getReg(instr.arg2, 0, {reg1});
        }
        emit_instr(x86_lib::add(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_fadd(quad &instr)
{
    emit_fload(instr.arg1);
    emit_fload(instr.arg2);
    emit_instr(x86_lib::faddp());
    emit_fstore(instr.result);
}

void emit_assign(quad &instr)
{
    if (!inside_fn) // global
        return;

    // Pointer ->

    // _debug_(stringify(instr));
    if (instr.arg1.entry && instr.arg1.entry->isArray)
    { // t0 = arr
        // spillAllReg();
        if (instr.arg1.entry->isGlobal || instr.arg1.entry->isStatic > 0)
        {
            int reg1 = getReg(instr.result, 1, {});
            emit_instr(x86_lib::lea(reg_names[reg1], instr.arg1.value));
            updateRegDesc(reg1, instr.result);
        }
        else
        {
            std::string mem = getMem(instr.arg1);
            int reg1 = getReg(instr.result, 1, {});
            emit_instr(x86_lib::lea(reg_names[reg1], mem));
            updateRegDesc(reg1, instr.result);
        }
    }
    else if (instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&')
    {
        // can Handle integer constant more optimally
        // t0(&) = 5
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {}); // TODO: both can be made 0
        if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
        {
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        }
        int reg2 = getReg(instr.result, 1, {reg1});
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg2], reg_names[reg1]));
        // make sure ki reg2 jis mem addr ko point kar raha hai uska instack = 1 honi chahiye
    }
    else if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        // y = to(&)
        _debug_(stringify(instr));
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {});
        int reg2 = getReg(instr.result, 1, {reg1});
        _debug_(instr.result.value, instr.arg1.value);
        bool flag = false;
        if (instr.result.entry->type == (instr.arg1.entry->type.substr(0, instr.arg1.entry->type.size() - 1)))
        {
            flag = true;
        }
        _debug_(flag);
        if (instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '*' && (!flag))
        {
            //ptr = t0(&)
            _debug_("here");
            emit_instr(x86_lib::mov(reg_names[reg2], reg_names[reg1]));
        }
        else
        {
            _debug_("here2");
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg1]));
        }
        updateRegDesc(reg2, instr.result);
        // make sure ki reg1 jis mem addr ko point kar raha hai uska instack = 1 honi chahiye
    }
    else
    {
        if (is_int_constant(instr.arg1.value))
        { // can use mov_mem_imm for more optimal code
            if (instr.result.entry && (instr.result.entry->isGlobal || instr.result.entry->isStatic > 0))
            {
                emit_instr(x86_lib::mov_mem_imm("dword", instr.result.value, instr.arg1.value));
            }
            else
            {
                std::string mem = getMem(instr.result);
                emit_instr(x86_lib::mov_mem_imm("dword", mem, instr.arg1.value));
            }
        }
        else
        {
            int reg1 = getReg(instr.arg1, 1, {});
            int reg2 = getReg(instr.result, 1, {reg1});
            emit_instr(x86_lib::mov(reg_names[reg2], reg_names[reg1]));
            updateRegDesc(reg2, instr.result);
        }
    }
}

void emit_fload(operand &arg)
{
    int recoverReg = -1;
    if (arg.entry && arg.entry->type.size() && arg.entry->type.back() == '&')
    {
        recoverReg = getReg(arg, 1, {});
        int reg = getReg(arg, 1, {recoverReg});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg], reg_names[reg]));
        emit_instr(x86_lib::mov_mem_reg(getMem(arg), reg_names[reg]));
    }

    if ((arg.value).substr(0, 4) == "__f_")
    {
        emit_instr(x86_lib::fld_mem("dword", arg.value));
    }
    else if (arg.entry && (arg.entry->isGlobal || arg.entry->isStatic > 0))
    {
        emit_instr(x86_lib::fld_mem("dword", arg.value));
    }
    else
    {
        emit_instr(x86_lib::fld_mem("dword", getMem(arg)));
    }

    if(recoverReg != -1)
    {
        updateRegDesc(recoverReg, arg);
        recoverReg = -1;
    }
}

void emit_fstore(operand &arg)
{
    if (arg.entry && (arg.entry->isGlobal || arg.entry->isStatic > 0))
    {
        emit_instr(x86_lib::fstp_mem("dword", arg.value));
    }
    else
    {
        emit_instr(x86_lib::fstp_mem("dword", getMem(arg)));
    }
}

void emit_fassign(quad &instr)
{
    if (!inside_fn) // global, TODO: check
        return;
    
    if(instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&')
    {
        std::cout << "in & fassign" << std::endl;
        spillAllReg();
        int reg1 = getReg(instr.result, 1, {}); // TODO: both can be made 0
        if ((instr.arg1.value).substr(0, 4) == "__f_"){
            int reg2 = getReg(instr.result, 1, {reg1});
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], instr.arg1.value));
            emit_instr(x86_lib::mov_mem_reg(reg_names[reg1], reg_names[reg2]));
        }else{
            int reg2 = getReg(instr.arg1, 1, {reg1});
            if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&'){
                emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg2]));
            }
            emit_instr(x86_lib::mov_mem_reg(reg_names[reg1], reg_names[reg2]));
        }
    }
    else{  
        emit_fload(instr.arg1);      
        emit_fstore(instr.result);
    }
}

void emit_intToFloat(quad &instr)
{
    if (is_int_constant(instr.arg1.value))
        return;
    int recoverReg = -1;
    if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        recoverReg = getReg(instr.arg1, 1, {});
        int reg = getReg(instr.arg1, 1, {recoverReg});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg], reg_names[reg]));
        emit_instr(x86_lib::mov_mem_reg(getMem(instr.arg1), reg_names[reg]));
    }

    if (instr.arg1.entry && (instr.arg1.entry->isGlobal || instr.arg1.entry->isStatic > 0))
        emit_instr(x86_lib::fild("dword", instr.arg1.value));
    else
        emit_instr(x86_lib::fild("dword", getMem(instr.arg1)));

    emit_fstore(instr.result);

    if(recoverReg != -1)
    {
        updateRegDesc(recoverReg, instr.arg1);
        recoverReg = -1;
    }


}

void emit_floatToInt(quad &instr)
{
    emit_fload(instr.arg1);

    if (instr.result.entry && instr.result.entry->isGlobal || instr.result.entry->isStatic > 0)
        emit_instr(x86_lib::fisttp("dword", instr.result.value));
    else
        emit_instr(x86_lib::fisttp("dword", getMem(instr.result)));
}

void emit_cassign(quad &instr)
{

    if (instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&')
    {
        // can Handle integer constant more optimally
        // t0(&) = 5
        spillAllReg();
        setParticularReg(EDX, instr.arg1); // TODO: both can be made 0
        if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
        {
            // emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
            emit_instr(x86_lib::movzx_reg_mem(reg_names[EDX], "byte", reg_names[EDX]));
        }
        int reg2 = getReg(instr.result, 1, {EDX});
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg2], "dl"));
        // make sure ki reg2 jis mem addr ko point kar raha hai uska instack = 1 honi chahiye
    }
    else if (instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        // can Handle integer constant more optimally
        // x = t0(&)
        // std::cout << instr.result.value << std::endl;
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {}); // TODO: both can be made 0
        // int reg2 = getReg(instr.result, 1, {reg1});
        setParticularReg(EDX, instr.result);
        emit_instr(x86_lib::movzx_reg_mem(reg_names[EDX], "byte", reg_names[reg1]));
        emit_instr(x86_lib::mov_mem_reg(getMem(instr.result), "dl"));
        // emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], getMem(instr.result)));
        // make sure ki reg2 jis mem addr ko point kar raha hai uska instack = 1 honi chahiye
    }
    else
    {
        spillAllReg();
        setParticularReg(EDX, instr.arg1);
        if (instr.result.entry && (instr.result.entry->isGlobal || instr.result.entry->isStatic > 0))
            emit_instr(x86_lib::mov_mem_reg(instr.result.value, "dl"));
        else
            emit_instr(x86_lib::mov_mem_reg(getMem(instr.result), "dl"));
    }
}

void emit_intToChar(quad &instr)
{
    spillAllReg();
    setParticularReg(EDX, instr.arg1);
    if(instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[EDX], reg_names[EDX]));
    }
    if (instr.result.entry && (instr.result.entry->isGlobal || instr.result.entry->isStatic > 0))
        emit_instr(x86_lib::mov_mem_reg(instr.result.value, "dl"));
    else
        emit_instr(x86_lib::mov_mem_reg(getMem(instr.result), "dl"));
}

void emit_charToInt(quad &instr)
{
    spillAllReg();
    setParticularReg(EDX, instr.arg1);

    if(instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&')
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[EDX], reg_names[EDX]));
    }
    // setParticularReg(EDX, instr.arg1);
    if (instr.result.entry && (instr.result.entry->isGlobal || instr.result.entry->isStatic > 0))
        emit_instr(x86_lib::mov_mem_reg(instr.result.value, reg_names[EDX]));
    else
        emit_instr(x86_lib::mov_mem_reg(getMem(instr.result), reg_names[EDX]));
}

void emit_va_instr(quad &instr)
{
    if (instr.op == "va_start")
    {
        std::string mem = getMem(instr.arg2);
        int offset = std::stoi(mem.substr(4)) + instr.arg2.entry->size;
        mem = mem.substr(0, 4) + std::to_string(offset);
        int reg1 = getReg(instr.arg1, 1, {});
        emit_instr(x86_lib::lea(reg_names[reg1], mem));
        updateRegDesc(reg1, instr.arg1);
    }
    else
    {
        int reg1 = getReg(instr.arg1, 1, {});
        int reg2 = getReg(instr.arg1, 1, {reg1});
        emit_instr(x86_lib::lea(reg_names[reg2], reg_names[reg1] + "+" + std::to_string(instr.arg2.entry->size)));
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        updateRegDesc(reg2, instr.arg1);
        updateRegDesc(reg1, instr.result);
    }
}

void get_constants()
{
    bool in_fn = false;
    int str_cnt = 0, float_cnt = 0;
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
        if (instr.arg1.value.length() > 0 && instr.arg1.value[0] == '\"')
        {
            if (string_literals.find(instr.arg1.value) == string_literals.end())
                string_literals[instr.arg1.value] = str_cnt++;
            instr.arg1.value = "__str_" + std::to_string(string_literals[instr.arg1.value]);
            instr.arg1.entry->size = 4;
        }
        if (instr.arg2.value.length() > 0 && instr.arg2.value[0] == '\"')
        {
            if (string_literals.find(instr.arg2.value) == string_literals.end())
                string_literals[instr.arg2.value] = str_cnt++;
            instr.arg2.value = "__str_" + std::to_string(string_literals[instr.arg2.value]);
            instr.arg2.entry->size = 4;
        }
        if (instr.result.value.length() > 0 && instr.result.value[0] == '\"')
        {
            if (string_literals.find(instr.result.value) == string_literals.end())
                string_literals[instr.result.value] = str_cnt++;
            instr.result.value = "__str_" + std::to_string(string_literals[instr.result.value]);
            instr.result.entry->size = 4;
        }
        if (in_fn)
        {
            if (instr.op == "intToFloat" && is_int_constant(instr.arg1.value))
            {
                if (float_constants.find(instr.arg1.value) == float_constants.end())
                    float_constants[instr.arg1.value] = float_cnt++;
                float_constants[instr.result.value] = float_constants[instr.arg1.value];
                instr.result.value = "__f_" + std::to_string(float_constants[instr.arg1.value]);
                instr.result.entry->size = 4;
                instr.result.entry->type = "float";
                instr.result.entry->isGlobal = true;
            }
            if (is_float_constant(instr.arg1.value))
            {
                if (float_constants.find(instr.arg1.value) == float_constants.end())
                    float_constants[instr.arg1.value] = float_cnt++;
                instr.arg1.value = "__f_" + std::to_string(float_constants[instr.arg1.value]);
                instr.arg1.entry->size = 4;
                instr.arg1.entry->type = "float";
            }
            else if (float_constants.find(instr.arg1.value) != float_constants.end() && !is_int_constant(instr.arg1.value))
            {
                instr.arg1.value = "__f_" + std::to_string(float_constants[instr.arg1.value]);
                instr.arg1.entry->size = 4;
                instr.arg1.entry->type = "float";
            }

            if (is_float_constant(instr.arg2.value))
            {
                if (float_constants.find(instr.arg2.value) == float_constants.end())
                    float_constants[instr.arg2.value] = float_cnt++;
                instr.arg2.value = "__f_" + std::to_string(float_constants[instr.arg2.value]);
                instr.arg2.entry->size = 4;
                instr.arg2.entry->type = "float";
            }
            else if (float_constants.find(instr.arg2.value) != float_constants.end() && !is_int_constant(instr.arg2.value))
            {
                instr.arg2.value = "__f_" + std::to_string(float_constants[instr.arg2.value]);
                instr.arg2.entry->size = 4;
                instr.arg2.entry->type = "float";
            }

            if (is_float_constant(instr.result.value))
            {
                if (float_constants.find(instr.result.value) == float_constants.end())
                    float_constants[instr.result.value] = float_cnt++;
                instr.result.value = "__f_" + std::to_string(float_constants[instr.result.value]);
                instr.result.entry->size = 4;
                instr.result.entry->type = "float";
            }
            else if (float_constants.find(instr.result.value) != float_constants.end() && !is_int_constant(instr.result.value))
            {
                instr.result.value = "__f_" + std::to_string(float_constants[instr.result.value]);
                instr.result.entry->size = 4;
                instr.result.entry->type = "float";
            }
        }
    }
}

void fixgotoLabels()
{
    std::map<int, int> label_map;
    for (const auto &instr : tac_code)
    {
        if (instr.op.back() == ':' && instr.op[0] == 'L')
        {
            int label_num = std::stoi(instr.op.substr(1, instr.op.length() - 3));
            label_map[instr.Label] = label_num;
        }
    }
    for (auto &instr : tac_code)
    {
        if (instr.gotoLabel != -1)
            instr.gotoLabel = label_map[instr.gotoLabel];
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

    for (auto &instr : tac_code)
    {
        if (instr.result.entry)
        {
            if (instr.result.entry->type == "char" && instr.result.value[0] == '\'')
            {
                // instr.result.value = std::to_string(static_cast<int>(instr.result.value[1]));
                instr.result.value = std::to_string(static_cast<int>(giveChar(instr.result.value)));
            }
        }
        if (instr.arg1.entry)
        {
            if (instr.arg1.entry->type == "char" && instr.arg1.value[0] == '\'')
            {
                instr.arg1.value = std::to_string(static_cast<int>(giveChar(instr.arg1.value)));
            }
        }
        if (instr.arg2.entry && instr.arg2.value[0] == '\'')
        {
            if (instr.arg2.entry->type == "char")
            {
                instr.arg2.value = std::to_string(static_cast<int>(giveChar(instr.arg2.value)));
            }
        }
    }

    size_t i = 0;
    while (i < tac_code.size())
    {
        auto instr = tac_code[i];
        if (instr.op == "CopyToOffset")
        {
            auto curr_array = instr.result;
            std::vector<std::string> array_init_values;
            while (i < tac_code.size())
            {
                instr = tac_code[i];
                if (instr.op != "CopyToOffset" || instr.result.value != curr_array.value)
                {
                    if (curr_array.entry)
                    {
                        int num_elements = curr_array.entry->size / 4;
                        if (curr_array.entry->type == "char*")
                            num_elements = curr_array.entry->size;
                        int missing_cnt = num_elements - array_init_values.size();
                        for (int j = 0; j < missing_cnt; j++)
                            array_init_values.push_back("0");
                    }
                    global_array_init[curr_array.value] = array_init_values;
                    break;
                }
                array_init_values.push_back(instr.arg1.value);
                i++;
            }
        }
        else
            i++;
    }
}

void update_ir()
{
    addgotoLabels();
    get_constants();
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
                    std::string data_string;
                    for (auto i = 0; i < global_array_init[name].size(); i++)
                    {
                        data_string += global_array_init[name][i];
                        if (i != global_array_init[name].size() - 1)
                            data_string += ", ";
                    }
                    emit_data(name + ": db " + data_string);
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
                    std::string data_string;
                    for (auto i = 0; i < global_array_init[name].size(); i++)
                    {
                        data_string += global_array_init[name][i];
                        if (i != global_array_init[name].size() - 1)
                            data_string += ", ";
                    }
                    emit_data(name + ": dd " + data_string);
                }
            }
            else
            {
                std::string init_val = global_init[name];
                emit_data(name + ": dd " + init_val);
            }
        }
    }

    for (const auto &op : static_vars)
    {
        auto entry = op.entry;
        std::string act_name = "_s_" + op.value;
        if (!entry)
            continue;
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
                    std::string name = act_name;
                    if (entry->isGlobal)
                        name = op.value;
                    std::string data_string;
                    for (auto i = 0; i < global_array_init[name].size(); i++)
                    {
                        data_string += global_array_init[name][i];
                        if (i != global_array_init[name].size() - 1)
                            data_string += ", ";
                    }
                    emit_data(act_name + ": db " + data_string);
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
                    std::string name = act_name;
                    if (entry->isGlobal)
                        name = op.value;
                    std::string data_string;
                    for (auto i = 0; i < global_array_init[name].size(); i++)
                    {
                        data_string += global_array_init[name][i];
                        if (i != global_array_init[name].size() - 1)
                            data_string += ", ";
                    }
                    emit_data(act_name + ": dd " + data_string);
                }
                else
                {
                    std::string init_val = global_init[act_name];
                    emit_data(act_name + ": dd " + init_val);
                }
            }
        }
    }
    for (auto i = 0; i < string_literals.size(); i++)
    {
        for (const auto &[str, cnt] : string_literals)
        {
            std::string modified_str = "`" + str.substr(1, str.length() - 2) + "`";
            if (cnt == i)
                emit_data("__str_" + std::to_string(cnt) + ": db " + modified_str + ", 0");
        }
    }
    for (auto i = 0; i < float_constants.size(); i++)
    {
        for (const auto &[str, cnt] : float_constants)
        {
            if (cnt == i)
            {
                if (is_float_constant(str))
                    emit_data("__f_" + std::to_string(cnt) + ": dd " + std::to_string(std::stof(str, nullptr)));
                else if (is_int_constant(str))
                    emit_data("__f_" + std::to_string(cnt) + ": dd " + std::to_string(std::stoi(str, nullptr, 0)) + ".0");
            }
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
        if (!entry)
            continue;
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
            if (I.result.entry->type.back() == '*' || I.result.entry->type.back() == '&')
            { // TODO: Handle more case also
                I.result.entry->isLive = 1;
            }
            else
            {
                I.result.entry->isLive = 0;
                I.result.entry->nextUse = -1;
            }
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
            std::cout << stringify(instr, true) << std::endl;
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
