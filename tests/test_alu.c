#include "alu.h"
#include "../tests/unity.h"

CPU *cpu;

void setUp()
{
    cpu = init_cpu();
    cpu->a = 0x00;
    cpu->flags = 0x00;
}
void tearDown()
{
    clean_up_cpu(cpu);
}

void test_alu_add()
{
    // ADD half carry
    cpu->a = 0x0F;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_add(cpu, cpu->b, false);
    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x20, cpu->flags);

    // ADD Zero + overflow
    cpu->a = 0xFF;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_add(cpu, cpu->b, false);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0xB0, cpu->flags);

    // ADD N-Flag Reset Test
    cpu->a = 0x01;
    cpu->b = 0x01;
    cpu->flags = 0x40; // N flag starts SET (Subtraction mode)
    alu_add(cpu, cpu->b, false);

    TEST_ASSERT_EQUAL_HEX8(0x02, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->flags);

    // ADC
    cpu->a = 0x01;
    cpu->b = 0x01;
    cpu->flags = 0x10;
    alu_add(cpu, cpu->b, true);
    TEST_ASSERT_EQUAL_HEX8(0x03, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->flags);

    // ADC 2
    cpu->a = 0xFF;
    cpu->b = 0x00;
    cpu->flags = 0x10;
    alu_add(cpu, cpu->b, true);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0xB0, cpu->flags);
}

void test_alu_sub()
{
    // SUB: No flags
    cpu->a = 0x02;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_sub(cpu, cpu->b, false);
    TEST_ASSERT_EQUAL_HEX8(0x01, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x40, cpu->flags); // N set

    // SUB: Zero Result
    cpu->a = 0x01;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_sub(cpu, cpu->b, false);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0xC0, cpu->flags); // Z | N

    // SUB: Half Borrow (H Flag)
    cpu->a = 0x10;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_sub(cpu, cpu->b, false);
    TEST_ASSERT_EQUAL_HEX8(0x0F, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x60, cpu->flags); // N | H

    // SUB: Full Underflow (C + H Flags)
    cpu->a = 0x00;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_sub(cpu, cpu->b, false);
    TEST_ASSERT_EQUAL_HEX8(0xFF, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x70, cpu->flags); // N | H | C

    // SBC: Subtract with Carry In
    cpu->a = 0x05;
    cpu->b = 0x01;
    cpu->flags = 0x10; // Carry set
    alu_sub(cpu, cpu->b, true);
    TEST_ASSERT_EQUAL_HEX8(0x03, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x40, cpu->flags); // N set

    // SBC: Ripple Underflow
    cpu->a = 0x00;
    cpu->b = 0x00;
    cpu->flags = 0x10; // Carry set
    alu_sub(cpu, cpu->b, true);
    TEST_ASSERT_EQUAL_HEX8(0xFF, cpu->a);     // 0 - 0 - 1 = 255
    TEST_ASSERT_EQUAL_HEX8(0x70, cpu->flags); // N | H | C
}

void test_alu_compare()
{
    // CP: Equal (Zero Flag)
    // 0x05 - 0x05 = 0
    cpu->a = 0x05;
    cpu->b = 0x05;
    cpu->flags = 0x00;
    alu_compare(cpu, cpu->b);
    TEST_ASSERT_EQUAL_HEX8(0x05, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0xC0, cpu->flags); // Z | N (Result 0)

    // CP: Less Than (Carry/Underflow)
    // 0x01 - 0x02 = Underflow
    cpu->a = 0x01;
    cpu->b = 0x02;
    cpu->flags = 0x00;
    alu_compare(cpu, cpu->b);
    TEST_ASSERT_EQUAL_HEX8(0x01, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x70, cpu->flags); // N | H | C

    // CP: Half Borrow (H Flag only)
    // 0x10 - 0x01 = 0x0F (Nibble borrow: 0 - 1)
    cpu->a = 0x10;
    cpu->b = 0x01;
    cpu->flags = 0x00;
    alu_compare(cpu, cpu->b);
    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x60, cpu->flags); // N | H
}

