CC = gcc
CC_FLAGS 	= -mavx -mfma -O3 -std=c99

default: all assemble

all: 
	$(CC) $(CFLAGS) $(OBJS) kernel_driver.c  -o genetic.x -march=native
run:
	./genetic.x

clean:
	rm -f *.x *~ *.o

assemble:
	objdump -s -d -f --source ./genetic.x > genetic.S