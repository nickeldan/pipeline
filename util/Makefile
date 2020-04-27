CC := gcc --std=c99 -O3

.PHONY: clean

util.o: util.c util.h
	$(CC) -c $<

clean:
	rm -f util.o