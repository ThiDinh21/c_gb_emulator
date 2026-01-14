#ifndef PPU_H
#define PPU_H

#include "types.h"

void ppu_step(CPU *cpu, uint8_t cycles);

void ppu_render_line(PPU *ppu);

uint8_t get_mode_3_length(PPU *ppu);

void send_vblank_interrupt(CPU *cpu);

void send_stat_interrupt(CPU *cpu);

#endif