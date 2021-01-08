# Project 03: Assembly visualizer

## Overview
In this project, you'll finish implementing a program that visualises the execution of ARM assembly code. 

### Learning objectives
After completing this project, you should be able to:
* Read/create/modify strings in C
* Use pointers in C to indirectly access values
* Calculate the contents of registers and the stack after the execution of common ARM assembly instructions

### Important tips
* Read the entirety of the project description before you write any code.
* Work on the project over multiple sessions. Do not try to complete the project in a single session on the day (before) the project is due. This will give you time to think about how to solve problems, allow you to ask questions, and result in better outcomes.
* Ensure you follow good program design, coding, testing, and debugging practices (details below).

## Getting started 

Your program will display the contents of the stack and registers after the execution of each assembly instruction in a file containing the output from objdump. Your program will support the following instructions:
* arithmetic (`add`, `sub`, `neg`, `mul`, `udiv`, `sdiv`)
* shifting/bitwise (`lsl`, `lsr`, `and`, `orr`)
* load/store/move (`ldr`, `ldp`, `str`, `stp`, `mov`)
* function calls (`bl`, `ret`)

As a challenge problem, you can add support for `cmp` and `b` instructions.

### Source code
Visit [this page](https://classroom.github.com/g/u9-hJG_A) to create and obtain access to a private git repository containing the starter code for the project. After the repository is created, you should clone your repository on a birds server.

The repository contains numerous source files:
* `code.c` contains a `load_code` function that loads assembly code from a file containing output from objdump
* `code.h` contains a function prototype for the `load_code` function
* `instructions.c` contains functions for all of the assembly instructions your visualizer supports, as well as arrays (`opcodes32`, `opcodes64`, and `opcodesMacros`) mapping assembly opcodes to the appropriate functions
*  `instructions.h` contains the definitions of the structs used to map assembly opcodes to the appropriate C functions 
* `machine.c` contains `init_machine` and `print_machine` functions for initializing and printing, respectively, the state (i.e., stack and registers) of a simulated ARM system; it also contains an `excute` function that looks up and calls the appropriate function in `instructions.c` for a particular opcode
* `machine.h` contains the definition of the struct that represents the simulated ARM system and prototypes for the functions in `machine.c`  
* `parse.c` contains functions for parsing a string containing (parts of) an assembly instruction
* `parse.h` contains prototypes for the functions in `parse.c`
* `simulator.c` contains the `main` function, which initializes the simulated ARM system and drives the execution of instructions and display of system state
* `test_parse.c`, `test_instructions.c`, and `test_functions.c` contain testcases to help you test your code for parts 1, 2, and 3 (respectively) of the project
* `examples` contains some assembly code for you to visualize


### Running the simulator
To compile the simulator and test cases, run the command
```bash
$ make
```

To simulate the execution of assembly instructions and display the system state, run the command
```bash
$ ./simulator CODE_FILE SP_INIT PC_INIT PC_END
```
replacing `CODE_FILE` with the path to a file containing output from objdump, `SP_INIT` with the initial value for the stack pointer, `PC_INIT` with the initial value for the program counter (i.e., the address for the first instruction to execute), and `PC_END` with the program counter value at which the simulation should stop (i.e., the address for the instruction immediately following the last instruction to execute).

For example, to run the simulator with the program in `examples/sum.txt` starting from the beginning of `main` and ending just before `sum` returns, run the command
```bash
$ ./simulator examples/sum.txt 0xFFE0 0x4005e8 0x4005e4
```

When your simulator is complete, its output should match the output included at the end of the project instructions.

### Tasks
Your task is to implement three important parts of the simulator
1. Implement the `parse_instruction` function in `parse.c`
2. Complete the instruction-specific functions (defined in `instructions.c`) for the arithmetic, shifting/bitwise, and a simple load/store/move (`ldr`, `str`, `mov`) instructions
3. Complete the instruction-specific functions (defined in `instructions.c`) for the complex load/store (`stp`, `ldp`) and function call (`bl`, `ret`) instructions

## Part 1: Parsing an instruction
Your first task is to **implement the `parse_instruction` function in `parse.c`**. This function takes a string containing an assembly instruction (e.g., `add x0, x1, x2`) and returns a heap-allocated array of strings containing each of the components of the instruction (e.g., `{ "add", "x0", "x1", "x2", NULL }`). Notice that the array includes `NULL` after the last string to denote the end of the instruction components.

You may assume that each component of the instruction is separated by a single whitespace character (space or tab). The provided instruction string also includes commas separating the operands; the commas **should not** be included in the strings in the array returned by the function.

### Testing and debugging
You should run the `test_parse` program to test your code. You may want to (but are not required to) add additional test cases.

If your program crashes with a segmentation fault, run `valgrind`. (You can ignore memory leaks.) If your function returns the wrong value, add `printf` statements to help you debug; **please remove or comment out these print statements** when you are done debugging your program.

## Part 2: Simulating an arithmetic, shifting/bitwise, and simple load/store/move instruction
Your second task is to **complete the instruction-specific functions (defined in `instructions.c`) for the arithmetic, shifting/bitwise, and simple load/store/move (`ldr`, `str`, `mov`) instructions**. 

For most instructions, there is both a function that simulates the execution of the instruction on 32-bit operands (e.g., `add32` handles the execution of `add w0, w1, w2`) and a function that simulates the execution of the instruction on 64-bit operands (e.g., `add64` handles the execution of `add x0, x1, x2`). The fundamental difference between the two functions is the type of the operands, and, for the 32-bit version, the clearing of the upper 32-bits of the destination register. You need to add a **single line of code** to each function. The code you add will be the same for both the 32-bit and 64-bit functions: e.g., `add32` and `add64` both contain the statement `*d = *o1 + *o2`.  The `add32` and `add64` functions have been written for you to provide a model. 

You **do not** need to complete the `bl`, `ret`, `ldp`, or `stp` functions at this time; you will complete these in part 3 of the project.

### Testing and debugging
You should run the `test_instructions` program to test your code. You may want to (but are not required to) add additional test cases. You should also run the simulator program with some of the assembly programs in the `examples` directory.

If your program crashes with a segmentation fault, run `valgrind`. (You can ignore memory leaks.) If your function returns the wrong value, add `printf` statements to help you debug; **please remove or comment out these print statements** when you are done debugging your program.

## Part 3: Simulating complex load/store and function call instructions
Your final task is to **complete the instruction-specific functions (defined in `instructions.c`) for the complex load/store (`stp`, `ldp`) and function call (`bl`, `ret`) instructions**.

The complex load/store instructions can take one of three forms: load/store only, load/store-then-update, or update-then-load/store. In all forms, the values of two registers are loaded/stored on the stack. You can implement these operations by writing and executing assembly instructions that perform each of these simpler operations. For the last two forms, you also need to update the value of the register used in computing the stack address where the values are loaded/stored. Again, you can implement this operation by writing and executing an assembly instruction that performs this simpler operation. The first part of the `ldp` function has been written for you to provide a model.

The function call instructions rely on both the `pc` and `x30` registers. You can refer to these registers using expressions like `machine.registers[pc]`.

### Testing and debugging
You should run the `test_functions` program to test your code. You may want to (but are not required to) add additional test cases. You should also run the simulator program with some of the assembly programs in the `examples` directory.

If your program crashes with a segmentation fault, run `valgrind`. (You can ignore memory leaks.) If your function returns the wrong value, add `printf` statements to help you debug; **please remove or comment out these print statements** when you are done debugging your program.

## Program design

You **must follow good program design and coding practices**, including:
* Properly indenting your code --- Recall that indentation is not semantically significant in C, but it makes your code much easier to read.
* Including comments --- If you write more than a few lines of code in the body of a function, then you must include comments; generally, you should include a comment before each conditional statement, loop, and set of statements that perform some calculation. **Do not** include a comment for every line of code, and **do not** simply restate the code.
* Making multiple commits to your git repository --- Do not wait until your entire program is working before you commit it to your git repository. You should commit your code each time you write and debug a piece of functionality.

## Challenge problem
Challenge problems in projects are an opportunity to earn a small amount of extra credit toward your grade for _this_ project.

The challenge problem for this project is to add support for compare (`cmp`) and branch (e.g., `b`, `b.eq`, `b.lt`) instructions. 

You will need to add one or more fields to the `struct system` in `machine.h` to store the outcome of the `cmp` instruction. You will also need to add functions for the various instructions to `instructions.c` and add entries for the instructions in the `opcodes32` and `opcodes64` arrays.

## Submission instructions
You should **commit and push** your updated files to your git repository. However, as noted above, do not wait until your entire program is working before you commit it to your git repository; you should commit your code each time you write and debug a piece of functionality. 

## Example output
```
$ ./simulator examples/sum.txt 0xFFE0 0x4005e8 0x4005e4

Registers:
	sp = 0xffe0
	pc = 0x4005e8
Stack:
	      sp-> +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005e8 stp x29 x30 [sp,#-16]! 
	Update-then-store
	add sp sp #-16] 
Registers:
	sp = 0xffd0
	pc = 0x4005ec
Stack:
	      sp-> +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	str x29 [sp] 
Registers:
	x29 = 0x0
	sp = 0xffd0
	pc = 0x4005ec
Stack:
	      sp-> +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	str x30 [sp,#8] 
Registers:
	x29 = 0x0
	x30 = 0x0
	sp = 0xffd0
	pc = 0x4005ec
Stack:
	      sp-> +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005ec mov x29 sp 
Registers:
	x29 = 0xffd0
	x30 = 0x0
	sp = 0xffd0
	pc = 0x4005f0
Stack:
	x29-> sp-> +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005f0 sub sp sp #0x10 
Registers:
	x29 = 0xffd0
	x30 = 0x0
	sp = 0xffc0
	pc = 0x4005f4
Stack:
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005f4 orr w8 wzr #0x4 
Registers:
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x0
	sp = 0xffc0
	pc = 0x4005f8
Stack:
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005f8 stur wzr [x29,#-4] 
Registers:
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x0
	sp = 0xffc0
	pc = 0x4005fc
Stack:
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005fc mov w0 w8 
Registers:
	x0 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x0
	sp = 0xffc0
	pc = 0x400600
Stack:
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x400600 mov w1 w8 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x0
	sp = 0xffc0
	pc = 0x400604
Stack:
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x400604 bl 4005c0 <sum> 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffc0
	pc = 0x4005c0
Stack:
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005c0 sub sp sp #0x10 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005c4
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005c4 str w0 [sp,#12] 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005c8
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 00 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005c8 str w1 [sp,#8] 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005cc
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005cc ldr w0 [sp,#12] 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005d0
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005d0 ldr w1 [sp,#8] 
Registers:
	x0 = 0x4
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005d4
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005d4 add w0 w0 w1 
Registers:
	x0 = 0x8
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005d8
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005d8 str w0 [sp,#4] 
Registers:
	x0 = 0x8
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005dc
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 08 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005dc ldr w0 [sp,#4] 
Registers:
	x0 = 0x8
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffb0
	pc = 0x4005e0
Stack:
	      sp-> +-------------------------+
	0x0000ffb0 | 00 00 00 00 08 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	           +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+


0x4005e0 add sp sp #0x10 
Registers:
	x0 = 0x8
	x1 = 0x4
	x8 = 0x4
	x29 = 0xffd0
	x30 = 0x400608
	sp = 0xffc0
	pc = 0x4005e4
Stack:
	           +-------------------------+
	0x0000ffb0 | 00 00 00 00 08 00 00 00 |
	           +-------------------------+
	0x0000ffb8 | 04 00 00 00 04 00 00 00 |
	      sp-> +-------------------------+
	0x0000ffc0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffc8 | 00 00 00 00 00 00 00 00 |
	x29->      +-------------------------+
	0x0000ffd0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffd8 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
	0x0000ffe0 | 00 00 00 00 00 00 00 00 |
	           +-------------------------+
```
