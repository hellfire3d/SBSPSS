/*=========================================================================

	GRECT.CPP	

	Author:  Gary Liddon @ Climax
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <algorithm>

/*	Glib
	---- */
#include <frame.hpp>

/*	Local
	----- */
#include "grect.h"


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static Rect & AddAnotherRect(RectVec & Result);
static void AddRectMessage(Rect const & T);


/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
Rect & AddAnotherRect(RectVec & Result)
{
	Result.resize(Result.size()+1);
	return(Result[Result.size()-1]);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void CutRect(Rect const & ToBeCut,Rect const & TheCutter,RectVec & Result)
{
	Rect Cutter=TheCutter;

	ToBeCut.ClipRect(Cutter);
	
	if (Cutter)
		{
		/* Is there a top rectangle hanging about? */

		if (ToBeCut.Y != Cutter.Y)
			{
			Rect & TopRect=AddAnotherRect(Result);
			TopRect=ToBeCut;
			TopRect.H=Cutter.Y-TopRect.Y;
			AddRectMessage(TopRect);
			}

		/* Is there a bottom rectangle hanging about? */

		if ((ToBeCut.Y+ToBeCut.H) != (Cutter.Y+Cutter.H))
			{
			Rect & TopRect=AddAnotherRect(Result);
			TopRect=ToBeCut;
			TopRect.Y=Cutter.Y+Cutter.H;
			TopRect.H=(ToBeCut.Y+ToBeCut.H)-(Cutter.Y+Cutter.H);
			AddRectMessage(TopRect);
			}

		/* Is there a left rectangle hanging about? */

		if (ToBeCut.X != Cutter.X)
			{
			Rect & TopRect=AddAnotherRect(Result);
			TopRect=Cutter;
			TopRect.X=ToBeCut.X;
			TopRect.W=Cutter.X-ToBeCut.X;
			AddRectMessage(TopRect);
			}

		/* Is there a right rectangle hanging about? */
	
		if ((ToBeCut.X+ToBeCut.W) != (Cutter.X+Cutter.W))
			{
			Rect & TopRect=AddAnotherRect(Result);
			TopRect=Cutter;
			TopRect.X=Cutter.X+Cutter.W;
			TopRect.W=(ToBeCut.X+ToBeCut.W)-(Cutter.X+Cutter.W);
			AddRectMessage(TopRect);
			}
		}

}

void AddRectMessage(Rect const & T)
{
}


/*===========================================================================
 end */
