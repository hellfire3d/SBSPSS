/*=========================================================================

	pshoes.cpp

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

#include "pickups\pbubmix.h"

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
void	CBubbleMixturePickup::init()
{
	CBasePickup::init();
	m_sin=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleMixturePickup::shutdown()
{
	CBasePickup::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int bubmix_bobspeed=10;
int bubmix_bobscale=3;
void	CBubbleMixturePickup::think(int _frames)
{
	CBasePickup::think(_frames);
	m_sin=(m_sin+(_frames*bubmix_bobspeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleMixturePickup::render()
{
	DVECTOR		ofs;
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	ofs=getRenderOffset();
	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__BUBBLEMIXTURE);
	x=Pos.vx-ofs.vx-(fh->W/2);
	y=Pos.vy-ofs.vy-(fh->H/2)+((msin(m_sin)*bubmix_bobscale)>>12);
	sprites->printFT4(fh,x,y,0,0,PICKUPS_OT_POS);

	CBasePickup::render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleMixturePickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

/*===========================================================================
end */
