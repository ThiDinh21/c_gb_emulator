#include "opcodes.h"
#include "errors.h"

uint8_t decode(CPU *cpu, uint8_t opcode)
{
    uint8_t cycle = 0;

    switch (opcode)
    {
    case 0x00:
        cycle = op_00();
        break;
    case 0x01:
        cycle = op_01(cpu);
        break;

    default:
        panic("Unrecognized opcode", ERR_UNKNOWN_INSTRUCTION);
    }

    return cycle;
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
    set_bc(cpu, new_de);
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