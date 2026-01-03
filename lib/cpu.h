#ifndef CPU_H
#define CPU_H

#include <stdlib.h>
#include <stdint.h>
#include "mmu.h"

typedef enum
{
    Z_FLAG = 1 << 7,
    N_FLAG = 1 << 6,
    H_FLAG = 1 << 5,
    C_FLAG = 1 << 4,
} FlagRegister;

typedef struct
{
    uint16_t sp;
    uint16_t pc;
    FlagRegister flags;
    uint8_t a, b, c, d, e, h, l;
    MMU *mmu;
} CPU;

#endif