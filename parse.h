#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdint.h>

char **parse_instruction(char *instruction);
char **parse_addr(char *addr);
char operand_size(char *val);
void *operand(char *val);
void *memory_operand(char *addr_str);

#endif // __PARSE_H__
