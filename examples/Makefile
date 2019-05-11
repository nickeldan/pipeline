.PHONY := all clean

CC := gcc -std=c99

PRIMARY_HEADER_FILES := ../pipeline/object.h ../util.h
PRIMARY_SOURCE_FILES := ../pipeline/object.c

ALL_FILES := tokenizer sizes

all: $(ALL_FILES)

tokenizer: tokenizer.c ../parser/token.c ../parser/token.h $(PRIMARY_SOURCE_FILES) $(PRIMARY_HEADER_FILES)
	$(CC) $< ../parser/token.c $(PRIMARY_SOURCE_FILES) -o $@

sizes: sizes.c $(PRIMARY_SOURCE_FILES) $(PRIMARY_HEADER_FILES) ../parser/token.c ../parser/token.h
	$(CC) $< $(PRIMARY_SOURCE_FILES) -o $@

clean:
	rm -f $(ALL_FILES)
