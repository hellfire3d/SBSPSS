/*=========================================================================

	fader.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "fader.h"

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __GFX_BUBICLES_H__
#include "gfx\bubicles.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#include	"system\vid.h"
#include	"pad\vibe.h"

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

CFader::FADE_MODE	CFader::s_fadeMode=FADED_IN;
CFader::FADE_STYLE	CFader::s_fadeStyle;
int					CFader::s_fadeValue=0;	
int					CFader::s_waitFrames=0;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::render()
{
int		TMode;
TPOLY_F4	*f4;
	
	TMode=(s_fadeStyle==BLACK_FADE?2:1);

	switch(s_fadeMode)
	{
		case FADED_IN:
			return;

		case PAUSE_BEFORE_FADING_IN:
		case FADING_IN:
			{
				switch(s_fadeStyle)
				{
					case BLACK_FADE:
					case WHITE_FADE:
					{
						f4=GetPrimTF4();
						setXYWH(f4,0,0,VidGetScrW()+1,VidGetScrH()+1);
						setRGB0(f4,s_fadeValue,s_fadeValue,s_fadeValue);
						setTShadeTex(f4,0);
						setTSemiTrans(f4,1);
						setTABRMode(f4,TMode);
						AddPrimToList(f4,0);
						break;
					}
				}
			}
			break;

		case FADING_OUT:
		case PAUSE_AFTER_FADING_OUT:
			{
				switch(s_fadeStyle)
				{
					case BLACK_FADE:
					case WHITE_FADE:
					{
						int			col;
						col=255-s_fadeValue;
						f4=GetPrimTF4();
						setXYWH(f4,0,0,VidGetScrW()+1,VidGetScrH()+1);
						setRGB0(f4,col,col,col);
						setTShadeTex(f4,0);
						setTSemiTrans(f4,1);
						setTABRMode(f4,TMode);
						AddPrimToList(f4,0);
						break;
					}
				}
			}
			break;

		case FADED_OUT:
			{
				f4=GetPrimTF4();
				setXYWH(f4,0,0,VidGetScrW()+1,VidGetScrH());
				switch(s_fadeStyle)
				{
					case BLACK_FADE:
						setRGB0(f4,0,0,0);
						break;
					case WHITE_FADE:
						setRGB0(f4,255,255,255);
						break;
				}
				AddPrimToList(f4,0);
				return;
			}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::think(int _frames)
{
	switch(s_fadeMode)
	{
		case FADING_IN:
		case FADING_OUT:
			s_fadeValue-=_frames*FADE_SPEED;
			switch(s_fadeStyle)
			{
				case BLACK_FADE:
				case WHITE_FADE:
				{
					if(s_fadeValue<0)
					{
						s_fadeValue=0;
						if(s_fadeMode==FADING_OUT)
						{
							s_waitFrames=FRAMES_TO_WAIT;
							s_fadeMode=PAUSE_AFTER_FADING_OUT;
							CSoundMediator::setVolume(CSoundMediator::VOL_FADE,0);
						}
						else
						{
							s_fadeMode=FADED_IN;
							CSoundMediator::setVolume(CSoundMediator::VOL_FADE,255);
						}
					}
					else
					{
						CSoundMediator::setVolume(CSoundMediator::VOL_FADE,s_fadeMode==FADING_OUT?s_fadeValue:255-s_fadeValue);
					}
					break;
				}
			}
			break;

		case PAUSE_BEFORE_FADING_IN:
			if(--s_waitFrames==0)
			{
				s_fadeMode=FADING_IN;
			}
			break;

		case PAUSE_AFTER_FADING_OUT:
			if(--s_waitFrames==0)
			{
				s_fadeMode=FADED_OUT;
			}
			break;

		default:
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::setFadingOut(FADE_STYLE _style)
{
	switch(_style)
	{
		case BLACK_FADE:
		case WHITE_FADE:
			s_fadeValue=255;
			break;
	}


	s_fadeMode=FADING_OUT;
	s_fadeStyle=_style;
	s_waitFrames=FRAMES_TO_WAIT;
	CPadVibrationManager::stopAllVibration();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CFader::setFadingIn(FADE_STYLE _style)
{
	switch(_style)
	{
		case BLACK_FADE:
		case WHITE_FADE:
			s_fadeValue=255;
			break;
	}
	s_fadeStyle=_style;
	s_fadeMode=PAUSE_BEFORE_FADING_IN;
	s_waitFrames=FRAMES_TO_WAIT;
	CPadVibrationManager::stopAllVibration();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFader::isFading()
{
	return s_fadeMode!=FADED_IN&&s_fadeMode!=FADED_OUT;
}


/*===========================================================================
 end */