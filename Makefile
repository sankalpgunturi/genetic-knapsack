CC = gcc
CFLAGS = -mavx -mfma -O3 -std=c99

default: all assemble

all: 
	$(CC) $(CFLAGS) $(OBJS) kernel_driver.c  -o kernel_d.x -march=native

compile_play:
	$(CC) $(CFLAGS) $(OBJS) playground_selection.c  -o playground_selection.x -march=native

run:
	./kernel_d.x 10
	./kernel_d.x 25
	./kernel_d.x 100
	./kernel_d.x 650
	./kernel_d.x 1000
	./kernel_d.x 2500
	./kernel_d.x 4500
	./kernel_d.x 6000
	./kernel_d.x 8000
	./kernel_d.x 10000


clean:
	rm -f *.x *~ *.o *~ *.S

assemble:
	objdump -s -d -f --source ./kernel_d.x > kernel_d.S

play_selection:
	./playground_selection.x
