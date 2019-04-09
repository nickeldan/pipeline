.PHONY := all clean

CC := gcc -std=c99

all: tokenizer sizes

tokenizer: tokenizer.c ../pipeline/pipeline.h ../lexer/token.h ../lexer/token.c
	$(CC) $< ../lexer/token.c -o $@

sizes: sizes.c ../pipeline/pipeline.h
	$(CC) $< -o $@

clean:
	rm -f tokenizer sizes