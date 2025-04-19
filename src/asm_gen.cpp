/*
Rules to follow
-> Don't use arg1, arg2 or result directly without reg or mem unless it is constant
-> always pass willYouModify = 1 in getReg for now
-> also update value through update reg_desciptors
-> Check bss section for int y = *x, *x = 5 ..
*/

#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "x86_32_lib.hpp"

void print_error(const std::string &message);
/* Variable declaration */
extern std::string outputDir;
std::ofstream asm_file;
std::map<std::string, int> string_literals;
bool inside_fn = false;
bool inside_main = false;
bool fn_prologue_emitted = false;
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
std::map<std::string,operand> mem_operand;


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
            if (op.entry && op.entry->addrDesc.inStack == 2)
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
        emit_instr(";" + op.value + " = " + std::to_string(offset));
        if (offset < 0)
            memAddr = reg_names[EBP] + "+" + std::to_string(-offset);
        else
        {
            if(op.entry->isArray) offset += op.entry->size;
            memAddr = reg_names[EBP] + "-" + std::to_string(offset + 4);
        }
        if (op.entry->addrDesc.inStack == 2)
        {
            if (op.entry->addrDesc.inRegs.size())
                emit_instr(x86_lib::mov_mem_reg(memAddr, reg_names[op.entry->addrDesc.inRegs[0]]));
            else
            {
                std::cerr << op.value + " Error: Operand not found anywhere (getMem)\n";
                exit(1);
            }
        }
        op.entry->addrDesc.inStack = 1;
        return memAddr;
    }
    std::cerr << "Error: Operand entry is null in getMem\n";
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

void spillAllReg(){
    emit_instr("; spilling all reg");
    for(int reg=0;reg<uRegCnt;reg++){
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
    { // TODO ; remove
        std::cout << "Warning Don't use willYouModify = 0\n";
        if (addrDesc.inRegs.size())
            return addrDesc.inRegs[0];
        return -1;
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
            // emit_instr("; getreg wala"+op.value);
            spillReg(bestReg); // TODO: check value also
            // emit_instr("; getreg jdd"+op.value);
            regDesc[bestReg].push_back(op);
            if (op.entry)
            {
                op.entry->addrDesc.inRegs.push_back(bestReg);
            }
            return bestReg;
        }
    }
    int bestReg = getBestReg(resReg);
    std::string memAddr = getMem(op);
    emit_instr(";" + op.value + "_" + memAddr);
    if (op.entry && (op.entry->isGlobal || op.entry->isStatic > 0))
    {
        emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], op.value)); //[lexeme]
    }
    else
        emit_instr(x86_lib::mov_reg_mem(reg_names[bestReg], memAddr));

    regDesc[bestReg].push_back(op);
    if (op.entry)
        op.entry->addrDesc.inRegs.push_back(bestReg);

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
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg], op.value)); //[lexeme]
        }
        else
        {
            std::string memAddr = getMem(op);
            emit_instr(x86_lib::mov_reg_mem(reg_names[reg], memAddr));
        }
    }
    regDesc[reg].push_back(op);
    if (op.entry)
    {
        op.entry->addrDesc.inRegs.push_back(reg);
    }
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
            eraseFromVector(regDesc[i], op);
        regDesc[reg].push_back(op);
        op.entry->addrDesc.inRegs.clear();
        op.entry->addrDesc.inRegs.push_back(reg);
        op.entry->addrDesc.inStack = 2;
        op.entry->addrDesc.inHeap = 0;
    }
}
//It should be called for lhs => Contact if you are using it for other asso 
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
            if (top.entry){
                eraseFromVector(top.entry->addrDesc.inRegs, reg);
            }
        }

        for (int i = 0; i < uRegCnt; i++)
            eraseFromVector(regDesc[i], op);

        regDesc[reg].clear();
        regDesc[reg].push_back(op);
        op.entry->addrDesc.inRegs.clear();
        op.entry->addrDesc.inRegs.push_back(reg);
        // std::string memAddr = getMem(op);
        // emit_instr(x86_lib::mov_mem_reg(memAddr, reg_names[reg]));
        // op.entry->addrDesc.inStack = 1;
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
            emit_instr("\t; Block begins");
        bool regs_spilled = false;
        for (auto &instr : block)
        {
            if (print_comments)
                emit_instr("\t; " + stringify(instr, true));
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
                    inside_fn = false;
                    if (!regs_spilled)
                    {
                        if (print_comments)
                            emit_instr("\t; (spilling all registers)");
                        for (int reg = 0; reg < uRegCnt; reg++)
                            spillReg(reg);

                        regs_spilled = true;
                    }
                    emit_fn_epilogue();
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
            else if (curr_op == "=" || curr_op == "(f)=")
                emit_assign(instr);
            else if (curr_op == "+")
                emit_add(instr);
            else if (curr_op == "-")
                emit_sub(instr);
            else if (curr_op == "*")
                emit_mul(instr);
            else if (curr_op == "/")
                emit_div(instr);
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
            else if (curr_op == "&&")
                emit_logical_and(instr);
            else if (curr_op == "||")
                emit_logical_or(instr);
            else if (curr_op == "!")
                emit_logical_not(instr);
            else if (curr_op == "GOTO")
            {
                if (!regs_spilled)
                {
                    if (print_comments)
                        emit_instr("\t; (spilling all registers)");
                    for (int reg = 0; reg < uRegCnt; reg++)
                        spillReg(reg);
                    regs_spilled = true;
                }
                emit_goto(instr);
            }
            else if (curr_op == "ptr+")
                emit_logical_ptr_add(instr);
            else if (curr_op == "ptr-")
                emit_logical_ptr_sub(instr);
            else if (curr_op == "CopyToOffset"){
                emit_copy_to_offset(instr);
            }

            printReg_addr_Desc(instr.Label);
        }
        if (!regs_spilled)
        {
            if (print_comments)
                emit_instr("\t; (spilling all registers)");
            for (int reg = 0; reg < uRegCnt; reg++)
                spillReg(reg);

            regs_spilled = true;
        }
    }

    emit_section(".data");
    emit_data_section(); // add initialized data
    emit_section(".bss");
    emit_bss_section(); // add uninitialized data
}

