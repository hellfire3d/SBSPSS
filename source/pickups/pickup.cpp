/*=========================================================================

	pickup.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include 	"system\vid.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\pickup.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

// For the factory..

#ifndef	__PICKUPS_PLIFE_H__
#include "pickups\plife.h"
#endif

#ifndef	__PICKUPS_PSPATULA_H__
#include "pickups\pspatula.h"
#endif

#ifndef	__PICKUPS_PJLAMMO_H__
#include "pickups\pjlammo.h"
#endif

#ifndef	__PICKUPS_PBUBMIX_H__
#include "pickups\pbubmix.h"
#endif

#ifndef	__PICKUPS_PBLOWER_H__
#include "pickups\pblower.h"
#endif

#ifndef	__PICKUPS_PSHOES_H__
#include "pickups\pshoes.h"
#endif

#ifndef	__PICKUPS_PBALLOON_H__
#include "pickups\pballoon.h"
#endif

#ifndef	__PICKUPS_PHELMET_H__
#include "pickups\phelmet.h"
#endif

#ifndef	__PICKUPS_PNET_H__
#include "pickups\pnet.h"
#endif

#ifndef	__PICKUPS_PQUEST_H__
#include "pickups\pquest.h"
#endif

#ifndef	__PICKUPS_PKELP_H__
#include "pickups\pkelp.h"
#endif

#ifndef	__PICKUPS_PGLOVE_H__
#include "pickups\pglove.h"
#endif

#include	"game/game.h"

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
void	CBasePickup::init()
{
	CPickupThing::init();

}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::shutdown()
{
	CPickupThing::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::think(int _frames)
{
	CPickupThing::think(_frames);
	thinkPickup(_frames);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::render()
{
	DVECTOR	ofs,pos;
	int		visibilityRadius;
	
	CPickupThing::render();
	if (canRender())
	{
		DVECTOR	&renderPos=getRenderPos();
		renderPickup(&renderPos);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
			collect((CPlayer*)_thisThing);
			CSoundMediator::playSfx(sfxToPlayWhenCollected());
			break;

		case TYPE_NPC:
			break;

		default:
			ASSERT(0);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::setPos(const DVECTOR *_pos)
{
	Pos=*_pos;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::collect(class CPlayer *_player)
{
	shutdown();
	CThingManager::DeleteThing(this);
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseRespawningPickup::init()
{
	CBasePickup::init();
	m_respawnTime=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int respawnFlashFrames=50;
void	CBaseRespawningPickup::think(int _frames)
{
	if(m_respawnTime<=respawnFlashFrames)
	{
		CBasePickup::think(_frames);
	}

	if(m_respawnTime)
	{
		m_respawnTime-=_frames;
		if(m_respawnTime<0)
		{
			m_respawnTime=0;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseRespawningPickup::render()
{
	if(m_respawnTime==0||
	   m_respawnTime<respawnFlashFrames&&!(m_respawnTime&4))
	{
		CBasePickup::render();
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseRespawningPickup::collect(class CPlayer *_player)
{
	m_respawnTime=getRespawnTime();
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseWeaponPickup::init()
{
	CBasePickup::init();

	m_dontAutoPickUpUntilPlayerMovesOffMe=true;
	m_collidedWithPlayer=true;
	m_hasBeenCollected=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseWeaponPickup::think(int _frames)
{
	CBasePickup::think(_frames);

	if(!m_collidedWithPlayer)
	{
		m_dontAutoPickUpUntilPlayerMovesOffMe=false;
	}
	else
	{
		m_collidedWithPlayer=false;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseWeaponPickup::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
			{
				CPlayer	*player;
				player=(CPlayer*)_thisThing;
				if(player->tryingToManuallyPickupWeapon()||
				   (!m_dontAutoPickUpUntilPlayerMovesOffMe&&player->tryingToAutomaticallyPickupWeapon()))
				{
					collect(player);
					CSoundMediator::playSfx(sfxToPlayWhenCollected());
				}
				else
				{
					m_collidedWithPlayer=true;
				}
			}
			break;

		case TYPE_NPC:
			break;

		default:
			ASSERT(0);
			break;
	}
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseWeaponSimplePickup::init()
{
	sFrameHdr	*fh;

	CBaseWeaponPickup::init();

	fh=CGameScene::getSpriteBank()->getFrameHeader(getWeaponSpriteFrame());
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CBaseWeaponSimplePickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(getWeaponSpriteFrame());
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
void	CBaseWeaponSimplePickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(getWeaponSpriteFrame());
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);
}







/*----------------------------------------------------------------------
	Function:
	Purpose:	This is basically a factory method for making pickups :)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CBasePickup	*createPickup(const PICKUP_TYPE _type,const DVECTOR *_pos)
{
	CBasePickup	*pickup;
	DVECTOR		pickupPos;

	pickup = (CBasePickup*)CThingManager::GetThing(CThing::TYPE_PICKUP,_type);
	if (!pickup)
	switch(_type)
	{
		case PICKUP__BIG_HEALTH:
		case PICKUP__MEDIUM_HEALTH:
		case PICKUP__SMALL_HEALTH:
			ASSERT(!"HEALTH PICKUPS ARE NO LONGER IN THE GAME");
return NULL;
			break;

		case PICKUP__LIFE:
			pickup=new ("LifePickup") CLifePickup();
			break;

		case PICKUP__SPATULA:
			pickup=new ("SpatulaPickup") CSpatulaPickup();
			break;

		case PICKUP__JELLY_LAUNCHER_AMMO:
			pickup=new ("JellyLauncherAmmoPickup") CJellyLauncherAmmoPickup();
			break;

		case PICKUP__BUBBLE_MIXTURE:
			pickup=new ("BubbleAmmoPickup") CBubbleMixturePickup();
			break;

		case PICKUP__BUBBLE_WAND:
			pickup=new ("BubbleWandPickup") CBubbleWandPickup();
			break;

		case PICKUP__NET:
			pickup=new ("NetPickup") CNetPickup();
			break;

		case PICKUP__GLASSES:
			ASSERT(!"GLASSES ARE NO LONGER IN THE GAME");
			break;

		case PICKUP__SQUEAKY_SHOES:
			pickup=new ("ShoesPickup") CShoesPickup();
			break;

		case PICKUP__BALLOON:
			pickup=new ("BalloonPickup") CBalloonPickup();
			break;

		case PICKUP__HELMET:
			pickup=new ("HelmetPickup") CHelmetPickup();
			break;

		case PICKUP__CORAL_BLOWER:
			pickup=new ("CoralBlowerPickup") CCoralBlowerPickup();
			break;

		case PICKUP__QUEST_ITEM:
			pickup=new ("QuestItemPickup") CQuestItemPickup();
			break;

		case PICKUP__BALLOON_AND_SPATULA:
			ASSERT(!"BALLOON AND SPATULA PICKUPS ARE NO LONGER IN THE GAME");
			break;

		case PICKUP__JELLY_LAUNCHER:
			pickup=new ("JellylauncherPickup") CJellyLauncherPickup();
			break;

		case PICKUP__KELP_TOKEN:
			pickup=new ("KelpTokenPickup") CKelpTokenPickup();
			break;

		case PICKUP__GLOVE:
			pickup=new ("GlovePickup") CGlovePickup();
			break;				

		default:
			ASSERT(!"UNKNOWN PICKUP TYPE");
			return NULL;
	}

	pickup->setThingSubType(_type);
	pickup->init();
//	pickupPos=pickup->getSizeForPlacement();
//	pickupPos.vx=_pos->vx-(pickupPos.vx/2);
//	pickupPos.vy=_pos->vy-(pickupPos.vy);
	pickupPos=*_pos;
	pickupPos.vy-=pickup->getSizeForPlacement().vy/2;
	pickup->setPos(&pickupPos);

	return pickup;
}

/*===========================================================================
end */
