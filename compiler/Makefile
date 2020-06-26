CC := gcc
COMPILER_FLAGS := --std=gnu99 -O3 -g -fpic
INCLUDES := -I../util -I../pipeline

.PHONY: all clean

all: libplparser.so

libplparser.so: scanner.o nameTable.o ast.o parser.tab.o
	$(CC) -shared -o $@ $^

scanner.o: scanner.c parser.tab.h ast.h marker.h ../util/plUtil.h ../util/plError.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

scanner.c: scanner.l
	flex -d -o $@ $<

nameTable.o: nameTable.c nameTable.h ../util/plUtil.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

ast.o: ast.c ast.h parser.tab.h ../util/plUtil.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

parser.tab.o: parser.tab.c parser.tab.h ast.h marker.h nameTable.h ../pipeline/plObject.h
	$(CC) $(COMPILER_FLAGS) $(INCLUDES) -c $<

parser.tab.h parser.tab.c: parser.y
	bison -dv $<

clean:
	rm -f scanner.c parser.tab.h parser.tab.c parser.output *.o libplparser.so
