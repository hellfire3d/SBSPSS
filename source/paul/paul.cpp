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

#ifndef __GFX_BUBICLES__
#include "gfx\bubicles.h"
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
char				*s_mem[3];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int ploopid1=0;
int ploopid2=0;
BubicleEmitterData data=
{
	206,200,40,40,		// m_x,m_y,m_w,m_h
	3,1,				// m_birthRate,m_birthAmount
	{					// m_bubicleBase
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		-40,-15,-200,		// m_vy,m_vdy,m_vymax
		13,10,				// m_w,m_h
		10,					// m_dvSizeChange
		0,100,				// m_theta,m_vtheta
		100,2,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		40,					// m_ot
		{ 128,128,128,	}	// m_colour
	},
	{					// m_bubicleRange
		100,				// m_life
		0,0,0,				// m_vx,m_vdx,m_vxmax
		20,10,0,			// m_vy,m_vdy,m_vymax
		5,5,				// m_w,m_h
		10,					// m_dvSizeChange
		4095,50,			// m_theta,m_vtheta
		100,5,0,			// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		0,					// m_ot
		{   0, 64,127,	}	// m_colour
	}
};
CBubicleEmitter tb;
int tbx=64,tby=230;
int	tbw=512-128,tbh=20;

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

	tb.init(&data);

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

	tb.render();

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

void CPaulScene::think()
{
	int				pad;
	int				sfxId=-1;
	int				setSfxVolume=false;
	int				setSongVolume=false;

	pad=PadGetDown(0);
	if(pad&PAD_R1)
	{
		tb.init(&data);
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
		if(pad&PAD_UP)
			tbh--;
		if(pad&PAD_DOWN)
			tbh++;
		if(pad&PAD_LEFT)
			tbw--;
		if(pad&PAD_RIGHT)
			tbw++;
	}
	else
	{
		if(pad&PAD_UP)
			tby-=5;
		if(pad&PAD_DOWN)
			tby+=5;
		if(pad&PAD_LEFT)
			tbx-=5;
		if(pad&PAD_RIGHT)
			tbx+=5;
	}
	tb.setPosAndSize(tbx,tby,tbw,tbh);

	tb.think();
}


/*===========================================================================
 end */