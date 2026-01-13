#include "timer.h"

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
    uint8_t mask = 1 << bit_index;

    if (tima_enabled)
    {
        // Check if the bit at the index position turn from 1 -> 0
        if ((old_internal_clock & mask) && !(timer->internal_clock & mask))
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
    uint8_t interrupt_flag = read_mem(cpu->mmu, 0xFF0F);
    write_mem(cpu->mmu, 0xFF0F, interrupt_flag | (1 << 2));
}
