#############################################
# module : GfxRenderer
# desc : auto gen by hjj
#############################################


LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/ENGINE_SRC_FILES.mk

MODULE_GfxRenderer_INCLUDES := \
		$(LOCAL_PATH)/../../ThirdParty                      \
		$(LOCAL_PATH)/../../ThirdParty/eastl                \
		$(LOCAL_PATH)/../../ThirdParty/event                \
		$(LOCAL_PATH)/../../ThirdParty/gli                  \
		$(LOCAL_PATH)/../../ThirdParty/glm                  \
		$(LOCAL_PATH)/../../ThirdParty/lodepng              \
		$(LOCAL_PATH)/../../ThirdParty/spirv-cross          \
		$(LOCAL_PATH)/../../ThirdParty/tinyxml              \
		$(LOCAL_PATH)/../../ThirdParty/rbtree               \
		$(LOCAL_PATH)/../../Engine                          \
		$(LOCAL_PATH)/../../Engine/Base                     \
		$(LOCAL_PATH)/../../Engine/Base/nvwa                \
		$(LOCAL_PATH)/../../Engine/Memory                   \
		$(LOCAL_PATH)/../../Engine/Renderer                 \
		$(LOCAL_PATH)/../../Engine/Renderer/GLES31          \
		$(LOCAL_PATH)/../../Engine/RenderSolution           \
		$(LOCAL_PATH)/../../Engine/RenderSolution/Task      \
		$(LOCAL_PATH)/../../Engine/Resource                 \
		$(LOCAL_PATH)/../../Engine/Scene                    \
		$(LOCAL_PATH)/../../Engine/Scene/Component          \
		$(LOCAL_PATH)/../../Engine/Scene/Task                
		
MODULE_GfxRenderer_LDLIBS := \
	-lz \
	-lc \
	-llog \
	-pthread \
	-lGLESv3 \


MODULE_GfxRenderer_CPP_FEATURES := \
	exception

MODULE_GfxRenderer_C_FLAGS := \
	-W -O0 \
	-DUSE_FILE32API \
	-Wno-psabi \
	-fpermissive \
	-DANDROID \
	-DPLATFORM_ANDROID \
	-DCORE_EXPORTS \
	-std=c++14 \
	-x c++

#############################################
# shared module : GfxRenderer
#############################################
include $(CLEAR_VARS)

NDK_TOOLCHAIN_VERSION := clang

LOCAL_MODULE := GfxRenderer

LOCAL_SRC_FILES := $(ENGINE_SRC_FILES)

LOCAL_EXPORT_C_INCLUDES := $(MODULE_GfxRenderer_INCLUDES)

LOCAL_C_INCLUDES := $(MODULE_GfxRenderer_INCLUDES)

LOCAL_LDLIBS := $(MODULE_GfxRenderer_LDLIBS)

LOCAL_EXPORT_LDLIBS := $(MODULE_GfxRenderer_LDLIBS)

LOCAL_STATIC_LIBRARIES := shaderc

LOCAL_WHOLE_STATIC_LIBRARIES := $(MODULE_GfxRenderer_STATIC_LIBS)

#LOCAL_CPP_FEATURES := $(MODULE_GfxRenderer_CPP_FEATURES)

LOCAL_CFLAGS := $(MODULE_GfxRenderer_C_FLAGS)

LOCAL_CFLAGS  += -DTHREAD_COUNT=4 -D_ANDROID

LOCAL_CPPFLAGS := $(MODULE_GfxRenderer_C_FLAGS)

LOCAL_EXPORT_CFLAGS := $(MODULE_GfxRenderer_C_FLAGS)

LOCAL_EXPORT_CPPFLAGS := $(MODULE_GfxRenderer_C_FLAGS)

LOCAL_ARM_NEON := true

LOCAL_ARM_MODE := arm

TARGET_ARCH := arm

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

APP_CPPFLAGS += -fexceptions

include $(BUILD_SHARED_LIBRARY)

$(call import-module, third_party/shaderc)
