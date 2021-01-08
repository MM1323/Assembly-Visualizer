.PHONY: clean
CC=clang
CFLAGS=-I. -g -Wall --std=gnu11 -fpic
SRCS=parse.c machine.c instructions.c code.c
OBJS=$(patsubst %.c,%.o,$(SRCS))
PROGRAM=simulator
TESTS=test_parse test_instructions test_functions

all: $(PROGRAM) $(TESTS) 

$(PROGRAM): $(OBJS) simulator.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_%: $(OBJS) test_%.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(PROGRAM) $(TESTS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lib%.so: $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)
