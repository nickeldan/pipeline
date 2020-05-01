CC := gcc --std=gnu99 -O3 -g

.PHONY: clean

plObject.o: plObject.c plObject.h ../util/plUtil.h ../util/plError.h
	$(CC) -I../util -c $<

clean:
	rm -f *.o
