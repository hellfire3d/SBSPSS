/*=========================================================================

	pballoon.cpp

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

#include "pickups\pballoon.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "layer\collision.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
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
void	CBalloonPickup::init()
{
	sFrameHdr	*fh;

	CBaseRespawningPickup::init();
	m_sin=0;

	fh=getSpriteBank()->getFrameHeader(FRM__BALLOON);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonPickup::collect(class CPlayer *_player)
{
	CBaseRespawningPickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int balloon_height=30;
int balloon_r=180;
int balloon_g=180;
int balloon_b=180;
int balloon_speed=25;
int balloon_scale1=5;
int balloon_scale2=2;
int balloon_phase=1024;
int balloon_vissize=40;
int		CBalloonPickup::getVisibilityRadius()
{
	return balloon_vissize;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBalloonPickup::getRespawnTime()
{
	return 60*10;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonPickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*balloon_speed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			xo1,xo2;
	int			x,y;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__BALLOON);

	xo1=((msin((m_sin+balloon_phase)&4095)*balloon_scale1)>>12);
	xo2=((msin(m_sin)*balloon_scale2)>>12);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x+xo1,y,0,0,PICKUPS_OT_POS);
setCollisionCentreOffset(xo1,0);

	x=_pos->vx;
	y=_pos->vy+(fh->H/2);
	DrawLine(x+xo1,y,x+xo2,y+balloon_height,balloon_r,balloon_g,balloon_b,PICKUPS_OT_POS);
}

/*===========================================================================
end */