void emit_copy_to_offset(quad &instr){
    spillAllReg();
    int reg1 = getReg(instr.arg1,1,{}); //reg1 -> value
    int reg2 = getReg(instr.arg2,1,{reg1}); //->reg2 -> offset
    int reg3 = getReg(instr.result,1,{reg1,reg2});
    std::string mem = getMem(instr.result); //arr
    emit_instr(x86_lib::lea(reg_names[reg3], mem));
    emit_instr(x86_lib::mov_mem_reg(reg_names[reg3]+"+"+reg_names[reg2], reg_names[reg1]));
}

void emit_logical_ptr_add(quad &instr){
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::add(reg_names[reg1], reg_names[reg2]));
    }
    // emit_instr("; khatam");
    updateRegDesc(reg1, instr.result);
}

void emit_logical_ptr_sub(quad &instr){
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::sub(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_unary_star(quad &instr){
    _debug_(stringify(instr));
    if(instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&'){ 
        // to = * ptr => to is a reference  
        int reg1 = getReg(instr.arg1, 1, {});
        // emit_instr(x86_lib::mov(reg_names[reg1], reg_names[reg1]));
        instr.result.entry->isDerefer = 1;
        updateRegDesc(reg1, instr.result);
    }else{
        spillAllReg();
        int reg1 = getReg(instr.arg1, 1, {});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg1], reg_names[reg1]));
        updateRegDesc(reg1, instr.result);
    }
}

void emit_unary_and(quad &instr){
    _debug_(stringify(instr));
    //ptr = &t1(&)
    if(instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&'){ 
        int reg1 = getReg(instr.arg1,1,{});
        // emit_instr(x86_lib::mov(reg_names[reg1], reg_names[reg1]));
        updateRegDesc(reg1,instr.result);
    }else{
        std::string mem = getMem(instr.arg1);
        int reg1 = getReg(instr.result,1,{});
        emit_instr(x86_lib::lea(reg_names[reg1], mem));
        updateRegDesc(reg1,instr.result);
    }
}

