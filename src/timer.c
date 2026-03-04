#include <stdlib.h>
#include "timer.h"
#include "mmu.h"

Timer *init_timer(void)
{
    Timer *timer = (Timer *)calloc(1, sizeof(Timer));
    return timer;
}

void clean_up_timer(Timer *timer)
{
    free(timer);
}

void timer_handling(CPU *cpu, uint8_t cycles)
{
    Timer *timer = cpu->timer;

    // Set divider
    uint16_t old_internal_clock = timer->internal_clock;
    timer->internal_clock += cycles;
    timer->divider = timer->internal_clock >> 8;

    // Timer counter increment
    uint8_t tima_enabled = (timer->timer_control >> 2) & 0b1; // Check bit 2 of TAC
    uint8_t tac_table_index = timer->timer_control & 0b11;    // Get the first 2 bit of timer control
    uint8_t bit_index = TIMER_CTRL_TABLE[tac_table_index];    // Get the index of the bit that tracks the freq of TIMA

    if (tima_enabled)
    {
        // Count falling edges of bit_index between old and new internal_clock.
        // A falling edge occurs at every multiple of 2^(bit_index+1).
        // Use linear arithmetic to handle uint16_t wrap-around.
        uint16_t clock_diff = (uint16_t)(timer->internal_clock - old_internal_clock);
        uint32_t linear_old = (uint32_t)old_internal_clock;
        uint32_t linear_new = linear_old + clock_diff;
        uint32_t falling_edges = (linear_new >> (bit_index + 1)) - (linear_old >> (bit_index + 1));

        for (uint32_t i = 0; i < falling_edges; i++)
        {
            uint8_t overflow = __builtin_add_overflow(timer->timer_counter, 1, &timer->timer_counter);
            if (overflow)
            {
                send_timer_interrupt(cpu);
                timer->timer_counter = timer->timer_modulo;
            }
        }
    }
}

void send_timer_interrupt(CPU *cpu)
{
    uint8_t interrupt_flag = read_mem(cpu, 0xFF0F);
    write_mem(cpu, 0xFF0F, interrupt_flag | (1 << 2));
}
