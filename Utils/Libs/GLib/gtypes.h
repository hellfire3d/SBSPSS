/* ==========================================================================
	File:			GTYPES.H

	Notes:			Standard Non Machine dependant data types used by LIBGAZ

	Author:			Gary Liddon @ 73b

	Created:		Saturday 16th March 1996

	Copyright (C) 1996 - 1997 Gary Liddon
	All rights reserved. 
  =========================================================================== */

#ifndef	__GTYPES_H__
#define	__GTYPES_H__

/* ---------------------------------------------------------------------------
	Includes
	-------- */

#include "mtypes.h"

/* ---------------------------------------------------------------------------
	Defines
	------- */
#ifndef NULL

#ifdef __cplusplus

#define NULL 0

#else

#define NULL ((void *)0)

#endif /* __cplusplus */
#endif /* NULL */

#ifndef NOP
#define NOP ((void)0)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

/* ---------------------------------------------------------------------------
	Typedefs
	-------- */

#ifndef __GLIB_uint__
#define __GLIB_uint__
typedef unsigned int   uint;
#endif

#ifndef __GLIB_uchar__
#define __GLIB_uchar__
typedef unsigned char  uchar;
#endif

#ifndef __GLIB_ushort__
#define __GLIB_ushort__
typedef unsigned short ushort;
#endif

#ifndef __GLIB_ulong__
#define __GLIB_ulong__
typedef unsigned long  ulong;
#endif

#ifndef __GLIB_UBYTE__
#define __GLIB_UBYTE__
typedef u8 UBYTE;
#endif

#ifndef __GLIB_UWORD__
#define __GLIB_UWORD__
typedef u16 UWORD;
#endif



typedef uint	UINT;
typedef uchar	UCHAR;
typedef ushort	USHORT;
typedef ulong	ULONG;

#ifdef	__GLIB_BOOLAsInt__
typedef int		BOOL;
#else
typedef u8		BOOL;
#endif


/* Lower case versions are defined in mtypes.h */

typedef s8  S8;   /* signed  8-bit */
typedef s16 S16;  /* signed 16-bit */
typedef s32 S32;  /* signed 32-bit */

typedef u8  U8;   /* unsigned  8-bit */
typedef u16 U16;  /* unsigned 16-bit */
typedef u32 U32;  /* unsigned 32-bit */


/* More alternative names */

typedef s8  int8;
typedef u8  uint8;
typedef u8  byte;

typedef s16 int16;
typedef u16 uint16;
typedef u16 word;


typedef s32 int32;
typedef u32 uint32;
typedef u32 dword;


/*	64 bit names
	------------ */
#ifndef __GLIB_No64__

typedef s64		S64;
typedef u64		U64;
typedef s64		int64;
typedef u64		uint64;
typedef u64		qword;

#endif

/*	GLIB Api Defintion
	------------------ */
#ifndef GLIB_API
#define GLIB_API
#endif



/* ---------------------------------------------------------------------------
	Globals
	------- */

/* --------------------------------------------------------------------------- */
#endif	/* __GTYPES_H__ */
/* ---------------------------------------------------------------------------
	ends */

