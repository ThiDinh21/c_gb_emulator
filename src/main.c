#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "cpu.h"
#include "alu.h"

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

    // ALU ADD tests
    cpu->a = 0x0f;
    alu_add(cpu, 0x00, true);
    printf("\nA after alu add: %x\n", cpu->a);
    printf("Flags: %x\n", cpu->flags);

    // ALU SUB tests
    cpu->a = 0x10;
    alu_sub(cpu, 0x0F, true);
    printf("\nA after alu sub: %x\n", cpu->a);
    printf("Flags: %x\n\n\n", cpu->flags);

    // ALU INC/DEC r8 tests
    cpu->flags = 0x10;
    cpu->b = 0x02;
    alu_dec(cpu, &(cpu->b));
    printf("B: %x    Flag: %x\n", cpu->b, cpu->flags);
    cpu->c = 0x01;
    alu_dec(cpu, &(cpu->c));
    printf("C: %x    C_Flag: %x\n", cpu->c, cpu->flags);
    cpu->h = 0x00;
    alu_dec(cpu, &(cpu->h));
    printf("H: %x    C_Flag: %x\n", cpu->h, cpu->flags);

    clean_up_cpu(cpu);
    return 0;
}