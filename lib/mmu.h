#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stdlib.h>
#include "types.h"

uint8_t read_mem(MMU *mmu, uint16_t addr);

uint16_t read_mem_u16(MMU *mmu, uint16_t addr);

uint8_t read_rom(MMU *mmu, uint16_t addr);

uint8_t read_vram(MMU *mmu, uint16_t addr);

uint8_t read_sram(MMU *mmu, uint16_t addr);

uint8_t read_wram(MMU *mmu, uint16_t addr);

uint8_t read_oam(MMU *mmu, uint16_t addr);

uint8_t read_io(MMU *mmu, uint16_t addr);

uint8_t read_hram(MMU *mmu, uint16_t addr);

void write_mem(MMU *mmu, uint16_t addr, uint8_t val);

void write_mem_u16(MMU *mmu, uint16_t addr, uint16_t val);

void write_rom(MMU *mmu, uint16_t addr, uint8_t val);

void write_vram(MMU *mmu, uint16_t addr, uint8_t val);

void write_sram(MMU *mmu, uint16_t addr, uint8_t val);

void write_wram(MMU *mmu, uint16_t addr, uint8_t val);

void write_oam(MMU *mmu, uint16_t addr, uint8_t val);

void write_io(MMU *mmu, uint16_t addr, uint8_t val);

void write_hram(MMU *mmu, uint16_t addr, uint8_t val);

#endif