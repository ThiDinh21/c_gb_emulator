#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>
#include "cpu.h"
#include "alu.h"

uint8_t decode(CPU *cpu, uint8_t op_size);

uint8_t op_00(void);
uint8_t op_01(CPU *cpu);
uint8_t op_02(CPU *cpu);
uint8_t op_03(CPU *cpu);
uint8_t op_04(CPU *cpu);
uint8_t op_05(CPU *cpu);
uint8_t op_06(CPU *cpu);
uint8_t op_07(CPU *cpu);
uint8_t op_08(CPU *cpu);
uint8_t op_09(CPU *cpu);
uint8_t op_0a(CPU *cpu);
uint8_t op_0b(CPU *cpu);
uint8_t op_0c(CPU *cpu);
uint8_t op_0d(CPU *cpu);
uint8_t op_0e(CPU *cpu);
uint8_t op_0f(CPU *cpu);
uint8_t op_10(CPU *cpu);
uint8_t op_11(CPU *cpu);
uint8_t op_12(CPU *cpu);
uint8_t op_13(CPU *cpu);
uint8_t op_14(CPU *cpu);
uint8_t op_15(CPU *cpu);
uint8_t op_16(CPU *cpu);
uint8_t op_17(CPU *cpu);
uint8_t op_18(CPU *cpu);
uint8_t op_19(CPU *cpu);
uint8_t op_1a(CPU *cpu);
uint8_t op_1b(CPU *cpu);
uint8_t op_1c(CPU *cpu);
uint8_t op_1d(CPU *cpu);
uint8_t op_1e(CPU *cpu);
uint8_t op_1f(CPU *cpu);

#endif