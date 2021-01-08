#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include <stdint.h>

struct opcode32 {
    char *name;
    void (*func)(uint32_t**);
};

struct opcode64 {
    char *name;
    void (*func)(uint64_t**);
};

struct opcodeMacro {
    char *name;
    void (*func)(char**);
};

extern struct opcode32 opcodes32[];
extern struct opcode64 opcodes64[];
extern struct opcodeMacro opcodesMacro[];

#endif // __INSTRUCTIONS_H__