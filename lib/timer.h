#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "types.h"

/// Timer control table can convert to {1024, 16, 64, 256} T-cycles.
/// Which is actually tracking when bit 9, 3, 5 and 7 respectively of the internal clock is cleared.
///
/// ref: https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
static const uint16_t TIMER_CTRL_TABLE[4] = {9, 3, 5, 7};

/// Allocates and initializes a Timer struct with default values.
/// Returns a pointer to the newly created Timer.
Timer *init_timer(void);

/// Frees all memory associated with the given Timer.
void clean_up_timer(Timer *timer);

/// Advances the timer and divider by the given number of CPU cycles.
/// Handles TIMA overflow (resets to TMA and requests interrupt),
/// and detects falling edges on the internal clock to increment TIMA.
///
/// ref: https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
void timer_handling(CPU *cpu, uint8_t cycles);

/// Sets the Timer interrupt flag (Bit 2) in the CPU's Interrupt Flag (IF) register.
///
/// ref: https://gbdev.io/pandocs/Interrupts.html#ff0f--if-interrupt-flag
void send_timer_interrupt(CPU *cpu);

#endif