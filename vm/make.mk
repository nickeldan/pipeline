THIS_EXTERNAL_DIRS := $(UTIL_DIR) $(VASQ_INCLUDE_DIR)
$(eval $(call build_dir_with_lib,VM,$(VM_DIR),plvm,$(THIS_EXTERNAL_DIRS)))
