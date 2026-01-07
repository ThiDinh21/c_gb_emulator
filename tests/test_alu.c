#include "alu.h"
#include "../tests/unity.h"

CPU *cpu;

void setUp()
{
    cpu = init_cpu();
}
void tearDown() {}

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
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_alu_add);
    return UNITY_END();
}