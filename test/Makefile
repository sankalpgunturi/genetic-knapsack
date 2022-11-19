# from stack overflow: ~/questions/21548464/how-to-write-a-makefile-to-compile-a-simple-c-program 
CC				= gcc
CC_FLAGS 	= -mavx -mfma -O2 -std=c99
RM 				= rm -f

default: all assemble

all: crossover

crossover:
	$(CC) $(CC_FLAGS) -o crossover.x crossover.c
	$(CC) $(CC_FLAGS) -o selection.x selection.c

assemble:
	objdump -s -d -f --source ./selection.x > selection.S

clean:
	rm -rf *.x *.S
