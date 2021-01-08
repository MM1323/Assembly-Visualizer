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
    // Execute bl instruction
    init_machine(0xFF00, 0x400468, NULL);
    char *bl[] = {"bl", "400ABC", "<test>", NULL};
    execute(bl);
    print_machine();
    XTEST((machine.registers[pc] == 0x400ABC), "bl instruction should store address in pc register");
    XTEST((machine.registers[x30] == 0x400468), "bl instruction should store resume address in x30");

    // Execute stp instruction
    init_machine(0xFFA0, 0, NULL);
    machine.registers[x1] = 0x1133557799;
    machine.registers[x2] = 0x2244668800;
    char *stp[] = {"stp", "x1", "x2", "[sp,#-16]", NULL};
    execute(stp);
    print_machine();
    uint64_t *mem64 = machine.stack;
    XTEST((machine.registers[sp] == 0xFF90), "stp instruction should subtract 16 from stack pointer");
    XTEST((mem64[0] == 0x1133557799), "stp instruction should store first register on stack");
    XTEST((mem64[1] == 0x2244668800), "stp instruction should store second register on stack");
    XTEST((machine.registers[x1] == 0x1133557799), "stp instruction should not change first source register");
    XTEST((machine.registers[x2] == 0x2244668800), "stp instruction should not change second source register");

    // Execute ldp instruction
    init_machine(0xFF80, 0, NULL);
    grow_stack(0xFF70);
    machine.registers[sp] = 0xFF70;
    mem64 = machine.stack;
    mem64[0] = 0x1234512345;
    mem64[1] = 0x6789067890;
    char *ldp[] = {"ldp", "x3", "x4", "[sp]", "#16", NULL};
    execute(ldp);
    print_machine();
    XTEST((machine.registers[sp] == 0xFF80), "ldp instruction should add 16 to stack pointer");
    XTEST((mem64[0] == 0x1234512345), "ldp instruction should not change first value on stack");
    XTEST((mem64[1] == 0x6789067890), "ldp instruction should not change second value on stack");
    XTEST((machine.registers[x3] == 0x1234512345), "ldp instruction should load first value on stack in first destination register");
    XTEST((machine.registers[x4] == 0x6789067890), "ldp instruction should load second value on stack in second destination register");

    printf("Tests complete.\n");
    if (!ok) {
      printf("At least one test failed.\n");
    } else {
      printf("All tests passed.\n");
    }
    return 0;
}