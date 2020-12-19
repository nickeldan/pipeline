CC ?= gcc
level ?= -1

INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include
ACTUAL_INCLUDE_DIRS := ./pipeline ./compiler ./vanilla_squad/include/vasq

COMPILER_FLAGS := -std=gnu11 -fpic -ffunction-sections -fdiagnostics-color -Wall -Wextra -DVASQ_ENABLE_LOGGING -DLL_USE=$(level)
ifeq ($(level),-1)
	COMPILER_FLAGS += -O3 -DNDEBUG
else
	COMPILER_FLAGS += -O0 -g -DDEBUG
endif

COMPILER_LIBNAME := plcompiler
COMPILER_OBJECT_FILES := compiler/nameTable.o compiler/scanner.o compiler/ast.o compiler/parser.o

PIPELINE_LIBNAME := pipeline
PIPELINE_OBJECT_FILES := pipeline/plObject.o pipeline/plUtil.o

VASQ_LIBNAME := vanillasquad

LIBNAMES := $(COMPILER_LIBNAME) $(PIPELINE_LIBNAME) $(VASQ_LIBNAME)

BINARIES := scanner_check parser_check
SHARED_LIBRARIES := $(patsubst %,lib/lib%.so,$(LIBNAMES))
STATIC_LIBRARIES := $(patsubst %,lib/lib%.a,$(LIBNAMES))

.PHONY: all clean sharedlibs staticlibs FORCE

all: $(BINARIES)

sharedlibs: $(SHARED_LIBRARIES)

staticlibs: $(STATIC_LIBRARIES)

%_check: %_check.o $(STATIC_LIBRARIES)
	$(CC) -o $@ $^

%.o: %.c $(patsubst %, %/*.h, $(ACTUAL_INCLUDE_DIRS))
	$(CC) $(COMPILER_FLAGS) $(patsubst %, -I%, $(INCLUDE_DIRS)) -c $< -o $@

lib/lib$(COMPILER_LIBNAME).so: $(COMPILER_OBJECT_FILES) lib
	$(CC) -shared -o $@ $(COMPILER_OBJECT_FILES)

lib/lib$(COMPILER_LIBNAME).a: $(COMPILER_OBJECT_FILES) lib
	ar rcs $@ $(COMPILER_OBJECT_FILES)

lib/lib$(PIPELINE_LIBNAME).so: $(PIPELINE_OBJECT_FILES) lib
	$(CC) -shared -o $@ $(PIPELINE_OBJECT_FILES)

lib/lib$(PIPELINE_LIBNAME).a: $(PIPELINE_OBJECT_FILES) lib
	ar rcs $@ $(PIPELINE_OBJECT_FILES)

lib/lib$(VASQ_LIBNAME).so lib/lib$(VASQ_LIBNAME).a: lib FORCE
	cd vanilla_squad && make $(notdir $@)
	cp vanilla_squad/$(notdir $@) $@

lib:
	mkdir $@

clean:
	rm -f $(BINARIES) *.o
	rm -rf lib
	rm -f $(COMPILER_OBJECT_FILES) $(PIPELINE_OBJECT_FILES)
	cd vanilla_squad && make clean
