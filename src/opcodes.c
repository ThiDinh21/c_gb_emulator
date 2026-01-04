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
uint8_t op_ob(CPU *cpu)
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