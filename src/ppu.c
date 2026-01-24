#include "ppu.h"
#include "errors.h"

// https://gbdev.io/pandocs/Rendering.html
void ppu_step(CPU *cpu, uint8_t cycles)
{
    PPU *ppu = cpu->ppu;
    uint16_t old_internal_cycle = ppu->internal_cycle;
    uint8_t curr_ppu_mode = ppu->lcd_status & 0b11;
    uint8_t stat_interrupt_condition = 0;
    ppu->internal_cycle = (ppu->internal_cycle + cycles) % 456; // each line take 456 T-cycles to render

    // Jump to new LCD line
    if (old_internal_cycle > ppu->internal_cycle)
    {
        ppu->lcd_y++;
        ppu->lcd_y %= 154; // Line is from 0..153
        ppu->mode_3_length = 0;
        // Update status reg bit 2
        if (ppu->lcd_y == ppu->lcd_y_cmp)
        {
            ppu->lcd_status |= 0b1 << 2;
            stat_interrupt_condition |= ppu->lcd_status & (0b1 << 6); // Check if LY=LYC flag is enabled
        }
        else
        {
            ppu->lcd_status &= ~(0b1 << 2);
        }
    }

    // Enters PPU MODE 1: Vertical Blank
    // from line 144 to 153
    if (ppu->lcd_y > 143)
    {
        if (curr_ppu_mode != 0b01)
        {
            send_vblank_interrupt(cpu);
            stat_interrupt_condition |= ppu->lcd_status & (0b1 << 4); // Check if Mode 1 flag is enabled
            // Set mode to 1
            ppu->lcd_status &= ~0b10; // Set bit 1 to 0
            ppu->lcd_status |= 0b1;   // Set bit 0 to 1
        }
    }

    // Enters PPU Mode 2: OAM scan
    // Emu doesn't need to scan
    else if (ppu->internal_cycle < 80 && curr_ppu_mode != 0b10)
    {
        stat_interrupt_condition |= ppu->lcd_status & (0b1 << 5); // Check if Mode 2 flag is enabled
        // Set mode to 2
        ppu->lcd_status &= ~0b1;   // Set bit 0 to 0
        ppu->lcd_status |= 1 << 1; // Set bit 1 to 1
    }

    // Enters PPU Mode 3: Render to LCD (we render whole line at the end of this mode)
    // Check if just switch from mode 2 to 3
    // If true calculate the length of mode 3 and switch to mode 3
    else if (ppu->internal_cycle >= 80 && curr_ppu_mode == 0b10)
    {
        ppu->mode_3_length = get_mode_3_length(ppu);
        ppu->lcd_status |= 0b11; // Set bit 0 and 1 to 1
    }

    // PPU Mode 0: Horizontal Blank
    else if (ppu->internal_cycle >= 80 + ppu->mode_3_length && curr_ppu_mode == 0b11)
    {
        stat_interrupt_condition |= ppu->lcd_status & (0b1 << 3); // Check if Mode 0 flag is enabled
        ppu_render_line(ppu);                                     // After mode 3 we render whole line
        ppu->lcd_status &= ~0b11;                                 // Set bit 0 and 1 to 0
    }

    if (stat_interrupt_condition)
    {
        send_stat_interrupt(cpu);
    }
}

void ppu_render_line(PPU *ppu)
{
    panic_unimplemented("PPU render");

    // Screen is 160 x 144, each line is 32 tiles, each tiles is 8x8 pixels
    uint8_t line_pixels[160] = {0};

    // FIFO Pixel fetcher (5 steps)
    // Step 1: Get tile
    // Always use tilemap 0x9800-0x9BFF or use 0x9C00-0x9FFF when:
    // - LCDC.3 is enabled and the X coordinate of the current scanline is not inside the window.
    // - LCDC.6 is enabled and the X coordinate of the current scanline is inside the window.
    uint8_t lcdc_3 = ppu->lcd_control & (1 << 3) ? 1 : 0;
    uint8_t lcdc_4 = ppu->lcd_control & (1 << 4) ? 1 : 0; // Addressing mode of tile data
    uint16_t bg_tile_map = (lcdc_3 ? 0x9C00 : 0x9800) - 0x8000;
    // Calc tile's row
    // Add pixels (ly + scy) then divide by 8 to get tile row
    // & 255 for the vertical wrap-around
    uint8_t abs_pixel_y = ppu->lcd_y + ppu->bg_y;
    uint8_t tile_y = abs_pixel_y / 8;

    // Loop through each pixel of a row
    for (int i = 0; i < 160; i++)
    {
        // Calc tile col
        uint8_t abs_pixel_x = i + ppu->bg_x;
        uint8_t tile_x = abs_pixel_x / 8; // From 0..31
        // Calculate Address: base + (row * 32) + col
        uint16_t tile_index = ppu->vram[bg_tile_map + tile_y * 32 + tile_x];
        uint8_t pixel_color_id = tile_index_to_pixel_bg_win(ppu, tile_index, lcdc_4, abs_pixel_x, abs_pixel_y);
        line_pixels[i] = get_color_from_palette(ppu->bg_palette, pixel_color_id);
    }

    uint8_t lcdc_6 = ppu->lcd_control & (1 << 6) ? 1 : 0;
    uint16_t window_tile_map = (lcdc_6 ? 0x9C00 : 0x9800) - 0x8000;
    // uint8_t win_x_start =

    // Get bg tile (9800–9BFF):
    // bg_x = ;

    // Step 2: Get tile data low
    // Step 3: Get tile data high
    // Step 4: Sleep (ignored)
    // Step 5: Push
}

uint8_t tile_index_to_pixel_bg_win(PPU *ppu, uint8_t tile_index, uint8_t lcdc4, uint8_t abs_x, uint8_t abs_y)
{
    uint8_t byte1, byte2;
    uint16_t index;
    uint8_t col = abs_x % 8; // both must in range 0..7 as a tile is 8x8
    uint8_t row = abs_y % 8;

    // Tile data address mode: https://gbdev.io/pandocs/Tile_Data.html#vram-tile-data
    if (lcdc4)
    {
        index = tile_index * 16 + row * 2;
    }
    else
    {
        int8_t signed_tile_index = (int8_t)tile_index;
        index = 0x1000 + signed_tile_index * 16 + row * 2;
    }
    byte1 = ppu->vram[index];
    byte2 = ppu->vram[index + 1];

    uint8_t bit_pos = 7 - col;
    uint8_t lsb = (byte1 >> bit_pos) & 0b1;
    uint8_t msb = (byte2 >> bit_pos) & 0b1;
    return (msb << 1) | lsb;
}

uint8_t get_color_from_palette(uint8_t palette_reg, uint8_t color_id)
{
    return (palette_reg >> (color_id * 2)) & 0b11;
}

// Get the length of mode 3 before rendering
// Formula = 160 (line length) + 12 (2 tile fetchers) + penalty
// with penalty = SCX % 8 + 6 () + penalty_for_each_obj
uint8_t get_mode_3_length(PPU *ppu)
{
    return 172; // Return the minimum value is probably good enough
    // TODO: impl exhaustive mode 3 length calculation
    ppu->mode_3_length = 172;
}

void send_vblank_interrupt(CPU *cpu)
{
    uint8_t interrupt_flag = read_mem(cpu, 0xFF0F);
    write_mem(cpu, 0xFF0F, interrupt_flag | 0b1);
}

void send_stat_interrupt(CPU *cpu)
{
    uint8_t interrupt_flag = read_mem(cpu, 0xFF0F);
    write_mem(cpu, 0xFF0F, interrupt_flag | (1 << 1));
}
