CC := gcc --std=c99 -O3

.PHONY: clean

parser: scanner.c parser.tab.c ast.c ../util/util.c ../pipeline/object.c
	$(CC) -I../pipeline -I../util $^ -o $@

scanner.c: scanner.l parser.tab.h ast.h marker.h ../util/util.h ../pipeline/object.h
	flex -d -o $@ $<

parser.tab.h parser.tab.c: parser.y ast.h marker.h ../pipeline/object.h
	bison -dv $<

clean:
	rm -f parser scanner.c parser.tab.h parser.tab.c parser.output