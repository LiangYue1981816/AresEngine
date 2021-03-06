#ifdef PLATFORM_WINDOWS
#define CALL_API                       __declspec(dllexport)
#else
#define CALL_API
#endif


#ifdef __LP64__
#define POINTER_SIZE                   8         // 8BYTE
#else
#define POINTER_SIZE                   4         // 4BYTE
#endif


#ifndef NO_ERROR
#define NO_ERROR                       0L
#endif

#ifndef _MAX_STRING
#define _MAX_STRING                    1024
#endif

#ifndef FLT_MAX
#define FLT_MAX                        3.402823466e+38F
#endif

#ifndef FLT_MIN
#define FLT_MIN                        1.175494351e-38F
#endif

#ifndef INVALID_HASHNAME
#define INVALID_HASHNAME               0xffffffff
#endif

#ifndef INVALID_HASHVALUE
#define INVALID_HASHVALUE              0xffffffff
#endif

#ifndef INVALID_VALUE
#define INVALID_VALUE                  0xffffffff
#endif

#ifndef ASSERT
#  if defined (_DEBUG) || defined (DEBUG)
#    define ASSERT                     assert
#  else
#    define ASSERT
#  endif
#endif

#define ASSERT_TEXT(expression, tag, format,  ...) \
	do {                                           \
		if ((!!(expression)) == false) {           \
			LogOutput(tag, format, ##__VA_ARGS__); \
			ASSERT(false);                         \
		}                                          \
	} while(false)

#define ALIGN_BYTE(a, b)               ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)                 ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)                ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)                ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)                ALIGN_BYTE(a, 4096)

#define LOG_TAG_MEMORY                 "Memory"
#define LOG_TAG_RENDERER               "GfxRenderer"

#define MAX_THREAD_COUNT               32


#define CALL_BOOL_FUNCTION_ASSERT(func)          \
{                                                \
	bool err = func;                             \
	ASSERT(err);                                 \
}

#define CALL_BOOL_FUNCTION_BREAK(func)           \
{                                                \
	bool err = func;                             \
	if (err != true) {                           \
		ASSERT(false);                           \
		break;                                   \
	}                                            \
}

#define CALL_BOOL_FUNCTION_RETURN(func)          \
{                                                \
	bool err = func;                             \
	if (err != true) {                           \
		ASSERT(false);                           \
		return;                                  \
	}                                            \
}

#define CALL_BOOL_FUNCTION_RETURN_BOOL(func)     \
{                                                \
	bool err = func;                             \
	if (err != true) {                           \
		ASSERT(false);                           \
		return false;                            \
	}                                            \
}

#define CALL_BOOL_FUNCTION_RETURN_NULLPTR(func)  \
{                                                \
	bool err = func;                             \
	if (err != true) {                           \
		ASSERT(false);                           \
		return nullptr;                          \
	}                                            \
}
