/*=========================================================================

	fmvthq.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\fmvthq.h"

#ifndef __FMV_HEADER__
#include "fmv\fmv.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static bool fmvPerFrameFunc()
{
	PadUpdate();
	if(PadGetDown(0)&(PAD_START|PAD_CROSS))
	{
		PadUpdate();
		return true;
	}
	return false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::select()
{
	StopLoad();
	FMV_play(FMV_THQ,&fmvPerFrameFunc);
	FMV_play(FMV_CLIMAX,&fmvPerFrameFunc);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndFMVTHQ::isReadyToExit()
{
	return true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndFMVTHQ::getNextMode()
{
	return CFrontEndScene::MODE__FMV_INTRO;
}

/*===========================================================================
 end */