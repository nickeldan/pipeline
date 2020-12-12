CC ?= gcc
debug ?= no
level ?= 3

INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include
ACTUAL_INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include/vasq

COMPILER_FLAGS := -std=gnu11 -fdiagnostics-color -Wall -Wextra -DVASQ_ENABLE_LOGGING
ifeq ($(debug),yes)
	COMPILER_FLAGS += -O0 -g -DDEBUG -DLL_USE=$(level)
else
	COMPILER_FLAGS += -O3 -DNDEBUG -DLL_USE=-1
endif

BINARIES := scanner_check parser_check

.PHONY: all clean FORCE

all: $(BINARIES)

%_check: %_check.o compiler/libplcompiler.a pipeline/libpipeline.a vanilla_squad/libvanillasquad.a
	$(CC) -o $@ $^

%.o: %.c $(patsubst %, %/*.h, $(ACTUAL_INCLUDE_DIRS))
	$(CC) $(COMPILER_FLAGS) $(patsubst %, -I%, $(INCLUDE_DIRS)) -c $<

%.a: FORCE
	cd $(dir $@) && make $(notdir $@) CC=$(CC) debug=$(debug) level=$(level)

clean:
	rm -f $(BINARIES) *.o
	cd pipeline && make clean
	cd compiler && make clean
	cd vanilla_squad && make clean
