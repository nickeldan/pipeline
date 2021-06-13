VM_LIBNAME := plvm
VM_SHARED_LIBRARY := $(VM_DIR)/lib$(VM_LIBNAME).so
VM_STATIC_LIBRARY := $(VM_DIR)/lib$(VM_LIBNAME).a

VM_SOURCE_FILES := $(wildcard $(VM_DIR)/*.c)
VM_OBJECT_FILES := $(patsubst %.c,%.o,$(VM_SOURCE_FILES))
VM_HEADER_FILES := $(wildcard $(VM_DIR)/*.h)
VM_INCLUDE_FLAGS := -I $(UTIL_DIR) -I$(VASQ_INCLUDE_DIR)

CLEAN_TARGETS += plvm_clean

VM_DEPS_FILE := $(VM_DIR)/deps.mk
DEPS_FILES += $(VM_DEPS_FILE)

$(VM_DEPS_FILE): $(VM_SOURCE_FILES) $(VM_HEADER_FILES) $(UTIL_HEADER_FILES) $(VASQ_HEADER_FILES)
	rm -f $@
	for file in $(VM_SOURCE_FILES); do \
	    echo "$(VM_DIR)/`$(CC) $(VM_INCLUDE_FLAGS) -MM $$file`" >> $@ && \
	    echo '\t$$(CC) $$(CFLAGS) -fpic -ffunction-sections $(VM_INCLUDE_FLAGS) -c $$< -o $$@' >> $@; \
	done
include $(VM_DEPS_FILE)

$(VM_SHARED_LIBRARY): $(VM_OBJECT_FILES)
	$(CC) -shared -o $@ $^

$(VM_STATIC_LIBRARY): $(VM_OBJECT_FILES)
	$(AR) rcs $@ $^

plvm_clean:
	rm -f $(VM_SHARED_LIBRARY) $(VM_STATIC_LIBRARY) $(VM_OBJECT_FILES)
