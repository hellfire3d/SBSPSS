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
static FontBank		s_fontBank;
char				*s_mem[3];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::init()
{
	s_fontBank.initialise(&standardFont);

	
	CSoundMediator::setSfxBank(CSoundMediator::INGAME);

	CSoundMediator::setSong(CSoundMediator::MUSIC);
	CSoundMediator::playSong();


//CXAStream::Init();
	PAUL_DBGMSG("initialised..");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::shutdown()
{
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPaulScene::render()
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int psfx=3;
int	pkill=0;
#include "sound\speech.h"
int mvol=10;
int svol=255;

void CPaulScene::think()
{
	int				pad;
	int				sfxId=-1;
	int				setVolumes=false;

	pad=PadGetDown(0);
	if(pad&PAD_R1)
	{
		PAUL_DBGMSG("-------------------");
	}
	else if(pad&PAD_CROSS)
	{
		sfxId=psfx;
	}
	else if(pad&PAD_SQUARE)
	{
		sfxId=0;
	}
	else if(pad&PAD_TRIANGLE)
	{
		sfxId=1;
	}
	else if(pad&PAD_CIRCLE)
	{
		sfxId=2;
	}
	if(sfxId!=-1)
	{
		CSoundMediator::playSfx(sfxId);

	}
	if(pad&PAD_START)
	{
		CSoundMediator::stopSfx((xmPlayingId)pkill);
	}


	pad=PadGetHeld(0);
	if(pad&PAD_UP)
	{
		if(++mvol>CSoundMediator::MAX_VOLUME)mvol=CSoundMediator::MAX_VOLUME;
		setVolumes=true;
	}
	if(pad&PAD_DOWN)
	{
		if(--mvol<CSoundMediator::MIN_VOLUME)mvol=CSoundMediator::MIN_VOLUME;
		setVolumes=true;
	}
	if(pad&PAD_RIGHT)
	{
		if(++svol>CSoundMediator::MAX_VOLUME)svol=CSoundMediator::MAX_VOLUME;
		setVolumes=true;
	}
	if(pad&PAD_LEFT)
	{
		if(--svol<CSoundMediator::MIN_VOLUME)svol=CSoundMediator::MIN_VOLUME;
		setVolumes=true;
	}
	if(setVolumes)
	{
		CSoundMediator::setVolume(CSoundMediator::SONG,mvol);
		CSoundMediator::setVolume(CSoundMediator::SFX,svol);
	}

//CXAStream::ControlXA();
}


/*===========================================================================
 end */