COMPILER_LIBNAME := plcompiler
COMPILER_SHARED_LIBRARY := $(COMPILER_DIR)/lib$(COMPILER_LIBNAME).so
COMPILER_STATIC_LIBRARY := $(COMPILER_DIR)/lib$(COMPILER_LIBNAME).a

COMPILER_OBJECT_FILES := $(patsubst %.c,%.o,$(wildcard $(COMPILER_DIR)/*.c))
COMPILER_HEADER_FILES := $(wildcard $(COMPILER_DIR)/*.h)

CLEAN_TARGETS += compiler_clean

$(COMPILER_SHARED_LIBRARY): $(COMPILER_OBJECT_FILES)
	$(CC) -shared -o $@ $^

$(COMPILER_STATIC_LIBRARY): $(COMPILER_OBJECT_FILES)
	ar rcs $@ $^

$(COMPILER_DIR)/%.o: $(COMPILER_DIR)/%.c $(COMPILER_HEADER_FILES) $(PLOBJECT_HEADER_FILES) $(VASQ_HEADER_FILES)
	$(CC) $(CFLAGS) -fpic -ffunction-sections -I$(PLOBJECT_DIR) -I$(VASQ_INCLUDE_DIR) -c $< -o $@

compiler_clean:
	rm -f $(COMPILER_SHARED_LIBRARY) $(COMPILER_STATIC_LIBRARY) $(COMPILER_OBJECT_FILES)
