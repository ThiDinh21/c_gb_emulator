#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"

int main(void)
{
    MMU *mmu = (MMU *)calloc(sizeof(MMU), 1);

    write_mem(mmu, 0x1000, 69);
    printf("0x1000: %d\n", read_mem(mmu, 0x1000));
    write_mem(mmu, 0xA060, 90);
    printf("0x1000: %d\n", read_mem(mmu, 0xA060));

    // unimplemented
    write_mem(mmu, 0xFF00, 0);

    // invalid mem access
    write_mem(mmu, 0xE010, 90);

    return 0;
}