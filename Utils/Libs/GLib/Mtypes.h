/* Ä--------------------------------------------------------------------------
	File:			MTYPES.H

	Notes:			Machine dependant typedefs for msvc

	Target:			PC MSVC
	
	Author:			G Robert Liddon @ Cliffs
	Created:		18th February 1996

	Copyright (C) 1997 Gary Liddon

	All rights reserved. 
  --------------------------------------------------------------------------- */

#ifndef	__MTYPES_H__
#define	__MTYPES_H__

/* ---------------------------------------------------------------------------
	Includes
	-------- */

/* ---------------------------------------------------------------------------
	Defines
	------- */
#define	__GLIB_No64__
#define	__GLIB_BOOLAsInt__


#ifdef __GL_WIN32DLL__
#define GLIB_API __declspec(dllexport)
#endif

/* ---------------------------------------------------------------------------
	Typedefs
	-------- */
typedef signed char			s8;
typedef signed short		s16;
typedef signed long int		s32;
typedef double long			s64;

typedef unsigned char		u8;
typedef unsigned short int	u16;
typedef unsigned long int	u32;

/* --------------------------------------------------------------------------- */
#endif	/* __MTYPES_H__ */
/* ---------------------------------------------------------------------------
	ends */
