#ifndef ALU_H
#define ALU_H

#include <stdbool.h>
#include "cpu.h"

void alu_add(CPU *cpu, uint8_t val, bool carry);

void alu_sub(CPU *cpu, uint8_t val, bool carry);

void alu_compare(CPU *cpu, uint8_t val);

void alu_inc(CPU *cpu, uint8_t *reg_ptr);

void alu_dec(CPU *cpu, uint8_t *reg_ptr);

void alu_add_u16(CPU *cpu, uint16_t val);

uint16_t alu_inc_u16(uint16_t reg_val);

uint16_t alu_dec_u16(uint16_t reg_val);

void alu_add_sp(CPU *cpu, uint8_t val);

void alu_and(CPU *cpu, uint8_t val);

void alu_not(CPU *cpu);

void alu_or(CPU *cpu, uint8_t val);

void alu_xor(CPU *cpu, uint8_t val);

#endif