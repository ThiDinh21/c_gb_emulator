#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "cpu.h"
#include "alu.h"
#include "logging.h"

int main(void)
{
    CPU *cpu = init_cpu();

    while (true)
    {
        cpu_step(cpu);
    }

    clean_up_cpu(cpu);
    return 0;
}