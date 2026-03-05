#ifndef CPU_H
#define CPU_H

#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "mmu.h"
#include "timer.h"

/// Initializes a CPU struct with Timer, MMU and PPU allocated.
/// Boot sequence:
/// - Sets up initial values for CPU registers.
/// - Sets all buttons and joypad to released.
/// - Initializes PPU registers
/// - Loads ROM and save file
///
/// ref: https://gbdev.io/pandocs/Power_Up_Sequence.html
CPU *init_cpu(void);

/// Writes save file and then deallocates CPU, Timer, PPU, MMU and their components memories.
void clean_up_cpu(CPU *cpu);

/// Loads the ROM specified from the filepath into the MMU
int load_rom(CPU *cpu, const char *filename);

/// Executes 1 cycle of the CPU: loads an opcode, executes it, handles interrupts if any
/// then returns the number of T-cycles taken.
uint8_t cpu_step(CPU *cpu);

/// Not implemented.
void cpu_stop(CPU *cpu);
/// Sets the halt flag, pausing CPU execution until an interrupt is fired.
void cpu_halt(CPU *cpu);

/// Getters for 16-bit register pairs (AF, BC, DE and HL)
uint16_t get_af(CPU *cpu);
uint16_t get_bc(CPU *cpu);
uint16_t get_de(CPU *cpu);
uint16_t get_hl(CPU *cpu);

/// Setters for 16-bit register pairs (AF, BC, DE and HL)
void set_af(CPU *cpu, uint16_t val);
void set_bc(CPU *cpu, uint16_t val);
void set_de(CPU *cpu, uint16_t val);
void set_hl(CPU *cpu, uint16_t val);

/// Sets value of the Status Flag Register (F flag)
/// Takes a FlagRegister enum that dictates which bit will be updated, and a value of 0 or 1 (will break if non-0-or-1 value is passed).
void set_flag(CPU *cpu, FlagRegister flag, uint8_t val);

/// Returns the 8-bit value stored at the address stored in the program counter, then increments the program counter.
uint8_t cpu_fetch_u8(CPU *cpu);
/// Returns the 16-bit value stored at the address stored in the program counter, then increments the program counter twice.
uint16_t cpu_fetch_u16(CPU *cpu);

/// Pushes the 16-bit value into the stack (HRAM), then decreases the stack pointer (stack is stored upside down).
void stack_push(CPU *cpu, uint16_t val);
/// Pops the top most item of the stack (HRAM), returns it and increases the stack pointer (stack is stored upside down).
uint16_t stack_pop(CPU *cpu);

/// Checks if interrupts are enabled, then handles the ones that were fired.
uint8_t interrupt_handling(CPU *cpu);

#endif