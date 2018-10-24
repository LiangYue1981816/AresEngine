#pragma once


#ifdef _WINDOWS

#include <io.h>
#include <windows.h>

#elif _ANDROID

#include <sys/time.h>

#include <jni.h>
#include <string.h>
#include <android/log.h>

#elif _IOS

#include <sys/time.h>

#else

#pragma message("Not support platform !!!")

#endif
