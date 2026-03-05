#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stdlib.h>
#include "types.h"

/// Routes read memory requests to specific regions based on the address ranges.
/// This function is the primary entry point and should be called instead of region-specific functions.
///
/// ref: https://gbdev.io/pandocs/Memory_Map.html
uint8_t read_mem(CPU *cpu, uint16_t addr);

/// Routes read memory requests to specific regions based on the address ranges.
/// Reads two bytes little-endian from addr and addr+1.
/// This function is the primary entry point and should be called instead of region-specific functions.
///
/// ref: https://gbdev.io/pandocs/Memory_Map.html
uint16_t read_mem_u16(CPU *cpu, uint16_t addr);

/// Returns a value from the ROM memory range (0x0000..0x7FFF), applying MBC bank switching for 0x4000..0x7FFF.
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_rom(MMU *mmu, uint16_t addr);

/// Returns a value from the VRAM memory range (0x8000..0x9FFF).
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_vram(PPU *ppu, uint16_t addr);

/// Returns a value from the SRAM (cartridge RAM) memory range (0xA000..0xBFFF).
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_sram(MMU *mmu, uint16_t addr);

/// Returns a value from the WRAM memory range (0xC000..0xDFFF).
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_wram(MMU *mmu, uint16_t addr);

/// Returns a value from the OAM (sprites) memory range (0xFE00..0xFE9F).
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_oam(PPU *ppu, uint16_t addr);

/// Returns a value from the I/O registers memory range (0xFF00..0xFF7F).
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_io(CPU *cpu, uint16_t addr);

/// Returns a value from the HRAM (high RAM) memory range (0xFF80..0xFFFE).
/// Internal helper called by read_mem. Do not call directly.
uint8_t read_hram(MMU *mmu, uint16_t addr);

/// Routes write memory requests to specific regions based on the address ranges.
/// This function is the primary entry point and should be called instead of region-specific functions.
///
/// ref: https://gbdev.io/pandocs/Memory_Map.html
void write_mem(CPU *cpu, uint16_t addr, uint8_t val);

/// Routes write memory requests to specific regions based on the address ranges.
/// Writes two bytes little-endian to addr and addr+1.
/// This function is the primary entry point and should be called instead of region-specific functions.
///
/// ref: https://gbdev.io/pandocs/Memory_Map.html
void write_mem_u16(CPU *cpu, uint16_t addr, uint16_t val);

/// Handles writes to the ROM address range (0x0000..0x7FFF).
/// Does not write to ROM directly — instead updates MBC control registers (bank selection, RAM enable, etc).
/// Internal helper called by write_mem. Do not call directly.
void write_rom(MMU *mmu, uint16_t addr, uint8_t val);

/// Writes a value to the VRAM memory range (0x8000..0x9FFF).
/// Internal helper called by write_mem. Do not call directly.
void write_vram(PPU *ppu, uint16_t addr, uint8_t val);

/// Writes a value to the SRAM (cartridge RAM) memory range (0xA000..0xBFFF).
/// Internal helper called by write_mem. Do not call directly.
void write_sram(MMU *mmu, uint16_t addr, uint8_t val);

/// Writes a value to the WRAM memory range (0xC000..0xDFFF).
/// Internal helper called by write_mem. Do not call directly.
void write_wram(MMU *mmu, uint16_t addr, uint8_t val);

/// Writes a value to the OAM (sprites) memory range (0xFE00..0xFE9F).
/// Internal helper called by write_mem. Do not call directly.
void write_oam(PPU *ppu, uint16_t addr, uint8_t val);

/// Writes a value to the I/O registers memory range (0xFF00..0xFF7F).
/// Internal helper called by write_mem. Do not call directly.
void write_io(CPU *cpu, uint16_t addr, uint8_t val);

/// Writes a value to the HRAM (high RAM) memory range (0xFF80..0xFFFE).
/// Internal helper called by write_mem. Do not call directly.
void write_hram(MMU *mmu, uint16_t addr, uint8_t val);

#endif