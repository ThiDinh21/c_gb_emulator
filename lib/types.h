#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/// Bitmask enum for CPU's Status flags (F flag) registers.
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
/// | +------------- Subtraction flag (BCD)
/// +--------------- Zero flag
///
/// ref: https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
typedef enum
{
    Z_FLAG = 1 << 7,
    N_FLAG = 1 << 6,
    H_FLAG = 1 << 5,
    C_FLAG = 1 << 4,
} FlagRegister;

/// Interrupt sources.
///
/// Each is the address that the PC will jump to when an interrupt fires.
///
/// ref: https://gbdev.io/pandocs/Interrupt_Sources.html
typedef enum
{
    INTERRUPT_VBLANK = 0x40,
    INTERRUPT_STAT = 0x48,
    INTERRUPT_TIMER = 0x50,
    INTERRUPT_SERIAL = 0x58,
    INTERRUPT_JOYPAD = 0x60,
} Interrupt;

/// Represents sprite information or Object Attribute Memory (OAM).
///
/// First 2 bytes are the position of the sprite on screen.
/// Byte 2 specifies the sprite's tile index.
/// Byte 3:
///                    7         6       5            4    3          2 1 0
/// Attributes	Priority	Y flip	X flip	DMG palette	Bank	CGB palette
///
/// ref: https://gbdev.io/pandocs/OAM.html
typedef struct
{
    uint8_t y;     // Byte 0
    uint8_t x;     // Byte 1
    uint8_t index; // Byte 2
    uint8_t flags; // Byte 3
} Sprite;          // OAM

/// Represents 3 parts:
/// - The Memory Management Unit (MMU) of the Game Boy.
/// - The Memory Bank Controller (MBC) of the Game Boy.
/// - Cartridge's data.
///
/// Not all of the memory map is in the MMU, although some graphic related regions
/// (OAM and VRAM) are stored in PPU instead.
///
/// ref:
/// - MMU: https://gbdev.io/pandocs/Memory_Map.html
/// - MBC: https://gbdev.io/pandocs/MBCs.html
/// - Cartridge: https://gbdev.io/pandocs/The_Cartridge_Header.html
typedef struct
{
    // Cartridge data
    uint8_t *rom;
    uint8_t *cart_ram;      // NULL if cartridge has no RAM
    uint32_t rom_size;      // total ROM size in bytes
    uint32_t cart_ram_size; // total cart RAM size in bytes (0 if none)
    uint32_t num_rom_banks; // rom_size / 0x4000
    char *sav_path;         // Path to .sav file derived from ROM path (NULL if no cart RAM)

    // MBC state
    uint8_t mbc_type;    // cartridge type from header byte 0x0147
    uint8_t rom_bank;    // selected ROM bank for 0x4000-0x7FFF (init: 1)
    uint8_t ram_bank;    // selected RAM bank for 0xA000-0xBFFF (init: 0)
    uint8_t ram_enabled; // whether cart RAM is accessible (init: 0)
    uint8_t mbc1_mode;   // MBC1: 0=ROM banking mode, 1=RAM banking mode
    uint8_t mbc1_upper;  // MBC1: 2-bit upper register (written via 0x4000-0x5FFF)

    // MMU memory regions
    uint8_t wram[0x2000]; // 0xC000..0xDFFF
#ifdef UNITY_TEST_RUN
    uint8_t prohibit[0xA0]; // mirrors OAM; tracks prohibited access regions for Unity tests
#endif
    uint8_t io[0x80];   // 0xFF00..0xFF7F
    uint8_t hram[0x7F]; // 0xFF80..0xFFFE
    // Control whether each type of interrupt is enabled, stored at 0xFFFF.
    // ref: https://gbdev.io/pandocs/Interrupts.html#ffff--ie-interrupt-enable
    uint8_t interrupt_enable;
    uint8_t sb; // Serial transfer data (SB) register, for Blargg's test ROMs

    // Joypad state (0xFF00) is stored separately from io registers since it is calculated dynamically.
    // Bits 0-3 only; 0=pressed, 1=released
    // ref: https://gbdev.io/pandocs/Joypad_Input.html
    uint8_t joypad_dpad;   // bit3=Down, bit2=Up, bit1=Left, bit0=Right
    uint8_t joypad_action; // bit3=Start, bit2=Select, bit1=B, bit0=A
} MMU;