void test_alu_inc_dec()
{
    uint8_t val;

    // --- INC TESTS ---

    // INC: Half Carry
    // 0x0F + 1 = 0x10 (Bit 3 overflows)
    val = 0x0F;
    cpu->flags = 0x10; // SET C, must not be cleared
    alu_inc(cpu, &val);
    TEST_ASSERT_EQUAL_HEX8(0x10, val);
    TEST_ASSERT_EQUAL_HEX8(0x30, cpu->flags); // H (0x20) | C (0x10 preserved)

    // INC: Overflow to Zero
    // 0xFF + 1 = 0x00
    val = 0xFF;
    cpu->flags = 0x00;
    alu_inc(cpu, &val);
    TEST_ASSERT_EQUAL_HEX8(0x00, val);
    TEST_ASSERT_EQUAL_HEX8(0xA0, cpu->flags); // Z (0x80) | H (0x20) | N=0

    // --- DEC TESTS ---

    // DEC: Half Borrow
    // 0x10 - 1 = 0x0F (borrow nibble)
    val = 0x10;
    cpu->flags = 0x10; // SET C
    alu_dec(cpu, &val);
    TEST_ASSERT_EQUAL_HEX8(0x0F, val);
    TEST_ASSERT_EQUAL_HEX8(0x70, cpu->flags); // N (0x40) | H (0x20) | C (0x10 preserved)

    // DEC: Zero Result
    val = 0x01;
    cpu->flags = 0x00;
    alu_dec(cpu, &val);
    TEST_ASSERT_EQUAL_HEX8(0x00, val);
    TEST_ASSERT_EQUAL_HEX8(0xC0, cpu->flags); // Z (0x80) | N (0x40)
}

void test_alu_add_u16()
{
    // ADD HL, rr: Half Carry (Bit 11 overflow)
    // 0x0FFF + 1 = 0x1000
    set_hl(cpu, 0x0FFF);
    cpu->flags = 0x80; // Z set beforehand (must be preserved)
    alu_add_u16(cpu, 0x0001);

    TEST_ASSERT_EQUAL_HEX16(0x1000, get_hl(cpu));
    // Z (Preserved) | H (Bit 11 overflow) | N=0 | C=0
    TEST_ASSERT_EQUAL_HEX8(0xA0, cpu->flags);

    // ADD HL, rr: Full Carry (Bit 15 overflow)
    // 0xFFFF + 1 = 0x0000
    set_hl(cpu, 0xFFFF);
    cpu->flags = 0x00;
    alu_add_u16(cpu, 0x0001);

    TEST_ASSERT_EQUAL_HEX16(0x0000, get_hl(cpu));
    // Z (0 preserved) | H (Bit 11 ovf) | C (Bit 15 ovf)
    TEST_ASSERT_EQUAL_HEX8(0x30, cpu->flags);
}

void test_alu_inc_dec_u16()
{
    // INC 16: Overflow
    uint16_t res;
    cpu->flags = 0xF0; // All flags set

    res = alu_inc_u16(0xFFFF);
    TEST_ASSERT_EQUAL_HEX16(0x0000, res);
    TEST_ASSERT_EQUAL_HEX8(0xF0, cpu->flags); // Must NOT change

    // DEC 16: Underflow
    cpu->flags = 0x00; // No flags set

    res = alu_dec_u16(0x0000);
    TEST_ASSERT_EQUAL_HEX16(0xFFFF, res);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->flags); // Must NOT change
}

void test_alu_add_sp()
{
    // Positive Offset
    cpu->sp = 0x1000;
    cpu->flags = 0xF0; // Z/N must be cleared
    alu_add_sp(cpu, 0x05);
    TEST_ASSERT_EQUAL_HEX16(0x1005, cpu->sp);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->flags);

    // Negative Offset (-1)
    // Math: 1 - 1 = 0. Flags: 1 + 0xFF overflows H & C
    cpu->sp = 0x0001;
    alu_add_sp(cpu, (uint8_t)-1);
    TEST_ASSERT_EQUAL_HEX16(0x0000, cpu->sp);
    TEST_ASSERT_EQUAL_HEX8(0x30, cpu->flags); // H | C

    // Half Carry Only
    cpu->sp = 0x000F;
    alu_add_sp(cpu, 0x01);
    TEST_ASSERT_EQUAL_HEX16(0x0010, cpu->sp);
    TEST_ASSERT_EQUAL_HEX8(0x20, cpu->flags); // H
}

void test_alu_bitwise()
{
    // AND: H Flag always 1
    cpu->a = 0x55;
    cpu->flags = 0x10; // C set (should clear)
    alu_and(cpu, 0xAA);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->a);     // 0x55 & 0xAA = 0
    TEST_ASSERT_EQUAL_HEX8(0xA0, cpu->flags); // Z=1 | H=1

    // OR: Clears Flags
    cpu->a = 0x01;
    cpu->flags = 0xF0; // All set (should clear N/H/C)
    alu_or(cpu, 0x02);
    TEST_ASSERT_EQUAL_HEX8(0x03, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->flags);

    // XOR: Zero Result
    cpu->a = 0xFF;
    cpu->flags = 0xF0;
    alu_xor(cpu, 0xFF);
    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x80, cpu->flags); // Z only

    // NOT (CPL): Preserves Z/C, Sets N/H
    cpu->a = 0xF0;
    cpu->flags = 0x90; // Z=1, C=1 (Must Preserve)
    alu_not(cpu);
    TEST_ASSERT_EQUAL_HEX8(0x0F, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0xF0, cpu->flags); // Z | N | H | C
}
