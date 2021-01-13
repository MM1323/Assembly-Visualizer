#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "machine.h"
#include "parse.h"

/*
 * Divides a string containing an ARM assembly instruction into an array of strings.
 */
char **parse_instruction(char *instruction) {
    char ** test = NULL;
    test = (char**)malloc(sizeof(char*)*20); //malloc the whole array 

    for(int i=0; i<50; i++) {
       test[i] = (char*)malloc(sizeof(char)*20); //malloc each part of the array
    }
    unsigned int length = strlen(instruction);
    char test_string[50];
    int count = 0;
    int instruction_count = 0;

    for(int x = 0; x < length; x++){
        // TEST FOR EACH SECTION
        //printf("%c\n", instruction[x]);

        if (isspace(instruction[x])) {
            if(test_string[count-1] == ','){
                test_string[count-1] = '\0';
            } else {
                test_string[count] = '\0';
            }
            test_string[count] = '\0';
            //Puts the string into the array
            strcpy(test[instruction_count], test_string);

            // TEST FOR INSTR_COUNT and ITEM IN COUNT
            // printf("%s\n", test[instruction_count]);
            // printf("instruction count: %d\n\n", instruction_count);

            //REST STRING
            count = 0;
            instruction_count+=1;
            test_string[0] = '\0';
            

        } else {
            test_string[count] = instruction[x];
            count++;
        
        }
        
    }

    //SETTING THE ADDRESS || SECOND OPERAND
    test_string[count] = '\0';
    strcpy(test[instruction_count], test_string);

    //SETTING THE LAST STRING TO NULL
    //printf("The instruction count: %d\n", instruction_count);
    test[instruction_count + 1] = NULL;

    // TESTS FOR ALL PARTS
    // printf("\nThe returned opcode: %s\n", test[0]);
    // printf("\nThe returned destination: %s\n", test[1]);
    // printf("\nThe first operand: %s\n", test[2]);
    // printf("\nThe rsecond operand || parse address: %s\n", test[3]);
    // printf("\nThe NULL: %s\n", test[4]);
    // printf("\n");

    return test;
}



/*
 * Divides a string containing an ARM memory operand into an array of strings.
 */
char **parse_addr(char *addr) {
    // Create a copy of the address string
    addr = strdup(addr);
    int len = strlen(addr);

    // Store the first part of the memory operand
    char **parts = malloc(sizeof(char *) * 2);
    parts[0] = addr + 1;

    // Locate the second part of the memory operand, if such an operand exists
    parts[1] = NULL;
    for (int i = 1; i < len-1; i++) {
        if (addr[i] == ',') {
            addr[i] = '\0';
            parts[1] = addr + i + 1; 
            break;
        }
    }

    // Removing closing square bracket
    if (addr[len-1] == ']') {
        addr[len-1] = '\0';
    }
    

    return parts;
}

/*
 * Determines the size of an operand.
 */
char operand_size(char *val) {
    switch (val[0]) {
        // xN, sp, and pc registers are 64-bits
        case 'x':
        case 's':
        case 'p':
            return WORD_SIZE_BITS;
        // wN registers are 32-bits
        case 'w':
            return HALFWORD_SIZE_BITS;
        // Constants are 16-bits
        case '#':
            return 16;
        // Memory locations depend on the operands in the memory address
        case '[':
            return operand_size(parse_addr(val)[0]);
        // Other types of operands are not supported
        default:
            return -1;
    }
}

/*
 * Get a pointer to an operand given a string representation of the operand.
 */
void *operand(char *val) {
    switch (val[0]) {
        // general-purpose register
        case 'x':
        case 'w':
            if (val[1] == 'z') {
                machine.used[xzr] = 1;
                return &(machine.registers[xzr]);
            } 
            else {
                int n = atoi(val+1);
                machine.used[n] = 1;
                return &(machine.registers[n]);
            }
        // Stack pointer
        case 's':
            machine.used[sp] = 1;
            return &(machine.registers[sp]);
        // Program counter
        case 'p':
            machine.used[pc] = 1;
            return &(machine.registers[pc]);
        // Constant
        case '#':
            machine.constant = strtol(val+1, NULL, 0);
            return &(machine.constant);
        // Code address
        case '4':
            machine.constant = strtol(val, NULL, 16);
            return &(machine.constant);
        // Memory location
        case '[':
            return memory_operand(val);
        default:
            return NULL;
    }
}

/**
 * Get a pointer to a location in main memory given a string representation of a memory operand.
 */
void *memory_operand(char *addr_str) {
    char **addr_parts = parse_addr(addr_str);
    uint64_t *base_op = operand(addr_parts[0]);
    long addr = *base_op;
    if (addr_parts[1] != NULL) {
        uint64_t *offset_op = operand(addr_parts[1]);
        addr += *offset_op;
    }
    if (addr < machine.stack_top || addr > machine.stack_bot) {
        grow_stack(addr);
    }
    return machine.stack + (addr - machine.stack_top);
}
