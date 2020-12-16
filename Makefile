CC ?= gcc
level ?= -1

INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include
ACTUAL_INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include/vasq

COMPILER_FLAGS := -std=gnu11 -fdiagnostics-color -Wall -Wextra -DVASQ_ENABLE_LOGGING -DLL_USE=$(level)
ifeq ($(level),-1)
	COMPILER_FLAGS += -O3 -DNDEBUG
else
	COMPILER_FLAGS += -O0 -g -DDEBUG
endif

BINARIES := scanner_check parser_check

.PHONY: all clean FORCE

all: $(BINARIES)

%_check: %_check.o compiler/libplcompiler.a pipeline/libpipeline.a vanilla_squad/libvanillasquad.a
	$(CC) -o $@ $^

%.o: %.c $(patsubst %, %/*.h, $(ACTUAL_INCLUDE_DIRS))
	$(CC) $(COMPILER_FLAGS) $(patsubst %, -I%, $(INCLUDE_DIRS)) -c $<

%.a: FORCE
	cd $(dir $@) && make $(notdir $@) CC=$(CC) level=$(level)

clean:
	rm -f $(BINARIES) *.o
	cd pipeline && make clean
	cd compiler && make clean
	cd vanilla_squad && make clean
