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
    case 0x40 ... 0x7F:
        return op_40_to_7f(cpu, opcode);
    case 0x80 ... 0xBF:
        return op_80_to_bf(cpu, opcode);
    case 0xC0:
        return op_c0(cpu); // RET NZ
    case 0xC1:
        return op_c1(cpu); // POP BC
    case 0xC2:
        return op_c2(cpu); // JP NZ, a16
    case 0xC3:
        return op_c3(cpu); // JP a16
    case 0xC4:
        return op_c4(cpu); // CALL NZ, a16
    case 0xC5:
        return op_c5(cpu); // PUSH BC
    case 0xC6:
        return op_c6(cpu); // ADD A, d8
    case 0xC7:
        return op_c7(cpu); // RST 00H
    case 0xC8:
        return op_c8(cpu); // RET Z
    case 0xC9:
        return op_c9(cpu); // RET
    case 0xCA:
        return op_ca(cpu); // JP Z, a16
    case 0xCB:
        return op_cb(cpu); // PREFIX CB
    case 0xCC:
        return op_cc(cpu); // CALL Z, a16
    case 0xCD:
        return op_cd(cpu); // CALL a16
    case 0xCE:
        return op_ce(cpu); // ADC A, d8
    case 0xCF:
        return op_cf(cpu); // RST 08H
    case 0xD0:
        return op_d0(cpu); // RET NC
    case 0xD1:
        return op_d1(cpu); // POP DE
    case 0xD2:
        return op_d2(cpu); // JP NC, a16
    case 0xD3:
        panic("Invalid opcode: 0xD3", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xD4:
        return op_d4(cpu); // CALL NC, a16
    case 0xD5:
        return op_d5(cpu); // PUSH DE
    case 0xD6:
        return op_d6(cpu); // SUB d8
    case 0xD7:
        return op_d7(cpu); // RST 10H
    case 0xD8:
        return op_d8(cpu); // RET C
    case 0xD9:
        return op_d9(cpu); // RETI (Return & Enable Interrupts)
    case 0xDA:
        return op_da(cpu); // JP C, a16
    case 0xDB:
        panic("Invalid opcode: 0xDB", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xDC:
        return op_dc(cpu); // CALL C, a16
    case 0xDD:
        panic("Invalid opcode: 0xDD", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xDE:
        return op_de(cpu); // SBC A, d8
    case 0xDF:
        return op_df(cpu); // RST 18H
    case 0xE0:
        return op_e0(cpu); // LDH (a8), A  (Write A to 0xFF00 + a8)
    case 0xE1:
        return op_e1(cpu); // POP HL
    case 0xE2:
        return op_e2(cpu); // LD (C), A    (Write A to 0xFF00 + C)
    case 0xE3:
        panic("Invalid opcode: 0xE3", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xE4:
        panic("Invalid opcode: 0xE4", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xE5:
        return op_e5(cpu); // PUSH HL
    case 0xE6:
        return op_e6(cpu); // AND d8
    case 0xE7:
        return op_e7(cpu); // RST 20H
    case 0xE8:
        return op_e8(cpu); // ADD SP, r8
    case 0xE9:
        return op_e9(cpu); // JP (HL)
    case 0xEA:
        return op_ea(cpu); // LD (a16), A
    case 0xEB:
        panic("Invalid opcode: 0xEB", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xEC:
        panic("Invalid opcode: 0xEC", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xED:
        panic("Invalid opcode: 0xED", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xEE:
        return op_ee(cpu); // XOR d8
    case 0xEF:
        return op_ef(cpu); // RST 28H
    case 0xF0:
        return op_f0(cpu); // LDH A, (a8)
    case 0xF1:
        return op_f1(cpu); // POP AF
    case 0xF2:
        return op_f2(cpu); // LD A, (C)
    case 0xF3:
        return op_f3(cpu); // DI (Disable Interrupts)
    case 0xF4:
        panic("Invalid opcode: 0xF4", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xF5:
        return op_f5(cpu); // PUSH AF
    case 0xF6:
        return op_f6(cpu); // OR d8
    case 0xF7:
        return op_f7(cpu); // RST 30H
    case 0xF8:
        return op_f8(cpu); // LD HL, SP+r8
    case 0xF9:
        return op_f9(cpu); // LD SP, HL
    case 0xFA:
        return op_fa(cpu); // LD A, (a16)
    case 0xFB:
        return op_fb(cpu); // EI (Enable Interrupts)
    case 0xFC:
        panic("Invalid opcode: 0xFC", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xFD:
        panic("Invalid opcode: 0xFD", ERR_UNKNOWN_INSTRUCTION);
        return op_00(); // Invalid (No operation)
    case 0xFE:
        return op_fe(cpu); // CP d8
    case 0xFF:
        return op_ff(cpu); // RST 38H
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

// HALT
uint8_t op_76(CPU *cpu)
{
    cpu_halt(cpu);
    return 4;
}

// LD r8/(HL), r8/(HL) and HALT
uint8_t op_40_to_7f(CPU *cpu, uint8_t opcode)
{
    // 0x76 is HALT unlike all others
    if (opcode == 0x76)
    {
        return op_76(cpu);
    }

    // opcode in this range can be written as 0xAABBBCCC
    // All except 1 have the following mnemonic: LD m, n
    // AA will always be 01
    // BBB from 000->111: determines m
    // CCC from 000->111: determines n
    uint8_t bbb = (opcode & 0b00111000) >> 3;
    uint8_t ccc = (opcode & 0b00000111);
    uint16_t hl = get_hl(cpu);

    uint8_t val;

    switch (ccc)
    {
    case 0b000:
        val = cpu->b;
        break;
    case 0b001:
        val = cpu->c;
        break;
    case 0b010:
        val = cpu->d;
        break;
    case 0b011:
        val = cpu->e;
        break;
    case 0b100:
        val = cpu->h;
        break;
    case 0b101:
        val = cpu->l;
        break;
    case 0b110:
        val = read_mem(cpu->mmu, hl);
        break;
    case 0b111:
        val = cpu->a;
        break;
    default:
        panic("Unrecognize opcode in block 0x40-0x7F", ERR_UNKNOWN_INSTRUCTION);
    }

    switch (bbb)
    {
    case 0b000:
        cpu->b = val;
        break;
    case 0b001:
        cpu->c = val;
        break;
    case 0b010:
        cpu->d = val;
        break;
    case 0b011:
        cpu->e = val;
        break;
    case 0b100:
        cpu->h = val;
        break;
    case 0b101:
        cpu->l = val;
        break;
    case 0b110:
        write_mem(cpu->mmu, hl, val);
        break;
    case 0b111:
        cpu->a = val;
        break;
    default:
        panic("Unrecognize opcode in block 0x40-0x7F", ERR_UNKNOWN_INSTRUCTION);
    }

    if (bbb == 0b110 || ccc == 0b110)
    {
        return 8;
    }
    return 4;
}

// ADD/ADC/SUV/SBC/AND/XOR/OR/CP A, r8/(HL)
uint8_t op_80_to_bf(CPU *cpu, uint8_t opcode)
{
    // opcode in this range can be written as 0xAABBBCCC
    // All except 1 have the following mnemonic: OP A, x
    // AA will always be 01
    // BBB from 000->111: determines OP
    // CCC from 000->111: determines x
    uint8_t bbb = (opcode & 0b00111000) >> 3;
    uint8_t ccc = (opcode & 0b00000111);
    uint16_t hl = get_hl(cpu);
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;

    uint8_t val;

    switch (ccc)
    {
    case 0b000:
        val = cpu->b;
        break;
    case 0b001:
        val = cpu->c;
        break;
    case 0b010:
        val = cpu->d;
        break;
    case 0b011:
        val = cpu->e;
        break;
    case 0b100:
        val = cpu->h;
        break;
    case 0b101:
        val = cpu->l;
        break;
    case 0b110:
        val = read_mem(cpu->mmu, hl);
        break;
    case 0b111:
        val = cpu->a;
        break;
    default:
        panic("Unrecognize opcode in block 0x80-0xBF", ERR_UNKNOWN_INSTRUCTION);
    }

    switch (bbb)
    {
    case 0b000:
        alu_add(cpu, val, false);
        break;
    case 0b001:
        alu_add(cpu, val, c_flag);
        break;
    case 0b010:
        alu_sub(cpu, val, false);
        break;
    case 0b011:
        alu_sub(cpu, val, c_flag);
        break;
    case 0b100:
        alu_and(cpu, val);
        break;
    case 0b101:
        alu_xor(cpu, val);
        break;
    case 0b110:
        alu_or(cpu, val);
        break;
    case 0b111:
        alu_compare(cpu, val);
        break;
    default:
        panic("Unrecognize opcode in block 0x80-0xBF", ERR_UNKNOWN_INSTRUCTION);
    }

    if (ccc == 0b110)
    {
        return 8;
    }
    return 4;
}

// RET NZ
uint8_t op_c0(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    if (z_flag)
    {
        return 8;
    }
    else
    {
        cpu->program_counter = stack_pop(cpu);
        return 20;
    }
}

// POP BC
uint8_t op_c1(CPU *cpu)
{
    set_bc(cpu, stack_pop(cpu));
    return 12;
}

// JP NZ, u16
uint8_t op_c2(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (z_flag)
    {
        return 12;
    }
    else
    {
        cpu->program_counter = operand;
        return 16;
    }
}

// JP u16
uint8_t op_c3(CPU *cpu)
{
    uint16_t operand = cpu_fetch_u16(cpu);
    cpu->program_counter = operand;
    return 16;
}

// CALL NZ, u16
uint8_t op_c4(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (z_flag)
    {
        return 12;
    }
    else
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = operand;
        return 24;
    }
}

// PUSH BC
uint8_t op_c5(CPU *cpu)
{
    stack_push(cpu, get_bc(cpu));
    return 16;
}

// ADD A, u8
uint8_t op_c6(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    alu_add(cpu, operand, false);
    return 8;
}

// RST 00h
uint8_t op_c7(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x00;
    return 16;
}

// RET Z
uint8_t op_c8(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    if (z_flag)
    {
        cpu->program_counter = stack_pop(cpu);
        return 20;
    }
    else
    {
        return 8;
    }
}

// RET
uint8_t op_c9(CPU *cpu)
{
    cpu->program_counter = stack_pop(cpu);
    return 16;
}

// JP Z, u16
uint8_t op_ca(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (z_flag)
    {
        cpu->program_counter = operand;
        return 16;
    }
    else
    {
        return 12;
    }
}

// PREFIX CB
uint8_t op_cb(CPU *cpu)
{
    panic_unimplemented("op_cb");
    cpu->h = 0;
}

// CALL Z, u16
uint8_t op_cc(CPU *cpu)
{
    bool z_flag = cpu->flags & Z_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (z_flag)
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = operand;
        return 24;
    }
    else
    {
        return 12;
    }
}

// CALL u16
uint8_t op_cd(CPU *cpu)
{
    uint16_t operand = cpu_fetch_u16(cpu);
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = operand;
    return 24;
}

// ADC A, u8
uint8_t op_ce(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    alu_add(cpu, operand, c_flag);
    return 8;
}

// RST 08h
uint8_t op_cf(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x08;
    return 16;
}

// RET NC
uint8_t op_d0(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    if (c_flag)
    {
        return 8;
    }
    else
    {
        cpu->program_counter = stack_pop(cpu);
        return 20;
    }
}

// POP DE
uint8_t op_d1(CPU *cpu)
{
    set_de(cpu, stack_pop(cpu));
    return 12;
}

// JP NC, u16
uint8_t op_d2(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (c_flag)
    {
        return 12;
    }
    else
    {
        cpu->program_counter = operand;
        return 16;
    }
}

// CALL NC, u16
uint8_t op_d4(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (c_flag)
    {
        return 12;
    }
    else
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = operand;
        return 24;
    }
}

// PUSH DE
uint8_t op_d5(CPU *cpu)
{
    stack_push(cpu, get_de(cpu));
    return 16;
}

// SUB A, u8
uint8_t op_d6(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    alu_sub(cpu, operand, false);
    return 8;
}

// RST 10h
uint8_t op_d7(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x10;
    return 16;
}

// RET C
uint8_t op_d8(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    if (c_flag)
    {
        cpu->program_counter = stack_pop(cpu);
        return 20;
    }
    else
    {
        return 8;
    }
}

// RETI
uint8_t op_d9(CPU *cpu)
{
    cpu->ime = 1;
    cpu->program_counter = stack_pop(cpu);
    return 16;
}

// JP C, u16
uint8_t op_da(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (c_flag)
    {
        cpu->program_counter = operand;
        return 16;
    }
    else
    {
        return 12;
    }
}

// CALL C, u16
uint8_t op_dc(CPU *cpu)
{
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    uint16_t operand = cpu_fetch_u16(cpu);
    if (c_flag)
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = operand;
        return 24;
    }
    else
    {
        return 12;
    }
}

// SBC A, u8
uint8_t op_de(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    bool c_flag = cpu->flags & C_FLAG ? 1 : 0;
    alu_sub(cpu, operand, c_flag);
    return 8;
}

// RST 18h
uint8_t op_df(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x18;
    return 16;
}

// LD (FF00+u8), A
uint8_t op_e0(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    write_mem(cpu->mmu, 0xFF00 + operand, cpu->a);
    return 12;
}

// POP HL
uint8_t op_e1(CPU *cpu)
{
    set_hl(cpu, stack_pop(cpu));
    return 12;
}

// LD (FF00+C), A
uint8_t op_e2(CPU *cpu)
{
    write_mem(cpu->mmu, 0xFF00 + cpu->c, cpu->a);
    return 8;
}

// PUSH HL
uint8_t op_e5(CPU *cpu)
{
    stack_push(cpu, get_hl(cpu));
    return 16;
}

// AND A, u8
uint8_t op_e6(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    alu_and(cpu, operand);
    return 8;
}

// RST 20h
uint8_t op_e7(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x20;
    return 16;
}

// ADD SP, i8
uint8_t op_e8(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    // operand is converted to int8_t inside here
    alu_add_sp(cpu, operand);
    return 16;
}

// JP HL
uint8_t op_e9(CPU *cpu)
{
    cpu->program_counter = get_hl(cpu);
    return 4;
}

// LD (u16), A
uint8_t op_ea(CPU *cpu)
{
    uint16_t operand = cpu_fetch_u16(cpu);
    write_mem(cpu->mmu, operand, cpu->a);
    return 16;
}

// XOR A, u8
uint8_t op_ee(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    alu_xor(cpu, operand);
    return 8;
}

// RST 28h
uint8_t op_ef(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x28;
    return 16;
}

// LD A, (FF00+u8)
uint8_t op_f0(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    cpu->a = read_mem(cpu->mmu, 0xFF00 + operand);
    return 12;
}

// POP AF
uint8_t op_f1(CPU *cpu)
{
    set_af(cpu, stack_pop(cpu));
    return 12;
}

// LD A, (FF00+C)
uint8_t op_f2(CPU *cpu)
{
    cpu->a = read_mem(cpu->mmu, 0xFF00 + cpu->c);
    return 8;
}

// DI
uint8_t op_f3(CPU *cpu)
{
    cpu->ime = 0;
    return 4;
}

// PUSH AF
uint8_t op_f5(CPU *cpu)
{
    stack_push(cpu, get_af(cpu));
    return 16;
}

// OR A, u8
uint8_t op_f6(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    alu_or(cpu, operand);
    return 8;
}

// RST 30h
uint8_t op_f7(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x30;
    return 16;
}

// LD HL, SP+i8
uint8_t op_f8(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    uint16_t old_sp = cpu->sp;
    // To set flags and convert operand to int8_t
    alu_add_sp(cpu, operand);
    set_hl(cpu, cpu->sp);
    cpu->sp = old_sp;
    return 12;
}

// LD SP, HL
uint8_t op_f9(CPU *cpu)
{
    cpu->sp = get_hl(cpu);
    return 8;
}

// LD A, (u16)
uint8_t op_fa(CPU *cpu)
{
    uint16_t operand = cpu_fetch_u16(cpu);
    cpu->a = read_mem(cpu->mmu, operand);
    return 16;
}

// EI
uint8_t op_fb(CPU *cpu)
{
    cpu->ime_pending = 1;
    return 4;
}

// CP A, u8
uint8_t op_fe(CPU *cpu)
{
    uint8_t operand = cpu_fetch_u8(cpu);
    alu_compare(cpu, operand);
    return 8;
}

// RST 38h
uint8_t op_ff(CPU *cpu)
{
    stack_push(cpu, cpu->program_counter);
    cpu->program_counter = 0x38;
    return 16;
}

// RLC r8/(HL)
void rlc(CPU *cpu, uint8_t *ptr)
{
    bool c_flag = *ptr >> 7;
    *ptr = (*ptr << 1) | c_flag;

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}

// RRC r8/(HL)
void rrc(CPU *cpu, uint8_t *ptr)
{
    bool c_flag = *ptr & 0x01;
    *ptr = (*ptr >> 1) | (c_flag << 7);

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}

// RL r8/(HL)
void rl(CPU *cpu, uint8_t *ptr)
{
    uint8_t c_flag_old = cpu->flags & C_FLAG ? 1 : 0;
    bool c_flag = *ptr >> 7;
    *ptr = (*ptr << 1) | c_flag_old;

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}

// RR r8/(HL)
void rr(CPU *cpu, uint8_t *ptr)
{
    bool c_flag_old = cpu->flags & C_FLAG ? 1 : 0;
    bool c_flag = *ptr & 0x01;
    *ptr = (*ptr >> 1) | (c_flag_old << 7);

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}

// SLA r8/(HL)
void sla(CPU *cpu, uint8_t *ptr)
{
    bool c_flag = *ptr >> 7;
    *ptr = *ptr << 1;

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}

// SRA r8/(HL)
void sra(CPU *cpu, uint8_t *ptr)
{
    bool c_flag = *ptr & 0x01;
    uint8_t msb = *ptr & (1 << 7);
    *ptr = *ptr >> 1 | msb;

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}

// SWAP r8/(HL)
void swap(CPU *cpu, uint8_t *ptr)
{
    uint16_t upper_byte = (*ptr & 0xF0) >> 4;
    *ptr = (*ptr << 4) | upper_byte;

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, 0);
}

// SRL r8/(HL)
void srl(CPU *cpu, uint8_t *ptr)
{
    bool c_flag = *ptr & 0x01;
    *ptr = *ptr >> 1;

    set_flag(cpu, Z_FLAG, *ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, c_flag);
}
