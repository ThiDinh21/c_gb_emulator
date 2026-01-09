#include "unity.h"
#include "opcodes.h"

CPU *cpu;
MMU *mmu;

void setUp()
{
    cpu = init_cpu();
    mmu = cpu->mmu;
    cpu->flags = 0x00;
}

void tearDown()
{
    clean_up_cpu(cpu);
}

void test_op_00_nop(void)
{
    uint8_t cycles = op_00();
    TEST_ASSERT_EQUAL_INT8(4, cycles);
}

void test_op_01_ld_bc_u16(void)
{
    // Inject: LD BC, 0xCAFE (Little Endian)
    write_mem(mmu, 0x0000, 0x01);
    write_mem(mmu, 0x0001, 0xFE);
    write_mem(mmu, 0x0002, 0xCA);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    // Verify Register, PC, and Timing
    TEST_ASSERT_EQUAL_HEX16(0xCAFE, get_bc(cpu));
    TEST_ASSERT_EQUAL_HEX16(0x0003, cpu->program_counter);
    TEST_ASSERT_EQUAL_INT(12, cycles);
}

void test_op_02_ld_bc_a(void)
{
    // Setup: A = 0x55, BC = 0xC000
    cpu->a = 0x55;
    set_bc(cpu, 0xA000);

    // Inject: LD (BC), A
    write_mem(mmu, 0x0000, 0x02);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    // Verify: Memory at 0xC000 matches A
    TEST_ASSERT_EQUAL_HEX8(0x55, read_mem(mmu, 0xA000));
    TEST_ASSERT_EQUAL_HEX16(0x0001, cpu->program_counter);
    TEST_ASSERT_EQUAL_INT(8, cycles);
}

void test_op_03_inc_bc(void)
{
    // Setup: BC = 0xFFFF
    set_bc(cpu, 0xFFFF);

    // Set flags to random, must not change
    cpu->flags = 0xF0;

    // Inject: INC BC
    write_mem(mmu, 0x0000, 0x03);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX16(0x0000, get_bc(cpu));
    TEST_ASSERT_EQUAL_HEX8(0xF0, cpu->flags);
    TEST_ASSERT_EQUAL_HEX16(0x0001, cpu->program_counter);
    TEST_ASSERT_EQUAL_INT(8, cycles);
}

void test_op_04_05_inc_dec_b(void)
{
    // --- INC B (0x04) ---
    // Case: Half Carry (0x0F + 1 = 0x10)
    cpu->b = 0x0F;
    cpu->flags = 0x00;

    write_mem(mmu, 0x0000, 0x04);
    cpu->program_counter = 0x0000;

    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->b);
    TEST_ASSERT_EQUAL_HEX8(0x20, cpu->flags); // H set, N=0
    TEST_ASSERT_EQUAL_INT(4, cycles);

    // --- DEC B (0x05) ---
    // Case: Zero Result (0x01 - 1 = 0x00)
    cpu->b = 0x01;
    cpu->flags = 0x00;

    write_mem(mmu, 0x0000, 0x05);
    cpu->program_counter = 0x0000;

    cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX8(0x00, cpu->b);
    TEST_ASSERT_EQUAL_HEX8(0xC0, cpu->flags); // Z set, N set
    TEST_ASSERT_EQUAL_INT(4, cycles);
}

void test_op_06_ld_b_u8(void)
{
    // Inject: LD B, 0x99
    write_mem(mmu, 0x0000, 0x06);
    write_mem(mmu, 0x0001, 0x99);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX8(0x99, cpu->b);
    TEST_ASSERT_EQUAL_HEX16(0x0002, cpu->program_counter);
    TEST_ASSERT_EQUAL_INT(8, cycles);
}

