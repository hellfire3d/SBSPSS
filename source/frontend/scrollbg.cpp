/*=========================================================================

	scrollbg.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "frontend\scrollbg.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScrollyBackground::init()
{
	m_sprites=new ("Scrolly Background sprites") SpriteBank;
	m_sprites->load(SPRITES_SPRITES_SPR);
	m_xOff=m_yOff=0;

	setSpeed(DEFAULT_X_SPEED,DEFAULT_Y_SPEED);
	setSpeedScale(DEFAULT_SPEED_SCALE);
	setOt(DEFAULT_OT);
	setFrame(FRM__BUTX);
	setTheDrawMode(DRAWMODE_NORMAL);
	setColour(128,128,128);
	setScale(NORMAL_SCALE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScrollyBackground::shutdown()
{
	m_sprites->dump();	delete m_sprites;	m_sprites=NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScrollyBackground::render()
{
	int			smode;
	POLY_FT4	*ft4;
	sFrameHdr	*fh;
	int			x,y,w,h;

	smode=m_drawMode;

	fh=m_sprites->getFrameHeader(m_frame);
	w=(fh->W*m_scale)>>8;
	h=(fh->H*m_scale)>>8;
	y=(m_yOff>>m_speedScale)-h;
	do
	{
		x=(m_xOff>>m_speedScale)-w;
		do
		{
			if(m_scale==NORMAL_SCALE)
			{
				ft4=m_sprites->printFT4(fh,x,y,0,0,m_ot);
			}
			else
			{
				ft4=m_sprites->printFT4Scaled(fh,x,y,0,0,m_ot,m_scale);
			}
			setShadeTex(ft4,0);
			setSemiTrans(ft4,true);
			ft4->tpage|=(smode<<5);
			setRGB0(ft4,m_r,m_g,m_b);
			x+=w;
		}
		while(x<512+64);
		y+=h;
	}
	while(y<256+64);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScrollyBackground::think(int _frames)
{
	sFrameHdr	*fh;
	int			w,h;

	fh=m_sprites->getFrameHeader(m_frame);
	w=(fh->W*m_scale)>>8;
	h=(fh->H*m_scale)>>8;

	m_xOff=(m_xOff+(_frames*m_xSpeed))%(w<<m_speedScale);
	m_yOff=(m_yOff+(_frames*m_ySpeed))%(h<<m_speedScale);
}


/*===========================================================================
 end */