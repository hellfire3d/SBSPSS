/*=========================================================================

	sndtest.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */
#if defined(__VERSION_DEBUG__)

#include "paul\sndtest.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
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

CSceneSoundTest	SoundTestScene;

static int			s_soundId;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSoundTest::init()
{
	m_font=new ("scene select font") ScalableFontBank();
	m_font->initialise(&standardFont);
	m_font->setJustification(FontBank::JUST_CENTRE);
	m_font->setOt(10);
	m_font->setScale(400);

	s_soundId=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSoundTest::shutdown()
{
	m_font->dump();				delete m_font;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSoundTest::render()
{
	POLY_F4	*f4;
	char	buf[20];

	f4=GetPrimF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,10,20,30);
	AddPrimToList(f4,15);

	sprintf(buf,"%d",s_soundId);
	m_font->print(256,80,buf);
	m_font->print(256,140,"UP/DOWN - Select");
	m_font->print(256,160,"X - Play");


}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CSceneSoundTest::think(int _frames)
{
	int	padDown,padRepeat;

	padDown=PadGetDown(0);
	padRepeat=PadGetRepeat(0);

	if(padRepeat&PAD_DOWN)
	{
		if(--s_soundId<0)
		{
			s_soundId=CSoundMediator::NUM_SFXIDS-1;
		}
	}
	else if(padRepeat&PAD_UP)
	{
		if(++s_soundId==CSoundMediator::NUM_SFXIDS)
		{
			s_soundId=0;
		}
	}

	if(padDown&PAD_CROSS)
	{
		CSoundMediator::playSfx((CSoundMediator::SFXID)s_soundId);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CSceneSoundTest::readyToShutdown()
{
	return false;
}

#else
void	SndTestSelTestDummyStub()
{
}
#endif

/*===========================================================================
 end */