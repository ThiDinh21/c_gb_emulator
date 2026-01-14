#include "ppu.h"

// void ppu_render(PPU *ppu, uint8_t cycles)
// {
//     // PPU MODE 1: Vertical Blank
//     if (ppu->lcd_y > 143)
//     {
//     }

//     // PPU Mode 2: Search for sprites on each line
//     Sprite sprites_to_render[40];
//     uint8_t sprites_count = 0;

//     for (uint8_t i = 0; i < 40; i++)
//     {
//         if (ppu->sprites[i].y == line)
//         {
//             sprites_to_render[sprites_count] = ppu->sprites[i];
//             sprites_count++;
//         }
//     }

//     // PPU Mode 3
//     // Draw pixels
// }
