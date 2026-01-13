#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
///
/// 7     bit     0
/// ----       ----
/// Z N H C 0 0 0 0
/// | | | | | | | |
/// | | | | | | | |
/// | | | | | | | |
/// | | | | +-+-+-+- Always 0
/// | | | +--------- Carry flag
/// | | + ---------- Half Carry flag (BCD)
/// | +------------- Substraction flag (BCD)
/// +--------------- Zero flag
typedef enum
{
    Z_FLAG = 1 << 7,
    N_FLAG = 1 << 6,
    H_FLAG = 1 << 5,
    C_FLAG = 1 << 4,
} FlagRegister;

typedef enum
{
    INTERRUPT_VBLANK = 0x40,
    INTERRUPT_STAT = 0x48,
    INTERRUPT_TIMER = 0x50,
    INTERRUPT_SERIAL = 0x58,
    INTERRUPT_JOYPAD = 0x60,
} Interrupt;

typedef struct
{
    // uint8_t memory[65536];
    uint8_t rom_0[0x4000];
    uint8_t rom_n[0x4000]; // !TODO: a switch bank, might need to make bigger
    // size_t rom_index;
    uint8_t vram[0x2000];
    uint8_t sram[0x2000];
    uint8_t wram[0x2000];
    // size_t wram_index;
    uint8_t oam[0xA0];
#ifdef UNITY_TEST_RUN
    uint8_t prohibit[0xA0];
#endif
    uint8_t io[0x80];
    uint8_t hram[0x7F];
    uint8_t interrupt_enable;
} MMU;

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

typedef enum
{
    ERR_NONE = 0,
    ERR_INVALID_MEMORY_ACCESS,
    ERR_ROM_LOAD_FAILURE,
    ERR_UNKNOWN_INSTRUCTION,
    ERR_UNIMPLEMENTED,
} ErrorCode;

#endif