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

int					CFader::s_fadeValue=0;	
CFader::FADE_MODE	CFader::s_fadeMode=FADED_IN;
CFader::FADE_STYLE	CFader::s_fadeStyle;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

BubicleEmitterData	s_fadeBubicleSpawner=
{
	0,0,512,40,					// m_x,m_y,m_w,m_h
	0,0,						// m_birthRate,m_birthAmount
	0,							// m_life
	false,						// m_applyMapOffsets

	// m_bubicleBase
	{
		10,							// m_life
		0,0,0,						// m_vx,m_vdx,m_vxmax
		-200,0,0,					// m_vy,m_vdy,m_vymax
		27,18,						// m_w,m_h
		0,							// m_dvSizeChange
		0,0,						// m_theta,m_vtheta
		0,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		0,							// m_ot
		{0,0,0}						// m_colour
	},
	// m_bubicleRange;
	{
		10,							// m_life
		0,0,0,						// m_vx,m_vdx,m_vxmax
		50,0,0,						// m_vy,m_vdy,m_vymax
		4,4,						// m_w,m_h
		0,							// m_dvSizeChange
		0,100,						// m_theta,m_vtheta
		10,0,0,						// m_wobbleWidth,m_vwobbleWidth,m_vdwobbleWidth
		0,							// m_ot
		{30,30,30}					// m_colour
	}
};

