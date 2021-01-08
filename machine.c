#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#include "machine.h"
#include "parse.h"
#include "code.h"

struct system machine;

/*
 * Initialize the machine
 */
void init_machine(uint64_t stack_top, uint64_t pc_start, char *code_path) {
    memset(machine.registers, 0, sizeof(machine.registers));
    memset(machine.used, 0, sizeof(machine.used));
    machine.stack_top = stack_top;
    machine.stack_bot = stack_top + WORD_SIZE_BYTES;
    machine.stack = malloc(machine.stack_bot - machine.stack_top);
    memset(machine.stack, 0, machine.stack_bot - machine.stack_top);
    machine.registers[sp] = machine.stack_top;
    machine.registers[pc] = pc_start;
    if (code_path != NULL) {
        load_code(code_path);
    }
    else {
        machine.code_start = 0;
        machine.code = NULL;
    }
}

/*
 * Allocate more space to keep track of values on the simulated stack.
 */
void grow_stack(uint64_t addr) {
    // Grow the stack upwards
    if (addr < machine.stack_top) {
        // Round down to a multiple of word size
        if (addr % WORD_SIZE_BYTES != 0) {
            addr -= addr % WORD_SIZE_BYTES;
        }

        // Allocate space and copy over old values 
        void *new_stack = malloc(machine.stack_bot - addr);
        memset(new_stack, 0, machine.stack_top - addr);
        if (machine.stack != NULL) {
            memcpy(new_stack + (machine.stack_top - addr), machine.stack, machine.stack_bot - machine.stack_top);
            free(machine.stack);
        }

        // Update machine
        machine.stack = new_stack;
        machine.stack_top = addr;
    }
    // Grow the stack downwards
    else if (addr > machine.stack_bot) {
        // Round up to a multiple of word size
        if (addr % WORD_SIZE_BYTES != 0) {
            addr += addr % WORD_SIZE_BYTES;
        }

        // Allocate space and copy over old values 
        void *new_stack = malloc(addr - machine.stack_top);
        memset(new_stack, 0, addr - machine.stack_bot);
        if (machine.stack != NULL) {
            memcpy(new_stack, machine.stack, machine.stack_bot - machine.stack_top);
            free(machine.stack);
        }

        // Update machine
        machine.stack = new_stack;
        machine.stack_bot = addr;
    }
}

void print_machine() {
    // Print out the value of all used registers
    printf("Registers:\n");
    for (int i = 0; i <= 30; i++) {
        if (machine.used[i]) {
            printf("\tx%d = 0x%lx\n", i, machine.registers[i]);
        }
    }
    printf("\tsp = 0x%lx\n", machine.registers[sp]);
    printf("\tpc = 0x%lx\n", machine.registers[pc]);

    // If necessary, grow the stack before printing it
    if (machine.registers[sp] < machine.stack_top) {
        grow_stack(machine.registers[sp]);
    }

    // Print out the value of all words on the stack
    printf("Stack:\n");
    unsigned char *stack = machine.stack;
    for (int i = 0; i < (machine.stack_bot - machine.stack_top); i += 8) {
        printf("\t");

        // Indicate where x29 and sp registers point
        if (machine.registers[x29] == i + machine.stack_top) {
            printf("x29-> ");
        } 
        else {
            printf("      ");
        }
        if (machine.registers[sp] == i + machine.stack_top) {
            printf("sp-> ");
        }
        else {
            printf("     ");
        }

        printf("+-------------------------+\n");
        printf("\t0x%08lx | ", i + machine.stack_top);
        for (int j = 0; j < 8; j++) {
            printf("%02X ", stack[i+j]);
        }
        printf("|\n");
    }
    printf("\t           +-------------------------+\n");
}

/*
 * Execute a parsed assembly instruction
 */
void execute(char **instruction) {
    // Display instruction
    int i = 0;
    while (instruction[i] != NULL) {
        printf("%s ", instruction[i]);
        i++;
    }
    printf("\n");

    // If instruction is a macro, then call the macro handler
    void (*func)(char**) = NULL;
    int k = 0;
    while (opcodesMacro[k].name != NULL) {
        if (strcmp(instruction[0], opcodesMacro[k].name) == 0) {
            func = opcodesMacro[k].func;
            break;
        }
        k++;
    }
    if (func != NULL) {
        (*func)(instruction);
        return;
    }

    // Convert operands
    void *operands[i];
    for (int j = 0; j < i-1; j++) {
        operands[j] = operand(instruction[j+1]);
    }

    // Determine size of operands
    char size = WORD_SIZE_BITS;
    if (instruction[1] != NULL)  {
        size = operand_size(instruction[1]);
    }

    // Invoke instruction
    if (size == 32) {
        void (*func)(uint32_t**) = NULL;
        int i = 0;
        while (opcodes32[i].name != NULL) {
            if (strcmp(instruction[0], opcodes32[i].name) == 0) {
                func = opcodes32[i].func;
                break;
            }
            i++;
        }
        if (func != NULL) {
            (*func)((uint32_t **)operands);
        }
        else {
            printf("Unhandled 32-bit instruction: %s\n", instruction[0]);
        }
    }
    else {
        void (*func)(uint64_t**) = NULL;
        int i = 0;
        while (opcodes64[i].name != NULL) {
            if (strcmp(instruction[0], opcodes64[i].name) == 0) {
                func = opcodes64[i].func;
                break;
            }
            i++;
        }
        if (func != NULL) {
            (*func)((uint64_t **)operands);
        }
        else {
            printf("Unhandled 64-bit instruction: %s\n", instruction[0]);
        }
    }
}