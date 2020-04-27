CC := gcc -std=c99 -O3

.PHONY: clean

plObject.o: plObject.c plObject.h ../util/util.h
	$(CC) -I../util -c $<

clean: rm -f plObject.o