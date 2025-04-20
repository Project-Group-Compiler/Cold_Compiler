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
        return "mov " + dest + ", " + src;
    }

    inline std::string mov_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "mov " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string mov_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        return "mov " + mem(dest_addr) + ", " + src_reg;
    }

    inline std::string mov_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        return "mov " + dest_reg + ", " + imm;
    }

    inline std::string mov_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        return "mov " + size + " " + mem(dest_addr) + ", " + imm;
    }

    // MOVZX instruction
    inline std::string movzx(const std::string &dest, const std::string &src)
    {
        return "movzx " + dest + ", " + src;
    }

    inline std::string movzx_reg_mem(const std::string &dest_reg, const std::string &size, const std::string &src_addr)
    {
        return "movzx " + dest_reg + ", " + size + " " + mem(src_addr);
    }

    // PUSH instruction
    inline std::string push(const std::string &reg)
    {
        return "push " + reg;
    }

    inline std::string push_mem(const std::string &size, const std::string &addr)
    {
        return "push " + size + " " + mem(addr);
    }

    inline std::string push_imm(const std::string &imm)
    {
        return "push " + imm;
    }

    // POP instruction
    inline std::string pop(const std::string &reg)
    {
        return "pop " + reg;
    }

    inline std::string pop_mem(const std::string &size, const std::string &addr)
    {
        return "pop " + size + " " + mem(addr);
    }

    // LEA instruction
    inline std::string lea(const std::string &dest_reg, const std::string &src_addr)
    {
        return "lea " + dest_reg + ", " + mem(src_addr);
    }

    // ===== Arithmetic Instructions =====

    // ADD instruction
    inline std::string add(const std::string &dest, const std::string &src)
    {
        return "add " + dest + ", " + src;
    }

    inline std::string add_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "add " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string add_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        return "add " + mem(dest_addr) + ", " + src_reg;
    }

    inline std::string add_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        return "add " + dest_reg + ", " + imm;
    }

    inline std::string add_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        return "add " + size + " " + mem(dest_addr) + ", " + imm;
    }

    // SUB instruction
    inline std::string sub(const std::string &dest, const std::string &src)
    {
        return "sub " + dest + ", " + src;
    }

    inline std::string sub_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "sub " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string sub_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        return "sub " + mem(dest_addr) + ", " + src_reg;
    }

    inline std::string sub_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        return "sub " + dest_reg + ", " + imm;
    }

    inline std::string sub_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        return "sub " + size + " " + mem(dest_addr) + ", " + imm;
    }

    // INC/DEC instructions
    inline std::string inc(const std::string &operand)
    {
        return "inc " + operand;
    }

    inline std::string inc_mem(const std::string &size, const std::string &addr)
    {
        return "inc " + size + " " + mem(addr);
    }

    inline std::string dec(const std::string &operand)
    {
        return "dec " + operand;
    }

    inline std::string dec_mem(const std::string &size, const std::string &addr)
    {
        return "dec " + size + " " + mem(addr);
    }

    // IMUL instruction
    inline std::string imul(const std::string &dest_reg, const std::string &src)
    {
        return "imul " + dest_reg + ", " + src;
    }

    inline std::string imul_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "imul " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string imul_three(const std::string &dest_reg, const std::string &src, const std::string &imm)
    {
        return "imul " + dest_reg + ", " + src + ", " + imm;
    }

    inline std::string imul_three_mem(const std::string &dest_reg, const std::string &src_addr, const std::string &imm)
    {
        return "imul " + dest_reg + ", " + mem(src_addr) + ", " + imm;
    }

    // IDIV instruction
    inline std::string idiv(const std::string &divisor)
    {
        return "idiv " + divisor;
    }

    inline std::string idiv_mem(const std::string &size, const std::string &addr)
    {
        return "idiv " + size + " " + mem(addr);
    }

    // ===== Logical Instructions =====

    // AND instruction
    inline std::string and_op(const std::string &dest, const std::string &src)
    {
        return "and " + dest + ", " + src;
    }

    inline std::string and_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "and " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string and_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        return "and " + mem(dest_addr) + ", " + src_reg;
    }

    inline std::string and_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        return "and " + dest_reg + ", " + imm;
    }

    inline std::string and_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        return "and " + size + " " + mem(dest_addr) + ", " + imm;
    }

    // OR instruction
    inline std::string or_op(const std::string &dest, const std::string &src)
    {
        return "or " + dest + ", " + src;
    }

    inline std::string or_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "or " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string or_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        return "or " + mem(dest_addr) + ", " + src_reg;
    }

    inline std::string or_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        return "or " + dest_reg + ", " + imm;
    }

    inline std::string or_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        return "or " + size + " " + mem(dest_addr) + ", " + imm;
    }

    // XOR instruction
    inline std::string xor_op(const std::string &dest, const std::string &src)
    {
        return "xor " + dest + ", " + src;
    }

    inline std::string xor_reg_mem(const std::string &dest_reg, const std::string &src_addr)
    {
        return "xor " + dest_reg + ", " + mem(src_addr);
    }

    inline std::string xor_mem_reg(const std::string &dest_addr, const std::string &src_reg)
    {
        return "xor " + mem(dest_addr) + ", " + src_reg;
    }

    inline std::string xor_reg_imm(const std::string &dest_reg, const std::string &imm)
    {
        return "xor " + dest_reg + ", " + imm;
    }

    inline std::string xor_mem_imm(const std::string &size, const std::string &dest_addr, const std::string &imm)
    {
        return "xor " + size + " " + mem(dest_addr) + ", " + imm;
    }

    // NOT & NEG instructions
    inline std::string not_op(const std::string &operand)
    {
        return "not " + operand;
    }

    inline std::string not_mem(const std::string &size, const std::string &addr)
    {
        return "not " + size + " " + mem(addr);
    }

    inline std::string neg(const std::string &operand)
    {
        return "neg " + operand;
    }

    inline std::string neg_mem(const std::string &size, const std::string &addr)
    {
        return "neg " + size + " " + mem(addr);
    }

    // SHL, SHR and SAR instructions
    inline std::string shl(const std::string &dest, const std::string &count)
    {
        return "shl " + dest + ", " + count;
    }

    inline std::string shl_mem(const std::string &size, const std::string &dest_addr, const std::string &count)
    {
        return "shl " + size + " " + mem(dest_addr) + ", " + count;
    }

    inline std::string shr(const std::string &dest, const std::string &count)
    {
        return "shr " + dest + ", " + count;
    }

    inline std::string shr_mem(const std::string &size, const std::string &dest_addr, const std::string &count)
    {
        return "shr " + size + " " + mem(dest_addr) + ", " + count;
    }

    inline std::string sar(const std::string &dest, const std::string &count)
    {
        return "sar " + dest + ", " + count;
    }

    inline std::string sar_mem(const std::string &size, const std::string &dest_addr, const std::string &count)
    {
        return "sar " + size + " " + mem(dest_addr) + ", " + count;
    }

    // ===== Control Flow Instructions =====
    // JMP instruction
    inline std::string jmp(const std::string &label)
    {
        return "jmp " + label;
    }

    // Conditional jumps
    inline std::string je(const std::string &label) { return "je " + label; }
    inline std::string jne(const std::string &label) { return "jne " + label; }
    inline std::string jz(const std::string &label) { return "jz " + label; }
    inline std::string jnz(const std::string &label) { return "jnz " + label; }
    inline std::string jg(const std::string &label) { return "jg " + label; }
    inline std::string jge(const std::string &label) { return "jge " + label; }
    inline std::string jl(const std::string &label) { return "jl " + label; }
    inline std::string jle(const std::string &label) { return "jle " + label; }
    inline std::string ja(const std::string &label) { return "ja " + label; }
    inline std::string jae(const std::string &label) { return "jae " + label; }
    inline std::string jb(const std::string &label) { return "jb " + label; }
    inline std::string jbe(const std::string &label) { return "jbe " + label; }

    // CMP instruction
    inline std::string cmp(const std::string &op1, const std::string &op2)
    {
        return "cmp " + op1 + ", " + op2;
    }

    inline std::string cmp_reg_mem(const std::string &reg, const std::string &addr)
    {
        return "cmp " + reg + ", " + mem(addr);
    }

    inline std::string cmp_mem_reg(const std::string &addr, const std::string &reg)
    {
        return "cmp " + mem(addr) + ", " + reg;
    }

    inline std::string cmp_reg_imm(const std::string &reg, const std::string &imm)
    {
        return "cmp " + reg + ", " + imm;
    }

    inline std::string cmp_mem_imm(const std::string &size, const std::string &addr, const std::string &imm)
    {
        return "cmp " + size + " " + mem(addr) + ", " + imm;
    }

    // CALL and RET instructions
    inline std::string call(const std::string &target)
    {
        return "call " + target;
    }

    inline std::string ret()
    {
        return "ret";
    }

    // NOP instruction
    inline std::string nop()
    {
        return "nop";
    }

    // Sign extension instructions
    inline std::string cdq() { return "cdq"; }
    inline std::string cbw() { return "cbw"; }
    inline std::string cwd() { return "cwd"; }
    inline std::string cwde() { return "cwde"; }

    // Stack frame setup/cleanup
    inline std::string enter(const std::string &size, const std::string &level = "0")
    {
        return "enter " + size + ", " + level;
    }

    inline std::string leave()
    {
        return "leave";
    }

    // ===== x87 FPU Instructions =====

    // Basic FPU Stack Operations
    inline std::string fld(const std::string &src)
    {
        return "fld " + src;
    }

    inline std::string fld_mem(const std::string &size, const std::string &addr)
    {
        return "fld " + size + " " + mem(addr);
    }

    inline std::string fld1() { return "fld1"; }     // Push +1.0 onto stack
    inline std::string fldz() { return "fldz"; }     // Push +0.0 onto stack
    inline std::string fldpi() { return "fldpi"; }   // Push π onto stack
    inline std::string fldl2e() { return "fldl2e"; } // Push log2(e) onto stack
    inline std::string fldl2t() { return "fldl2t"; } // Push log2(10) onto stack
    inline std::string fldlg2() { return "fldlg2"; } // Push log10(2) onto stack
    inline std::string fldln2() { return "fldln2"; } // Push loge(2) onto stack

    inline std::string fst(const std::string &dest)
    {
        return "fst " + dest;
    }

    inline std::string fst_mem(const std::string &size, const std::string &addr)
    {
        return "fst " + size + " " + mem(addr);
    }

    inline std::string fstp(const std::string &dest)
    {
        return "fstp " + dest;
    }

    inline std::string fstp_mem(const std::string &size, const std::string &addr)
    {
        return "fstp " + size + " " + mem(addr);
    }

    inline std::string fxch(const std::string &reg = "st1")
    {
        return "fxch " + reg;
    }

    // FPU Arithmetic Instructions
    inline std::string fadd(const std::string &src = "")
    {
        return src.empty() ? "fadd" : "fadd " + src;
    }

    inline std::string fadd_mem(const std::string &size, const std::string &addr)
    {
        return "fadd " + size + " " + mem(addr);
    }

    inline std::string faddp()
    {
        return "faddp";
    }

    inline std::string fsub(const std::string &src = "")
    {
        return src.empty() ? "fsub" : "fsub " + src;
    }

    inline std::string fsub_mem(const std::string &size, const std::string &addr)
    {
        return "fsub " + size + " " + mem(addr);
    }

    inline std::string fsubp()
    {
        return "fsubp";
    }

    inline std::string fsubr(const std::string &src = "")
    {
        return src.empty() ? "fsubr" : "fsubr " + src;
    }

    inline std::string fsubr_mem(const std::string &size, const std::string &addr)
    {
        return "fsubr " + size + " " + mem(addr);
    }

    inline std::string fsubrp(const std::string &dest = "st1")
    {
        return "fsubrp " + dest;
    }

    inline std::string fmul(const std::string &src = "")
    {
        return src.empty() ? "fmul" : "fmul " + src;
    }

    inline std::string fmul_mem(const std::string &size, const std::string &addr)
    {
        return "fmul " + size + " " + mem(addr);
    }

    inline std::string fmulp()
    {
        return "fmulp";
    }

    inline std::string fdiv(const std::string &src = "")
    {
        return src.empty() ? "fdiv" : "fdiv " + src;
    }

    inline std::string fdiv_mem(const std::string &size, const std::string &addr)
    {
        return "fdiv " + size + " " + mem(addr);
    }

    inline std::string fdivp()
    {
        return "fdivp";
    }

    inline std::string fdivr(const std::string &src = "")
    {
        return src.empty() ? "fdivr" : "fdivr " + src;
    }

    inline std::string fdivr_mem(const std::string &size, const std::string &addr)
    {
        return "fdivr " + size + " " + mem(addr);
    }

    inline std::string fdivrp(const std::string &dest = "st1")
    {
        return "fdivrp " + dest;
    }

    // FPU Comparison Instructions
    inline std::string fcom(const std::string &src = "")
    {
        return src.empty() ? "fcom" : "fcom " + src;
    }

    inline std::string fcom_mem(const std::string &size, const std::string &addr)
    {
        return "fcom " + size + " " + mem(addr);
    }

    inline std::string fcomp(const std::string &src = "")
    {
        return src.empty() ? "fcomp" : "fcomp " + src;
    }

    inline std::string fcomp_mem(const std::string &size, const std::string &addr)
    {
        return "fcomp " + size + " " + mem(addr);
    }

    inline std::string fcompp() { return "fcompp"; }
    inline std::string ftst() { return "ftst"; } // Compare ST0 with 0.0
    inline std::string fxam() { return "fxam"; } // Examine ST0
    inline std::string fucom(const std::string &src = "")
    {
        return src.empty() ? "fucom" : "fucom " + src;
    }
    inline std::string fucomp(const std::string &src = "")
    {
        return src.empty() ? "fucomp" : "fucomp " + src;
    }
    inline std::string fucompp() { return "fucompp"; }
    inline std::string fcomi(const std::string &src = "st1") { return "fcomi " + src; }
    inline std::string fcomip() { return "fcomip"; }
    inline std::string fucomi(const std::string &src = "st1") { return "fucomi " + src; }
    inline std::string fucomip(const std::string &src = "st1") { return "fucomip " + src; }

    // FPU Transcendental Instructions
    inline std::string fsqrt() { return "fsqrt"; }
    inline std::string fsin() { return "fsin"; }
    inline std::string fcos() { return "fcos"; }
    inline std::string fsincos() { return "fsincos"; }
    inline std::string fptan() { return "fptan"; }
    inline std::string fpatan() { return "fpatan"; }
    inline std::string f2xm1() { return "f2xm1"; }     // 2^x-1
    inline std::string fyl2x() { return "fyl2x"; }     // ST(1) * log2(ST(0))
    inline std::string fyl2xp1() { return "fyl2xp1"; } // ST(1) * log2(ST(0) + 1.0)

    // FPU Control Instructions
    inline std::string finit() { return "finit"; }   // Initialize FPU
    inline std::string fninit() { return "fninit"; } // Initialize FPU (no wait)
    inline std::string fclex() { return "fclex"; }   // Clear exceptions
    inline std::string fnclex() { return "fnclex"; } // Clear exceptions (no wait)
    inline std::string fstcw(const std::string &addr) { return "fstcw " + mem(addr); }
    inline std::string fnstcw(const std::string &addr) { return "fnstcw " + mem(addr); }
    inline std::string fldcw(const std::string &addr) { return "fldcw " + mem(addr); }
    inline std::string fstenv(const std::string &addr) { return "fstenv " + mem(addr); }
    inline std::string fnstenv(const std::string &addr) { return "fnstenv " + mem(addr); }
    inline std::string fldenv(const std::string &addr) { return "fldenv " + mem(addr); }
    inline std::string fsave(const std::string &addr) { return "fsave " + mem(addr); }
    inline std::string fnsave(const std::string &addr) { return "fnsave " + mem(addr); }
    inline std::string frstor(const std::string &addr) { return "frstor " + mem(addr); }
    inline std::string ffree(const std::string &reg = "st0") { return "ffree " + reg; }
    inline std::string fincstp() { return "fincstp"; } // Increment FPU stack pointer
    inline std::string fdecstp() { return "fdecstp"; } // Decrement FPU stack pointer
    inline std::string fwait() { return "fwait"; }     // Wait for FPU
    inline std::string fnop() { return "fnop"; }       // FPU no operation

    // Integer Transfers
    inline std::string fild(const std::string &size, const std::string &addr)
    {
        return "fild " + size + " " + mem(addr);
    }

    inline std::string fist(const std::string &size, const std::string &addr)
    {
        return "fist " + size + " " + mem(addr);
    }

    inline std::string fistp(const std::string &size, const std::string &addr)
    {
        return "fistp " + size + " " + mem(addr);
    }

    inline std::string fisttp(const std::string &size, const std::string &addr)
    {
        return "fisttp " + size + " " + mem(addr);
    }

} // namespace x86_lib

#endif
