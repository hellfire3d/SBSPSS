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
void	CGlassesPickup::render()
{
	DVECTOR		ofs;
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	ofs=getRenderOffset();
	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__GLASSES);
	x=Pos.vx-ofs.vx-(fh->W/2);
	y=Pos.vy-ofs.vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,PICKUPS_OT_POS);

	CBasePickup::render();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGlassesPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

/*===========================================================================
end */
