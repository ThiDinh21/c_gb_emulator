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
        panic_unimplemented();
    case 0xA000 ... 0xBFFF:
        // RAM (switchable)
        return read_ram(mmu, addr);
    case 0xC000 ... 0xDFFF:
        // WRAM (switchable)
        // !TODO
        panic_unimplemented();
    case 0xE000 ... 0xFDFF:
        // Prohibited memory segment
        // !TODO: Echo RAM, some games use this, might need to implement later
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFE00 ... 0xFE9F:
        // OAM (Object attribute memory)
        // !TODO
        panic_unimplemented();
    case 0xFEA0 ... 0xFEFF:
        // Prohibited memory segment
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFF00 ... 0xFF7F:
        // Input
        panic_unimplemented();
    case 0xFF80 ... 0xFFFE:
        // HRAM
        panic_unimplemented();
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
        panic_unimplemented();
    case 0xA000 ... 0xBFFF:
        // RAM (switchable)
        write_ram(mmu, addr, val);
        return;
    case 0xC000 ... 0xDFFF:
        // WRAM (switchable)
        // !TODO
        panic_unimplemented();
    case 0xE000 ... 0xFDFF:
        // Prohibited memory segment
        // !TODO: Echo RAM, some games use this, might need to implement later
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFE00 ... 0xFE9F:
        // OAM (Object attribute memory)
        // !TODO
        panic_unimplemented();
    case 0xFEA0 ... 0xFEFF:
        // Prohibited memory segment
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFF00 ... 0xFF7F:
        // Input
        panic_unimplemented();
    case 0xFF80 ... 0xFFFE:
        // HRAM
        panic_unimplemented();
    case 0xFFFF:
        // Interrupt enable register
        mmu->interrupt_enable = val;
        return;
    default:
        panic("Attempt to access unused memory region", ERR_INVALID_MEMORY_ACCESS);
    }
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