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

#include "pickups\pshoes.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

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
void	CShoesPickup::init()
{
	CBaseRespawningPickup::init();
	m_sin=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CShoesPickup::collect(class CPlayer *_player)
{
	CBaseRespawningPickup::collect(_player);
	_player->setMode(PLAYER_MODE_SQUEAKYBOOTS);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CShoesPickup::getRespawnTime()
{
	return 60*30;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int shoes_bobspeed=100;
int shoes_bobscale=2;
int shoes_seperation=4;
void	CShoesPickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*shoes_bobspeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CShoesPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y,yoff;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__SHOE);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	yoff=((msin(m_sin)*shoes_bobscale)>>12);
	sprites->printFT4(fh,x+shoes_seperation,y+yoff,0,0,PICKUPS_OT_POS);
	sprites->printFT4(fh,x-shoes_seperation,y-yoff,0,0,PICKUPS_OT_POS);
}

/*===========================================================================
end */
