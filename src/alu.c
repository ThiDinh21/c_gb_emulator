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

void alu_inc(CPU *cpu, uint8_t *reg_ptr)
{
    *reg_ptr += 1;

    set_flag(cpu, Z_FLAG, *reg_ptr == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, (*reg_ptr & 0x0F) == 0);
}

void alu_dec(CPU *cpu, uint8_t *reg_ptr)
{
    *reg_ptr -= 1;

    set_flag(cpu, Z_FLAG, *reg_ptr == 0);
    set_flag(cpu, N_FLAG, 1);
    set_flag(cpu, H_FLAG, (*reg_ptr & 0x0F) == 0x0F);
}

void alu_add_u16(CPU *cpu, uint16_t val)
{
    uint16_t hl_val = get_hl(cpu);
    bool h_flag = (hl_val & 0x0FFF) + (val & 0x0FFF) > 0x0FFF;
    bool c_flag = __builtin_add_overflow(hl_val, val, &hl_val);

    set_hl(cpu, hl_val);

    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, h_flag);
    set_flag(cpu, C_FLAG, c_flag);
}

uint16_t alu_inc_u16(uint16_t reg_val)
{
    uint16_t new_val = 0;
    __builtin_add_overflow(reg_val, 1, &new_val);
    return new_val;
}

uint16_t alu_dec_u16(uint16_t reg_val)
{
    uint16_t new_val = 0;
    __builtin_sub_overflow(reg_val, 1, &new_val);
    return new_val;
}

void alu_add_sp(CPU *cpu, uint8_t val)
{
    // val is 8-bit signed offset
    int8_t signed_offset = (int8_t)val;

    bool h_flag = (cpu->sp & 0x000F) + (signed_offset & 0x000F) > 0x0F;
    bool c_flag = (cpu->sp & 0x00FF) + (signed_offset & 0x00FF) > 0xFF;

    __builtin_add_overflow(cpu->sp, signed_offset, &(cpu->sp));

    set_flag(cpu, Z_FLAG, 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, h_flag);
    set_flag(cpu, C_FLAG, c_flag);
}

void alu_and(CPU *cpu, uint8_t val)
{
    cpu->a &= val;

    set_flag(cpu, Z_FLAG, cpu->a == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 1);
    set_flag(cpu, C_FLAG, 0);
}

void alu_not(CPU *cpu)
{
    cpu->a = ~(cpu->a);

    set_flag(cpu, N_FLAG, 1);
    set_flag(cpu, H_FLAG, 1);
}

void alu_or(CPU *cpu, uint8_t val)
{
    cpu->a |= val;

    set_flag(cpu, Z_FLAG, cpu->a == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, 0);
}

void alu_xor(CPU *cpu, uint8_t val)
{
    cpu->a ^= val;

    set_flag(cpu, Z_FLAG, cpu->a == 0);
    set_flag(cpu, N_FLAG, 0);
    set_flag(cpu, H_FLAG, 0);
    set_flag(cpu, C_FLAG, 0);
}