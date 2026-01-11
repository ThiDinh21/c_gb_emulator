#include <stdio.h>
#include "opcodes.h"
#include "errors.h"

uint8_t decode(CPU *cpu, uint8_t opcode)
{
    switch (opcode)
    {
    case 0x00:
        // NOP
        return op_00();
    case 0x01:
        // LD BC, u16
        return op_01(cpu);
    case 0x02:
        // LD (BC), A
        return op_02(cpu);
    case 0x03:
        // INC BC
        return op_03(cpu);
    case 0x04:
        // INC B
        return op_04(cpu);
    case 0x05:
        // DEC B
        return op_05(cpu);
    case 0x06:
        // LD B, u8
        return op_06(cpu);
    case 0x07:
        // RLCA
        return op_07(cpu);
    case 0x08:
        // LD (u16), SP
        return op_08(cpu);
    case 0x09:
        // ADD HL, BC
        return op_09(cpu);
    case 0x0A:
        // LD A, (BC)
        return op_0a(cpu);
    case 0x0B:
        // DEC BC
        return op_0b(cpu);
    case 0x0C:
        // INC C
        return op_0c(cpu);
    case 0x0D:
        // DEC C
        return op_0d(cpu);
    case 0x0E:
        // LD C, u8
        return op_0e(cpu);
    case 0x0F:
        // RRCA
        return op_0f(cpu);
    case 0x10:
        // STOP
        return op_10(cpu);
    case 0x11:
        // LD DE, u16
        return op_11(cpu);
    case 0x12:
        // LD (DE), A
        return op_12(cpu);
    case 0x13:
        // INC DE
        return op_13(cpu);
    case 0x14:
        // INC D
        return op_14(cpu);
    case 0x15:
        // DEC D
        return op_15(cpu);
    case 0x16:
        // LD D, u8
        return op_16(cpu);
    case 0x17:
        // LD RLA
        return op_17(cpu);
    case 0x18:
        // JR i8
        return op_18(cpu);
    case 0x19:
        // ADD HL, DE
        return op_19(cpu);
    case 0x1A:
        // LD A, (DE)
        return op_1a(cpu);
    case 0x1B:
        // DEC DE
        return op_1b(cpu);
    case 0x1C:
        // INC E
        return op_1c(cpu);
    case 0x1D:
        // DEC E
        return op_1d(cpu);
    case 0x1E:
        // LD E, u8
        return op_1e(cpu);
    case 0x1F:
        // RRA
        return op_1f(cpu);
    case 0x20:
        // JR NZ, i8
        return op_20(cpu);
    case 0x21:
        // LD HL, u16
        return op_21(cpu);
    case 0x22:
        // LD (HL+), A
        return op_22(cpu);
    case 0x23:
        // INC HL
        return op_23(cpu);
    case 0x24:
        return op_24(cpu);
    case 0x25:
        return op_25(cpu);
    case 0x26:
        return op_26(cpu);
    case 0x27:
        return op_27(cpu);
    case 0x28:
        return op_28(cpu);
    case 0x29:
        return op_29(cpu);
    case 0x2A:
        return op_2a(cpu);
    case 0x2B:
        return op_2b(cpu);
    case 0x2C:
        return op_2c(cpu);
    case 0x2D:
        return op_2d(cpu);
    case 0x2E:
        return op_2e(cpu);
    case 0x2F:
        return op_2f(cpu);
    case 0x30:
        return op_30(cpu);
    case 0x31:
        return op_31(cpu);
    case 0x32:
        return op_32(cpu);
    case 0x33:
        return op_33(cpu);
    case 0x34:
        return op_34(cpu);
    case 0x35:
        return op_35(cpu);
    case 0x36:
        return op_36(cpu);
    case 0x37:
        return op_37(cpu);
    case 0x38:
        return op_38(cpu);
    case 0x39:
        return op_39(cpu);
    case 0x3A:
        return op_3a(cpu);
    case 0x3B:
        return op_3b(cpu);
    case 0x3C:
        return op_3c(cpu);
    case 0x3D:
        return op_3d(cpu);
    case 0x3E:
        return op_3e(cpu);
    case 0x3F:
        return op_3f(cpu);
    default:
        char err_msg[30];
        snprintf(err_msg, 30, "Opcode not recognized: 0x%x\n", opcode);
        panic(err_msg, ERR_UNKNOWN_INSTRUCTION);
    }
}

