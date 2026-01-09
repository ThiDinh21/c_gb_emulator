#include <stdio.h>
#include "cpu.h"
#include "opcodes.h"
#include "errors.h"

CPU *init_cpu(void)
{
    // !TODO: make this more robust, just temp right now
    MMU *mmu = (MMU *)calloc(1, sizeof(MMU));
    CPU *cpu = (CPU *)calloc(1, sizeof(CPU));

    // after boot ROM finishes
    // https://gbdev.io/pandocs/Power_Up_Sequence.html
    cpu->program_counter = 0x0100;
    cpu->mmu = mmu;
    return cpu;
}

void clean_up_cpu(CPU *cpu)
{
    free(cpu->mmu);
    free(cpu);
}

uint8_t cpu_step(CPU *cpu)
{

    // Fetch opcode
    uint8_t opcode = cpu_fetch_u8(cpu);
    uint8_t cycles = 0;

    // Execute opcode
    switch (opcode)
    {
    case 0x00:
        // NOP
        cycles = op_00();
        break;
    case 0x01:
        // LD BC, u16
        cycles = op_01(cpu);
        break;
    case 0x02:
        // LD (BC), A
        cycles = op_02(cpu);
        break;
    case 0x03:
        // INC BC
        cycles = op_03(cpu);
        break;
    case 0x04:
        // INC B
        cycles = op_04(cpu);
        break;
    case 0x05:
        // DEC B
        cycles = op_05(cpu);
        break;
    case 0x06:
        // LD B, u8
        cycles = op_06(cpu);
        break;
    case 0x07:
        // RLCA
        cycles = op_07(cpu);
        break;
    case 0x08:
        // LD (u16), SP
        cycles = op_08(cpu);
        break;
    case 0x09:
        // ADD HL, BC
        cycles = op_09(cpu);
        break;
    case 0x0A:
        // LD A, (BC)
        cycles = op_0a(cpu);
        break;
    case 0x0B:
        // DEC BC
        cycles = op_0b(cpu);
        break;
    case 0x0C:
        // INC C
        cycles = op_0c(cpu);
        break;
    case 0x0D:
        // DEC C
        cycles = op_0d(cpu);
        break;
    case 0x0E:
        // LD C, u8
        cycles = op_0e(cpu);
        break;
    case 0x0F:
        // RRCA
        cycles = op_0f(cpu);
        break;
    // case 0x10 ... 0xFF:
    //     panic_unimplemented();
    default:
        char err_msg[30];
        snprintf(err_msg, 30, "Opcode not recognized: 0x%x\n", opcode);
        panic(err_msg, ERR_UNKNOWN_INSTRUCTION);
    }
    return cycles;
}

void cpu_stop(CPU *cpu)
{
    // !TODO
    panic_unimplemented("cpu_stop");
}

uint16_t get_af(CPU *cpu)
{
    return (cpu->a << 8) | cpu->flags;
}

uint16_t get_bc(CPU *cpu)
{
    return (cpu->b << 8) | cpu->c;
}

uint16_t get_de(CPU *cpu)
{
    return (cpu->d << 8) | cpu->e;
}

uint16_t get_hl(CPU *cpu)
{
    return (cpu->h << 8) | cpu->l;
}

void set_af(CPU *cpu, uint16_t val)
{
    cpu->a = val >> 8;
    cpu->flags = val & 0x00F0;
}

void set_bc(CPU *cpu, uint16_t val)
{
    cpu->b = val >> 8;
    cpu->c = val & 0x00FF;
}

void set_de(CPU *cpu, uint16_t val)
{
    cpu->d = val >> 8;
    cpu->e = val & 0x00FF;
}

void set_hl(CPU *cpu, uint16_t val)
{
    cpu->h = val >> 8;
    cpu->l = val & 0x00FF;
}

void set_flag(CPU *cpu, FlagRegister flag, uint8_t val)
{
    switch (flag)
    {
    case Z_FLAG:
        val <<= 7;
        break;
    case N_FLAG:
        val <<= 6;
        break;
    case H_FLAG:
        val <<= 5;
        break;
    case C_FLAG:
        val <<= 4;
        break;
    default:
        panic("Unrecognized flag", ERR_INVALID_MEMORY_ACCESS);
    }
    cpu->flags = (cpu->flags & ~flag) | val;
}

uint8_t cpu_fetch_u8(CPU *cpu)
{
    uint8_t val = read_mem(cpu->mmu, cpu->program_counter);
    cpu->program_counter++;
    return val;
}

uint16_t cpu_fetch_u16(CPU *cpu)
{
    uint16_t val = read_mem_u16(cpu->mmu, cpu->program_counter);
    cpu->program_counter += 2;
    return val;
}