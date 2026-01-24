#ifndef PPU_H
#define PPU_H

#include "types.h"

void ppu_step(CPU *cpu, uint8_t cycles);

void ppu_render_line(PPU *ppu);

uint8_t tile_index_to_pixel_bg_win(PPU *ppu, uint8_t tile_index, uint8_t lcdc4, uint8_t abs_x, uint8_t abs_y);

uint8_t get_color_from_palette(uint8_t palette_reg, uint8_t color_id);

uint8_t get_mode_3_length(PPU *ppu);

void send_vblank_interrupt(CPU *cpu);

void send_stat_interrupt(CPU *cpu);

#endif