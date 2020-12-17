CC ?= gcc
level ?= -1

LIBNAME := plcompiler
INCLUDE_DIRS := ../pipeline ../vanilla_squad/include
ACTUAL_INCLUDE_DIRS := ../pipeline ../vanilla_squad/include/vasq

COMPILER_FLAGS := -std=gnu11 -fpic -ffunction-sections -fdiagnostics-color -Wall -Wextra -DVASQ_ENABLE_LOGGING -DLL_USE=$(level)
ifeq ($(level),-1)
	COMPILER_FLAGS += -O3 -DNDEBUG
else
	COMPILER_FLAGS += -O0 -g -DDEBUG
endif

SHARED_LIBRARY := lib$(LIBNAME).so
STATIC_LIBRARY := lib$(LIBNAME).a
OBJECT_FILES := nameTable.o scanner.o ast.o parser.o

.PHONY: all clean

all: $(SHARED_LIBRARY) $(STATIC_LIBRARY)

$(SHARED_LIBRARY): $(OBJECT_FILES)
	$(CC) -shared -o $@ $^

$(STATIC_LIBRARY): $(OBJECT_FILES)
	ar rcs $@ $^

%.o: %.c *.h $(patsubst %, %/*.h, $(ACTUAL_INCLUDE_DIRS))
	$(CC) $(COMPILER_FLAGS) $(patsubst %, -I%, $(INCLUDE_DIRS)) -c $<

clean:
	rm -f $(SHARED_LIBRARY) $(STATIC_LIBRARY) $(OBJECT_FILES)
