#include <string.h>
#include "ppu.h"
#include "mmu.h"
#include "errors.h"
#include "logging.h"

// https://gbdev.io/pandocs/Rendering.html
void ppu_step(CPU *cpu, uint8_t cycles)
{
    PPU *ppu = cpu->ppu;

    // LCD off (LCDC bit 7 = 0): freeze LY at 0, mode at 0, no rendering or interrupts
    // https://gbdev.io/pandocs/LCDC.html#lcdc7--lcd-enable
    if (!(ppu->lcd_control & (1 << 7)))
    {
        ppu->lcd_y = 0;
        ppu->internal_cycle = 0;
        ppu->lcd_status &= ~0b11; // mode = 0 (HBlank)
        return;
    }

    uint16_t old_internal_cycle = ppu->internal_cycle;
    uint8_t curr_ppu_mode = ppu->lcd_status & 0b11;
    uint8_t stat_interrupt_condition = 0;
    ppu->internal_cycle = (ppu->internal_cycle + cycles) % 456; // each line take 456 T-cycles to render

    // Jump to new LCD line
    if (old_internal_cycle > ppu->internal_cycle)
    {
        ppu->lcd_y++;
        ppu->lcd_y %= 154; // Line is from 0..153
        if (ppu->lcd_y == 0)
        {
            ppu->window_internal_line = 0;
        }
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
    // Screen is 160 x 144, each line is 32 tiles, each tiles is 8x8 pixels
    uint8_t line_pixels[160] = {0};
    uint8_t bg_raw[160] = {0}; // Raw 2-bit color IDs (before palette) for sprite priority

    // FIFO Pixel fetcher (5 steps)
    // Step 1: Get tile
    // Step 2: Get tile data low
    // Step 3: Get tile data high
    // Step 4: Sleep (ignored)
    // Step 5: Push
    // Always use tilemap 0x9800-0x9BFF or use 0x9C00-0x9FFF when:
    // - LCDC.3 is enabled and the X coordinate of the current scanline is not inside the window.
    // - LCDC.6 is enabled and the X coordinate of the current scanline is inside the window.
    uint8_t bg_win_enable = ppu->lcd_control & (1 << 0); // LCDC bit 0: BG+Window enable
    uint8_t lcdc_3 = ppu->lcd_control & (1 << 3) ? 1 : 0;
    uint8_t lcdc_6 = ppu->lcd_control & (1 << 6) ? 1 : 0;
    uint16_t bg_tile_map = (lcdc_3 ? 0x9C00 : 0x9800) - 0x8000;
    uint16_t window_tile_map = (lcdc_6 ? 0x9C00 : 0x9800) - 0x8000;
    uint8_t lcdc_4 = ppu->lcd_control & (1 << 4) ? 1 : 0;        // Addressing mode of tile data
    uint8_t window_enable = ppu->lcd_control & (1 << 5) ? 1 : 0; // LCDC 5

    if (bg_win_enable)
    {
        // Calc tile's row
        // Add pixels (ly + scy) then divide by 8 to get tile row
        // & 255 for the vertical wrap-around
        uint8_t abs_pixel_y = ppu->lcd_y + ppu->bg_y;
        uint8_t tile_y = abs_pixel_y / 8;
        uint8_t bg_tile_index, window_tile_index;
        uint8_t is_window_rendered = 0;

        // Loop through each pixel of a row
        for (int i = 0; i < 160; i++)
        {
            /* Layer 2: Window */
            if (window_enable && i + 7 >= ppu->win_x && ppu->lcd_y >= ppu->win_y)
            {
                uint8_t win_x = ppu->win_x < 7 ? i : i - (ppu->win_x - 7);
                uint8_t tile_x = win_x / 8;
                window_tile_index = ppu->vram[window_tile_map + (ppu->window_internal_line / 8) * 32 + tile_x];
                uint8_t pixel_color_id = tile_index_to_pixel_bg_win(ppu, window_tile_index, lcdc_4, win_x, ppu->window_internal_line);
                bg_raw[i] = pixel_color_id;
                line_pixels[i] = get_color_from_palette(ppu->bg_palette, pixel_color_id);
                is_window_rendered = 1;
                continue;
            }

            /* Layer 3: Background */
            uint8_t abs_pixel_x = i + ppu->bg_x;
            uint8_t tile_x = abs_pixel_x / 8;
            bg_tile_index = ppu->vram[bg_tile_map + tile_y * 32 + tile_x];
            uint8_t pixel_color_id = tile_index_to_pixel_bg_win(ppu, bg_tile_index, lcdc_4, abs_pixel_x, abs_pixel_y);
            bg_raw[i] = pixel_color_id;
            line_pixels[i] = get_color_from_palette(ppu->bg_palette, pixel_color_id);
        }

        if (is_window_rendered)
            ppu->window_internal_line++;
    }

    /* Layer 1: Objects */
    uint8_t obj_enable = ppu->lcd_control & (1 << 1);
    uint8_t visible_sprites[10];
    uint8_t sprite_count = oam_scan(ppu, visible_sprites);
    if (obj_enable)
        render_sprites(ppu, line_pixels, bg_raw, visible_sprites, sprite_count);

    memcpy(ppu->test_line_buffer, line_pixels, 160);
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

uint8_t get_mode_3_length(PPU *ppu)
{
    (void)ppu;
    // TODO: impl exhaustive mode 3 length calculation
    // Formula: 172 (base) + SCX%8 + penalty per visible sprite
    return 172; // minimum value; good enough for now
}

void render_sprites(PPU *ppu, uint8_t *line_pixels, const uint8_t *bg_raw,
                    const uint8_t *visible_sprites, uint8_t sprite_count)
{
    uint8_t height = (ppu->lcd_control & (1 << 2)) ? 16 : 8;

    // Render in reverse OAM order: lower OAM index is higher priority and drawn last
    for (int s = sprite_count - 1; s >= 0; s--)
    {
        Sprite *sprite = &ppu->sprites[visible_sprites[s]];
        // Which row of the tile falls on the current scanline
        uint8_t row = ppu->lcd_y - (sprite->y - 16);

        // Y flip (flag bit 6)
        if (sprite->flags & (1 << 6))
        {
            row = height - 1 - row;
        }

        // 8x16 mode: bit 0 of tile index is ignored; top/bottom tile selected by row
        uint8_t tile_index = sprite->index;
        if (height == 16)
        {
            tile_index &= 0xFE; // force even (top tile)
            if (row >= 8)
            {
                tile_index |= 0x01; // bottom tile
                row -= 8;
            }
        }

        // Sprites always use $8000 addressing
        uint16_t tile_addr = (uint16_t)tile_index * 16 + row * 2;
        uint8_t byte1 = ppu->vram[tile_addr];
        uint8_t byte2 = ppu->vram[tile_addr + 1];

        uint8_t palette = (sprite->flags & (1 << 4)) ? ppu->obj_palette_1 : ppu->obj_palette_0;
        uint8_t priority = sprite->flags & (1 << 7); // 1 = sprite behind BG colors 1-3

        for (int col = 0; col < 8; col++)
        {
            int screen_x = (int)sprite->x - 8 + col;
            if (screen_x < 0 || screen_x >= 160)
            {
                continue;
            }

            // X flip (flag bit 5): read bits from the opposite end
            uint8_t bit_pos = (sprite->flags & (1 << 5)) ? col : 7 - col;
            uint8_t lsb = (byte1 >> bit_pos) & 0x1;
            uint8_t msb = (byte2 >> bit_pos) & 0x1;
            uint8_t color_id = (msb << 1) | lsb;

            // Color 0 is always transparent for sprites
            if (color_id == 0)
            {
                continue;
            }

            // BG priority: sprite is behind BG colors 1-3
            if (priority && bg_raw[screen_x] != 0)
            {
                continue;
            }

            line_pixels[screen_x] = get_color_from_palette(palette, color_id);
        }
    }
}

uint8_t oam_scan(PPU *ppu, uint8_t out_indices[10])
{
    uint8_t height = (ppu->lcd_control & (0b1 << 2)) ? 16 : 8;
    uint8_t count = 0;

    for (uint8_t i = 0; i < 40 && count < 10; i++)
    {
        Sprite *s = &ppu->sprites[i];
        // Sprite Y is stored as screen_y + 16, so visible if:
        // s->y <= lcd_y + 16 < s->y + height
        uint16_t ly16 = (uint16_t)ppu->lcd_y + 16;
        if (ly16 >= s->y && ly16 < (uint16_t)s->y + height)
        {
            out_indices[count++] = i;
        }
    }

    return count;
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
