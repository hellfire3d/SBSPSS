/*=========================================================================

	pbubmix.cpp

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
void	CBubbleMixturePickup::init()
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
void	CBubbleMixturePickup::collect(class CPlayer *_player)
{
	_player->setMode(PLAYER_MODE_BUBBLE_MIXTURE);
	CBaseRespawningPickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBubbleMixturePickup::getRespawnTime()
{
	return 60*10;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int bubmix_bobspeed=10;
int bubmix_bobscale=3;
void	CBubbleMixturePickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*bubmix_bobspeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleMixturePickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__BUBBLEMIXTURE);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2)+((msin(m_sin)*bubmix_bobscale)>>12);
	sprites->printFT4(fh,x,y,0,0,PICKUPS_OT_POS);
}

/*===========================================================================
end */