void emit_fn_defn(quad &instr)
{
    if (instr.op.substr(5, 5) == "4main")
    {
        emit_label("\nmain");
        inside_main = true;
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
        if (func_size % 4 != 0)
            func_size += 4 - (func_size % 4);
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

    if (instr.arg1.entry)
    {
        if (instr.arg1.value.substr(0, 6) == "__str_")
            emit_instr(x86_lib::push(instr.arg1.value));
        else if (instr.arg1.entry->isGlobal || instr.arg1.entry->isStatic > 0)
            emit_instr(x86_lib::push_mem("dword", instr.arg1.value)); // TODO : Check
        else if (instr.arg1.entry->addrDesc.inRegs.size())
            emit_instr(x86_lib::push(reg_names[instr.arg1.entry->addrDesc.inRegs[0]]));
        else if (instr.arg1.entry->addrDesc.inStack == 1)
            emit_instr(x86_lib::push_mem("dword", getMem(instr.arg1))); // TODO : Check

        int sz = instr.arg1.entry->size;
        if (sz % 4 != 0)
            sz += 4 - (sz % 4);
        params_size += sz;
    }
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
    if (inside_main)
    {
        emit_instr(x86_lib::xor_op(reg_names[EAX], reg_names[EAX]));
        inside_main = false;
    }
    emit_instr(x86_lib::pop(reg_names[EDI]));
    emit_instr(x86_lib::pop(reg_names[ESI]));
    emit_instr(x86_lib::pop(reg_names[EBX]));
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());
}

void emit_return(quad &instr)
{

    setParticularReg(EAX, instr.arg1);
    if (inside_main) // return at end of main -> OK
        inside_main = false;
}

void emit_goto(quad &instr)
{
    if (instr.arg1.value == "IF")
    {
        int reg2 = getReg(instr.arg2, 1, {});
        emit_instr(x86_lib::cmp_reg_imm(reg_names[reg2], "1"));
        emit_instr(x86_lib::je("L" + std::to_string(instr.gotoLabel)));
    }
    else
        emit_instr(x86_lib::jmp("L" + std::to_string(instr.gotoLabel)));
}
/* Logical Operator */
void emit_logical_and(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    int reg2 = getReg(instr.arg2, 1, {reg1});
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
    /* t0 = a && b
        if reg1 == 0 -> L1
        if reg2 == 0 -> L1
        t0 = 1 -> L2
        L1 -> to = 0
        L2:
    */
}

void emit_logical_or(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    int reg2 = getReg(instr.arg2, 1, {reg1});

    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);

    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg1], "1"));
    emit_instr(x86_lib::je(label1));
    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg2], "1"));
    emit_instr(x86_lib::je(label1));
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "0"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "1"));
    emit_label(label2);

    updateRegDesc(reg1, instr.result);

    /* t0 = a || b
        if reg1 == 1 -> L1
        if reg2 == 1 -> L1
        t0 = 0 -> L2
        L1 -> to = 1
        L2:
    */
}

void emit_logical_not(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});

    std::string label1 = "LBL" + std::to_string(labelCnt++);
    std::string label2 = "LBL" + std::to_string(labelCnt++);

    emit_instr(x86_lib::cmp_reg_imm(reg_names[reg1], "1"));
    emit_instr(x86_lib::je(label1));
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "1"));
    emit_instr(x86_lib::jmp(label2));
    emit_label(label1);
    emit_instr(x86_lib::mov_reg_imm(reg_names[reg1], "0"));
    emit_label(label2);

    updateRegDesc(reg1, instr.result);
    /* t0 = !a
        if reg1 == 1 -> L1
        t0 = 1 -> L2
        L1 -> to = 0
        L2:
    */
}

