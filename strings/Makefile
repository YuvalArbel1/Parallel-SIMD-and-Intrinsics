# 315112870 Mor Hanania 206945107 Yuval Arbel


CC = gcc
AS = as
CFLAGS = -Wall -Wextra -std=c11 -g -O3 -msse4.2 -mavx2 -mavx512f -mavx512cd -mavx512er -mavx512pf -mavx512dq -mavx512bw -mavx512vl -mavx512vbmi -mavx512vbmi2 -mavx512vnni -mavx512bitalg -mavx512vpopcntdq -mavx5124fmaps -mavx5124vnniw -mavx512ifma -mavx512vp2intersect


SRCS = main.c libstr.h b64.c hamming.s
OBJS = main.o b64.o hamming.o

.PHONY: all clean run debug

all: part1.out

part1.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -no-pie

main.o: main.c libstr.h
	$(CC) $(CFLAGS) -c -o $@ main.c

b64.o: b64.c libstr.h
	$(CC) $(CFLAGS) -c -o $@ b64.c

hamming.o: hamming.s
	$(AS) -g hamming.s -o hamming.o

run: part1.out
	./part1.out

debug: part1.out
	gdb -ex run --args ./a.out

clean:
	rm -f part1.out $(OBJS)