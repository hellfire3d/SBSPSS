/*=========================================================================

	CROSS.CPP	

	Author:  Gary Liddon @ Climax
	Created:
	Project: Diablo Playstation Conversion
	Purpose: Find A Crosshair on a frame

	Copyright (c) 1996 Director's Cut Ltd.

===========================================================================*/

#ifndef __CROSS_HPP__
#define __CROSS_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	STL
	--- */

/*	Glib
	---- */
#include <gobject.hpp>
#include <frame.hpp>


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Description:
		This structure is used to find centering cross hairs
		in frame objects using the FindCrossHair method.
		If a cross hair is found, the results are put in
		the x and y member vars.
  ---------------------------------------------------------------------- */
struct CROSS_RES
{
	int		x;			/* x co-ord of cross found */
	int		y;			/* y co-ord of cross found */

	bool	FindCrossHair(Frame const & Fr,u8 CrossCol);

private:
	int		FindLargestIndex(int * CountCol,int Width);
};


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*---------------------------------------------------------------------- */

#endif	/* __CROSS_HPP__ */

/*===========================================================================
 end */


