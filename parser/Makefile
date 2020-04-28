CC := gcc --std=c99 -O3
INCLUDES := -I../pipeline -I../util
FLEX_LD_FLAG := -lfl

.PHONY: clean

parser: scanner.o parser.tab.o ../util/plUtil.o ../pipeline/plObject.o
	$(CC) $(FLEX_LD_FLAG) $^ -o $@

../util/util.o ../pipeline/plObject.o:
	cd $(dir $@) && make $(notdir $@)

scanner.o: scanner.c parser.tab.h ast.h marker.h ../util/plUtil.h ../util/plError.h ../pipeline/plObject.h
	$(CC) $(INCLUDES) -c $<

scanner.c: scanner.l
	flex -d -o $@ $<

parser.tab.o: parser.tab.c parser.tab.h ast.h marker.h nameTable.h ../pipeline/plObject.h
	$(CC) $(INCLUDES) -c $<

parser.tab.h parser.tab.c: parser.y
	bison -dv $<

clean:
	rm -f parser scanner.c parser.tab.h parser.tab.c parser.output *.o