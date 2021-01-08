#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "machine.h"
#include "parse.h"
#include "code.h"

int main(int argc, char **argv) {

    if (argc != 5) {
        printf("Usage: %s CODE SP PC PC_END\n", argv[0]);
        exit(1);
    }

    char *code = argv[1];
    uint64_t sp_start = strtol(argv[2], NULL, 0);
    uint64_t pc_start = strtol(argv[3], NULL, 0);
    uint64_t pc_end = strtol(argv[4], NULL, 0);

    //init_machine(0xFF00, 0x4006ec, "loan.txt");
    //uint64_t pc_end = 0x400740;
    //init_machine(0xFF00, 0x4005e8, "sum.txt");
    //uint64_t pc_end = 0x4005e4;
    init_machine(sp_start, pc_start, code);

    print_machine();
    printf("\n\n");
    while (machine.registers[pc] != pc_end) {
        printf("0x%lx ", machine.registers[pc]);
        int offset = (machine.registers[pc] - machine.code_start) / 4 ;
        char *instruction = machine.code[offset];
        machine.registers[pc] += 4;
        execute(parse_instruction(instruction));
        print_machine();
        printf("\n\n");
    }
}
