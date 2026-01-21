#include "errors.h"
#include "mmu.h"
#include "ppu.h"
#include "logging.h"

uint8_t read_mem(CPU *cpu, uint16_t addr)
{
    MMU *mmu = cpu->mmu;
    PPU *ppu = cpu->ppu;

    switch (addr)
    {
    case 0x0000 ... 0x7FFF:
        // ROM
        return read_rom(mmu, addr);
    case 0x8000 ... 0x9FFF:
        // VRAM
        return read_vram(ppu, addr);
    case 0xA000 ... 0xBFFF:
        // RAM (switchable)
        return read_sram(mmu, addr);
    case 0xC000 ... 0xDFFF:
        // WRAM
        return read_wram(mmu, addr);
    case 0xE000 ... 0xFDFF:
        // Echo WRAM
        // Ninetendo said this is prohibited memory segment
        return read_mem(cpu, addr - 0x2000);
    case 0xFE00 ... 0xFE9F:
        // OAM (Object attribute memory)
        return read_oam(ppu, addr);
    case 0xFEA0 ... 0xFEFF:
#ifdef UNITY_TEST_RUN
        return mmu->prohibit[addr - 0xFEA0];
#endif
        // Prohibited memory segment
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFF00 ... 0xFF7F:
        // Input
        return read_io(cpu, addr);
    case 0xFF80 ... 0xFFFE:
        // HRAM
        return read_hram(mmu, addr);
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

uint16_t read_mem_u16(CPU *cpu, uint16_t addr)
{
    uint8_t lower_byte = read_mem(cpu, addr);
    __builtin_add_overflow(addr, 1, &addr);
    uint8_t upper_byte = read_mem(cpu, addr);
    return upper_byte << 8 | lower_byte;
}

uint8_t read_vram(PPU *ppu, uint16_t addr)
{
    // TODO: Block vram access during some PPU modesc
    return ppu->vram[addr - 0x8000];
}

uint8_t read_sram(MMU *mmu, uint16_t addr)
{
    // !TODO: temporary, will need to be expanded with MBCs later
    return mmu->sram[addr - 0xA000];
}

uint8_t read_wram(MMU *mmu, uint16_t addr)
{
    return mmu->wram[addr - 0xC000];
}

uint8_t read_oam(PPU *ppu, uint16_t addr)
{
    // !TODO: tmp, not sure if need updating

    // 0xFE00 ... 0xFE9F
    uint8_t sprite_index = (addr - 0xFE00) >> 2;
    uint8_t byte_index = (addr - 0xFE00) & 0b11;

    switch (byte_index)
    {
    case 0:
        return ppu->sprites[sprite_index].y;
    case 1:
        return ppu->sprites[sprite_index].x;
    case 2:
        return ppu->sprites[sprite_index].index;
    case 3:
        return ppu->sprites[sprite_index].flags;
    }
    panic("read_oam fall through switch cases, should not happen", ERR_INVALID_MEMORY_ACCESS);
}

uint8_t read_io(CPU *cpu, uint16_t addr)
{
    // !TODO: tmp, not sure if need updating
    MMU *mmu = cpu->mmu;
    Timer *timer = cpu->timer;
    PPU *ppu = cpu->ppu;

    switch (addr)
    {
    // TIMER registers
    case 0xFF04: // Divider register
        return timer->divider;
    case 0xFF05: // Timer counter
        return timer->timer_counter;
    case 0xFF06: // Timer modulo
        return timer->timer_modulo;
    case 0xFF07: // Timer control
        return timer->timer_control;
    // LCD registers
    case 0xFF40: // LCD control
        return ppu->lcd_control;
    case 0xFF41: // STAT (LCD status)
        return ppu->lcd_status;
    case 0xFF42: // Background viewport Y
        return ppu->bg_y;
    case 0xFF43: // Background viewport X
        return ppu->bg_x;
    case 0xFF44: // LCD Y coord (read-only)
        return ppu->lcd_y;
    case 0xFF45: // LCD Y compare
        return ppu->lcd_y_cmp;
    case 0xFF47: // BG palette
        return ppu->bg_palette;
    case 0xFF48: // OBJ palette 0
        return ppu->obj_palette_0;
    case 0xFF49: // OBJ palette 1
        return ppu->obj_palette_1;
    case 0xFF4A: // Window pos Y
        return ppu->win_y;
    case 0xFF4B: // Window pos X
        return ppu->win_x;
    default:
        return mmu->io[addr - 0xFF00];
    }
}

uint8_t read_hram(MMU *mmu, uint16_t addr)
{
    // !TODO: tmp, not sure if need updating
    return mmu->hram[addr - 0xFF80];
}

void write_mem(CPU *cpu, uint16_t addr, uint8_t val)
{
    MMU *mmu = cpu->mmu;
    PPU *ppu = cpu->ppu;

    switch (addr)
    {
    case 0x0000 ... 0x7FFF:
        // ROM
        write_rom(mmu, addr, val);
        return;
    case 0x8000 ... 0x9FFF:
        // VRAM
        write_vram(ppu, addr, val);
        return;
    case 0xA000 ... 0xBFFF:
        // RAM (switchable)
        write_sram(mmu, addr, val);
        return;
    case 0xC000 ... 0xDFFF:
        // WRAM
        write_wram(mmu, addr, val);
        return;
    case 0xE000 ... 0xFDFF:
        // Echo WRAM
        // Ninetendo said this is prohibited memory segment
        write_mem(cpu, addr - 0x2000, val);
        return;
    case 0xFE00 ... 0xFE9F:
        // OAM (Object attribute memory)
        write_oam(ppu, addr, val);
        return;
    case 0xFEA0 ... 0xFEFF:
        // Prohibited memory segment
#ifdef UNITY_TEST_RUN
        mmu->prohibit[addr - 0xFEA0] = val;
        return;
#endif
        panic("Attempt to access prohibited memory region", ERR_INVALID_MEMORY_ACCESS);
    case 0xFF00 ... 0xFF7F:
        // Input
        write_io(cpu, addr, val);
        return;
    case 0xFF80 ... 0xFFFE:
        // HRAM
        write_hram(mmu, addr, val);
        return;
    case 0xFFFF:
        // Interrupt enable register
        mmu->interrupt_enable = val;
        return;
    default:
        panic("Attempt to access unused memory region", ERR_INVALID_MEMORY_ACCESS);
    }
}

void write_mem_u16(CPU *cpu, uint16_t addr, uint16_t val)
{
    uint8_t lower_byte = val & 0x00FF;
    uint8_t upper_byte = val >> 8;

    write_mem(cpu, addr, lower_byte);
    __builtin_add_overflow(addr, 1, &addr);
    write_mem(cpu, addr, upper_byte);
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

void write_vram(PPU *ppu, uint16_t addr, uint8_t val)
{
    // !TODO: tmp, not sure if need updating
    ppu->vram[addr - 0x8000] = val;
}

void write_sram(MMU *mmu, uint16_t addr, uint8_t val)
{
    // !TODO: temporary, will need to be expanded with MBCs later
    mmu->sram[addr - 0xA000] = val;
}

void write_wram(MMU *mmu, uint16_t addr, uint8_t val)
{
    mmu->wram[addr - 0xC000] = val;
}

void write_oam(PPU *ppu, uint16_t addr, uint8_t val)
{
    // !TODO: tmp, not sure if need updating

    // 0xFE00 ... 0xFE9F
    uint8_t sprite_index = (addr - 0xFE00) >> 2;
    uint8_t byte_index = (addr - 0xFE00) & 0b11;

    switch (byte_index)
    {
    case 0:
        ppu->sprites[sprite_index].y = val;
        break;
    case 1:
        ppu->sprites[sprite_index].x = val;
        break;
    case 2:
        ppu->sprites[sprite_index].index = val;
        break;
    case 3:
        ppu->sprites[sprite_index].flags = val;
        break;
    }
}

void write_io(CPU *cpu, uint16_t addr, uint8_t val)
{
    // !TODO: tmp, not sure if need updating
    MMU *mmu = cpu->mmu;
    Timer *timer = cpu->timer;
    PPU *ppu = cpu->ppu;

    switch (addr)
    {
    // Serial registers for Blargg's test ROMs
    case 0xFF01:
        mmu->sb = val;
        break;
    case 0xFF02:
        if (val == 0x81) // 0x81 means "Transfer requested"
        {
            LOG_DEBUG("%c", mmu->sb);
        }
        break;
    // TIMER registers
    case 0xFF04: // Divider register
        timer->divider = val;
        break;
    case 0xFF05: // Timer counter
        timer->timer_counter = val;
        break;
    case 0xFF06: // Timer modulo
        timer->timer_modulo = val;
        break;
    case 0xFF07: // Timer control
        timer->timer_control = val;
        break;
    // LCD registers
    case 0xFF40: // LCD control
        ppu->lcd_control = val;
        break;
    case 0xFF41: // STAT (LCD status)
        ppu->lcd_status = val;
        break;
    case 0xFF42: // Background viewport Y
        ppu->bg_y = val;
        break;
    case 0xFF43: // Background viewport X
        ppu->bg_x = val;
        break;
    case 0xFF44: // LCD Y coord (read-only)
        break;
    case 0xFF45: // LCD Y compare
        ppu->lcd_y_cmp = val;
        if (ppu->lcd_y_cmp == ppu->lcd_y)
        {
            // Set compare flag and fire interrupt if allowed
            ppu->lcd_status |= 0b1 << 2;
            if (ppu->lcd_status & (0b1 << 6)) // Check if LY=LYC flag is enabled
            {
                send_stat_interrupt(cpu);
            }
        }
        else
        {
            ppu->lcd_status &= ~(0b1 << 2);
        }
        break;
    case 0xFF47: // BG palette
        ppu->bg_palette = val;
        break;
    case 0xFF48: // OBJ palette 0
        ppu->obj_palette_0 = val;
        break;
    case 0xFF49: // OBJ palette 1
        ppu->obj_palette_1 = val;
        break;
    case 0xFF4A: // Window pos Y
        ppu->win_y = val;
        break;
    case 0xFF4B: // Window pos X
        ppu->win_x = val;
        break;
    default:
        mmu->io[addr - 0xFF00] = val;
    }
}

void write_hram(MMU *mmu, uint16_t addr, uint8_t val)
{
    // !TODO: tmp, not sure if need updating
    mmu->hram[addr - 0xFF80] = val;
}
