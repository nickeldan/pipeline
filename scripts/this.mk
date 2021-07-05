DEPS_FILES :=
CLEAN_TARGETS :=

define this_template
THIS := $(1)
$$(THIS)_DIR := $(2)
$$(THIS)_LIBNAME := $(3)
$$(THIS)_EXTERNAL_DIRS := $(4)

LITTLE_THIS := $$(shell echo $$(THIS) | tr A-Z a-z)

ifneq ($$($$(THIS)_LIBNAME),)
    $$(THIS)_SHARED_LIBRARY := $$($$(THIS)_DIR)/lib$$($$(THIS)_LIBNAME).so
    $$(THIS)_STATIC_LIBRARY := $$($$(THIS)_DIR)/lib$$($$(THIS)_LIBNAME).a
endif

$$(THIS)_SOURCE_FILES := $$(wildcard $$($$(THIS)_DIR)/*.c)
$$(THIS)_OBJECT_FILES := $$(patsubst %.c,%.o,$$($$(THIS)_SOURCE_FILES))
$$(THIS)_HEADER_FILES := $$(wildcard $$($$(THIS)_DIR)/*.h)

$$(THIS)_DEPS_FILE := $$($$(THIS)_DIR)/deps.mk
DEPS_FILES += $$($$(THIS)_DEPS_FILE)

CLEAN_TARGETS += $$(LITTLE_THIS)_clean

$$(THIS)_EXTERNAL_HEADER_FILES := $$(foreach dir,$$($$(THIS)_EXTERNAL_DIRS),$$(shell find $$(dir) -name '*.h'))
$$(THIS)_INCLUDE_FLAGS := $$(patsubst %,-I%,$$($$(THIS)_EXTERNAL_DIRS))

$$($$(THIS)_DEPS_FILE): T := $$(THIS)
$$($$(THIS)_DEPS_FILE): $$($$(THIS)_SOURCE_FILES) $$($$(THIS)_HEADER_FILES) $$($$(THIS)_EXTERNAL_HEADER_FILES)
	rm -f $$@
	for file in $$($$(T)_SOURCE_FILES); do \
	    echo "$$($$(T)_DIR)/`$$(CC) $$($$(T)_INCLUDE_FLAGS) -MM $$$$file`" >> $$@ && \
	    echo '\t$$$$(CC) $$$$(CFLAGS) -fpic -ffunction-sections $$($$(T)_INCLUDE_FLAGS) -c $$$$< -o $$$$@' >> $$@; \
	done
include $$($$(THIS)_DEPS_FILE)

$$(LITTLE_THIS)_clean: T := $$(THIS)
$$(LITTLE_THIS)_clean:
	rm -f $$($$(T)_OBJECT_FILES) $$($$(T)_SHARED_LIBRARY) $$($$(T)_STATIC_LIBRARY)
endef

define libraries_template
THIS := $(1)

$$($$(THIS)_SHARED_LIBRARY): $$($$(THIS)_OBJECT_FILES)
	$$(CC) -shared -o $$@ $$^

$$($$(THIS)_STATIC_LIBRARY): $$($$(THIS)_OBJECT_FILES)
	$$(AR) rcs $$@ $$^
endef

define build_dir
$$(eval $$(call this_template,$(1),$(2),$(3),$(4)))
endef

define build_dir_with_libs
$$(eval $$(call this_template,$(1),$(2),$(3),$(4)))
$$(eval $$(call libraries_template,$$(THIS)))
endef
