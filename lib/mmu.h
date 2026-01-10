#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    // uint8_t memory[65536];
    uint8_t rom_0[0x4000];
    uint8_t rom_n[0x4000]; // !TODO: a switch bank, might need to make bigger
    // size_t rom_index;
    uint8_t vram[0x2000];
    uint8_t sram[0x2000];
    uint8_t wram[0x2000];
    // size_t wram_index;
    uint8_t oam[0xA0];
    uint8_t io[0x80];
    uint8_t hram[0x7F];
    uint8_t interrupt_enable;
} MMU;

uint8_t read_mem(MMU *mmu, uint16_t addr);

uint16_t read_mem_u16(MMU *mmu, uint16_t addr);

uint8_t read_rom(MMU *mmu, uint16_t addr);

uint8_t read_sram(MMU *mmu, uint16_t addr);

uint8_t read_wram(MMU *mmu, uint16_t addr);

void write_mem(MMU *mmu, uint16_t addr, uint8_t val);

void write_mem_u16(MMU *mmu, uint16_t addr, uint16_t val);

void write_rom(MMU *mmu, uint16_t addr, uint8_t val);

void write_sram(MMU *mmu, uint16_t addr, uint8_t val);

void write_wram(MMU *mmu, uint16_t addr, uint8_t val);

#endif