CC := gcc --std=gnu99 -O3 -g

.PHONY: clean

plUtil.o: plUtil.c plUtil.h plError.h
	$(CC) -c $<

clean:
	rm -f *.o
