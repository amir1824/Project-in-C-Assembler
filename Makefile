# Basic compilation macros
CC = gcc # GCC Compiler
CFLAGS = -ansi -Wall -pedantic # Flags
GLOBAL_DEPS = globals.h # Dependencies for everything
EXE_DEPS = assembler.o file1.o fpass.o spass.o file2.o table.o utils.o writefiles.o # Deps for exe

## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

## Main:
assembler.o: assembler.c $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

## Code helper functions:
code.o: file1.c file1.h $(GLOBAL_DEPS)
	$(CC) -c file1.c $(CFLAGS) -o $@

## First Pass:
fpass.o: first_pass.c first_pass.h $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

## Second Pass:
spass.o: second_pass.c second_pass.h $(GLOBAL_DEPS)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

## Instructions helper functions:
instructions.o: file2.c file2.h $(GLOBAL_DEPS)
	$(CC) -c file2.c $(CFLAGS) -o $@

## Table:
table.o: table.c table.h $(GLOBAL_DEPS)
	$(CC) -c table.c $(CFLAGS) -o $@

## Useful functions:
utils.o: utils.c file2.h $(GLOBAL_DEPS)
	$(CC) -c utils.c $(CFLAGS) -o $@

## Output Files:
writefiles.o: writefiles.c writefiles.h $(GLOBAL_DEPS)
	$(CC) -c writefiles.c $(CFLAGS) -o $@

# Clean Target (remove leftovers)
clean:
	rm -rf *.o
