#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "mmu.h"

typedef struct
{
    uint16_t internal_clock;
    uint8_t divider;
    uint8_t timer_counter;
    uint8_t timer_modulo;
    uint8_t timer_control;
} Timer;

typedef struct
{
    uint16_t sp;
    uint16_t program_counter;
    uint8_t flags;
    uint8_t ime;
    uint8_t ime_pending; // Since EI delay enabling ime by 1 operation
    uint8_t a, b, c, d, e, h, l;
    uint8_t halt;
    MMU *mmu;
    Timer *timer;
} CPU;

#endif