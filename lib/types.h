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

// https://gbdev.io/pandocs/OAM.html
// Byte 3:
//                     7         6       5            4    3          2 1 0
// Attributes	Priority	Y flip	X flip	DMG palette	Bank	CGB palette
typedef struct
{
    uint8_t x;     // Byte 1
    uint8_t y;     // Byte 0
    uint8_t index; // Byte 2
    uint8_t flags; // Byte 3
} Sprite;          // OAM

typedef struct
{
    uint8_t rom_0[0x4000];
    uint8_t rom_n[0x4000]; // !TODO: a switch bank, might need to make bigger
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
    uint8_t sb; // Serial transfer data (SB) register, for Blargg's test ROMs
} MMU;

typedef struct
{
    uint16_t internal_clock;
    uint8_t divider;
    uint8_t timer_counter;
    uint8_t timer_modulo;
    uint8_t timer_control;
} Timer;

// https://gbdev.io/pandocs/LCDC.html#ff40--lcdc-lcd-control
// https://gbdev.io/pandocs/STAT.html
// https://gbdev.io/pandocs/Scrolling.html#lcd-position-and-scrolling
// https://gbdev.io/pandocs/Palettes.html
typedef struct
{
    Sprite sprites[40];      // OAM
    uint8_t vram[0x2000];    // VRAM: 0x8000..0x9FFF
    uint8_t lcd_control;     // 0xFF40
    uint8_t lcd_y;           // 0xFF44
    uint8_t lcd_y_cmp;       // 0xFF45
    uint8_t lcd_status;      // 0xFF41
    uint8_t bg_x;            // 0xFF42
    uint8_t bg_y;            // 0xFF43
    uint8_t win_x;           // 0xFF4B
    uint8_t win_y;           // 0xFF4A
    uint8_t bg_palette;      // 0xFF47
    uint8_t obj_palette_0;   // 0xFF48
    uint8_t obj_palette_1;   // 0xFF49
    uint16_t internal_cycle; // Keep track of which cycle the PPU at to sync with CPU
    // TODO: reset each frame
    uint8_t window_internal_line; // Keep track of how much has the window been rendered, reset each frame
    uint8_t mode_3_length;        // Track how many cycles PPU needs during mode 3 for current line, altho keep at 172 is good enough
} PPU;

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
    PPU *ppu;
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