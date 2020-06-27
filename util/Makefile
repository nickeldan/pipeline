CC := gcc
COMPILER_FLAGS := --std=gnu99 -O3 -g -fpic

LIB_NAME :=libplutil

.PHONY: all clean

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: plUtil.o
	$(CC) -shared -o $@ $<

$(LIB_NAME).a: plUtil.o
	ar rcs $@ $^

plUtil.o: plUtil.c plUtil.h plError.h
	$(CC) $(COMPILER_FLAGS) -c $<

clean:
	rm -f *.o $(LIB_NAME).so $(LIB_NAME).a
