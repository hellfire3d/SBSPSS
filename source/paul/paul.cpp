/*=========================================================================

	paul.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "paul\paul.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __GUI_GUI_H__
#include "gui\gui.h"
#endif

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GREADOUT_H__
#include "gui\greadout.h"
#endif

#ifndef __GUI_GBUTTON_H__
#include "gui\gbutton.h"
#endif

//#ifndef __GUI_GTEXTENT_H__
//#include "gui\gtextent.h"
//#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__FRONTEND_SCROLLBG_H__
#include "frontend\scrollbg.h"
#endif

#ifndef __SCRIPT_SCRIPT_H__
#include "script\script.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif


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
static FontBank		s_fontBank;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::init()
{
	s_fontBank.initialise(&standardFont);
	s_fontBank.setColour(255,255,255);
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::shutdown()
{
	s_fontBank.dump();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int showDebugLog=false;
void CPaulScene::render()
{
	if(showDebugLog)
	{
		int		logCount;
		int		i,y,charHeight;
		logCount=getNumberOfDbgLinesInLog();
		y=20;
		charHeight=s_fontBank.getCharHeight();
		for(i=0;i<logCount;i++)
		{
			s_fontBank.print(20,y,getDbgLineFromLog(i));
			y+=charHeight;
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int sfxNum=0;
void CPaulScene::think(int _frames)
{
	if(PadGetHeld(0)&PAD_R1&&PadGetDown(0)&PAD_R2)
	{
		showDebugLog=!showDebugLog;
	}

	int	pad=PadGetDown(1);
	if(pad&PAD_CIRCLE)
	{
		CSoundMediator::playSfx((CSoundMediator::SFXID)sfxNum);
	}
	else if(pad&PAD_UP)
	{
		if(++sfxNum>CSoundMediator::NUM_SFXIDS-1)sfxNum=CSoundMediator::NUM_SFXIDS-1;
	}
	else if(pad&PAD_DOWN)
	{
		if(--sfxNum<0)sfxNum=0;
	}
}


/*===========================================================================
 end */