void CFader::render()
{
	switch(s_fadeMode)
	{
		case FADED_IN:
			return;

		case FADING_IN:
			{
				switch(s_fadeStyle)
				{
					case BUBBLE_FADE:
					{				
						if(s_fadeValue>0)
						{
							POLY_F4		*f4;
							POLY_G4		*g4;
							POLY_FT3	*ft3;

							f4=GetPrimF4();
							setXYWH(f4,0,-FADE_BORDER_SIZE,512,s_fadeValue-(FADE_SMOOTH_BORDER_SIZE/2)+FADE_BORDER_SIZE);
							setRGB0(f4,255,255,255);
							AddPrimToList(f4,0);

							g4=GetPrimG4();
							setXYWH(g4,0,s_fadeValue-(FADE_SMOOTH_BORDER_SIZE/2),512,FADE_SMOOTH_BORDER_SIZE);
							setRGB0(g4,255,255,255);
							setRGB1(g4,255,255,255);
							setRGB2(g4,0,0,0);
							setRGB3(g4,0,0,0);
							setShadeTex(g4,0);
							setSemiTrans(g4,1);
							AddPrimToList(g4,0);

							ft3=GetPrimFT3();
							setShadeTex(ft3,1);
							setSemiTrans(ft3,1);
							ft3->tpage=(1<<5);
							setXY3(ft3,512,512,512,512,512,512);
							AddPrimToList(ft3,0);
						}
						break;
					}

					case BLACK_FADE:
					{
						POLY_F4		*f4;
						POLY_FT3	*ft3;
						f4=GetPrimF4();
						setXYWH(f4,0,0,511,255);
						setRGB0(f4,s_fadeValue,s_fadeValue,s_fadeValue);
						setShadeTex(f4,0);
						setSemiTrans(f4,1);
						AddPrimToList(f4,0);
						ft3=GetPrimFT3();
						setPolyFT3(ft3);
						setShadeTex(ft3,1);
						setSemiTrans(ft3,1);
						ft3->tpage=(2<<5);
						setXY3(ft3,512,512,512,512,512,512);
						AddPrimToList(ft3,0);
						break;
					}
				}
			}
			break;

		case FADING_OUT:
			{
				switch(s_fadeStyle)
				{
					case BUBBLE_FADE:
					{				
						POLY_F4		*f4;
						POLY_G4		*g4;
						POLY_FT3	*ft3;
						
						f4=GetPrimF4();
						setXYWH(f4,0,s_fadeValue+(FADE_SMOOTH_BORDER_SIZE/2),512,256-s_fadeValue-(FADE_SMOOTH_BORDER_SIZE/2));
						setRGB0(f4,255,255,255);
						AddPrimToList(f4,0);
						
						g4=GetPrimG4();
						setXYWH(g4,0,s_fadeValue-(FADE_SMOOTH_BORDER_SIZE/2),512,FADE_SMOOTH_BORDER_SIZE);
						setRGB0(g4,0,0,0);
						setRGB1(g4,0,0,0);
						setRGB2(g4,255,255,255);
						setRGB3(g4,255,255,255);
						setShadeTex(g4,0);
						setSemiTrans(g4,1);
						AddPrimToList(g4,0);
						
						ft3=GetPrimFT3();
						setShadeTex(ft3,1);
						setSemiTrans(ft3,1);
						ft3->tpage=(1<<5);
						setXY3(ft3,512,512,512,512,512,512);
						AddPrimToList(ft3,0);
						break;
					}

					case BLACK_FADE:
					{
						POLY_F4		*f4;
						POLY_FT3	*ft3;
						int			col;
						col=255-s_fadeValue;
						f4=GetPrimF4();
						setXYWH(f4,0,0,511,255);
						setRGB0(f4,col,col,col);
						setShadeTex(f4,0);
						setSemiTrans(f4,1);
						AddPrimToList(f4,0);
						ft3=GetPrimFT3();
						setPolyFT3(ft3);
						setShadeTex(ft3,1);
						setSemiTrans(ft3,1);
						ft3->tpage=(2<<5);
						setXY3(ft3,512,512,512,512,512,512);
						AddPrimToList(ft3,0);
						break;
					}
				}
			}
			break;

		case FADED_OUT:
			{
				POLY_F4		*f4;
				
				f4=GetPrimF4();
				setXYWH(f4,0,0,512,256);
				switch(s_fadeStyle)
				{
					case BUBBLE_FADE:
						setRGB0(f4,255,255,255);
						break;
					case BLACK_FADE:
						setRGB0(f4,0,0,0);
						break;
				}
				AddPrimToList(f4,0);
				return;
			}
	}

	if(s_fadeStyle==BUBBLE_FADE)
	{
		s_fadeBubicleSpawner.m_y=s_fadeValue-20;
		for(int i=0;i<BUBBLES_PER_FRAME;i++)
			CBubicleFactory::spawnParticle(&s_fadeBubicleSpawner);
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
	if(s_fadeMode!=FADED_IN&&s_fadeMode!=FADED_OUT)
	{
		s_fadeValue-=_frames*FADE_SPEED;
		switch(s_fadeStyle)
		{
			case BUBBLE_FADE:
			{
				if(s_fadeValue<-FADE_BORDER_SIZE)
				{
					if(s_fadeMode==FADING_OUT)
					{
						s_fadeMode=FADED_OUT;
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
					if(s_fadeValue>=0&&s_fadeValue<=255)
					{
						CSoundMediator::setVolume(CSoundMediator::VOL_FADE,s_fadeMode==FADING_OUT?s_fadeValue:255-s_fadeValue);
					}
				}
				break;
			}

			case BLACK_FADE:
			{
				if(s_fadeValue<0)
				{
					s_fadeValue=0;
					if(s_fadeMode==FADING_OUT)
					{
						s_fadeMode=FADED_OUT;
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
		case BUBBLE_FADE:
			s_fadeValue=256+FADE_BORDER_SIZE;
			break;
		case BLACK_FADE:
			s_fadeValue=255;
			break;
	}

	s_fadeMode=FADING_OUT;
	s_fadeStyle=_style;
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
		case BUBBLE_FADE:
			s_fadeValue=256+FADE_BORDER_SIZE;
			break;
		case BLACK_FADE:
			s_fadeValue=255;
			break;
	}
	s_fadeMode=FADING_IN;
	s_fadeStyle=_style;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CFader::isFading()
{
	return s_fadeMode==FADING_IN||s_fadeMode==FADING_OUT;
}


/*===========================================================================
 end */