#ifndef CPU_H
#define CPU_H

#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "mmu.h"
#include "timer.h"

CPU *init_cpu(void);

void clean_up_cpu(CPU *cpu);

int load_rom(CPU *cpu, const char *filename);

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