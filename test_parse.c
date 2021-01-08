#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

bool ok = true;

#define XTEST(expr, errmsg) \
  if (!expr) { \
    printf("%s (line %d of %s)\n", errmsg, __LINE__, __FILE__); \
    ok = false; \
  } \

int main(int argc, char **argv) {
    // Parse add instruction
    char **add = parse_instruction("add x0, x1, x2");
    XTEST((strcmp(add[0], "add") == 0), "Failed to parse opcode for add instruction");
    XTEST((strcmp(add[1], "x0") == 0), "Failed to parse destination for add instruction");
    XTEST((strcmp(add[2], "x1") == 0), "Failed to parse first operand for add instruction");
    XTEST((strcmp(add[3], "x2") == 0), "Failed to parse second operand for add instruction");
    XTEST((add[4] == NULL), "Failed to add NULL to end of components for add instruction");

    // Parse sub instruction
    char **sub = parse_instruction("sub x3, x3, #0x10");
    XTEST((strcmp(sub[0], "sub") == 0), "Failed to parse opcode for sub instruction");
    XTEST((strcmp(sub[1], "x3") == 0), "Failed to parse destination for sub instruction");
    XTEST((strcmp(sub[2], "x3") == 0), "Failed to parse first operand for sub instruction");
    XTEST((strcmp(sub[3], "#0x10") == 0), "Failed to parse second operand for sub instruction");
    XTEST((sub[4] == NULL), "Failed to sub NULL to end of components for sub instruction");

    // Parse ldr instruction
    char **ldr = parse_instruction("ldr x3, [sp]");
    XTEST((strcmp(ldr[0], "ldr") == 0), "Failed to parse opcode for ldr instruction");
    XTEST((strcmp(ldr[1], "x3") == 0), "Failed to parse destination for ldr instruction");
    XTEST((strcmp(ldr[2], "[sp]") == 0), "Failed to parse address for ldr instruction");
    XTEST((ldr[3] == NULL), "Failed to add NULL to end of components for ldr instruction");

    // Parse str instruction
    char **str = parse_instruction("str w4, [x5,#8]");
    XTEST((strcmp(str[0], "str") == 0), "Failed to parse opcode for str instruction");
    XTEST((strcmp(str[1], "w4") == 0), "Failed to parse source for str instruction");
    XTEST((strcmp(str[2], "[x5,#8]") == 0), "Failed to parse address for str instruction");
    XTEST((str[3] == NULL), "Failed to add NULL to end of components for str instruction");

    // Parse bl inbluction
    char **bl = parse_instruction("bl 40048c <foo>");
    XTEST((strcmp(bl[0], "bl") == 0), "Failed to parse opcode for bl instruction");
    XTEST((strcmp(bl[1], "40048c") == 0), "Failed to parse address for bl instruction");
    XTEST((strcmp(bl[2], "<foo>") == 0), "Failed to parse label for bl instruction");
    XTEST((bl[3] == NULL), "Failed to add NULL to end of components for bl instruction");

    printf("Tests complete.\n");
    if (!ok) {
      printf("At least one test failed.\n");
    } else {
      printf("All tests passed.\n");
    }
    return 0;
}