#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "parse.h"
#include "machine.h"

void add32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    uint32_t *o2 = operands[2];
    *d = *o1+ *o2;
    *(d+1) = 0; // Clear upper 32 bits
}

void add64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    uint64_t *o2 = operands[2];
    *d = *o1+ *o2;
}

void sub32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    uint32_t *o2 = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void sub64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    uint64_t *o2 = operands[2];
    // TODO
}

void neg32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void neg64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    // TODO
}

void mul32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    uint32_t *o2 = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void mul64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    uint64_t *o2 = operands[2];
    // TODO
}

void udiv32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    uint32_t *o2 = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void sdiv64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    uint64_t *o2 = operands[2];
    // TODO
}

void lsl32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *r = operands[1];
    uint32_t *v = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void lsl64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *r = operands[1];
    uint64_t *v = operands[2];
    // TODO
}

void lsr32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *r = operands[1];
    uint32_t *v = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void lsr64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *r = operands[1];
    uint64_t *v = operands[2];
    // TODO
}

void and32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    uint32_t *o2 = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void and64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    uint64_t *o2 = operands[2];
    // TODO
}

void orr32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *o1 = operands[1];
    uint32_t *o2 = operands[2];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void orr64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *o1 = operands[1];
    uint64_t *o2 = operands[2];
    // TODO
}

void ldr32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *addr = operands[1];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void ldr64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *addr = operands[1];
    // TODO
}

void str32(uint32_t **operands) {
    uint32_t *s = operands[0];
    uint32_t *addr = operands[1];
    // TODO
}

void str64(uint64_t **operands) {
    uint64_t *s = operands[0];
    uint64_t *addr = operands[1];
    // TODO
}

void mov32(uint32_t **operands) {
    uint32_t *d = operands[0];
    uint32_t *s = operands[1];
    // TODO
    *(d+1) = 0; // Clear upper 32 bits
}

void mov64(uint64_t **operands) {
    uint64_t *d = operands[0];
    uint64_t *s = operands[1];
    // TODO
}

void bl(uint64_t **operands) {
    // TODO
}

void ret(uint64_t **operands) {
    // TODO
}

struct opcode32 opcodes32[] = {
    { "add", add32 },
    { "sub", sub32 },
    { "neg", neg32 },
    { "mul", mul32 },
    { "udiv", udiv32 },
    { "lsl", lsl32 },
    { "lsr", lsr32 },
    { "and", and32 },
    { "orr", orr32 },
    { "ldr", ldr32 },
    { "ldur", ldr32 },
    { "str", str32 },
    { "stur", str32 },
    { "mov", mov32 },
    { NULL, NULL }
};

struct opcode64 opcodes64[] = {
    { "add", add64 },
    { "sub", sub64 },
    { "neg", neg64 },
    { "mul", mul64 },
    { "sdiv", sdiv64 },
    { "lsl", lsl64 },
    { "lsr", lsr64 },
    { "and", and64 },
    { "orr", orr64 },
    { "ldr", ldr64 },
    { "ldur", ldr64 },
    { "str", str64 },
    { "stur", str64 },
    { "mov", mov64 },
    { "bl", bl },
    { "ret", ret },
    { NULL, NULL }
};

void ldp(char **instruction) {
    // Break the third operand into its constituent parts
    char **addr = parse_addr(instruction[3]); 

    // ldp D1, D2, [X0]
    if (addr[1] == NULL && instruction[4] == NULL) {
        printf("\tLoad-only\n");

        // Load the first value
        char first_addr[6];
        sprintf(first_addr, "[%s]", addr[0]);
        char *first[] = { "ldr", instruction[1], first_addr, NULL };
        printf("\t");
        execute(first);

        // Load the second value
        char second_addr[9];
        sprintf(second_addr, "[%s,#8]", addr[0]);
        char *second[] = { "ldr", instruction[2], second_addr, NULL };
        printf("\t");
        execute(second);
    }
    else if (addr[1] != NULL && instruction[4] == NULL) {
        printf("\tUpdate-then-load\n");
        // TODO
    }
    else if (addr[1] == NULL && instruction[4] != NULL) {
        printf("\tLoad-then-update\n");
        // TODO
    }
    else {
        printf("Unhandled %s instruction\n", instruction[0]);
    }
}

void stp(char **instruction) {
    // Break the third operand into its constituent parts
    char **addr = parse_addr(instruction[3]); 

    // stp D1, D2, [X0]
    if (addr[1] == NULL && instruction[4] == NULL) {
        printf("\tStore-only\n");
        // TODO
    }
    else if (addr[1] != NULL && instruction[4] == NULL) {
        printf("\tUpdate-then-store\n");
        // TODO
    }
    else if (addr[1] == NULL && instruction[4] != NULL) {
        printf("\tStore-then-update\n");
        // TODO
    }
    else {
        printf("Unhandled %s instruction\n", instruction[0]);
    }
}

struct opcodeMacro opcodesMacro[] = {
    { "ldp", ldp },
    { "stp", stp },
    { NULL, NULL }
};
