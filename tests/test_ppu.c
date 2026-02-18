#include "unity.h"
#include "cpu.h"
#include "ppu.h"
#include "mmu.h"
#include "logging.h"

CPU *cpu;
PPU *ppu;

void setUp(void)
{
    cpu = init_cpu();
    ppu = cpu->ppu;

    // Default: LCD Enabled, Mode 2 (Start of line)
    ppu->lcd_control = 0x80;
    ppu->lcd_status = 0;

    // Setup a specific tile pattern in VRAM
    // Tile 0 Data (Address 0x8000 - 0x800F) -> All Zeros
    for (int i = 0; i < 16; i++)
        write_mem(cpu, 0x8000 + i, 0x00);
    // Tile 1 Data (Address 0x8010 - 0x801F) -> All Ones (0xFF)
    // 2 bits per pixel. 0xFF low byte + 0xFF high byte = Color 3.
    for (int i = 0; i < 16; i++)
        write_mem(cpu, 0x8010 + i, 0xFF);

    // Fill Background Map (0x9800) with Tile 0
    // Fill Window Map (0x9C00) with Tile 1
    for (int i = 0; i < 1024; i++)
    {
        write_mem(cpu, 0x9800 + i, 0x00); // BG uses Tile 0 (White)
        write_mem(cpu, 0x9C00 + i, 0x01); // Win uses Tile 1 (Black)
    }
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

    // VBlank Interrupt should have fired
    TEST_ASSERT_BIT_HIGH(0, read_mem(cpu, 0xFF0F));
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
    TEST_ASSERT_BIT_HIGH(1, read_mem(cpu, 0xFF0F));
}

/* * TEST: Window Mid-Scanline Cutoff
 * Scenario:
 * - Background is White (Tile 0)
 * - Window is Black (Tile 1)
 * - Window is enabled at X = 40 (WX = 47)
 * * Expected Result:
 * - Pixels 0 to 39: WHITE (Color 0)
 * - Pixels 40 to 159: BLACK (Color 3)
 */
void test_window_trigger_mid_scanline(void)
{
    // 1. Configure Registers
    // LCDC: LCD On (Bit 7), Win Map=9C00 (Bit 6), Win On (Bit 5), BG Map=9800 (Bit 3), BG On (Bit 0)
    // Binary: 1111 0001 -> 0xF1
    ppu->lcd_control = 0xF1;
    ppu->lcd_y = 0;         // Current Line 0
    ppu->win_y = 0;         // Window starts at Line 0
    ppu->win_x = 40 + 7;    // Window starts at Pixel 40 (WX is X+7)
    ppu->bg_x = 0;          // No BG Scroll
    ppu->bg_palette = 0xE4; // Palette: 3=Black, 0=White

    // 2. Run exactly one scanline: 456 dots (cycles)
    for (int i = 0; i < 456; i++)
    {
        ppu_step(cpu, 1);
    }

    // 3. Verify Pixels before the window (0-39)
    // Should be Color 0 (White) from Background
    char message[64];
    for (int x = 0; x < 40; x++)
    {
        sprintf(message, "BG Pixel at X=%d should be White (0)", x);
        // Assuming your output buffer stores the resolved palette color (0-3)
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, ppu->test_line_buffer[x], message);
    }

    // 4. Verify Pixels inside the window (40-159)
    // Should be Color 3 (Black) from Window
    for (int x = 40; x < 160; x++)
    {
        sprintf(message, "Window Pixel at X=%d should be Black (3)", x);
        TEST_ASSERT_EQUAL_INT_MESSAGE(3, ppu->test_line_buffer[x], message);
    }
}

/* OAM SCAN TESTS */

// A sprite at Y=16 (screen Y=0) with height=8 should be visible on scanline 0
void test_oam_scan_finds_sprite_on_current_line(void)
{
    ppu->lcd_control = 0x80; // LCD on, 8x8 sprites
    ppu->lcd_y = 0;

    // Place sprite 0 at screen Y=0 (raw Y = 0 + 16 = 16)
    write_mem(cpu, 0xFE00, 16); // Y
    write_mem(cpu, 0xFE01, 8);  // X
    write_mem(cpu, 0xFE02, 0);  // tile index
    write_mem(cpu, 0xFE03, 0);  // flags

    uint8_t indices[10];
    uint8_t count = oam_scan(ppu, indices);

    TEST_ASSERT_EQUAL_UINT8(1, count);
    TEST_ASSERT_EQUAL_UINT8(0, indices[0]);
}

// A sprite above the scanline should not be found
void test_oam_scan_ignores_sprite_above_line(void)
{
    ppu->lcd_control = 0x80;
    ppu->lcd_y = 10;

    // Sprite at screen Y=0..7, scanline is 10 -> not visible
    write_mem(cpu, 0xFE00, 16); // raw Y = 16 -> screen Y = 0
    write_mem(cpu, 0xFE01, 8);
    write_mem(cpu, 0xFE02, 0);
    write_mem(cpu, 0xFE03, 0);

    uint8_t indices[10];
    uint8_t count = oam_scan(ppu, indices);

    TEST_ASSERT_EQUAL_UINT8(0, count);
}

// Only the first 10 sprites should be returned even if more are visible
void test_oam_scan_caps_at_10_sprites(void)
{
    ppu->lcd_control = 0x80;
    ppu->lcd_y = 0;

    // Place 12 sprites all at screen Y=0 (raw Y=16)
    for (int i = 0; i < 12; i++)
    {
        write_mem(cpu, 0xFE00 + i * 4, 16); // Y
        write_mem(cpu, 0xFE01 + i * 4, 8);  // X
        write_mem(cpu, 0xFE02 + i * 4, 0);  // tile
        write_mem(cpu, 0xFE03 + i * 4, 0);  // flags
    }

    uint8_t indices[10];
    uint8_t count = oam_scan(ppu, indices);

    TEST_ASSERT_EQUAL_UINT8(10, count);
}

// In 8x16 mode, sprite height is 16 so it covers two extra scanlines
void test_oam_scan_8x16_extended_height(void)
{
    ppu->lcd_control = 0x80 | (1 << 2); // LCD on + 8x16
    ppu->lcd_y = 12;

    // Sprite at screen Y=0 (raw Y=16) — in 8x16 it covers lines 0..15
    write_mem(cpu, 0xFE00, 16);
    write_mem(cpu, 0xFE01, 8);
    write_mem(cpu, 0xFE02, 0);
    write_mem(cpu, 0xFE03, 0);

    uint8_t indices[10];
    uint8_t count = oam_scan(ppu, indices);

    TEST_ASSERT_EQUAL_UINT8(1, count);
}