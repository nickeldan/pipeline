CC := gcc
COMPILER_FLAGS := -std=gnu99 -O3 -g -fpic -Wall
INCLUDES := -I../util -I../pipeline

LIB_NAME := libplcompiler

.PHONY: all clean

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: lex.yy.o nameTable.o ast.o parser.tab.o
	$(CC) -shared -o $@ $^

$(LIB_NAME).a: lex.yy.o nameTable.o ast.o parser.tab.o
	ar rcs $@ $^

lex.yy.o: lex.yy.c parserWrapper.h parser.tab.h ast.h marker.h ../util/plUtil.h ../util/plError.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

lex.yy.c lex.yy.h: scanner.l
	flex $<

nameTable.o: nameTable.c ../util/plUtil.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

ast.o: ast.c ast.h parserWrapper.h parser.tab.h ../util/plUtil.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

parser.tab.o: parser.tab.c parserWrapper.h parser.tab.h lex.yy.h ast.h marker.h nameTable.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

parser.tab.h parser.tab.c: parser.y
	bison -dv $<

clean:
	rm -f parser.output *.o $(LIB_NAME).so $(LIB_NAME).a
