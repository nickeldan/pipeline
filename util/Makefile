CC := gcc
COMPILER_FLAGS := --std=gnu99 -O3 -g -fpic

.PHONY: all clean

all: plUtil.o

plUtil.o: plUtil.c plUtil.h plError.h
	$(CC) $(COMPILER_FLAGS) -c $<

clean:
	rm -f *.o
