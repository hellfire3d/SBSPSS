/*=========================================================================

	GRECT.CPP	

	Author:  Gary Liddon @ Climax
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 Climax Development Ltd

===========================================================================*/

#ifndef __GRECT_H__
#define __GRECT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>

/*	Glib
	---- */
#include <frame.hpp>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef std::vector<Rect> RectVec;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */
void CutRect(Rect const & ToBeCut,Rect const & TheCutter,RectVec & Result);
void ReportRectVec(RectVec & Result);
void GRectTest(void);

/*---------------------------------------------------------------------- */

#endif	/* __GRECT_H__ */

/*===========================================================================
 end */
