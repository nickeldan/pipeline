CC := gcc --std=c99 -O3

.PHONY: clean

plObject.o: plObject.c plObject.h ../util/plUtil.h ../util/plError.h
	$(CC) -I../util -c $<

clean:
	rm -f *.o