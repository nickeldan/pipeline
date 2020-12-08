CC ?= gcc
debug ?= no

VASQ_DIR := ../../vanilla_squad

COMPILER_FLAGS := -std=gnu11 -fpic -fdiagnostics-color -Wall -Wextra
ifeq ($(debug),yes)
	COMPILER_FLAGS += -O0 -g -DDEBUG -DVASQ_ENABLE_LOGGING
else
	COMPILER_FLAGS += -O3 -DNDEBUG
endif

LIBNAME := pipeline
OBJECT_FILES := plObject.o

.PHONY: all clean

all: lib$(LIBNAME).so lib$(LIBNAME).a

lib$(LIBNAME).so: $(OBJECT_FILES)
	$(CC) -shared -o $@ $^

lib$(LIBNAME).a: $(OBJECT_FILES)
	ar rcs $@ $^

%.o: %.c *.h $(VASQ_DIR)/include/vasq/*.h
	$(CC) $(COMPILER_FLAGS) -I$(VASQ_DIR)/include -c $<

clean:
	rm -f lib$(LIBNAME).so lib$(LIBNAME).a $(OBJECT_FILES)
