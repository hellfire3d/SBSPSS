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

#include "frontend\scrollbg.h"

//#ifndef	__UTILS_HEADER__
//#include "utils\utils.h"
//#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_FRONTEND_H__
#include <frontend.h>
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
	m_sprites->load(FRONTEND_FRONTEND_SPR);
	m_xOff=m_yOff=0;

	setSpeed(DEFAULT_X_SPEED,DEFAULT_Y_SPEED);
	setSpeedScale(DEFAULT_SPEED_SCALE);
	setOt(DEFAULT_OT);
	setFrame(FRM__BG1);
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
	POLY_FT4	*ft4;
	sFrameHdr	*fh;
	int			x,y,w,h;

	fh=m_sprites->getFrameHeader(m_frame);
	w=fh->W;
	h=fh->H;
	y=(m_yOff>>m_speedScale)-h;
	do
	{
		x=(m_xOff>>m_speedScale)-w;
		do
		{
			ft4=m_sprites->printFT4(fh,x,y,0,0,m_ot);
			setSemiTrans(ft4,true);
			x+=w;
		}
		while(x<512);
		y+=h;
	}
	while(y<256);
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

	fh=m_sprites->getFrameHeader(m_frame);

	m_xOff=(m_xOff+(_frames*m_xSpeed))%(fh->W<<m_speedScale);
	m_yOff=(m_yOff+(_frames*m_ySpeed))%(fh->H<<m_speedScale);
}


/*===========================================================================
 end */