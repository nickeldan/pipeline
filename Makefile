include scripts/this.mk

CC ?= gcc
level ?= -1
debug ?= no

CFLAGS := -std=gnu11 -fdiagnostics-color -Wall -Wextra -DLL_USE=$(level) -DVASQ_ALLOW_PLACEHOLDER -DVASQ_WARN_PLACEHOLDER
ifeq ($(debug),yes)
	CFLAGS += -O0 -g -DDEBUG
else
	CFLAGS += -O2 -DNDEBUG
endif

all: _all

VASQ_DIR := ./vanilla_squad
include vanilla_squad/make.mk

UTIL_DIR := ./util
include util/make.mk

VM_DIR := ./vm
include vm/make.mk

COMPILER_DIR := ./compiler
include compiler/make.mk

ROOT_EXTERNAL_DIRS := $(patsubst %,$(COMPILER_DIR)/%,$(COMPILER_SUBDIRS)) $(VM_DIR) $(UTIL_DIR) $(VASQ_INCLUDE_DIR)
$(eval $(call build_dir,ROOT,.,,$(ROOT_EXTERNAL_DIRS)))

BINARIES := scanner_check parser_check
STATIC_LIBRARIES := $(COMPILER_STATIC_LIBRARIES) $(VM_STATIC_LIBRARY) $(UTIL_STATIC_LIBRARY) $(VASQ_STATIC_LIBRARY)

.PHONY: all _all staticlibs clean $(CLEAN_TARGETS)

_all: $(BINARIES)

staticlibs: $(STATIC_LIBRARIES)

%_check: %_check.o $(STATIC_LIBRARIES)
	$(CC) -o $@ $^

clean: $(CLEAN_TARGETS)
	rm -f $(BINARIES) $(DEPS_FILES)
