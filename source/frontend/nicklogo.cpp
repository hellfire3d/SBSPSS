/*=========================================================================

	nicklogo.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\nicklogo.h"

#ifndef	__GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
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
void CFrontEndNickLogo::select()
{
	m_readyToExit=false;

	m_image=LoadPakScreen(BACKDROP_NICK_GFX);
	ASSERT(m_image);
	SetScreenImage(m_image);

	CFader::setFadingIn();
	m_frameCount=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndNickLogo::unselect()
{
	MemFree(m_image);	m_image=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFrontEndNickLogo::think(int _frames)
{
	if(!CFader::isFading())
	{
		m_frameCount+=_frames;

		if(!m_readyToExit&&
		   (PadGetDown(0)&(PAD_CROSS|PAD_START)||m_frameCount>DISPLAY_FRAMES))
		{
			CFader::setFadingOut();
			m_readyToExit=true;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFrontEndNickLogo::isReadyToExit()
{
	return !CFader::isFading()&&m_readyToExit;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CFrontEndScene::FrontEndMode CFrontEndNickLogo::getNextMode()
{
	return CFrontEndScene::MODE__FMV_THQ;
}

/*===========================================================================
 end */