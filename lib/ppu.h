#ifndef PPU_H
#define PPU_H

#include "types.h"

Sprite bytes_to_sprite(uint8_t bytes[4]);

void sprite_to_bytes(Sprite oam, uint8_t buffer[4]);

#endif