#ifndef __BASETYPES_H__
#define __BASETYPES_H__

#ifndef	s32
typedef long s32;
#endif

#ifndef	u32
typedef unsigned long u32;
#endif

#ifndef	s16
typedef short s16;
#endif

#ifndef	u16
typedef unsigned short u16;
#endif

#ifndef	s8
//typedef char s8;
#endif

#ifndef	u8
typedef unsigned char u8;
#endif

#ifndef	byte
typedef u8 byte;
#endif

// Versions with capitals which are equivalent to the above.

#ifndef	U32
typedef unsigned long		U32;
#endif
#ifndef	U16
typedef unsigned short int	U16;
#endif
#ifndef	U8
typedef unsigned char		U8;
#endif
#ifndef	S32
typedef signed long			S32;
#endif

typedef float real;
typedef double dblreal;

#ifndef NULL
#define NULL 0
#endif

#endif