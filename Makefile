CC ?= gcc
debug ?= no

INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include
ACTUAL_INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include/vasq

COMPILER_FLAGS := -std=gnu11 -fdiagnostics-color -Wall -Wextra -DVASQ_ENABLE_LOGGING
ifeq ($(debug),yes)
	COMPILER_FLAGS += -O0 -g -DDEBUG
else
	COMPILER_FLAGS += -O3 -DNDEBUG
endif

.PHONY: all clean FORCE

all: token_check

token_check: token_check.o pipeline/libpipeline.a compiler/libplcompiler.a vanilla_squad/libvanillasquad.a
	$(CC) -o $@ $^

%.o: %.c $(patsubst %, %/*.h, $(ACTUAL_INCLUDE_DIRS))
	$(CC) $(COMPILER_FLAGS) $(patsubst %, -I%, $(INCLUDE_DIRS)) -c $<

%.a: FORCE
	cd $(dir $@) && make $(notdir $@) CC=$(CC) debug=$(debug)

clean:
	rm -f token_check *.o
	cd pipeline && make clean
	cd compiler && make clean
	cd vanilla_squad && make clean