void test_op_07_rlca(void)
{
    // A = 0x85 (1000 0101)
    // Result -> 0x0B (0000 1011) | Carry = 1
    cpu->a = 0x85;
    cpu->flags = 0x80; // Set Z to verify it gets CLEARED

    write_mem(mmu, 0x0000, 0x07);
    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX8(0x0B, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->flags); // Z=0, C=1
    TEST_ASSERT_EQUAL_INT(4, cycles);
}

// !TODO: enable after impl RAM
/*
void test_op_08_ld_addr_sp(void)
{
    // SP = 0xF00D
    // Address to write to: 0xC000
    cpu->sp = 0xF00D;

    write_mem(mmu, 0x0000, 0x08);
    write_mem(mmu, 0x0001, 0x00); // Low Addr
    write_mem(mmu, 0x0002, 0xC0); // High Addr

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    // Verify RAM gets SP value (Little Endian)
    TEST_ASSERT_EQUAL_HEX8(0x0D, read_mem(mmu, 0xC000));
    TEST_ASSERT_EQUAL_HEX8(0xF0, read_mem(mmu, 0xC001));
    TEST_ASSERT_EQUAL_INT(20, cycles);
}
*/

void test_op_09_add_hl_bc(void)
{
    // HL = 0x0FFF, BC = 0x0001
    // Result = 0x1000 (Bit 11 carry -> H Flag)
    set_hl(cpu, 0x0FFF);
    set_bc(cpu, 0x0001);
    cpu->flags = 0x80; // Z should be preserved

    write_mem(mmu, 0x0000, 0x09);
    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX16(0x1000, get_hl(cpu));
    TEST_ASSERT_EQUAL_HEX8(0xA0, cpu->flags); // Z=1 (Preserved) | H=1 | N=0
    TEST_ASSERT_EQUAL_INT(8, cycles);
}

// !TODO: enable after impl RAM
/*
void test_op_0A_ld_a_bc(void)
{
    // BC -> 0xC000, Mem[0xC000] = 0x77
    set_bc(cpu, 0xC000);
    write_mem(mmu, 0xC000, 0x77);

    write_mem(mmu, 0x0000, 0x0A);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX8(0x77, cpu->a);
    TEST_ASSERT_EQUAL_INT(8, cycles);
}
*/

void test_op_0B_dec_bc(void)
{
    set_bc(cpu, 0x0000);
    cpu->flags = 0xF0; // Ensure flags untouched

    write_mem(mmu, 0x0000, 0x0B);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX16(0xFFFF, get_bc(cpu));
    TEST_ASSERT_EQUAL_HEX8(0xF0, cpu->flags);
    TEST_ASSERT_EQUAL_INT(8, cycles);
}

void test_op_0C_0D_0E_c_ops(void)
{
    // 0x0E: LD C, 0x10
    write_mem(mmu, 0x0000, 0x0E);
    write_mem(mmu, 0x0001, 0x10);
    cpu->program_counter = 0x0000;
    cpu_step(cpu);
    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->c);

    // 0x0C: INC C (0x10 -> 0x11)
    write_mem(mmu, 0x0002, 0x0C);
    // PC is already at 2
    cpu_step(cpu);
    TEST_ASSERT_EQUAL_HEX8(0x11, cpu->c);

    // 0x0D: DEC C (0x11 -> 0x10)
    write_mem(mmu, 0x0003, 0x0D);
    cpu_step(cpu);
    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->c);
}

void test_op_0F_rrca(void)
{
    // A = 0x01 (0000 0001)
    // Result -> 0x80 (1000 0000) | Carry = 1
    cpu->a = 0x01;
    cpu->flags = 0x80;

    write_mem(mmu, 0x0000, 0x0F);

    cpu->program_counter = 0x0000;
    uint8_t cycles = cpu_step(cpu);

    TEST_ASSERT_EQUAL_HEX8(0x80, cpu->a);
    TEST_ASSERT_EQUAL_HEX8(0x10, cpu->flags); // Z=0, C=1
    TEST_ASSERT_EQUAL_INT(4, cycles);
}