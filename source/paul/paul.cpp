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


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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
int ploopid1=0;
int ploopid2=0;

static SpriteBank	*sprites=NULL;

void CPaulScene::init()
{
	s_fontBank.initialise(&standardFont);

	
	CSoundMediator::setSfxBank(CSoundMediator::INGAME);

	CSoundMediator::setSong(CSoundMediator::MUSIC);
	CSoundMediator::playSong();


//CXAStream::Init();
	PAUL_DBGMSG("initialised..");

	ploopid1=CSoundMediator::playSfx(0);
	ploopid2=CSoundMediator::playSfx(0);

	sprites=new ("bg sprite") SpriteBank();
	sprites->load(INGAMEFX_INGAMEFX_SPR);
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

	POLY_FT4	*ft4=sprites->printFT4(FRM_BACKGROUND,0,0,0,0,1023);
	setXYWH(ft4,20,20,512-40,256-40);
	setRGB0(ft4,64,64,64);
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
VECTOR ppos;
int psp=0;

void CPaulScene::think(int _frames)
{
	int				pad;
	int				sfxId=-1;
	int				setSfxVolume=false;
	int				setSongVolume=false;

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
//		PAUL_DBGMSG("stop loopers..");
//		CSoundMediator::stopSfx((xmPlayingId)ploopid1);
//		CSoundMediator::stopSfx((xmPlayingId)ploopid2);
		PAUL_DBGMSG("speech..\n");
		CSoundMediator::playSpeech((SpeechEquate)psp);
	}
	if(pad&PAD_R2)
	{
		PAUL_DBGMSG("stop all");
		CSoundMediator::stopAllSound();
	}
#ifdef __USER_paul__
	if(pad&PAD_L2)
	{
		extern int dump;
		dump=true;
	}
#endif

//CSoundMediator::setposition((xmPlayingId)ploopid,&ppos);

	pad=PadGetHeld(0);
	if(pad&PAD_UP)
	{
		mvol+=16;
		if(mvol>CSoundMediator::MAX_VOLUME)mvol=CSoundMediator::MAX_VOLUME;
		setSongVolume=true;
	}
	if(pad&PAD_DOWN)
	{
		mvol-=16;
		if(mvol<CSoundMediator::MIN_VOLUME)mvol=CSoundMediator::MIN_VOLUME;
		setSongVolume=true;
	}
	if(pad&PAD_RIGHT)
	{
		svol+=16;
		if(svol>CSoundMediator::MAX_VOLUME)svol=CSoundMediator::MAX_VOLUME;
		setSfxVolume=true;
	}
	if(pad&PAD_LEFT)
	{
		svol-=16;
		if(svol<CSoundMediator::MIN_VOLUME)svol=CSoundMediator::MIN_VOLUME;
		setSfxVolume=true;
	}
	if(setSongVolume)		CSoundMediator::setVolume(CSoundMediator::SPEECH,mvol);
	if(setSfxVolume)		CSoundMediator::setVolume(CSoundMediator::SFX,svol);

	if(setSongVolume||setSfxVolume)
	{
		PAUL_DBGMSG("speech:%d   sfx:%d",mvol,svol);
	}





//CXAStream::ControlXA();

	pad=PadGetHeld(1);
	if(pad&PAD_L1)
	{
	}
	else
	{
	}

}


/*===========================================================================
 end */