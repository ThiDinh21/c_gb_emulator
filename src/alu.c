#include "alu.h"

void alu_add(CPU *cpu, uint8_t val, bool carry)
{
    uint8_t res;
    bool new_carry = __builtin_add_overflow(cpu->a, val, &res);

    if (carry)
    {
        new_carry = __builtin_add_overflow(res, 1, &res) | new_carry;
    }

    set_flag(cpu, Z_FLAG, res == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, (cpu->a & 0x0F) + (val & 0x0F) + carry > 0x0F);
    set_flag(cpu, C_FLAG, new_carry);

    cpu->a = res;
}

void alu_sub(CPU *cpu, uint8_t val, bool carry)
{
    uint8_t res;
    bool h_flag = false;

    if ((cpu->a & 0x0F) < (val & 0x0F))
    {
        h_flag = true;
    }
    bool new_carry = __builtin_sub_overflow(cpu->a, val, &res);

    if (carry)
    {
        h_flag = res < 1;
        new_carry = __builtin_sub_overflow(res, 1, &res) | new_carry;
    }

    set_flag(cpu, Z_FLAG, res == 0);
    set_flag(cpu, N_FLAG, 1);
    set_flag(cpu, H_FLAG, h_flag);
    set_flag(cpu, C_FLAG, new_carry);

    cpu->a = res;
}

void alu_compare(CPU *cpu, uint8_t val)
{
    uint8_t res;
    bool h_flag = false;

    if ((cpu->a & 0x0F) < (val & 0x0F))
    {
        h_flag = true;
    }
    bool new_carry = __builtin_sub_overflow(cpu->a, val, &res);

    set_flag(cpu, Z_FLAG, res == 0);
    set_flag(cpu, N_FLAG, 1);
    set_flag(cpu, H_FLAG, h_flag);
    set_flag(cpu, C_FLAG, new_carry);
}

// uint8_t alu_inc(CPU *cpu, uint8_t val);

// uint8_t alu_dec(CPU *cpu, uint8_t val);

// void alu_add_u16(CPU *cpu, uint16_t val);

// uint16_t alu_dec_r16(CPU *cpu, uint16_t val);

// uint16_t alu_inc_r16(CPU *cpu, uint16_t val);

// void alu_and(CPU *cpu, uint8_t val);

// void alu_not(CPU *cpu);

// void alu_or(CPU *cpu, uint8_t val);

// void alu_xor(CPU *cpu, uint8_t val);