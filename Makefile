CC = gcc
CFLAGS = -mavx -mfma -O3 -std=c99

default: all assemble

all: 
	$(CC) $(CFLAGS) $(OBJS) kernel_driver.c  -o kernel_d.x -march=native
run:
	./kernel_d.x

clean:
	rm -f *.x *~ *.o

assemble:
	objdump -s -d -f --source ./kernel_d.x > kernel_d.S