// NOP
uint8_t op_00(void)
{
    return 4;
}

// LD BC, u16
uint8_t op_01(CPU *cpu)
{
    set_bc(cpu, cpu_fetch_u16(cpu));
    return 12;
}

// LD (BC), A
uint8_t op_02(CPU *cpu)
{
    write_mem(cpu->mmu, get_bc(cpu), cpu->a);
    return 8;
}

// INC BC
uint8_t op_03(CPU *cpu)
{
    uint16_t new_bc = alu_inc_u16(get_bc(cpu));
    set_bc(cpu, new_bc);
    return 8;
}

// INC B
uint8_t op_04(CPU *cpu)
{
    alu_inc(cpu, &(cpu->b));
    return 4;
}

// DEC B
uint8_t op_05(CPU *cpu)
{
    alu_dec(cpu, &(cpu->b));
    return 4;
}

// LD B, u8
uint8_t op_06(CPU *cpu)
{
    cpu->b = cpu_fetch_u8(cpu);
    return 8;
}

// RLCA
uint8_t op_07(CPU *cpu)
{
    bool c_flag = cpu->a >> 7;
    cpu->a = (cpu->a << 1) | c_flag;

    set_flag(cpu, Z_FLAG, 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
    return 4;
}

// LD (u16), SP
uint8_t op_08(CPU *cpu)
{
    uint16_t addr = cpu_fetch_u16(cpu);
    write_mem_u16(cpu->mmu, addr, cpu->sp);
    return 20;
}

// ADD HL, BC
uint8_t op_09(CPU *cpu)
{
    uint16_t bc = get_bc(cpu);
    alu_add_u16(cpu, bc);
    return 8;
}

// LD A, (BC)
uint8_t op_0a(CPU *cpu)
{
    uint8_t val = read_mem(cpu->mmu, get_bc(cpu));
    cpu->a = val;
    return 8;
}

// DEC BC
uint8_t op_0b(CPU *cpu)
{
    uint16_t new_bc = alu_dec_u16(get_bc(cpu));
    set_bc(cpu, new_bc);
    return 8;
}

// INC C
uint8_t op_0c(CPU *cpu)
{
    alu_inc(cpu, &(cpu->c));
    return 4;
}

// DEC C
uint8_t op_0d(CPU *cpu)
{
    alu_dec(cpu, &(cpu->c));
    return 4;
}

// LD C, u8
uint8_t op_0e(CPU *cpu)
{
    uint8_t val = cpu_fetch_u8(cpu);
    cpu->c = val;
    return 8;
}

// RRCA
uint8_t op_0f(CPU *cpu)
{
    bool c_flag = cpu->a & 0x01;
    cpu->a = (cpu->a >> 1) | (c_flag << 7);

    set_flag(cpu, Z_FLAG, 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
    return 4;
}

// STOP
uint8_t op_10(CPU *cpu)
{
    cpu_stop(cpu);
    return 4;
}

// LD DE, u16
uint8_t op_11(CPU *cpu)
{
    set_de(cpu, cpu_fetch_u16(cpu));
    return 12;
}

// LD (DE), A
uint8_t op_12(CPU *cpu)
{
    write_mem(cpu->mmu, get_de(cpu), cpu->a);
    return 8;
}

// INC DE
uint8_t op_13(CPU *cpu)
{
    uint16_t new_de = alu_inc_u16(get_de(cpu));
    set_de(cpu, new_de);
    return 8;
}

// INC D
uint8_t op_14(CPU *cpu)
{
    alu_inc(cpu, &(cpu->d));
    return 4;
}

// DEC D
uint8_t op_15(CPU *cpu)
{
    alu_dec(cpu, &(cpu->d));
    return 4;
}

// LD D, u8
uint8_t op_16(CPU *cpu)
{
    cpu->d = cpu_fetch_u8(cpu);
    return 8;
}

// RLA
uint8_t op_17(CPU *cpu)
{
    uint8_t c_flag_old = cpu->flags & C_FLAG ? 1 : 0;
    bool c_flag = cpu->a >> 7;
    cpu->a = (cpu->a << 1) | c_flag_old;

    set_flag(cpu, Z_FLAG, 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
    return 4;
}

// JR i8
uint8_t op_18(CPU *cpu)
{
    int8_t operand = (int8_t)cpu_fetch_u8(cpu);
    cpu->program_counter += operand;
    return 12;
}

// ADD HL, DE
uint8_t op_19(CPU *cpu)
{
    uint16_t de = get_de(cpu);
    alu_add_u16(cpu, de);
    return 8;
}

// LD A, (DE)
uint8_t op_1a(CPU *cpu)
{
    uint8_t val = read_mem(cpu->mmu, get_de(cpu));
    cpu->a = val;
    return 8;
}

// DEC DE
uint8_t op_1b(CPU *cpu)
{
    uint16_t new_de = alu_dec_u16(get_de(cpu));
    set_de(cpu, new_de);
    return 8;
}

// INC E
uint8_t op_1c(CPU *cpu)
{
    alu_inc(cpu, &(cpu->e));
    return 4;
}

// DEC E
uint8_t op_1d(CPU *cpu)
{
    alu_dec(cpu, &(cpu->e));
    return 4;
}

// LD E, u8
uint8_t op_1e(CPU *cpu)
{
    uint8_t val = cpu_fetch_u8(cpu);
    cpu->e = val;
    return 8;
}

// RRA
uint8_t op_1f(CPU *cpu)
{
    bool c_flag_old = cpu->flags & C_FLAG ? 1 : 0;
    bool c_flag = cpu->a & 0x01;
    cpu->a = (cpu->a >> 1) | (c_flag_old << 7);

    set_flag(cpu, Z_FLAG, 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
    return 4;
}

// JR NZ, i8
uint8_t op_20(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    int8_t operand = (int8_t)cpu_fetch_u8(cpu);
    if (z_flag)
    {
        return 8;
    }
    else
    {
        cpu->program_counter += operand;
        return 12;
    }
}

// LD HL, u16
uint8_t op_21(CPU *cpu)
{
    set_hl(cpu, cpu_fetch_u16(cpu));
    return 12;
}

// LD (HL+), A
uint8_t op_22(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    write_mem(cpu->mmu, hl, cpu->a);
    set_hl(cpu, alu_inc_u16(hl));
    return 8;
}

// INC HL
uint8_t op_23(CPU *cpu)
{
    uint16_t curr_hl = get_hl(cpu);
    set_hl(cpu, alu_inc_u16(curr_hl));
    return 8;
}

// INC H
uint8_t op_24(CPU *cpu)
{
    alu_inc(cpu, &(cpu->h));
    return 4;
}

// DEC H
uint8_t op_25(CPU *cpu)
{
    alu_dec(cpu, &(cpu->h));
    return 4;
}

// LD H, u8
uint8_t op_26(CPU *cpu)
{
    cpu->h = cpu_fetch_u8(cpu);
    return 8;
}

// DAA
uint8_t op_27(CPU *cpu)
{
    bool n_flag = cpu->flags & N_FLAG ? 1 : 0;
    bool h_flag = cpu->flags & H_FLAG ? 1 : 0;
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    uint8_t adjustment = 0;

    if (n_flag)
    {
        adjustment += h_flag ? 0x06 : 0;
        adjustment += c_flag ? 0x60 : 0;
        __builtin_sub_overflow(cpu->a, adjustment, &(cpu->a));
    }
    else
    {
        if (h_flag || (cpu->a & 0x0F) > 0x09)
        {
            adjustment += 0x06;
        }
        if (c_flag || cpu->a > 0x99)
        {
            adjustment += 0x60;
            set_flag(cpu, C_FLAG, true);
        }
        __builtin_add_overflow(cpu->a, adjustment, &(cpu->a));
    }

    set_flag(cpu, Z_FLAG, cpu->a == 0);
    set_flag(cpu, H_FLAG, 0);
    return 4;
}

// JR Z, i8
uint8_t op_28(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    int8_t operand = (int8_t)cpu_fetch_u8(cpu);
    if (z_flag)
    {
        cpu->program_counter += operand;
        return 12;
    }
    else
    {
        return 8;
    }
}

// ADD HL, HL
uint8_t op_29(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    alu_add_u16(cpu, hl);
    return 8;
}

// LD A, (HL+)
uint8_t op_2a(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    uint8_t val = read_mem(cpu->mmu, hl);
    cpu->a = val;
    set_hl(cpu, alu_inc_u16(hl));
    return 8;
}

// DEC HL
uint8_t op_2b(CPU *cpu)
{
    set_hl(cpu, alu_dec_u16(get_hl(cpu)));
    return 8;
}

// INC L
uint8_t op_2c(CPU *cpu)
{
    alu_inc(cpu, &(cpu->l));
    return 4;
}

// DEC L
uint8_t op_2d(CPU *cpu)
{
    alu_dec(cpu, &(cpu->l));
    return 4;
}

// LD L, u8
uint8_t op_2e(CPU *cpu)
{
    uint8_t val = cpu_fetch_u8(cpu);
    cpu->l = val;
    return 8;
}

// CPL
uint8_t op_2f(CPU *cpu)
{
    alu_not(cpu);
    return 4;
}

// JR NC, i8
uint8_t op_30(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    int8_t operand = (int8_t)cpu_fetch_u8(cpu);
    if (c_flag)
    {
        return 8;
    }
    else
    {
        cpu->program_counter += operand;
        return 12;
    }
}

// LD SP, u16
uint8_t op_31(CPU *cpu)
{
    cpu->sp = cpu_fetch_u16(cpu);
    return 12;
}

// LD (HL-), A
uint8_t op_32(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    write_mem(cpu->mmu, hl, cpu->a);
    set_hl(cpu, alu_dec_u16(hl));
    return 8;
}

// INC SP
uint8_t op_33(CPU *cpu)
{
    cpu->sp = alu_inc_u16(cpu->sp);
    return 8;
}

// INC (HL)
uint8_t op_34(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    uint8_t val = read_mem(cpu->mmu, hl);
    val++;
    write_mem(cpu->mmu, hl, val);

    set_flag(cpu, Z_FLAG, val == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, (val & 0x0F) == 0);
    return 12;
}

// DEC (HL)
uint8_t op_35(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    uint8_t val = read_mem(cpu->mmu, hl);
    val--;
    write_mem(cpu->mmu, hl, val);

    set_flag(cpu, Z_FLAG, val == 0);
    set_flag(cpu, N_FLAG, 1);
    set_flag(cpu, H_FLAG, (val & 0x0F) == 0x0F);
    return 12;
}

// LD (HL), u8
uint8_t op_36(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    write_mem(cpu->mmu, hl, cpu_fetch_u8(cpu));
    return 12;
}

// SCF
uint8_t op_37(CPU *cpu)
{
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, 1);
    return 4;
}

// JR C, i8
uint8_t op_38(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    int8_t operand = (int8_t)cpu_fetch_u8(cpu);
    if (c_flag)
    {
        cpu->program_counter += operand;
        return 12;
    }
    else
    {
        return 8;
    }
}

// ADD HL, SP
uint8_t op_39(CPU *cpu)
{
    alu_add_u16(cpu, cpu->sp);
    return 8;
}

// LD A, (HL-)
uint8_t op_3a(CPU *cpu)
{
    uint16_t hl = get_hl(cpu);
    uint8_t val = read_mem(cpu->mmu, hl);
    cpu->a = val;
    set_hl(cpu, alu_dec_u16(hl));
    return 8;
}

// DEC SP
uint8_t op_3b(CPU *cpu)
{
    cpu->sp--;
    return 8;
}

// INC A
uint8_t op_3c(CPU *cpu)
{
    alu_inc(cpu, &(cpu->a));
    return 4;
}

// DEC A
uint8_t op_3d(CPU *cpu)
{
    alu_dec(cpu, &(cpu->a));
    return 4;
}

// LD A, u8
uint8_t op_3e(CPU *cpu)
{
    uint8_t val = cpu_fetch_u8(cpu);
    cpu->a = val;
    return 8;
}

// CCF
uint8_t op_3f(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;

    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, !c_flag);
    return 4;
}