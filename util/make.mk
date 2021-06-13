UTIL_LIBNAME := plutil
UTIL_SHARED_LIBRARY := $(UTIL_DIR)/lib$(UTIL_LIBNAME).so
UTIL_STATIC_LIBRARY := $(UTIL_DIR)/lib$(UTIL_LIBNAME).a

UTIL_SOURCE_FILES := $(wildcard $(UTIL_DIR)/*.c)
UTIL_OBJECT_FILES := $(patsubst %.c,%.o,$(UTIL_SOURCE_FILES))
UTIL_HEADER_FILES := $(wildcard $(UTIL_DIR)/*.h)
UTIL_INCLUDE_FLAGS := -I$(VASQ_INCLUDE_DIR)

CLEAN_TARGETS += util_clean

UTIL_DEPS_FILE := $(UTIL_DIR)/deps.mk
DEPS_FILES += $(UTIL_DEPS_FILE)

$(UTIL_DEPS_FILE): $(UTIL_SOURCE_FILES) $(UTIL_HEADER_FILES) $(VASQ_HEADER_FILES)
	rm -f $@
	for file in $(UTIL_SOURCE_FILES); do \
	    echo "$(UTIL_DIR)/`$(CC) $(UTIL_INCLUDE_FLAGS) -MM $$file`" >> $@ && \
	    echo '\t$$(CC) $$(CFLAGS) -fpic -ffunction-sections $(UTIL_INCLUDE_FLAGS) -c $$< -o $$@' >> $@; \
	done
include $(UTIL_DEPS_FILE)

$(UTIL_SHARED_LIBRARY): $(UTIL_OBJECT_FILES)
	$(CC) -shared -o $@ $^

$(UTIL_STATIC_LIBRARY): $(UTIL_OBJECT_FILES)
	$(AR) rcs $@ $^

util_clean:
	rm -f $(UTIL_SHARED_LIBRARY) $(UTIL_STATIC_LIBRARY) $(UTIL_OBJECT_FILES)