void emit_cmp(quad &instr)
{
    int reg1 = getReg(instr.arg1, 1, {});
    int reg2 = getReg(instr.arg2, 1, {reg1});
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

/* Bitwise Operator */
void emit_right_shift(quad &instr)
{ // check correct value of cl
    setParticularReg(ECX, instr.arg2);
    int reg1 = getReg(instr.arg1, 1, {ECX});
    emit_instr(x86_lib::shr(reg_names[reg1], "cl"));
    updateRegDesc(reg1, instr.result);
}

void emit_left_shift(quad &instr)
{
    setParticularReg(ECX, instr.arg2);
    int reg1 = getReg(instr.arg1, 1, {ECX});
    emit_instr(x86_lib::shl(reg_names[reg1], "cl"));
    updateRegDesc(reg1, instr.result);
}

void emit_or(quad &instr)
{
    // x = y | z
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::or_op(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_xor(quad &instr)
{
    // x = y ^ z
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::xor_op(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}
void emit_and(quad &instr)
{
    // x = y & z
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
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
    emit_instr(x86_lib::neg(reg_names[reg]));
    updateRegDesc(reg, instr.result);
}

void emit_not(quad &instr)
{
    int reg = getReg(instr.arg1, 1, {});
    emit_instr(x86_lib::not_op(reg_names[reg]));
    updateRegDesc(reg, instr.result);
}

void emit_div(quad &instr)
{ // c = a/b
    // mov arg1 to eax
    setParticularReg(EAX, instr.arg1);
    spillReg(EDX);
    emit_instr(x86_lib::xor_op(reg_names[EDX], reg_names[EDX]));

    int reg2 = getReg(instr.arg2, 1, {EAX, EDX});
    emit_instr(x86_lib::idiv(reg_names[reg2]));
    updateRegDesc(EAX, instr.result);
}

void emit_mod(quad &instr)
{ // c = a/b
    // mov arg1 to eax
    setParticularReg(EAX, instr.arg1);
    spillReg(EDX);
    emit_instr(x86_lib::xor_op(reg_names[EDX], reg_names[EDX]));

    int reg2 = getReg(instr.arg2, 1, {EAX, EDX});
    emit_instr(x86_lib::idiv(reg_names[reg2]));
    updateRegDesc(EDX, instr.result);
}

void emit_mul(quad &instr)
{
    // x = y * z
    int reg1 = getReg(instr.arg1, 1, {});
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
        emit_instr(x86_lib::imul(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_sub(quad &instr)
{
    // x = y - z
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::sub(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}
void emit_add(quad &instr)
{
    // x = y + z
    int reg1 = getReg(instr.arg1, 1, {});
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
        int reg2 = getReg(instr.arg2, 1, {reg1});
        emit_instr(x86_lib::add(reg_names[reg1], reg_names[reg2]));
    }
    updateRegDesc(reg1, instr.result);
}

void emit_assign(quad &instr)
{
    if (!inside_fn) // global
        return;

    // Pointer ->
    // TODO : is there any case of *ptr1 = *ptr2

    _debug_(stringify(instr));   
    if(instr.arg1.entry && instr.arg1.entry->isArray){//t0 = arr
        // spillAllReg();
        std::string mem = getMem(instr.arg1);
        int reg1 = getReg(instr.result,1,{});
        emit_instr(x86_lib::lea(reg_names[reg1], mem));
        updateRegDesc(reg1,instr.result);
    }else if(instr.result.entry && instr.result.entry->type.size() && instr.result.entry->type.back() == '&'){ 
        // can Handle integer constant more optimally
        //t0(&) = 5
        spillAllReg();
        int reg1 = getReg(instr.arg1,1,{});
        int reg2 = getReg(instr.result,1,{reg1});
        emit_instr(x86_lib::mov_mem_reg(reg_names[reg2], reg_names[reg1]));
        //make sure ki reg2 jis mem addr ko point kar raha hai uska instack = 1 honi chahiye
    }else if(instr.arg1.entry && instr.arg1.entry->type.size() && instr.arg1.entry->type.back() == '&'){
        //y = to(&)
        spillAllReg();
        int reg1 = getReg(instr.arg1,1,{});
        int reg2 = getReg(instr.result,1,{reg1});
        emit_instr(x86_lib::mov_reg_mem(reg_names[reg2], reg_names[reg1]));
        updateRegDesc(reg2,instr.result);
        //make sure ki reg1 jis mem addr ko point kar raha hai uska instack = 1 honi chahiye
    }
    else{
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
            std::string modified_str = "`" + str.substr(1, str.length() - 2) + "`";
            if (cnt == i)
                emit_data("__str_" + std::to_string(cnt) + ": db " + modified_str + ", 0");
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
            if(I.result.entry->type.back() == '*' || I.result.entry->type.back() == '&'){//TODO: Handle more case also
                I.result.entry->isLive = 1;
            }else{
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
