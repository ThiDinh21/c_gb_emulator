#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "timer.h"
#include "logging.h"

int main(void)
{
    CPU *cpu = init_cpu();

    while (true)
    {
        uint8_t cycles = cpu_step(cpu);
        ppu_step(cpu, cycles);
        timer_handling(cpu, cycles);
    }

    clean_up_cpu(cpu);
    return 0;
}