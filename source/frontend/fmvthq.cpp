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

static	bool s_finished;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool fmvPerFrameFunc()
{
	PadUpdate();
	if(PadGetDown(0)&(PAD_START|PAD_CROSS))
	{
		PadUpdate();
		s_finished=true;
	}
	return s_finished;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::init()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::shutdown()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::select()
{
	s_finished=false;
	FMV_play(FMV_THQ,&fmvPerFrameFunc);
	s_finished=true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::unselect()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::render()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndFMVTHQ::think(int _frames)
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndFMVTHQ::isReadyToExit()
{
	return s_finished;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndFMVTHQ::getNextMode()
{
	return CFrontEndScene::MODE__MAIN_TITLES;
}

/*===========================================================================
 end */