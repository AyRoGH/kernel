#ifndef __INCLUDE__TYPES_H__
#define __INCLUDE__TYPES_H__

#define S8_MIN  ((s8)-128)
#define S8_MAX  ((s8)127)
#define S16_MIN ((s16)-32768)
#define S16_MAX ((s16)32767)
#define S32_MIN ((s32)-2147483648)
#define S32_MAX ((s32)2147483647)
#define S64_MIN ((s64)-9223372036854775808L)
#define S64_MAX ((s64)9223372036854775807L)

#define U8_MIN  ((u8)0)
#define U8_MAX  ((u8)255)
#define U16_MIN ((u16)0)
#define U16_MAX ((u16)65535)
#define U32_MIN ((u32)0)
#define U32_MAX ((u32)4294967295U)
#define U64_MIN ((u64)0)
#define U64_MAX ((u64)18446744073709551615UL)

#define F32_MIN  ((f32)-3.402823466e+38F)
#define F32_MAX  ((f32)3.402823466e+38F)
#define F64_MIN  ((f64)-1.7976931348623157e+308)
#define F64_MAX  ((f64)1.7976931348623157e+308)
#define F80_MIN  ((f80)-1.1897314953572317650212638530309737165e+4932L)
#define F80_MAX  ((f80)1.1897314953572317650212638530309737165e+4932L)

#define NULL ((void *)0)

typedef char    s8;
typedef short   s16;
typedef int     s32;

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

typedef float       f32;
typedef double      f64;
typedef long double f80;

/* Architecture-specific types */
#ifdef __x86_64__
    typedef long            s64;
    typedef unsigned long   u64;
    typedef u64             uintptr_t;
    typedef s64             intptr_t;
    typedef u64             size_t;
    typedef s64             ssize_t;
#elif defined(__i386__)
    typedef long long               s64;
    typedef unsigned long long      u64;
    typedef u32                     uintptr_t;
    typedef s32                     intptr_t;
    typedef u32                     size_t;
    typedef s32                     ssize_t;
#else
    #error "Unsupported architecture"
#endif

#endif /* __INCLUDE__TYPES_H__ */