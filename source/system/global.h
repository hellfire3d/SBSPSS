/**********************/
/*** Global Defines ***/
/**********************/

#ifndef _GLOBAL_HEADER_
#define _GLOBAL_HEADER_

#include 	<stddef.h>
#include 	<stdlib.h>
#include 	<stdio.h>
#include 	<strings.h>
#include 	<libetc.h>
#include 	<libgte.h>
#include 	<libgpu.h>
#include 	<libsn.h>
#include 	<libcd.h>
#include 	<libspu.h>
#include 	<libapi.h>
#include 	<inline_c.h>
#include 	<sys\types.h>
#include 	"utils\replace.h"
#include 	<gtemac.h>

/*****************************************************************************/
typedef signed char			s8;
typedef signed short		s16;
typedef signed long			s32;
typedef long long			s64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;

typedef s8					S8;
typedef s16					S16;
typedef s32					S32;
typedef s64					S64;

typedef u8					U8;
typedef u16					U16;
typedef u32					U32;
typedef u64					U64;

/*****************************************************************************/
#define SCRATCH_RAM 		0x1f800000
#define	FAST_STACK			(SCRATCH_RAM+0x3f0)

enum
{
	FRACTION_SHIFT	= 8,
};

enum DRAW_TYPE
{
	DT_NORMAL,
	DT_TRANSPARENT,
	DT_SUBTRACT,
	DT_ADDITION,
	DT_MULTIPLY,
};


/*****************************************************************************/
#include 	"mem\memory.h"
#include 	"system\gte.h"
#include	"utils\cmxmacro.h"
#include 	"utils\fixed.h"

#include 	"system\dbg.h"
#include	"system\info.h"
#include	"system\lnkopt.h"

/*****************************************************************************/

#endif
