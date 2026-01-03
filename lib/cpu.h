#ifndef CPU_H
#define CPU_H

#include <stdlib.h>
#include <stdint.h>
#include "mmu.h"

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

typedef struct
{
    uint16_t sp;
    uint16_t pc;
    uint8_t flags;
    uint8_t a, b, c, d, e, h, l;
    MMU *mmu;
} CPU;

CPU *init_cpu(void);

void clean_up_cpu(CPU *cpu);

uint16_t get_af(CPU *cpu);
uint16_t get_bc(CPU *cpu);
uint16_t get_de(CPU *cpu);
uint16_t get_hl(CPU *cpu);

void set_af(CPU *cpu, uint16_t val);
void set_bc(CPU *cpu, uint16_t val);
void set_de(CPU *cpu, uint16_t val);
void set_hl(CPU *cpu, uint16_t val);

#endif