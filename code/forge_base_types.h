#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#ifdef _WIN32
#define ASSERT_BREAK() (__debugbreak())
#elif __GNUC__
#define ASSERT_BREAK() (__builtin_trap())
#endif

#define STATIC_ASSERT_BREAK(expr) _Static_assert((expr), "(" #expr ") failed")

#if defined(FR_ENABLE_ASSERT)
#define ASSERT(expr) if (expr){ LOG_FATAL(#expr); ASSERT_BREAK(); }
#define STATIC_ASSERT(expr) STATIC_ASSERT_BREAK(expr)
#else
#define ASSERT(expr)
#define STATIC_ASSERT(expr)
#endif

#define internal     static
#define global       static
#define local        static
#define global_const static const
#define local_const  static const

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8       b8;
typedef u8       b16;
typedef u32      b32;
typedef u64      b64;

typedef float    f32;
typedef double   f64;
typedef void     VoidFunction(void);

global u8 u8_MAX   = 0xFF;
global u8 u8_MIN   = 0;

global u16 u16_MAX = 0xFFFF;
global u16 u16_MIN = 0;

global u32 u32_MAX = 0xFFFFFFFF;
global u32 u32_MIN = 0;

global u64 u64_MAX = 0xFFFFFFFFFFFFFFFF;
global u64 u64_MIN = 0;

global i8 i8_MAX   = 0x7F;
global i8 i8_MIN   = -1 - 0x7F;

global i16 i16_MAX = 0x7FFF;
global i16 i16_MIN = -1 - 0x7FFF;

global i32 i32_MAX = 0x7FFFFFFF;
global i32 i32_MIN = -1 - 0x7FFFFFFF;

global i64 i64_MAX = 0x7FFFFFFFFFFFFFFF;
global i64 i64_MIN = -1 - 0x7FFFFFFFFFFFFFFF;

#define B(value)
#define KB(value) (value << 10)
#define MB(value) (value << 20)
#define GB(value) ((u64)(value) << 30)
#define TB(value) ((u64)(value) << 40)

#ifdef __gnu_linux__
#define FR_PLATFORM_LINUX 1
#elif _WIN32
#define FR_PLATFORM_WINDOWS 1
#else
#error missing platform detection
#endif

#ifdef FR_PLATFORM_LINUX
#define FR_PLATFORM_EXT ".so"
#elif FR_PLATFORM_WINDOWS
#define FR_PLATFORM_EXT ".dll"
#else
#error missing platform detection
#endif 

#ifdef FR_DYNAMIC_LINK_API
#define FR_API __declspec(dllexport) 
#elif  FR_STATIC_LINK_API
#define FR_API __declspec(dllimport) 
#else
#define FR_API
#endif

#define ARRAY_COUNT(a)              (sizeof(a) / sizeof(*a))

#define INT_FROM_PTR(p)             (ui64)((char*)p - (char*)0)
#define PTR_FROM_INT(n)             (void*)((char*)0 + (n))

#define MEMBER(t, m)                (((T*)0)->m)
#define OFFSET_OF_MEMBER(t, m)      INT_FROM_PTR(&MEMBER(t, m))

#define MEMORY_ZERO(p, s)           memset(p, 0, s);
#define MEMORY_ZERO_ARRAY(p, s)     MEMORY_ZERO(p, s);
#define MEMORY_ZERO_STRUCT(p, s)    MEMORY_ZERO(p, s);

#define MEMORY_COPY(a, b, s)        memcpy(a, b, s);
#define MEMORY_COPY_ARRAY(a, b, s)  MEMORY_COPY(a, b, s);
#define MEMORY_COPY_STRUCT(a, b, s) MEMORY_COPY(a, b, s);

#define MEMORY_MATCH(a, b, z)       (memcmp(a, b, sizeof(z)) == 0)

#define MEMORY_MALLOC(s) 		   malloc(s);
#define MEMORY_FREE(p)   		   free(p);

typedef struct Vector2i
{
	i32 x, y;
}Vector2i;

typedef struct Vector3i
{
	i32 x, y, z;
}Vector3i;

typedef struct Vector4i
{
	i32 x, y, z, w;
}Vector4i;

typedef struct Vector2f
{
	f32 x, y;
}Vector2f;

typedef struct Vector3f
{
	f32 x, y, z;
}Vector3f;

typedef struct Vector4f
{
	f32 x, y, z, w;
}Vector4f;

FR_API inline Vector2i v2i(i32 x, i32 y);
FR_API inline Vector3i v3i(i32 x, i32 y, i32 z);
FR_API inline Vector4i v4i(i32 x, i32 y, i32 z, i32 w);

FR_API inline Vector2f v2f(f32 x, f32 y);
FR_API inline Vector3f v3f(f32 x, f32 y, f32 z);
FR_API inline Vector4f v4f(f32 x, f32 y, f32 z, f32 w);

#endif //BASE_TYPES_H