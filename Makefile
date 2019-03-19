.PHONY := all test clean

all: test

test:
	cd examples && make

clean:
	cd examples && make clean