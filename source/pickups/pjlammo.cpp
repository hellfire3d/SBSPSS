/*=========================================================================

	pjlammo.cpp

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

#include "pickups\pjlammo.h"

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
void	CJellyLauncherAmmoPickup::init()
{
	CBasePickup::init();
	m_rattle=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CJellyLauncherAmmoPickup::shutdown()
{
	CBasePickup::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int jlammo_rattlespeed=500;
int jlammo_rattlescale=90;
int jlammo_rattlecount=5;
int jlammo_waitcount=10;
void	CJellyLauncherAmmoPickup::think(int _frames)
{
	CBasePickup::think(_frames);
	m_rattle+=jlammo_rattlespeed*_frames;
	if(m_rattle>(jlammo_rattlecount+jlammo_waitcount)*4095)m_rattle=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CJellyLauncherAmmoPickup::render()
{
	DVECTOR		ofs;
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;
	int			angle;

	ofs=getRenderOffset();
	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__JELLYAMMO);
	x=Pos.vx-ofs.vx;
	y=Pos.vy-ofs.vy;
	if(m_rattle<=jlammo_rattlecount*4095)
	{
		angle=((msin(m_rattle&4095)*jlammo_rattlescale)>>12)&4095;
	}
	else
	{
		angle=0;
	}
	sprites->printRotatedScaledSprite(fh,x,y,4096,4096,angle,PICKUPS_OT_POS);

	CBasePickup::render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CJellyLauncherAmmoPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

/*===========================================================================
end */
