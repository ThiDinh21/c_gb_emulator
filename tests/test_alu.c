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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_alu_add);
    RUN_TEST(test_alu_sub);
    return UNITY_END();
}