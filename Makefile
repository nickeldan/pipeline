CC ?= gcc
level ?= -1
debug ?= no

INCLUDE_DIRS := ./plobject ./compiler ./vanilla_squad/include
ACTUAL_INCLUDE_DIRS := ./plobject ./compiler ./vanilla_squad/include/vasq

COMPILER_FLAGS := -std=gnu11 -fpic -ffunction-sections -fdiagnostics-color -Wall -Wextra
COMPILER_FLAGS += -DVASQ_ENABLE_LOGGING -DLL_USE=$(level)
ifeq ($(debug),yes)
	COMPILER_FLAGS += -O0 -g -DDEBUG
else
	COMPILER_FLAGS += -O3 -DNDEBUG
endif

COMPILER_LIBNAME := plcompiler
COMPILER_OBJECT_FILES := $(patsubst %.c,%.o,$(wildcard compiler/*.c))

PLOBJECT_LIBNAME := plobject
PLOBJECT_OBJECT_FILES := $(patsubst %.c,%.o,$(wildcard plobject/*.c))

_all: all

VASQ_DIR := $(PWD)/vanilla_squad
include vanilla_squad/vasq.mk

LIBNAMES := $(COMPILER_LIBNAME) $(PLOBJECT_LIBNAME) $(VASQ_LIBNAME)

BINARIES := scanner_check parser_check
SHARED_LIBRARIES := $(patsubst %,lib/lib%.so,$(LIBNAMES))
STATIC_LIBRARIES := $(patsubst %,lib/lib%.a,$(LIBNAMES))

.PHONY: _all all clean sharedlibs staticlibs $(VASQ_PHONY_TARGETS)

all: $(BINARIES)

sharedlibs: $(SHARED_LIBRARIES)

staticlibs: $(STATIC_LIBRARIES)

%_check: %_check.o $(STATIC_LIBRARIES)
	$(CC) -o $@ $^

%.o: %.c $(patsubst %, %/*.h, $(ACTUAL_INCLUDE_DIRS))
	$(CC) $(COMPILER_FLAGS) $(patsubst %,-I%,$(INCLUDE_DIRS)) -c $< -o $@

lib/lib$(COMPILER_LIBNAME).so: $(COMPILER_OBJECT_FILES) lib
	$(CC) -shared -o $@ $(COMPILER_OBJECT_FILES)

lib/lib$(COMPILER_LIBNAME).a: $(COMPILER_OBJECT_FILES) lib
	ar rcs $@ $(COMPILER_OBJECT_FILES)

lib/lib$(PLOBJECT_LIBNAME).so: $(PLOBJECT_OBJECT_FILES) lib
	$(CC) -shared -o $@ $(PLOBJECT_OBJECT_FILES)

lib/lib$(PLOBJECT_LIBNAME).a: $(PLOBJECT_OBJECT_FILES) lib
	ar rcs $@ $(PLOBJECT_OBJECT_FILES)

lib/lib$(VASQ_LIBNAME).so: $(VASQ_SHARED_LIBRARY) lib
	cp $< $@

lib/lib$(VASQ_LIBNAME).a: $(VASQ_STATIC_LIBRARY) lib
	cp $< $@

lib:
	mkdir $@

clean: vasq_clean
	rm -f $(BINARIES) *.o $(COMPILER_OBJECT_FILES) $(PLOBJECT_OBJECT_FILES)
	rm -rf lib
