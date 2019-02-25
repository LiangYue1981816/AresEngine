THIRD_PARTY_PATH := $(call my-dir)

# Set the location of glslang
ifeq ($(GLSLANG_LOCAL_PATH),)
	GLSLANG_LOCAL_PATH:=$(THIRD_PARTY_PATH)/glslang
endif
include $(GLSLANG_LOCAL_PATH)/Android.mk

# Set the location of SPIRV-Tools.
# Allow the user to override it, but default it to under our third_party directory.
ifeq ($(SPVTOOLS_LOCAL_PATH),)
  SPVTOOLS_LOCAL_PATH:=$(THIRD_PARTY_PATH)/spirv-tools
endif
ifeq ($(SPVHEADERS_LOCAL_PATH),)
   # Use the third party dir if it exists.
   ifneq ($(wildcard $(THIRD_PARTY_PATH)/spirv-headers/include/spirv/spirv.xml),)
     SPVHEADERS_LOCAL_PATH:=$(THIRD_PARTY_PATH)/spirv-headers
   else
     # Let SPIRV-Tools find its own headers and hope for the best.
   endif
endif

# Now include the SPIRV-Tools dependency
include $(SPVTOOLS_LOCAL_PATH)/Android.mk
