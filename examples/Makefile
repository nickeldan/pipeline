.PHONY := all clean

CC := gcc -std=c99

all: tokenizer

tokenizer: tokenizer.c ../pipeline/pipeline.h ../lexer/token.h ../lexer/token.c
	$(CC) -D DEBUG $< ../lexer/token.c -o $@

clean:
	rm -f tokenizer