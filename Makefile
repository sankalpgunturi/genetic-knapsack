CC = gcc
CFLAGS = -mavx -mfma -O3 -std=c99 -fopenmp

default: all assemble

all: 
	$(CC) $(CFLAGS) $(OBJS) kernel_driver.c  -o kernel_d.x -march=native

compile_play:
	$(CC) $(CFLAGS) $(OBJS) playground_selection.c  -o playground_selection.x -march=native

run:
	./kernel_d.x

clean:
	rm -f *.x *~ *.o *~ *.S

assemble:
	objdump -s -d -f --source ./kernel_d.x > kernel_d.S

play_selection:
	./playground_selection.x
