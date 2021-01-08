#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <stdint.h>

#define WORD_SIZE_BYTES 8
#define WORD_SIZE_BITS (WORD_SIZE_BYTES * 8)
#define HALFWORD_SIZE_BITS (WORD_SIZE_BITS / 2)

enum registers { x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, sp, pc, xzr };
struct system {
    void *stack;
    uint64_t stack_top;
    uint64_t stack_bot;
    uint64_t registers[34];
    unsigned char used[34];
    uint64_t constant;
    uint64_t code_start;
    char **code;
};

extern struct system machine;

void init_machine(uint64_t stack_top, uint64_t pc_start, char *code_path);
void grow_stack(uint64_t addr);
void print_machine();
void execute(char **instruction);

#endif // __MACHINE_H__