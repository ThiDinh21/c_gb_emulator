#ifndef CPU_H
#define CPU_H

#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "mmu.h"
#include "timer.h"

/// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
///
/// 7     bit     0
/// ----       ----
/// Z N H C 0 0 0 0
/// | | | | | | | |
/// | | | | | | | |
/// | | | | | | | |
/// | | | | +-+-+-+- Always 0
/// | | | +--------- Carry flag
/// | | + ---------- Half Carry flag (BCD)
/// | +------------- Substraction flag (BCD)
/// +--------------- Zero flag
typedef enum
{
    Z_FLAG = 1 << 7,
    N_FLAG = 1 << 6,
    H_FLAG = 1 << 5,
    C_FLAG = 1 << 4,
} FlagRegister;

typedef enum
{
    INTERRUPT_VBLANK = 0x40,
    INTERRUPT_STAT = 0x48,
    INTERRUPT_TIMER = 0x50,
    INTERRUPT_SERIAL = 0x58,
    INTERRUPT_JOYPAD = 0x60,
} Interrupt;

CPU *init_cpu(void);

void clean_up_cpu(CPU *cpu);

uint8_t cpu_step(CPU *cpu);
void cpu_stop(CPU *cpu);
void cpu_halt(CPU *cpu);

// uint8_t get_opcode(MMU *mmu, uint16_t addr);

uint16_t get_af(CPU *cpu);
uint16_t get_bc(CPU *cpu);
uint16_t get_de(CPU *cpu);
uint16_t get_hl(CPU *cpu);

void set_af(CPU *cpu, uint16_t val);
void set_bc(CPU *cpu, uint16_t val);
void set_de(CPU *cpu, uint16_t val);
void set_hl(CPU *cpu, uint16_t val);

void set_flag(CPU *cpu, FlagRegister flag, uint8_t val);

uint8_t cpu_fetch_u8(CPU *cpu);
uint16_t cpu_fetch_u16(CPU *cpu);

void stack_push(CPU *cpu, uint16_t val);
uint16_t stack_pop(CPU *cpu);

uint8_t interrupt_handling(CPU *cpu);

#endif