#include "cpu.h"
#include "opcodes.h"
#include "errors.h"

CPU *init_cpu(void)
{
    // !TODO: make this more robust, just temp right now
    MMU *mmu = (MMU *)calloc(1, sizeof(MMU));
    CPU *cpu = (CPU *)calloc(1, sizeof(CPU));
    Timer *timer = init_timer();

    // after boot ROM finishes
    // https://gbdev.io/pandocs/Power_Up_Sequence.html
    cpu->program_counter = 0x0100;
    cpu->mmu = mmu;
    cpu->timer = timer;
    return cpu;
}

void clean_up_cpu(CPU *cpu)
{
    free(cpu->mmu);
    clean_up_timer(cpu->timer);
    free(cpu);
}

uint8_t cpu_step(CPU *cpu)
{

    // Fetch opcode
    uint8_t opcode = cpu_fetch_u8(cpu);
    uint8_t cycles = 0;

    // Execute opcode
    cycles = decode(cpu, opcode);

    if (cpu->ime_pending)
    {
        cpu->ime = 1;
        cpu->ime_pending = 0;
    }

    cycles += interrupt_handling(cpu);

    return cycles;
}

void cpu_stop(CPU *cpu)
{
    // !TODO
    panic_unimplemented("cpu_stop");
    cpu->h = 0;
}

void cpu_halt(CPU *cpu)
{
    // !TODO
    panic_unimplemented("cpu_halt");
    cpu->h = 0;
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

void stack_push(CPU *cpu, uint16_t val)
{

    cpu->sp -= 2;
    write_mem_u16(cpu->mmu, cpu->sp, val);
}

uint16_t stack_pop(CPU *cpu)
{
    uint16_t res = read_mem_u16(cpu->mmu, cpu->sp);
    cpu->sp += 2;
    return res;
}

uint8_t interrupt_handling(CPU *cpu)
{
    // Check if interrupt master enable register is set
    if (!cpu->ime)
    {
        if (read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & 0b00011111)
        {
            // ignore HALT bug: https://gbdev.io/pandocs/halt.html
            cpu->halt = 0;
        }

        return 0;
    }

    if (!(read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & 0b00011111))
    {
        return 0;
    }

    cpu->halt = 0;

    // read_mem everytime to avoid outdated data
    // If bit 0 is set -> VBLANK interrupt
    if (read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & 0x01)
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = INTERRUPT_VBLANK;
        write_mem(cpu->mmu, 0xFF0F, read_mem(cpu->mmu, 0xFF0F) & (~0x01));
        cpu->ime = 0;
        // (2 NOP + 2 PUSH + 1 SET) * 4 T-cycles
        return 20;
    }

    // read_mem everytime to avoid outdated data
    // If bit 1 is set -> STAT interrupt
    if (read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & (1 << 1))
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = INTERRUPT_STAT;
        write_mem(cpu->mmu, 0xFF0F, read_mem(cpu->mmu, 0xFF0F) & (~(1 << 1)));
        cpu->ime = 0;
        // (2 NOP + 2 PUSH + 1 SET) * 4 T-cycles
        return 20;
    }

    // read_mem everytime to avoid outdated data
    // If bit 2 is set -> TIMER interrupt
    if (read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & (1 << 2))
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = INTERRUPT_TIMER;
        write_mem(cpu->mmu, 0xFF0F, read_mem(cpu->mmu, 0xFF0F) & (~(1 << 2)));
        cpu->ime = 0;
        // (2 NOP + 2 PUSH + 1 SET) * 4 T-cycles
        return 20;
    }

    // read_mem everytime to avoid outdated data
    // If bit 3 is set -> SERIAL interrupt
    if (read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & (1 << 3))
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = INTERRUPT_SERIAL;
        write_mem(cpu->mmu, 0xFF0F, read_mem(cpu->mmu, 0xFF0F) & (~(1 << 3)));
        cpu->ime = 0;
        // (2 NOP + 2 PUSH + 1 SET) * 4 T-cycles
        return 20;
    }

    // read_mem everytime to avoid outdated data
    // If bit 4 is set -> JOYPAD interrupt
    if (read_mem(cpu->mmu, 0xFFFF) & read_mem(cpu->mmu, 0xFF0F) & (1 << 4))
    {
        stack_push(cpu, cpu->program_counter);
        cpu->program_counter = INTERRUPT_JOYPAD;
        write_mem(cpu->mmu, 0xFF0F, read_mem(cpu->mmu, 0xFF0F) & (~(1 << 4)));
        cpu->ime = 0;
        // (2 NOP + 2 PUSH + 1 SET) * 4 T-cycles
        return 20;
    }

    return 0;
}
