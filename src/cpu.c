#include "cpu.h"
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

void run_cpu(CPU *cpu)
{
    while (1)
    {
        // Fetch opcode
        // uint8_t opcode = get_opcode(cpu->mmu, cpu->program_counter);
        cpu->program_counter++;

        // Execute opcode

        panic_unimplemented();
    }
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