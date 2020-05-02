CC := gcc
COMPILER_FLAGS := --std=gnu99 -O3 -g -fpic

.PHONY: all clean

all: plObject.o

plObject.o: plObject.c plObject.h ../util/plUtil.h ../util/plError.h
	$(CC) $(COMPILER_FLAGS) -I../util -c $<

clean:
	rm -f *.o
