/*=========================================================================

	pglasses.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\pglasses.h"

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGlassesPickup::init()
{
	CBasePickup::init();

	m_glint=0;
	m_glintRot=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CGlassesPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=getSpriteBank()->getFrameHeader(FRM__GLASSES);
	size.vx=fh->W;
	size.vy=fh->H;
	return size;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGlassesPickup::collect(class CPlayer *_player)
{
	_player->giveGlasses();
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int glasses_glintframes=16;
int glasses_glintwait=400;
int glasses_glintspeed=1;
int glasses_glintRotSpeed=200;
DVECTOR glasses_gxy={2,2};
static const int glasses_glintFrames[]={FRM__GLINT1,FRM__GLINT2,FRM__GLINT3,FRM__GLINT4,FRM__GLINT4,FRM__GLINT3,FRM__GLINT2,FRM__GLINT1};
void	CGlassesPickup::thinkPickup(int _frames)
{
	m_glint+=_frames;
	if(m_glint>glasses_glintframes+glasses_glintwait)m_glint=0;
	m_glintRot+=_frames*glasses_glintRotSpeed;
	m_glintRot&=4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGlassesPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__GLASSES);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,PICKUPS_OT_POS);

	if(m_glint<=glasses_glintframes)
	{
		fh=sprites->getFrameHeader(glasses_glintFrames[(m_glint>>glasses_glintspeed)&0x07]);
		sprites->printRotatedScaledSprite(fh,x+glasses_gxy.vx,y+glasses_gxy.vy,4095,4095,m_glintRot,PICKUPS_OT_POS-1);
	}
}

/*===========================================================================
end */
