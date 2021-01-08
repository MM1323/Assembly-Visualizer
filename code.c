#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "machine.h"

void load_code(char *filepath) {
    // Open source code
    FILE *source = fopen(filepath, "r");
    if (NULL == source) {
        perror("Failed to load code");
        exit(1);
    }

    // Read in lines of source code
    char line[100];
    int j = 0;
    machine.code = malloc(sizeof(char *) * (j + 1));
    machine.code[j] = NULL;
    while(fgets(line, 100, source) != NULL) {
        // Skip blank lines or function name lines
        if (line[0] == '\n' || line[strlen(line)-2] == ':') {
            continue;
        }

        // Skip indendentation
        int i = 0;
        while (isspace(line[i])) {
            i++;
        }

        // Extract address 
        char *addr = line+i;
        while (!isspace(line[i])) {
            i++;
        }
        line[i] = '\0';
        line[i++] = '\0';

        // Store address of first instruction
        if (machine.code_start == 0) {
            machine.code_start = strtol(addr, NULL, 16);
        }

        // Ignore instruction encoding
        while (!isspace(line[i])) {
            i++;
        }
        line[i++] = '\0';
        line[i++] = '\0';

        // Extract instruction
        char *instruction = line+i;
        instruction[strlen(instruction)-1] = '\0';
        machine.code[j++] = strdup(instruction);
        machine.code = realloc(machine.code, sizeof(char *) * (j+1));
        machine.code[j] = NULL;
    }

    fclose(source);

    /*int k = 0;
    while(machine.code[k] != NULL) {
        printf("%p %s\n", (void *)(machine.code_start + k * 4), machine.code[k]);
        k++;
    }*/
}