CC := gcc --std=c99 -O3

.PHONY: clean

plUtil.o: plUtil.c plUtil.h plError.h
	$(CC) -c $<

clean:
	rm -f *.o