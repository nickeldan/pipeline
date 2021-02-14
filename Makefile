CC ?= gcc
level ?= -1
verbose ?= no
debug ?= no

CFLAGS := -std=gnu11 -fdiagnostics-color -Wall -Wextra -DLL_USE=$(level)
ifeq ($(verbose),yes)
	CFLAGS += -DPL_FULL_LOGGING
endif
ifeq ($(debug),yes)
	CFLAGS += -O0 -g -DDEBUG
else
	CFLAGS += -O3 -DNDEBUG
endif

all: _all

VASQ_DIR := ./vanilla_squad
include vanilla_squad/make.mk

PLOBJECT_DIR := ./plobject
include plobject/make.mk

COMPILER_DIR := ./compiler
include compiler/make.mk

BINARIES := scanner_check parser_check
SHARED_LIBRARIES := $(COMPILER_SHARED_LIBRARY) $(PLOBJECT_SHARED_LIBRARY) $(VASQ_SHARED_LIBRARY)
STATIC_LIBRARIES := $(COMPILER_STATIC_LIBRARY) $(PLOBJECT_STATIC_LIBRARY) $(VASQ_STATIC_LIBRARY)

.PHONY: all _all sharedlibs staticlibs clean $(CLEAN_TARGETS)

_all: $(BINARIES)

sharedlibs: $(SHARED_LIBRARIES)

staticlibs: $(STATIC_LIBRARIES)

%_check: %_check.o $(STATIC_LIBRARIES)
	$(CC) -o $@ $^

%_check.o: %_check.c $(COMPILER_HEADER_FILES) $(PLOBJECT_HEADER_FILES) $(VASQ_HEADER_FILES)
	$(CC) $(CFLAGS) -I$(COMPILER_DIR) -I$(PLOBJECT_DIR) -I$(VASQ_INCLUDE_DIR) -c $< -o $@

clean: $(CLEAN_TARGETS)
	rm -f $(BINARIES)
