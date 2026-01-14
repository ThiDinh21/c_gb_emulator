#include "ppu.h"
#include "errors.h"

// https://gbdev.io/pandocs/Rendering.html
void ppu_step(CPU *cpu, uint8_t cycles)
{
    PPU *ppu = cpu->ppu;
    uint16_t old_internal_cycle = ppu->internal_cycle;
    uint8_t curr_ppu_mode = ppu->lcd_status & 0b11;
    uint8_t stat_interrupt_condition = 0;
    ppu->internal_cycle = (ppu->internal_cycle + cycles) % 456; // each line take 456 T-cycles to render

    // Jump to new LCD line
    if (old_internal_cycle > ppu->internal_cycle)
    {
        ppu->lcd_y++;
        ppu->lcd_y %= 154; // Line is from 0..153
        ppu->mode_3_length = 0;
        // Update status reg bit 2
        if (ppu->lcd_y == ppu->lcd_y_cmp)
        {
            ppu->lcd_status |= 0b1 << 2;
            stat_interrupt_condition |= ppu->lcd_status & (0b1 << 6); // Check if LY=LYC flag is enabled
        }
        else
        {
            ppu->lcd_status &= ~(0b1 << 2);
        }
    }

    // Enters PPU MODE 1: Vertical Blank
    // from line 144 to 153
    if (ppu->lcd_y > 143)
    {
        if (curr_ppu_mode != 0b01)
        {
            send_vblank_interrupt(cpu);
            stat_interrupt_condition |= ppu->lcd_status & (0b1 << 4); // Check if Mode 1 flag is enabled
            // Set mode to 1
            ppu->lcd_status &= ~0b10; // Set bit 1 to 0
            ppu->lcd_status |= 0b1;   // Set bit 0 to 1
        }
    }

    // Enters PPU Mode 2: OAM scan
    // Emu doesn't need to scan
    else if (ppu->internal_cycle < 80 && curr_ppu_mode != 0b10)
    {
        stat_interrupt_condition |= ppu->lcd_status & (0b1 << 5); // Check if Mode 2 flag is enabled
        // Set mode to 2
        ppu->lcd_status &= ~0b1;   // Set bit 0 to 0
        ppu->lcd_status |= 1 << 1; // Set bit 1 to 1
    }

    // Enters PPU Mode 3: Render to LCD (we render whole line at the end of this mode)
    // Check if just switch from mode 2 to 3
    // If true calculate the length of mode 3 and switch to mode 3
    else if (ppu->internal_cycle >= 80 && curr_ppu_mode == 0b10)
    {
        ppu->mode_3_length = get_mode_3_length(ppu);
        ppu->lcd_status |= 0b11; // Set bit 0 and 1 to 1
    }

    // PPU Mode 0: Horizontal Blank
    else if (ppu->internal_cycle >= 80 + ppu->mode_3_length && curr_ppu_mode == 0b11)
    {
        stat_interrupt_condition |= ppu->lcd_status & (0b1 << 3); // Check if Mode 0 flag is enabled
        ppu_render_line(ppu);                                     // After mode 3 we render whole line
        ppu->lcd_status &= ~0b11;                                 // Set bit 0 and 1 to 0
    }

    if (stat_interrupt_condition)
    {
        send_stat_interrupt(cpu);
    }
}

void ppu_render_line(PPU *ppu)
{
    panic_unimplemented("PPU render");
    ppu->mode_3_length = 172;
}

// Get the length of mode 3 before rendering
// Formula = 160 (line length) + 12 (2 tile fetchers) + penalty
// with penalty = SCX % 8 + 6 () + penalty_for_each_obj
uint8_t get_mode_3_length(PPU *ppu)
{
    return 172; // Return the minimum value is probably good enough
    // TODO: impl exhaustive mode 3 length calculation
    ppu->mode_3_length = 172;
}

void send_vblank_interrupt(CPU *cpu)
{
    panic_unimplemented("send_vblank_int");
    cpu->a = 0;
}

void send_stat_interrupt(CPU *cpu)
{
    panic_unimplemented("STAT interrupt");
    cpu->a = 0;
}
