#ifndef PPU_H
#define PPU_H

#include "types.h"

/*
 * Advances the PPU state machine by the given number of CPU cycles.
 * Handles PPU mode transitions (OAM Search, Pixel Transfer,
 * HBlank, VBlank), updates the LY register, and triggers rendering
 * or interrupts when necessary.
 */
void ppu_step(CPU *cpu, uint8_t cycles);

/*
 * Renders the full current scanline (determined by the LY register)
 * https://gbdev.io/pandocs/pixel_fifo.html
 */
void ppu_render_line(PPU *ppu);

/*
 * Decodes a tile index into a raw 2-bit pixel color (0-3).
 * It calculates the memory address based on the addressing mode
 * (controlled by lcdc4) and extracts the specific bits using the
 * provided X and Y coordinates of the pixel.
 * https://gbdev.io/pandocs/Tile_Data.html
 */
uint8_t tile_index_to_pixel_bg_win(PPU *ppu, uint8_t tile_index, uint8_t lcdc4, uint8_t abs_x, uint8_t abs_y);

/*
 * Maps a raw 2-bit color ID (0-3) to the actual display color.
 * https://gbdev.io/pandocs/Palettes.html
 */
uint8_t get_color_from_palette(uint8_t palette_reg, uint8_t color_id);

/*
 * Get the length of mode 3 before rendering
 * Formula = 160 (line length) + 12 (2 tile fetchers) + penalty
 * with penalty = SCX % 8 + 6 () + penalty_for_each_obj.
 *
 * Warning: unused right now.
 */
uint8_t get_mode_3_length(PPU *ppu);

/*
 * Scans the OAM for sprites visible on the current scanline (ppu->lcd_y).
 * Stores up to 10 matching sprite indices (by OAM order) into out_indices.
 * Returns the number of sprites found.
 * https://gbdev.io/pandocs/OAM.html
 */
uint8_t oam_scan(PPU *ppu, uint8_t out_indices[10]);

/*
 * Sets the VBlank interrupt flag (Bit 0) in the CPU's Interrupt Flag (IF) register.
 * https://gbdev.io/pandocs/Interrupts.html#ff0f--if-interrupt-flag
 */
void send_vblank_interrupt(CPU *cpu);

/*
 * Sets the LCD STAT interrupt flag (Bit 1) in the CPU's Interrupt Flag (IF) register.
 * https://gbdev.io/pandocs/Interrupts.html#ff0f--if-interrupt-flag
 */
void send_stat_interrupt(CPU *cpu);

#endif