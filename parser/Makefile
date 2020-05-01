CC := gcc --std=gnu99 -O3 -g
INCLUDES := -I../util -I../pipeline 
FLEX_LD_FLAG := -lfl

.PHONY: clean

parser: scanner.o parser.tab.o ast.o nameTable.o ../util/plUtil.o ../pipeline/plObject.o
	$(CC) $(FLEX_LD_FLAG) $^ -o $@

../util/util.o ../pipeline/plObject.o:
	cd $(dir $@) && make $(notdir $@)

scanner.o: scanner.c parser.tab.h ast.h marker.h ../util/plUtil.h ../util/plError.h ../pipeline/plObject.h
	$(CC) $(INCLUDES) -c $<

scanner.c: scanner.l
	flex -d -o $@ $<

nameTable.o: nameTable.c nameTable.h ../util/plUtil.h
	$(CC) $(INCLUDES) -c $<

ast.o: ast.c ast.h parser.tab.h ../util/plUtil.h ../pipeline/plObject.h
	$(CC) $(INCLUDES) -c $<

parser.tab.o: parser.tab.c parser.tab.h ast.h marker.h nameTable.h ../pipeline/plObject.h
	$(CC) $(INCLUDES) -c $<

parser.tab.h parser.tab.c: parser.y
	bison -dv $<

clean:
	rm -f parser scanner.c parser.tab.h parser.tab.c parser.output *.o
