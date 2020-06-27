CC := gcc
COMPILER_FLAGS := -std=gnu99 -O3 -g
INCLUDE_DIRS := -I./compiler -I./pipeline -I./util

.PHONY: all clean

all: parser_check

parser_check: parser_check.o compiler/libplcompiler.a pipeline/libplobject.a util/libplutil.a
	$(CC) -o $@ $^

parser_check.o: parser_check.c pipeline/plObject.h compiler/ast.h compiler/parser.tab.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_DIRS) -c $<

compiler/libplcompiler.a compiler/parser.tab.h pipeline/libplobject.a util/libplutil.a:
	cd $(dir $@) && make $(notdir $@)

clean:
	rm -f parser_check parser_check.o
