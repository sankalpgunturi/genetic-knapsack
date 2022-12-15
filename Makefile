CC = gcc
CFLAGS = -mavx -mfma -O3 -std=c99 -fopenmp

default: all assemble

all: 
	$(CC) $(CFLAGS) $(OBJS) kernel_driver.c  -o kernel_d.x -march=native

compile_play:
	$(CC) $(CFLAGS) $(OBJS) playground_selection.c  -o playground_selection.x -march=native

run:
	./kernel_d.x 512 65536

run128:
	./kernel_d.x 128 8
	./kernel_d.x 128 16
	./kernel_d.x 128 64
	./kernel_d.x 128 128
	./kernel_d.x 128 256
	./kernel_d.x 128 512
	./kernel_d.x 128 1024
	./kernel_d.x 128 2048
	./kernel_d.x 128 4096
	./kernel_d.x 128 8192
	./kernel_d.x 128 16384
	./kernel_d.x 128 32768
	./kernel_d.x 128 65536
	./kernel_d.x 128 131072

run256:
	./kernel_d.x 256 8
	./kernel_d.x 256 16
	./kernel_d.x 256 64
	./kernel_d.x 256 128
	./kernel_d.x 256 256
	./kernel_d.x 256 512
	./kernel_d.x 256 1024
	./kernel_d.x 256 2048
	./kernel_d.x 256 4096
	./kernel_d.x 256 8192
	./kernel_d.x 256 16384
	./kernel_d.x 256 32768
	./kernel_d.x 256 65536
	./kernel_d.x 256 131072

run512:
	./kernel_d.x 512 8
	./kernel_d.x 512 16
	./kernel_d.x 512 64
	./kernel_d.x 512 128
	./kernel_d.x 512 256
	./kernel_d.x 512 512
	./kernel_d.x 512 1024
	./kernel_d.x 512 2048
	./kernel_d.x 512 4096
	./kernel_d.x 512 8192
	./kernel_d.x 512 16384
	./kernel_d.x 512 32768
	./kernel_d.x 512 65536
	./kernel_d.x 512 131072

run1024:
	./kernel_d.x 1024 8 
	./kernel_d.x 1024 16 
	./kernel_d.x 1024 64 
	./kernel_d.x 1024 128 
	./kernel_d.x 1024 256 
	./kernel_d.x 1024 512 
	./kernel_d.x 1024 1024 
	./kernel_d.x 1024 2048 
	./kernel_d.x 1024 4096 
	./kernel_d.x 1024 8192 
	./kernel_d.x 1024 16384 
	./kernel_d.x 1024 32768 
	./kernel_d.x 1024 65536 
	./kernel_d.x 1024 131072 

run2048:
	./kernel_d.x 2048 8 4
	./kernel_d.x 2048 16 4
	./kernel_d.x 2048 64 4
	./kernel_d.x 2048 128 4
	./kernel_d.x 2048 256 4
	./kernel_d.x 2048 512 4
	./kernel_d.x 2048 1024 4
	./kernel_d.x 2048 2048 4
	./kernel_d.x 2048 4096 4
	./kernel_d.x 2048 8192 4
	./kernel_d.x 2048 16384 4
	./kernel_d.x 2048 32768 4
	./kernel_d.x 2048 65536 4
	./kernel_d.x 2048 131072 4

run4096:
	./kernel_d.x 4096 8 2
	./kernel_d.x 4096 16 2
	./kernel_d.x 4096 64 2
	./kernel_d.x 4096 128 2
	./kernel_d.x 4096 256 2
	./kernel_d.x 4096 512 2
	./kernel_d.x 4096 1024 2
	./kernel_d.x 4096 2048 2
	./kernel_d.x 4096 4096 2
	./kernel_d.x 4096 8192 2
	./kernel_d.x 4096 16384 2
	./kernel_d.x 4096 32768 2
	./kernel_d.x 4096 65536 2
	./kernel_d.x 4096 131072 2

run8192:
	./kernel_d.x 8192 8 1
	./kernel_d.x 8192 16 1
	./kernel_d.x 8192 64 1
	./kernel_d.x 8192 128 1
	./kernel_d.x 8192 256 1
	./kernel_d.x 8192 512 1
	./kernel_d.x 8192 1024 1
	./kernel_d.x 8192 2048 1
	./kernel_d.x 8192 4096 1
	./kernel_d.x 8192 8192 1
	./kernel_d.x 8192 16384 1
	./kernel_d.x 8192 32768 1
	./kernel_d.x 8192 65536 1
	./kernel_d.x 8192 131072 1

run9986:
	./kernel_d.x 9986 8 1
	./kernel_d.x 9986 16 1
	./kernel_d.x 9986 64 1
	./kernel_d.x 9986 128 1
	./kernel_d.x 9986 256 1
	./kernel_d.x 9986 512 1
	./kernel_d.x 9986 1024 1
	./kernel_d.x 9986 2048 1
	./kernel_d.x 9986 4096 1
	./kernel_d.x 9986 8192 1
	./kernel_d.x 9986 16384 1
	./kernel_d.x 9986 32768 1
	./kernel_d.x 9986 65536 1
	./kernel_d.x 9986 131072 1

runall: run run128 run256 run512 run1024 run2048 run4096 run8192 run9986

rungen8192:
	./kernel_d.x 128 8192
	./kernel_d.x 256 8192
	./kernel_d.x 512 8192
	./kernel_d.x 1024 8192
	./kernel_d.x 2048 8192 4
	./kernel_d.x 4096 8192 2
	./kernel_d.x 8192 8192 1

rungen16384:
	./kernel_d.x 128 16384
	./kernel_d.x 256 16384
	./kernel_d.x 512 16384
	./kernel_d.x 1024 16384
	./kernel_d.x 2048 16384 4
	./kernel_d.x 4096 16384 2
	./kernel_d.x 8192 16384 1

rungen32768:
	./kernel_d.x 128 32768
	./kernel_d.x 256 32768
	./kernel_d.x 512 32768
	./kernel_d.x 1024 32768
	./kernel_d.x 2048 32768 4
	./kernel_d.x 4096 32768 2
	./kernel_d.x 8192 32768 1

rungen65536:
	./kernel_d.x 128 65536
	./kernel_d.x 256 65536
	./kernel_d.x 512 65536
	./kernel_d.x 1024 65536
	./kernel_d.x 2048 65536 4
	./kernel_d.x 4096 65536 2
	./kernel_d.x 8192 65536 1 

rungen131072:
	./kernel_d.x 128 131072
	./kernel_d.x 256 131072
	./kernel_d.x 512 131072
	./kernel_d.x 1024 131072
	./kernel_d.x 2048 131072 4
	./kernel_d.x 4096 131072 2
	./kernel_d.x 8192 131072 1 

clean:
	rm -f *.x *~ *.o *~ *.S

assemble:
	objdump -s -d -f --source ./kernel_d.x > kernel_d.S

play_selection:
	./playground_selection.x

