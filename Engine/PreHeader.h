#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#ifdef PLATFORM_WINDOWS
#  include <io.h>
#  include <windows.h>
#elif PLATFORM_ANDROID
#  include <dirent.h>
#  include <sys/time.h>
#  include <sys/sysconf.h>
#  include <jni.h>
#  include <string.h>
#  include <android/log.h>
#elif PLATFORM_IOS
#  include <sys/time.h>
#  include <sys/sysconf.h>
#else
#  pragma message("Not support platform !!!")
#endif


#include <algorithm>
#include <atomic>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#ifdef HAVE_EASTL
#  include <EASTL/string.h>
#  include <EASTL/map.h>
#  include <EASTL/vector.h>
#  include <EASTL/unordered_map.h>
#else
#  define eastl std
#endif


#include "Define.h"
