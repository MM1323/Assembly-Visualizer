#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "machine.h"

bool ok = true;

#define XTEST(expr, errmsg) \
  if (!expr) { \
    printf("FAILURE: %s (line %d of %s)\n", errmsg, __LINE__, __FILE__); \
    ok = false; \
  } \

int main(int argc, char **argv) {
    // Execute sub64 instruction
    init_machine(0xFF00, 0, NULL);
    machine.registers[x2] = 0x1122334455;
    machine.registers[x3] = 0x22004400;
    char *sub[] = {"sub", "x1", "x2", "x3", NULL};
    execute(sub);
    print_machine();
    XTEST((machine.registers[x1] == 0x1100330055), "sub instruction should store sum of operands in destination register");
    XTEST((machine.registers[x2] == 0x1122334455), "sub instruction should not change first operand");
    XTEST((machine.registers[x3] == 0x22004400), "sub instruction should not change second operand");

    // Execute mul32 instruction
    init_machine(0xFF00, 0, NULL);
    machine.registers[x4] = 0xFFFFFFFFFFFFFFFF;
    machine.registers[x5] = 0x10;
    machine.registers[x6] = 0xC;
    char *mul[] = {"mul", "x4", "x5", "x6", NULL};
    execute(mul);
    print_machine();
    XTEST((machine.registers[x4] == 0xC0), "mul instruction should store product of operands in destination register");
    XTEST((machine.registers[x5] == 0x10), "mul instruction should not change first operand");
    XTEST((machine.registers[x6] == 0xC), "mul instruction should not change second operand");

    // Execute ldr64 instruction
    init_machine(0xFF00, 0, NULL);
    uint64_t *mem64 = machine.stack;
    *mem64 = 0x1122334455;
    char *ldr[] = {"ldr", "x7", "[sp]", NULL};
    execute(ldr);
    print_machine();
    XTEST((machine.registers[x7] == 0x1122334455), "ldr instruction should store value from stack in destination register");
    XTEST((*mem64 == 0x1122334455), "ldr instruction should not change stack");

    // Execute str32 instruction
    init_machine(0xFF00, 0, NULL);
    uint32_t *mem32 = machine.stack;
    machine.registers[x8] = 0x667788;
    char *str[] = {"str", "w8", "[sp,#4]", NULL};
    execute(str);
    print_machine();
    XTEST((*(mem32+1) == 0x667788), "str instruction should store value from source reigsters in stack");
    XTEST((machine.registers[x8] == 0x667788), "str instruction should not change register");

    printf("Tests complete.\n");
    if (!ok) {
      printf("At least one test failed.\n");
    } else {
      printf("All tests passed.\n");
    }
    return 0;
}