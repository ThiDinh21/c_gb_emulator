#include "unity.h"
#include "cpu.h"
#include "ppu.h"
#include "mmu.h"

CPU *cpu;
PPU *ppu;
MMU *mmu;

void setUp(void)
{
    cpu = init_cpu();
    ppu = cpu->ppu;
    mmu = cpu->mmu;

    // Default: LCD Enabled, Mode 2 (Start of line)
    ppu->lcd_control = 0x80;
    ppu->lcd_status = 0;
}

void tearDown(void)
{
    clean_up_cpu(cpu);
}

void test_ppu_completes_one_line_in_456_cycles(void)
{
    // Setup: Start at Line 0, Cycle 0
    ppu->lcd_y = 0;
    ppu->internal_cycle = 0;

    // Action: Run for exactly 456 cycles
    ppu_step(cpu, 200);
    ppu_step(cpu, 200);
    ppu_step(cpu, 56);

    // Assert: Should be on Line 1 now
    TEST_ASSERT_EQUAL_UINT8(1, ppu->lcd_y);
    // Cycle counter should wrap back to 0
    TEST_ASSERT_EQUAL_UINT16(0, ppu->internal_cycle);
}

void test_ppu_enters_vblank_after_144_lines(void)
{
    // Setup: Start at Line 143 (last visible line)
    ppu->lcd_y = 143;
    ppu->internal_cycle = 450;

    // Action: Step past the end of the line (6 cycles)
    ppu_step(cpu, 6);

    // Assert: Should be Line 144 (VBlank)
    TEST_ASSERT_EQUAL_UINT8(144, ppu->lcd_y);
    // Mode (Bits 0-1) should be 1 (V-Blank)
    TEST_ASSERT_EQUAL_UINT8(1, ppu->lcd_status & 0b11);

    // TODO: Check Interrupt Flag 0xFF0f bit 0 was set
}

void test_ppu_stat_interrupt_mode_2(void)
{
    // Setup: Enable Mode 2 Interrupt (Bit 5 of STAT)
    ppu->lcd_status = (1 << 5);
    ppu->lcd_y = 0;
    ppu->internal_cycle = 0; // Mode 2 starts at cycle 0

    // Action: Step into Mode 2
    ppu_step(cpu, 4);

    // STAT Interrupt should have fired
    // TEST_ASSERT_BIT_HIGH(1, read_mem(&mmu, 0xFF0F));
    // TODO: Check Interrupt Flag 0xFF0F bit 1 was set
}
