#ifndef ALU_H
#define ALU_H

#include <stdbool.h>
#include "types.h"

/// Adds val to the A register. If carry is true, also adds the current C flag.
/// Flags: Z=result, N=0, H=half-carry, C=carry.
void alu_add(CPU *cpu, uint8_t val, bool carry);

/// Subtracts val from the A register. If carry is true, also subtracts the current C flag.
/// Flags: Z=result, N=1, H=half-borrow, C=borrow.
void alu_sub(CPU *cpu, uint8_t val, bool carry);

/// Subtracts val from A without storing the result, only updating flags.
/// Flags: Z=result, N=1, H=half-borrow, C=borrow.
void alu_compare(CPU *cpu, uint8_t val);

/// Increments the value at reg_ptr by 1.
/// Flags: Z=result, N=0, H=half-carry. C unaffected.
void alu_inc(CPU *cpu, uint8_t *reg_ptr);

/// Decrements the value at reg_ptr by 1.
/// Flags: Z=result, N=1, H=half-borrow. C unaffected.
void alu_dec(CPU *cpu, uint8_t *reg_ptr);

/// Adds val to the HL register pair.
/// Flags: N=0, H=half-carry, C=carry. Z unaffected.
void alu_add_u16(CPU *cpu, uint16_t val);

/// Increments a 16-bit register value by 1 and returns the result. No flags affected.
uint16_t alu_inc_u16(uint16_t reg_val);

/// Decrements a 16-bit register value by 1 and returns the result. No flags affected.
uint16_t alu_dec_u16(uint16_t reg_val);

/// Adds a signed 8-bit offset to the stack pointer (SP).
/// Flags: Z=0, N=0, H=half-carry, C=carry.
void alu_add_sp(CPU *cpu, uint8_t val);

/// Bitwise AND of val and A, result stored in A.
/// Flags: Z=result, N=0, H=1, C=0.
void alu_and(CPU *cpu, uint8_t val);

/// Bitwise complement of A (flips all bits), result stored in A.
/// Flags: N=1, H=1. Z and C unaffected.
void alu_not(CPU *cpu);

/// Bitwise OR of val and A, result stored in A.
/// Flags: Z=result, N=0, H=0, C=0.
void alu_or(CPU *cpu, uint8_t val);

/// Bitwise XOR of val and A, result stored in A.
/// Flags: Z=result, N=0, H=0, C=0.
void alu_xor(CPU *cpu, uint8_t val);

#endif
