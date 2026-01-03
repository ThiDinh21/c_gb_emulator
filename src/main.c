#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "cpu.h"

int main(void)
{
    CPU *cpu = init_cpu();
    MMU *mmu = cpu->mmu;

    write_mem(mmu, 0x1000, 69);
    printf("0x1000: %d\n", read_mem(mmu, 0x1000));
    write_mem(mmu, 0xA060, 90);
    printf("0x1000: %d\n", read_mem(mmu, 0xA060));

    // unimplemented
    // write_mem(mmu, 0xFF00, 0);

    // invalid mem access
    // write_mem(mmu, 0xE010, 90);

    printf("CARRY FLAG should not be 0: %d\n", 0b00010000 & C_FLAG);
    printf("ZERO FLAG should be 0: %d\n", 0b01101111 & Z_FLAG);

    // 16 bit regs test
    set_af(cpu, 0xABCD);
    set_bc(cpu, 0x87B0);
    printf("AF: %x\nA :%x\nF: %x\n", get_af(cpu), cpu->a, cpu->flags);
    printf("BC: %x\nB :%x\nC: %x\n", get_bc(cpu), cpu->b, cpu->c);

    clean_up_cpu(cpu);
    return 0;
}