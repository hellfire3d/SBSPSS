/************************************************************
yytdefs.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Microsoft defines */
#ifdef _MSC_VER

/* version 6.0 compatibility */
#if (_MSC_VER <= 600)
#define __cdecl     _cdecl
#define __far       _far
#define __near      _near
#define __segname   _segname
#endif

/* structure alignment */
#ifdef _WIN32
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,8)
#else
#pragma pack(2)
#endif

/* stream initialisation */
#ifdef _DLL
#define YYNINITSTREAM
#endif

/* 32/16-bit modifiers */
#ifndef YYCDECL
#define YYCDECL __cdecl
#endif

/* 16-bit modifiers */
#ifndef _WIN32
#ifndef YYDCDECL
#define YYDCDECL __cdecl
#endif
#ifndef YYNEAR
#define YYNEAR __near
#endif
#ifndef YYNEARFAR
#if defined(M_I86TM) || defined(M_I86SM)
#define YYNEARFAR __near
#else
#define YYNEARFAR __far
#endif
#endif
#ifndef YYBASED_CODE
#if defined(M_I86MM) || defined(M_I86LM) || defined(M_I86HM)
#define YYBASED_CODE __based(__segname("_CODE"))
#endif
#endif
#endif

/* function prototypes */
#ifndef YYPROTOTYPE
#define YYPROTOTYPE
#endif

/* qualifiers */
#ifndef YYCONST
#define YYCONST const
#endif

#endif


/* Borland defines */
#ifdef __BORLANDC__

#if !defined(RC_INVOKED)

#if defined(__STDC__)
#pragma warn -nak
#endif

#if (__BORLANDC__) >= 0x0530
#pragma pack(push, 1)
#pragma nopackwarning
#else
#pragma option -a-
#endif  /* (__BORLANDC__) >= 0x0530 */

#pragma option -a-

#endif  /* !RC_INVOKED */

/* stream initialisation */
#ifdef _RTLDLL
#if (__BORLANDC__) >= 0x0530
#define YYNINITSTREAM
#endif
#endif

/* 32/16-bit modifiers */
#ifndef YYCDECL
#define YYCDECL __cdecl
#endif
#ifndef YYDCDECL
#define YYDCDECL __cdecl
#endif

/* 16-bit modifiers */
#ifndef __WIN32__
#ifndef YYNEAR
#define YYNEAR __near
#endif
#ifndef YYFAR
#ifdef __DLL__
#define YYFAR __far
#endif
#endif
#ifndef YYNEARFAR
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#define YYNEARFAR __near
#else
#define YYNEARFAR __far
#endif
#endif
#endif

/* function prototypes */
#ifndef YYPROTOTYPE
#define YYPROTOTYPE
#endif

/* qualifiers */
#ifndef YYCONST
#define YYCONST const
#endif

#endif

#ifdef __cplusplus
};
#endif
