#include "cpu.h"

CPU *init_cpu(void)
{
    // !TODO: make this more robust, just temp right now
    MMU *mmu = (MMU *)calloc(1, sizeof(MMU));
    CPU *cpu = (CPU *)calloc(1, sizeof(CPU));

    cpu->mmu = mmu;
    return cpu;
}

void clean_up_cpu(CPU *cpu)
{
    free(cpu->mmu);
    free(cpu);
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