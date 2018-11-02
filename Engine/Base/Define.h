#include <stdint.h>


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
#define NO_ERROR                       0
#endif

#ifndef _MAX_STRING
#define _MAX_STRING                    260
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

#ifndef INVALID_VALUE
#define INVALID_VALUE                  0xffffffff
#endif

#ifndef ASSERT
#define ASSERT                         assert
#endif

#ifndef max
#define max(a,b)                       (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)                       (((a) < (b)) ? (a) : (b))
#endif

#ifndef RGB
#define RGB(r,g,b)                     ((COLORREF)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))
#endif

#ifndef RGBA
#define RGBA(r,g,b,a)                  ((COLORREF)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16))|(((uint32_t)(uint8_t)(a))<<24))
#endif

#ifndef GET_RED
#define GET_RED(color)                 (uint8_t)((color)  &  0xff)
#endif

#ifndef GET_GRN
#define GET_GRN(color)                 (uint8_t)(((color) >> 0x08) & 0xff)
#endif

#ifndef GET_BLU
#define GET_BLU(color)                 (uint8_t)(((color) >> 0x10) & 0xff)
#endif

#define SWAP(a, b, tmp)                { (tmp) = (a); (a) = (b); (b) = (tmp); }

#define ALIGN_BYTE(a, b)               ((((a) + (b) - 1) / (b)) * (b))
#define ALIGN_4BYTE(a)                 ALIGN_BYTE(a, 4)
#define ALIGN_16BYTE(a)                ALIGN_BYTE(a, 16)
#define ALIGN_1KBYTE(a)                ALIGN_BYTE(a, 1024)
#define ALIGN_4KBYTE(a)                ALIGN_BYTE(a, 4096)
