/************************************************************
yybdefs.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999 Bumble-Bee Software Ltd.
************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Microsoft defines */
#ifdef _MSC_VER

/* structure alignment */
#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif

#endif


/* Borland defines */
#ifdef __BORLANDC__

#if !defined(RC_INVOKED)

#if (__BORLANDC__) >= 0x0530
/* restore default packing */
#pragma pack(pop)
#pragma nopackwarning
#else
#pragma option -a. /* restore default packing */
#endif  /* (__BORLANDC__) >= 0x0530 */

#if defined(__STDC__)
#pragma warn .nak
#endif

#endif  /* !RC_INVOKED */

#endif

#ifdef __cplusplus
};
#endif
