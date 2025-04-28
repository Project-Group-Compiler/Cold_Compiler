#ifndef X86_32_LIB_HPP
#define X86_32_LIB_HPP

#include <string>

namespace x86_lib
{
    // ===== Helper functions =====
    inline std::string mem(const std::string &addr)
    {
        return "[" + addr + "]";
    }

    // ===== Data Movement Instructions =====
    // MOV instruction
    inline std::string mov(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "mov";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "mov " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string mov_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "mov";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "mov " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string mov_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        x86_instr instr;
        instr.op = "mov";
        instr.arg1 = mem(dest_addr);
        instr.arg2 = src_reg;
        instr.printing = "mov " + mem(dest_addr) + ", " + src_reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string mov_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "mov";
        instr.arg1 = dest_reg;
        instr.arg2 = imm;
        instr.printing = "mov " + dest_reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string mov_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "mov";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = imm;
        instr.printing = "mov " + size + " " + mem(dest_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // MOVZX instruction
    inline std::string movzx(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "movzx";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "movzx " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string movzx_reg_mem(const std::string &dest_reg, const std::string &size, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "movzx";
        instr.size = size;
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "movzx " + dest_reg + ", " + size + " " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // PUSH instruction
    inline std::string push(const std::string &reg)
    {
        x86_instr instr;
        instr.op = "push";
        instr.arg1 = reg;
        instr.printing = "push " + reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string push_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "push";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "push " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string push_imm(const std::string &imm)
    {
        x86_instr instr;
        instr.op = "push";
        instr.arg1 = imm;
        instr.printing = "push " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // POP instruction
    inline std::string pop(const std::string &reg)
    {
        x86_instr instr;
        instr.op = "pop";
        instr.arg1 = reg;
        instr.printing = "pop " + reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string pop_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "pop";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "pop " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // LEA instruction
    inline std::string lea(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "lea";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "lea " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // ===== Arithmetic Instructions =====

    // ADD instruction
    inline std::string add(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "add";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "add " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string add_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "add";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "add " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string add_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        x86_instr instr;
        instr.op = "add";
        instr.arg1 = mem(dest_addr);
        instr.arg2 = src_reg;
        instr.printing = "add " + mem(dest_addr) + ", " + src_reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string add_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "add";
        instr.arg1 = dest_reg;
        instr.arg2 = imm;
        instr.printing = "add " + dest_reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string add_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "add";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = imm;
        instr.printing = "add " + size + " " + mem(dest_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // SUB instruction
    inline std::string sub(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "sub";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "sub " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string sub_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "sub";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "sub " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string sub_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        x86_instr instr;
        instr.op = "sub";
        instr.arg1 = mem(dest_addr);
        instr.arg2 = src_reg;
        instr.printing = "sub " + mem(dest_addr) + ", " + src_reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string sub_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "sub";
        instr.arg1 = dest_reg;
        instr.arg2 = imm;
        instr.printing = "sub " + dest_reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string sub_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "sub";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = imm;
        instr.printing = "sub " + size + " " + mem(dest_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // INC/DEC instructions
    inline std::string inc(const std::string &operand)
    {
        x86_instr instr;
        instr.op = "inc";
        instr.arg1 = operand;
        instr.printing = "inc " + operand;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string inc_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "inc";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "inc " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string dec(const std::string &operand)
    {
        x86_instr instr;
        instr.op = "dec";
        instr.arg1 = operand;
        instr.printing = "dec " + operand;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string dec_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "dec";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "dec " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // IMUL instruction
    inline std::string imul(const std::string &dest_reg, const std::string &src)
    {
        x86_instr instr;
        instr.op = "imul";
        instr.arg1 = dest_reg;
        instr.arg2 = src;
        instr.printing = "imul " + dest_reg + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string imul_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "imul";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "imul " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string imul_three(const std::string &dest_reg, const std::string &src, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "imul";
        instr.arg1 = dest_reg;
        instr.arg2 = src;
        instr.arg3 = imm;
        instr.printing = "imul " + dest_reg + ", " + src + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string imul_three_mem(const std::string &dest_reg, const std::string &src_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "imul";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.arg3 = imm;
        instr.printing = "imul " + dest_reg + ", " + mem(src_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // IDIV instruction
    inline std::string idiv(const std::string &divisor)
    {
        x86_instr instr;
        instr.op = "idiv";
        instr.arg1 = divisor;
        instr.printing = "idiv " + divisor;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string idiv_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "idiv";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "idiv " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // ===== Logical Instructions =====

    // AND instruction
    inline std::string and_op(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "and";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "and " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string and_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "and";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "and " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string and_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        x86_instr instr;
        instr.op = "and";
        instr.arg1 = mem(dest_addr);
        instr.arg2 = src_reg;
        instr.printing = "and " + mem(dest_addr) + ", " + src_reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string and_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "and";
        instr.arg1 = dest_reg;
        instr.arg2 = imm;
        instr.printing = "and " + dest_reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string and_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "and";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = imm;
        instr.printing = "and " + size + " " + mem(dest_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // OR instruction
    inline std::string or_op(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "or";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "or " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string or_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "or";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "or " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string or_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        x86_instr instr;
        instr.op = "or";
        instr.arg1 = mem(dest_addr);
        instr.arg2 = src_reg;
        instr.printing = "or " + mem(dest_addr) + ", " + src_reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string or_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "or";
        instr.arg1 = dest_reg;
        instr.arg2 = imm;
        instr.printing = "or " + dest_reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string or_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "or";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = imm;
        instr.printing = "or " + size + " " + mem(dest_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // XOR instruction
    inline std::string xor_op(const std::string &dest, const std::string &src)
    {
        x86_instr instr;
        instr.op = "xor";
        instr.arg1 = dest;
        instr.arg2 = src;
        instr.printing = "xor " + dest + ", " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string xor_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        x86_instr instr;
        instr.op = "xor";
        instr.arg1 = dest_reg;
        instr.arg2 = mem(src_addr);
        instr.printing = "xor " + dest_reg + ", " + mem(src_addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string xor_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        x86_instr instr;
        instr.op = "xor";
        instr.arg1 = mem(dest_addr);
        instr.arg2 = src_reg;
        instr.printing = "xor " + mem(dest_addr) + ", " + src_reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string xor_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "xor";
        instr.arg1 = dest_reg;
        instr.arg2 = imm;
        instr.printing = "xor " + dest_reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string xor_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "xor";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = imm;
        instr.printing = "xor " + size + " " + mem(dest_addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // NOT & NEG instructions
    inline std::string not_op(const std::string &operand)
    {
        x86_instr instr;
        instr.op = "not";
        instr.arg1 = operand;
        instr.printing = "not " + operand;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string not_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "not";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "not " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string neg(const std::string &operand)
    {
        x86_instr instr;
        instr.op = "neg";
        instr.arg1 = operand;
        instr.printing = "neg " + operand;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string neg_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "neg";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "neg " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // SHL, SHR and SAR instructions
    inline std::string shl(const std::string &dest, const std::string &count)
    {
        x86_instr instr;
        instr.op = "shl";
        instr.arg1 = dest;
        instr.arg2 = count;
        instr.printing = "shl " + dest + ", " + count;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string shl_mem(const std::string &size, const std::string &dest_addr, const std::string &count)
    {
        x86_instr instr;
        instr.op = "shl";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = count;
        instr.printing = "shl " + size + " " + mem(dest_addr) + ", " + count;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string shr(const std::string &dest, const std::string &count)
    {
        x86_instr instr;
        instr.op = "shr";
        instr.arg1 = dest;
        instr.arg2 = count;
        instr.printing = "shr " + dest + ", " + count;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string shr_mem(const std::string &size, const std::string &dest_addr, const std::string &count)
    {
        x86_instr instr;
        instr.op = "shr";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = count;
        instr.printing = "shr " + size + " " + mem(dest_addr) + ", " + count;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string sar(const std::string &dest, const std::string &count)
    {
        x86_instr instr;
        instr.op = "sar";
        instr.arg1 = dest;
        instr.arg2 = count;
        instr.printing = "sar " + dest + ", " + count;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string sar_mem(const std::string &size, const std::string &dest_addr, const std::string &count)
    {
        x86_instr instr;
        instr.op = "sar";
        instr.size = size;
        instr.arg1 = mem(dest_addr);
        instr.arg2 = count;
        instr.printing = "sar " + size + " " + mem(dest_addr) + ", " + count;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // ===== Control Flow Instructions =====
    // JMP instruction
    inline std::string jmp(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jmp";
        instr.arg1 = label;
        instr.printing = "jmp " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // Conditional jumps
    inline std::string je(const std::string &label)
    {
        x86_instr instr;
        instr.op = "je";
        instr.arg1 = label;
        instr.printing = "je " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jne(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jne";
        instr.arg1 = label;
        instr.printing = "jne " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jz(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jz";
        instr.arg1 = label;
        instr.printing = "jz " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jnz(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jnz";
        instr.arg1 = label;
        instr.printing = "jnz " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jg(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jg";
        instr.arg1 = label;
        instr.printing = "jg " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jge(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jge";
        instr.arg1 = label;
        instr.printing = "jge " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jl(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jl";
        instr.arg1 = label;
        instr.printing = "jl " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jle(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jle";
        instr.arg1 = label;
        instr.printing = "jle " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string ja(const std::string &label)
    {
        x86_instr instr;
        instr.op = "ja";
        instr.arg1 = label;
        instr.printing = "ja " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jae(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jae";
        instr.arg1 = label;
        instr.printing = "jae " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jb(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jb";
        instr.arg1 = label;
        instr.printing = "jb " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string jbe(const std::string &label)
    {
        x86_instr instr;
        instr.op = "jbe";
        instr.arg1 = label;
        instr.printing = "jbe " + label;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // CMP instruction
    inline std::string cmp(const std::string &op1, const std::string &op2)
    {
        x86_instr instr;
        instr.op = "cmp";
        instr.arg1 = op1;
        instr.arg2 = op2;
        instr.printing = "cmp " + op1 + ", " + op2;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cmp_reg_mem(const std::string &reg, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "cmp";
        instr.arg1 = reg;
        instr.arg2 = mem(addr);
        instr.printing = "cmp " + reg + ", " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cmp_mem_reg(const std::string &addr, const std::string &reg)
    {
        x86_instr instr;
        instr.op = "cmp";
        instr.arg1 = mem(addr);
        instr.arg2 = reg;
        instr.printing = "cmp " + mem(addr) + ", " + reg;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cmp_reg_imm(const std::string &reg, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "cmp";
        instr.arg1 = reg;
        instr.arg2 = imm;
        instr.printing = "cmp " + reg + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cmp_mem_imm(const std::string &size, const std::string &addr, const std::string &imm)
    {
        x86_instr instr;
        instr.op = "cmp";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.arg2 = imm;
        instr.printing = "cmp " + size + " " + mem(addr) + ", " + imm;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // CALL and RET instructions
    inline std::string call(const std::string &target)
    {
        x86_instr instr;
        instr.op = "call";
        instr.arg1 = target;
        instr.printing = "call " + target;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string ret()
    {
        x86_instr instr;
        instr.op = "ret";
        instr.printing = "ret";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // NOP instruction
    inline std::string nop()
    {
        x86_instr instr;
        instr.op = "nop";
        instr.printing = "nop";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // Sign extension instructions
    inline std::string cdq()
    {
        x86_instr instr;
        instr.op = "cdq";
        instr.printing = "cdq";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cbw()
    {
        x86_instr instr;
        instr.op = "cbw";
        instr.printing = "cbw";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cwd()
    {
        x86_instr instr;
        instr.op = "cwd";
        instr.printing = "cwd";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string cwde()
    {
        x86_instr instr;
        instr.op = "cwde";
        instr.printing = "cwde";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // Stack frame setup/cleanup
    inline std::string enter(const std::string &size, const std::string &level = "0")
    {
        x86_instr instr;
        instr.op = "enter";
        instr.arg1 = size;
        instr.arg2 = level;
        instr.printing = "enter " + size + ", " + level;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string leave()
    {
        x86_instr instr;
        instr.op = "leave";
        instr.printing = "leave";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // ===== x87 FPU Instructions =====

    // Basic FPU Stack Operations
    inline std::string fld(const std::string &src)
    {
        x86_instr instr;
        instr.op = "fld";
        instr.arg1 = src;
        instr.printing = "fld " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fld_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fld";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fld " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fld1()
    {
        x86_instr instr;
        instr.op = "fld1";
        instr.printing = "fld1";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fldz()
    {
        x86_instr instr;
        instr.op = "fldz";
        instr.printing = "fldz";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fldpi()
    {
        x86_instr instr;
        instr.op = "fldpi";
        instr.printing = "fldpi";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fldl2e()
    {
        x86_instr instr;
        instr.op = "fldl2e";
        instr.printing = "fldl2e";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fldl2t()
    {
        x86_instr instr;
        instr.op = "fldl2t";
        instr.printing = "fldl2t";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fldlg2()
    {
        x86_instr instr;
        instr.op = "fldlg2";
        instr.printing = "fldlg2";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fldln2()
    {
        x86_instr instr;
        instr.op = "fldln2";
        instr.printing = "fldln2";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fst(const std::string &dest)
    {
        x86_instr instr;
        instr.op = "fst";
        instr.arg1 = dest;
        instr.printing = "fst " + dest;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fst_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fst";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fst " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fstp(const std::string &dest)
    {
        x86_instr instr;
        instr.op = "fstp";
        instr.arg1 = dest;
        instr.printing = "fstp " + dest;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fstp_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fstp";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fstp " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // FPU Arithmetic Instructions
    inline std::string fadd(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fadd";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fadd" : "fadd " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fadd_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fadd";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fadd " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string faddp()
    {
        x86_instr instr;
        instr.op = "faddp";
        instr.printing = "faddp";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsub(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fsub";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fsub" : "fsub " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsub_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fsub";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fsub " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsubp()
    {
        x86_instr instr;
        instr.op = "fsubp";
        instr.printing = "fsubp";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsubr(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fsubr";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fsubr" : "fsubr " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsubr_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fsubr";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fsubr " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsubrp(const std::string &dest = "st1")
    {
        x86_instr instr;
        instr.op = "fsubrp";
        instr.arg1 = dest;
        instr.printing = "fsubrp " + dest;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fmul(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fmul";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fmul" : "fmul " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fmul_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fmul";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fmul " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fmulp()
    {
        x86_instr instr;
        instr.op = "fmulp";
        instr.printing = "fmulp";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fdiv(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fdiv";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fdiv" : "fdiv " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fdiv_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fdiv";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fdiv " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fdivp()
    {
        x86_instr instr;
        instr.op = "fdivp";
        instr.printing = "fdivp";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fdivr(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fdivr";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fdivr" : "fdivr " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fdivr_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fdivr";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fdivr " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fdivrp(const std::string &dest = "st1")
    {
        x86_instr instr;
        instr.op = "fdivrp";
        instr.arg1 = dest;
        instr.printing = "fdivrp " + dest;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // FPU Comparison Instructions
    inline std::string fcomp(const std::string &src = "")
    {
        x86_instr instr;
        instr.op = "fcomp";
        instr.arg1 = src;
        instr.printing = src.empty() ? "fcomp" : "fcomp " + src;
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fcomp_mem(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fcomp";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fcomp " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fcompp()
    {
        x86_instr instr;
        instr.op = "fcompp";
        instr.printing = "fcompp";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fcomip()
    {
        x86_instr instr;
        instr.op = "fcomip";
        instr.printing = "fcomip";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // FPU Transcendental Instructions
    inline std::string fsqrt()
    {
        x86_instr instr;
        instr.op = "fsqrt";
        instr.printing = "fsqrt";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsin()
    {
        x86_instr instr;
        instr.op = "fsin";
        instr.printing = "fsin";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fcos()
    {
        x86_instr instr;
        instr.op = "fcos";
        instr.printing = "fcos";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fsincos()
    {
        x86_instr instr;
        instr.op = "fsincos";
        instr.printing = "fsincos";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fptan()
    {
        x86_instr instr;
        instr.op = "fptan";
        instr.printing = "fptan";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fpatan()
    {
        x86_instr instr;
        instr.op = "fpatan";
        instr.printing = "fpatan";
        asm_instr.push_back(instr);
        return instr.printing;
    }

    // Integer Transfers
    inline std::string fild(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fild";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fild " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fist(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fist";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fist " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fistp(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fistp";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fistp " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

    inline std::string fisttp(const std::string &size, const std::string &addr)
    {
        x86_instr instr;
        instr.op = "fisttp";
        instr.size = size;
        instr.arg1 = mem(addr);
        instr.printing = "fisttp " + size + " " + mem(addr);
        asm_instr.push_back(instr);
        return instr.printing;
    }

} // namespace x86_lib

#endif