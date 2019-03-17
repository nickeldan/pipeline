.PHONY := all test clean

all: test

test: examples/tokenizer

examples/tokenizer: examples/tokenizer.c pipeline/pipeline.h lexer/token.h lexer/token.c
	gcc -std=c99 -D DEBUG $< lexer/token.c -o $@

clean:
	rm -f examples/tokenizer