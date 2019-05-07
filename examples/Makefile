.PHONY := all clean

CC := gcc -std=c99

PRIMARY_HEADER_FILES := ../pipeline/pipeline.h ../pipeline/util.h
PRIMARY_SOURCE_FILES := ../pipeline/pipeline.c

ALL_FILES := tokenizer sizes

all: $(ALL_FILES)

tokenizer: tokenizer.c ../lexer/token.c ../lexer/token.h $(PRIMARY_SOURCE_FILES) $(PRIMARY_HEADER_FILES)
	$(CC) $< ../lexer/token.c $(PRIMARY_SOURCE_FILES) -o $@

sizes: sizes.c $(PRIMARY_SOURCE_FILES) $(PRIMARY_HEADER_FILES) ../lexer/token.c ../lexer/token.h
	$(CC) $< $(PRIMARY_SOURCE_FILES) -o $@

clean:
	rm -f $(ALL_FILES)
