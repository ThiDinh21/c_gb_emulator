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
        // Prohibited memory segment — reads return 0xFF on real hardware
        return 0xFF;
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

// Returns the effective ROM bank number for the 0x4000-0x7FFF window.
// MBC1: banks 0x00/0x20/0x40/0x60 are not directly selectable — hardware bumps them to +1.
static uint8_t mbc1_effective_rom_bank(const MMU *mmu)
{
    uint8_t bank = ((mmu->mbc1_upper & 0x03) << 5) | (mmu->rom_bank & 0x1F);
    if ((bank & 0x1F) == 0)
    {
        bank++;
    }
    if (mmu->num_rom_banks > 0)
    {
        bank &= (mmu->num_rom_banks - 1);
    }
    return bank;
}

uint8_t read_rom(MMU *mmu, uint16_t addr)
{
    if (addr < 0x4000)
    {
        // ROM bank 0 window
        // MBC1 RAM banking mode: upper 2 bits select which 16KB group
        uint32_t base = 0;
        if (mmu->mbc_type >= 0x01 && mmu->mbc_type <= 0x03 && mmu->mbc1_mode == 1)
        {
            base = (uint32_t)(mmu->mbc1_upper & 0x03) << 19; // << 5 banks, each 16KB
            if (mmu->num_rom_banks > 0)
            {
                base &= (uint32_t)(mmu->num_rom_banks - 1) << 14;
            }
        }
        return mmu->rom[base + addr];
    }
    else
    {
        // Switchable ROM bank window (0x4000-0x7FFF)
        uint32_t bank_offset;
        if (mmu->mbc_type == 0x00)
        {
            bank_offset = 0x4000; // ROM only: always bank 1
        }
        else
        {
            bank_offset = (uint32_t)mbc1_effective_rom_bank(mmu) << 14;
        }
        return mmu->rom[bank_offset + (addr - 0x4000)];
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
    // TODO: Block reads during PPU mode 3 (returns 0xFF on real hardware)
    return ppu->vram[addr - 0x8000];
}

uint8_t read_sram(MMU *mmu, uint16_t addr)
{
    if (!mmu->cart_ram || !mmu->ram_enabled)
    {
        return 0xFF;
    }
    uint8_t bank = (mmu->mbc_type >= 0x01 && mmu->mbc_type <= 0x03 && mmu->mbc1_mode == 1)
                       ? (mmu->mbc1_upper & 0x03)
                       : 0;
    uint32_t phys = ((uint32_t)bank << 13) + (addr - 0xA000);
    if (phys >= mmu->cart_ram_size)
    {
        return 0xFF;
    }
    return mmu->cart_ram[phys];
}

uint8_t read_wram(MMU *mmu, uint16_t addr)
{
    return mmu->wram[addr - 0xC000];
}

uint8_t read_oam(PPU *ppu, uint16_t addr)
{
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
    case 0xFF00: // Joypad
    {
        uint8_t sel = mmu->io[0] & 0x30;
        if (!(sel & 0x10))
            return 0xC0 | 0x10 | (mmu->joypad_dpad & 0x0F);
        if (!(sel & 0x20))
            return 0xC0 | 0x20 | (mmu->joypad_action & 0x0F);
        return 0xCF; // nothing selected
    }
    case 0xFF0F: // Interrupt flag register (upper 3 bits always read as 1)
        return mmu->io[0x0F] | 0xE0;
    default:
        return 0xFF; // Open bus - unimplemented IO reads as 0xFF
    }
}

uint8_t read_hram(MMU *mmu, uint16_t addr)
{
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
#ifdef UNITY_TEST_RUN
        mmu->prohibit[addr - 0xFEA0] = val;
        return;
#endif
        // Prohibited memory segment — writes are ignored on real hardware
        return;
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
    // ROM-only: ignore all writes
    if (mmu->mbc_type == 0x00)
    {
        return;
    }

    // MBC1 register writes
    if (mmu->mbc_type >= 0x01 && mmu->mbc_type <= 0x03)
    {
        if (addr <= 0x1FFF)
        {
            // RAM enable: lower nibble 0x0A enables, anything else disables
            mmu->ram_enabled = ((val & 0x0F) == 0x0A) ? 1 : 0;
        }
        else if (addr <= 0x3FFF)
        {
            // ROM bank number (lower 5 bits); writing 0 is treated as 1
            uint8_t bank = val & 0x1F;
            mmu->rom_bank = (bank == 0) ? 1 : bank;
        }
        else if (addr <= 0x5FFF)
        {
            // 2-bit upper register: selects RAM bank or upper ROM bank bits
            mmu->mbc1_upper = val & 0x03;
        }
        else
        {
            // Banking mode select: 0=ROM banking mode, 1=RAM banking mode
            mmu->mbc1_mode = val & 0x01;
        }
    }
}

void write_vram(PPU *ppu, uint16_t addr, uint8_t val)
{
    // TODO: Block writes during PPU mode 3 (ignored on real hardware)
    ppu->vram[addr - 0x8000] = val;
}

void write_sram(MMU *mmu, uint16_t addr, uint8_t val)
{
    if (!mmu->cart_ram || !mmu->ram_enabled)
    {
        return;
    }
    uint8_t bank = (mmu->mbc_type >= 0x01 && mmu->mbc_type <= 0x03 && mmu->mbc1_mode == 1)
                       ? (mmu->mbc1_upper & 0x03)
                       : 0;
    uint32_t phys = ((uint32_t)bank << 13) + (addr - 0xA000);
    if (phys >= mmu->cart_ram_size)
    {
        return;
    }
    mmu->cart_ram[phys] = val;
}

void write_wram(MMU *mmu, uint16_t addr, uint8_t val)
{
    mmu->wram[addr - 0xC000] = val;
}

void write_oam(PPU *ppu, uint16_t addr, uint8_t val)
{
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
    MMU *mmu = cpu->mmu;
    Timer *timer = cpu->timer;
    PPU *ppu = cpu->ppu;

    switch (addr)
    {
    case 0xFF00: // Joypad
        mmu->io[0] = val;
        break;
    case 0xFF01: // Serial registers for Blargg's test ROMs
        mmu->sb = val;
        break;
    case 0xFF02:
        if (val == 0x81) // 0x81 means transfer requested
        {
            LOG_DEBUG("%c", mmu->sb);
        }
        break;
    // TIMER registers
    case 0xFF04: // Divider register - writing any value resets the internal clock
        timer->divider = 0;
        timer->internal_clock = 0;
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
    case 0xFF46: // OAM DMA transfer
    {
        // Copy 160 bytes from (val << 8) into OAM
        uint16_t src = (uint16_t)val << 8;
        for (uint8_t i = 0; i < 160; i++)
        {
            write_oam(ppu, 0xFE00 + i, read_mem(cpu, src + i));
        }
        break;
    }
    case 0xFF0F: // Interrupt flag register
        mmu->io[addr - 0xFF00] = val;
        break;
    default:
        mmu->io[addr - 0xFF00] = val;
    }
}

void write_hram(MMU *mmu, uint16_t addr, uint8_t val)
{
    mmu->hram[addr - 0xFF80] = val;
}
