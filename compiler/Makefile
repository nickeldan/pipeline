CC := gcc
COMPILER_FLAGS := --std=gnu99 -O3 -g -fpic
INCLUDES := -I../util -I../pipeline

LIB_NAME := libplcompiler

.PHONY: all clean

all: $(LIB_NAME).so $(LIB_NAME).a

$(LIB_NAME).so: scanner.o nameTable.o ast.o parser.tab.o
	$(CC) -shared -o $@ $^

$(LIB_NAME).a: scanner.o nameTable.o ast.o parser.tab.o
	ar rcs $@ $^

scanner.o: scanner.c parser.tab.h ast.h marker.h ../util/plUtil.h ../util/plError.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

scanner.c: scanner.l
	flex -o $@ $<

nameTable.o: nameTable.c nameTable.h ../util/plUtil.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

ast.o: ast.c ast.h parser.tab.h ../util/plUtil.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

parser.tab.o: parser.tab.c parser.tab.h ast.h marker.h nameTable.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

parser.tab.h parser.tab.c: parser.y
	bison -dv $<

clean:
	rm -f scanner.c parser.tab.h parser.tab.c parser.output *.o $(LIB_NAME).so $(LIB_NAME).a
