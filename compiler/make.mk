COMPILER_SUBDIRS := parser generator
COMPILER_SUBDIRS_LINK_ORDER := generator parser
COMPILER_SHARED_LIBRARIES :=
COMPILER_STATIC_LIBRARIES :=

COMPILER_EXTERNAL_DIRS := $(VM_DIR) $(UTIL_DIR) $(VASQ_INCLUDE_DIR)

define build_compiler_subdir
ARG1 := COMPILER_$$(shell echo $(1) | tr a-z A-Z)
ARG2 := $(COMPILER_DIR)/$(1)
ARG3 := plcompiler$(1)

$$(eval $$(call build_dir_with_lib,$$(ARG1),$$(ARG2),$$(ARG3),$$(COMPILER_EXTERNAL_DIRS)))

COMPILER_EXTERNAL_DIRS := $$(ARG2) $$(COMPILER_EXTERNAL_DIRS)
COMPILER_STATIC_LIBRARIES := $$($$(THIS)_STATIC_LIBRARY) $$(COMPILER_STATIC_LIBRARIES)
endef

$(foreach subdir,$(COMPILER_SUBDIRS),$(eval $(call build_compiler_subdir,$(subdir))))
