CC := gcc
COMPILER_FLAGS := -std=gnu99 -O3 -g -fpic

LIB_NAME := libplobject

.PHONY: all clean

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: plObject.o
	$(CC) -shared -o $@ $<

$(LIB_NAME).a: plObject.o
	ar rcs $@ $^

plObject.o: plObject.c plObject.h ../util/plUtil.h ../util/plError.h
	$(CC) $(COMPILER_FLAGS) -I../util -c $<

clean:
	rm -f *.o $(LIB_NAME).so $(LIB_NAME).a
