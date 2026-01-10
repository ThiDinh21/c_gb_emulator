#include "errors.h"
#include "mmu.h"

uint8_t read_mem(MMU *mmu, uint16_t addr)
{
    switch (addr)
    {
    case 0x0000 ... 0x7FFF:
        // ROM
        return read_rom(mmu, addr);
    case 0x8000 ... 0x9FFF:
        // VRAM
        // !TODO
        panic_unimplemented("read_mem vram");
    case 0xA000 ... 0xBFFF:
        // RAM (switchable)
        return read_ram(mmu, addr);
    case 0xC000 ... 0xDFFF:
        // WRAM (switchable)
        // !TODO
        panic_unimplemented("read_mem wram");
    case 0xE000 ... 0xFDFF:
        // Mirror RAM
        // Ninetendo said this is prohibited memory segment
        addr -= 0x2000;
        return read_mem(mmu, addr);
    case 0xFE00 ... 0xFE9F:
        // OAM (Object attribute memory)
        // !TODO
        panic_unimplemented("read_mem oam");
    case 0xFEA0 ... 0xFEFF:
        // Prohibited memory segment
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFF00 ... 0xFF7F:
        // Input
        panic_unimplemented("read_mem input");
    case 0xFF80 ... 0xFFFE:
        // HRAM
        panic_unimplemented("read_mem hram");
    case 0xFFFF:
        // Interrupt enable register
        return mmu->interrupt_enable;
    default:
        panic("Attempt to access unused memory region", ERR_INVALID_MEMORY_ACCESS);
    }
}

uint8_t read_rom(MMU *mmu, uint16_t addr)
{
    if (addr < 0x4000)
    {
        // ROM 0
        return mmu->rom_0[addr];
    }
    else
    {
        // ROM N
        // !TODO: temporary, will need to be expanded with MBCs later
        return mmu->rom_n[addr - 0x4000];
    }
}

uint16_t read_mem_u16(MMU *mmu, uint16_t addr)
{
    uint8_t lower_byte = read_mem(mmu, addr);
    __builtin_add_overflow(addr, 1, &addr);
    uint8_t upper_byte = read_mem(mmu, addr);
    return upper_byte << 8 | lower_byte;
}

uint8_t read_ram(MMU *mmu, uint16_t addr)
{
    // !TODO: temporary, will need to be expanded with MBCs later
    return mmu->ram[addr - 0xA000];
}

void write_mem(MMU *mmu, uint16_t addr, uint8_t val)
{
    switch (addr)
    {
    case 0x0000 ... 0x7FFF:
        // ROM
        write_rom(mmu, addr, val);
        return;
    case 0x8000 ... 0x9FFF:
        // VRAM
        // !TODO
        panic_unimplemented("write_mem vram");
    case 0xA000 ... 0xBFFF:
        // RAM (switchable)
        write_ram(mmu, addr, val);
        return;
    case 0xC000 ... 0xDFFF:
        // WRAM (switchable)
        // !TODO
        panic_unimplemented("write_mem wram");
    case 0xE000 ... 0xFDFF:
        // Mirror RAM
        // Ninetendo said this is prohibited memory segment
        addr -= 0x2000;
        write_mem(mmu, addr, val);
        return;
    case 0xFE00 ... 0xFE9F:
        // OAM (Object attribute memory)
        // !TODO
        panic_unimplemented("write_mem oam");
    case 0xFEA0 ... 0xFEFF:
        // Prohibited memory segment
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFF00 ... 0xFF7F:
        // Input
        panic_unimplemented("write_mem_input");
    case 0xFF80 ... 0xFFFE:
        // HRAM
        panic_unimplemented("write_mem hram");
    case 0xFFFF:
        // Interrupt enable register
        mmu->interrupt_enable = val;
        return;
    default:
        panic("Attempt to access unused memory region", ERR_INVALID_MEMORY_ACCESS);
    }
}

void write_mem_u16(MMU *mmu, uint16_t addr, uint16_t val)
{
    uint8_t lower_byte = val & 0x00FF;
    uint8_t upper_byte = val >> 8;

    write_mem(mmu, addr, lower_byte);
    __builtin_add_overflow(addr, 1, &addr);
    write_mem(mmu, addr, upper_byte);
}

void write_rom(MMU *mmu, uint16_t addr, uint8_t val)
{
    if (addr < 0x4000)
    {
        // ROM 0
        mmu->rom_0[addr] = val;
    }
    else
    {
        // ROM N
        // !TODO: temporary, will need to be expanded with MBCs later
        mmu->rom_n[addr - 0x4000] = val;
    }
}

void write_ram(MMU *mmu, uint16_t addr, uint8_t val)
{
    // !TODO: temporary, will need to be expanded with MBCs later
    mmu->ram[addr - 0xA000] = val;
}