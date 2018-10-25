APP_ABI := armeabi-v7a

APP_OPTIM := release

APP_PLATFORM := android-24

APP_STL := c++_static

APP_CPPFLAGS := -fexceptions -Wno-psabi -Wno-error=format-security -fpermissive -DANDROID -DCORE_EXPORTS -DGLEW_NO_GLU
APP_CPPFLAGS += -std=c++14
APP_CPPFLAGS += -w

APP_MODULES := GfxRenderer

