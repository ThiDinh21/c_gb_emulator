#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "types.h"

// https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
// Timer control table can convert to {1024, 16, 64, 256} T-cycles
// Which is actually tracking when the 9th, 3rd, 5th and 7th respectively of the internal clock is cleared
static const uint16_t TIMER_CTRL_TABLE[4] = {9, 3, 5, 7};

Timer *init_timer(void);
void clean_up_timer(Timer *timer);

void timer_handling(CPU *cpu, uint8_t cycles);
void send_timer_interrupt(CPU *cpu);

#endif