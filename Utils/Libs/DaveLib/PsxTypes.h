/*****************/
/*** Psx Types ***/
/*****************/

#ifndef	__PSX_TYPES_HEADER__
#define	__PSX_TYPES_HEADER__

//***************************************************************************
typedef signed char				s8;
typedef signed short            s16;
typedef signed long				s32;
//typedef long long				s64;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned long			u32;
//typedef unsigned long long		u64;

typedef s8						S8;
typedef s16						S16;
typedef s32						S32;
//typedef s64						S64;

typedef u8						U8;
typedef u16						U16;
typedef u32						U32;
//typedef u64						U64;

typedef	unsigned char		u_char;
typedef	unsigned short		u_short;
typedef	unsigned long		u_long;
typedef	u_char				u8;
typedef	u_short				u16;
typedef	u_long				u32;

//***************************************************************************

#define	NO_PROTOTYPE	1
#include	"..\..\..\tools\psyq\include\libgte.h"

#define	GPU_PolyF3Tag		(4)
#define	GPU_PolyF3Code		(0x20)
#define	GPU_PolyF4Tag		(5)
#define	GPU_PolyF4Code		(0x28)
#define	GPU_PolyFT3Tag		(7)
#define	GPU_PolyFT3Code		(0x24)
#define	GPU_PolyFT4Tag		(9)
#define	GPU_PolyFT4Code		(0x2c)
#define	GPU_PolyG4Tag		(8)
#define	GPU_PolyG4Code		(0x38)
#define	GPU_PolyGT3Tag		(9)
#define	GPU_PolyGT3Code		(0x34)
#define	GPU_PolyGT4Tag		(12)
#define	GPU_PolyGT4Code		(0x3c)

#define	GPUCode_ShadeTex	(1<<0)	// Setting this **DISABLES** texture shading.
#define	GPUCode_SemiTrans	(1<<1)	// Setting this enables semi-transparent mode
#define	GPUCode_Textured	(1<<2)
#define	GPUCode_Quad		(1<<3)
#define	GPUCode_Gouraud		(1<<4)
#define	GPUCode_Global		(1<<5)

#endif