#pragma once


#ifdef PLATFORM_WINDOWS

#include <io.h>
#include <windows.h>

#elif PLATFORM_ANDROID

#include <dirent.h>
#include <sys/time.h>

#include <jni.h>
#include <string.h>
#include <android/log.h>

#elif PLATFORM_IOS

#include <sys/time.h>

#else

#pragma message("Not support platform !!!")

#endif


#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#ifdef HAVE_EASTL

#include <eastl/string.h>
#include <eastl/map.h>
#include <eastl/vector.h>
#include <eastl/unordered_map.h>

#else

#define eastl std

#endif


#include "Define.h"
