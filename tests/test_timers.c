#include "unity.h"
#include "cpu.h"
#include "timer.h"
#include "mmu.h"

CPU *cpu;
Timer *timer;

void setUp(void)
{
    cpu = init_cpu();
    timer = cpu->timer;

    // Enable Timer (Bit 2) + Clock Select 16 cycles (01 -> Bit 3)
    // TAC = 0000 0101 = 0x05
    timer->timer_control = 0x05;
}

void tearDown(void)
{
    clean_up_cpu(cpu);
}

void test_read_write_time_registers(void)
{
    write_mem(cpu, 0xFF04, 0x12);
    // Writing to Divider register (0xFF04) reset it to 0
    TEST_ASSERT_EQUAL_UINT8(0x00, read_mem(cpu, 0xFF04));

    write_mem(cpu, 0xFF05, 0xFF);
    TEST_ASSERT_EQUAL_UINT8(0xFF, read_mem(cpu, 0xFF05));

    write_mem(cpu, 0xFF06, 0x09);
    TEST_ASSERT_EQUAL_UINT8(0x09, read_mem(cpu, 0xFF06));

    write_mem(cpu, 0xFF07, 0x10);
    TEST_ASSERT_EQUAL_UINT8(0x10, read_mem(cpu, 0xFF07));
}

void test_timer_rising_edge_does_not_tick(void)
{
    // Setup: Clock is 0. Bit 3 is 0.
    timer->internal_clock = 0;
    timer->timer_counter = 0;

    // Action: Advance 8 cycles.
    // Clock becomes 8 (0b01000). Bit 3 goes 0 -> 1 (Rising Edge).
    timer_handling(cpu, 8);

    // Assert: Counter should NOT have changed.
    TEST_ASSERT_EQUAL_UINT8(0, timer->timer_counter);
}

void test_timer_falling_edge_ticks(void)
{
    // Setup: Clock is 8. Bit 3 is 1.
    timer->internal_clock = 8;
    timer->timer_counter = 0;

    // Action: Advance 8 cycles.
    // Clock becomes 16 (0b10000). Bit 3 goes 1 -> 0 (Falling Edge).
    timer_handling(cpu, 8);

    // Assert: Counter should increment.
    TEST_ASSERT_EQUAL_UINT8(1, timer->timer_counter);
}

void test_timer_overflow_and_interrupt(void)
{
    // Setup:
    // Counter is at max (255)
    // Modulo (TMA) is set to 50
    // Internal clock is just before a flip (8 -> 16)
    timer->timer_counter = 0xFF;
    timer->timer_modulo = 50;
    timer->internal_clock = 8;

    // Clear Interrupt Flag (IF)
    write_mem(cpu, 0xFF0F, 0x00);

    // Action: Trigger the tick
    timer_handling(cpu, 8);

    // Assert 1: Counter should have reloaded from TMA (50)
    TEST_ASSERT_EQUAL_UINT8(50, timer->timer_counter);

    // Assert 2: Timer Interrupt Flag (Bit 2 of 0xFF0F) should be set
    uint8_t if_reg = read_mem(cpu, 0xFF0F);
    TEST_ASSERT_BIT_HIGH(2, if_reg); // Checks if Bit 2 is 1
}