/// The internal timer and divider of the CPU.
///
/// An internal clock increments after every execution of an opcode, by how many T-cycles that execution took.
/// timer_counter will increment every time a certain bit of the internal clock transitions from 1 to 0 (falling edge),
/// which bit is decided by the frequency set by timer_control.
/// A timer interrupt will be fired everytime the timer_counter overflows.
///
/// ref: https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
typedef struct
{
    uint16_t internal_clock; // increments after opcode execution by that opcode's T-cycle count
    uint8_t divider;         // DIV register (0xFF04): upper 8 bits of internal_clock
    uint8_t timer_counter;   // TIMA register (0xFF05): increments by the frequency set by timer_control
    uint8_t timer_modulo;    // TMA register (0xFF06): value loaded into timer_counter when overflow
    uint8_t timer_control;   // TAC register (0xFF07): control timer enable and frequency
} Timer;

/// Picture Processing Unit of the Game Boy.
///
/// Stores the graphic-related memory regions (OAM/sprite, VRAM), as well as graphic registers.
///
/// ref: https://gbdev.io/pandocs/Graphics.html
typedef struct
{
    Sprite sprites[40];   // OAM: 0xFE00..0xFE9F
    uint8_t vram[0x2000]; // VRAM: 0x8000..0x9FFF

    // LCD registers: https://gbdev.io/pandocs/Scrolling.html
    uint8_t lcd_control; // 0xFF40: https://gbdev.io/pandocs/LCDC.html#ff40--lcdc-lcd-control
    uint8_t lcd_y;       // 0xFF44: current scan line
    uint8_t lcd_y_cmp;   // 0xFF45: fires STAT interrupt when lcd_y reaches this value
    uint8_t lcd_status;  // 0xFF41: https://gbdev.io/pandocs/STAT.html
    uint8_t bg_x;        // 0xFF42
    uint8_t bg_y;        // 0xFF43
    uint8_t win_x;       // 0xFF4B
    uint8_t win_y;       // 0xFF4A

    // Palette registers: https://gbdev.io/pandocs/Palettes.html
    uint8_t bg_palette;    // 0xFF47
    uint8_t obj_palette_0; // 0xFF48
    uint8_t obj_palette_1; // 0xFF49

    uint16_t internal_cycle;        // Keep track of which cycle the PPU is at to sync with CPU
    uint8_t window_internal_line;   // Keep track of how much has the window been rendered, reset each frame
    uint8_t mode_3_length;          // Track how many cycles PPU needs during mode 3 for current line, keep at 172 is good enough
    uint8_t test_line_buffer[160];  // Testing purposes
    uint8_t framebuffer[144 * 160]; // Full frame: shade 0-3 per pixel, row-major
    uint8_t frame_ready;            // Set to 1 on VBlank start, cleared by renderer
} PPU;

/// Central Processing Unit of the Game Boy.
///
/// Stores CPU registers as well as pointers to other major parts of the Game Boy.
///
/// ref: https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
typedef struct
{
    uint16_t sp;                 // Stack pointer
    uint16_t program_counter;    // Current instruction addr
    uint8_t flags;               // CPU Status flags register
    uint8_t ime;                 // Interrupt Master Enable flag: if 0, disable all interrupt handlers, regardless of IE
    uint8_t ime_pending;         // EI opcode enables ime after 1 instruction delay; this flag tracks the pending stage
    uint8_t a, b, c, d, e, h, l; // CPU registers
    uint8_t halt;                // Set if CPU is in HALT mode, resumes on any interrupts
    // All 3 are owned externally
    MMU *mmu;
    Timer *timer;
    PPU *ppu;
} CPU;

/// Error codes returned by panic() and panic_unimplemented().
typedef enum
{
    ERR_NONE = 0,
    ERR_INVALID_MEMORY_ACCESS,
    ERR_ROM_LOAD_FAILURE,
    ERR_UNKNOWN_INSTRUCTION,
    ERR_UNIMPLEMENTED,
} ErrorCode;

#endif